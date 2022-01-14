#include "currentcircuit.h"

#include "gate_tables.h"
#include <stack>
#include <vector>
#include "atpg_circuit.h"

Circuit CurrentCircuit::circ;

std::string CurrentCircuit::delay_file_name;

CurrentCircuit::CurrentCircuit()
{

}

void CurrentCircuit::crit_path(ATPGCircuit circ) {
    std::vector<std::string> visited;
    std::vector<ATPGCircuitElement*> pis = circ.get_pis();
    std::vector<ATPGCircuitElement*> pos = circ.get_pos();

    std::vector<std::string> path;
    std::stack<ATPGCircuitElement*> traverse;

    // initial start from PO
    int max_arrival = 0;
    ATPGCircuitElement* curr = nullptr;
    for(auto po : pos) {
        if(po->arrival > max_arrival) {
            max_arrival = po->arrival;
            curr = po;
        }
    }

    bool found = false;
    traverse.push(curr);
    while(!traverse.empty()) {
        curr = traverse.top();
        visited.push_back(curr->name);
        path.push_back(curr->name);

        if(curr->inputs.size() != 0) { // node
            std::vector<ATPGCircuitElement*> inputs = circ.get_inputs(curr->name);
            max_arrival = -1;
            ATPGCircuitElement* next = nullptr;
            for(auto input : inputs) {
                if(std::find(visited.begin(), visited.end(), input->name) == visited.end()) {
                    // not visited
                    if(input->arrival > max_arrival) {
                        max_arrival = input->arrival;
                        next = input;
                    }
                }
            }

            if(!next) {
                traverse.pop();
            } else {
                traverse.push(next);
            }
        } else { // PI node
            path.clear();
            std::stack<ATPGCircuitElement*> copy_traverse = traverse;
            while(!copy_traverse.empty()) {
                ATPGCircuitElement* element = copy_traverse.top();
                copy_traverse.pop();
                path.push_back(element->name);
            }
            bool sensitizable = path_sensitization(circ, path);
            if(sensitizable) {
                found = true;
                break;
            } else {
                traverse.pop();
            }
        }
    }

    if(found) {
        printf("Sensitizable critical path found\n");
        for(auto e : path) {
            printf("%s ", e.c_str());
        }
        printf("\n");
    } else {
        printf("There are no sensitizable critical paths available\n");
    }
}

bool d_algorithm_helper(ATPGCircuit& circ, std::vector<std::string> tried_d_frontiers, std::string activated_gate);

bool path_sensitization_helper(ATPGCircuit& circ, bool first_run) {
    auto cj_frontiers = circ.get_cj_frontiers();

    if(!first_run && circ.has_conflict("")) {
        return false;
    }

    if(first_run) {
        first_run = false;
    }

    // if fault effect reached at least one PO
    if(cj_frontiers.size() == 0) {
        return true;
    }

    // select a gate in J frontier
    ATPGCircuitElement* gate = cj_frontiers[0];

    while(true) { // gate not justified
        auto gate_inputs = circ.get_inputs(gate->name);

        // select an unassigned input of gate
        ATPGCircuitElement* unassigned_input = nullptr;
        for(auto input : gate_inputs) {
            if(input->cvalue == 'x') {
                unassigned_input = input;
                break;
            }
        }

        if(unassigned_input == nullptr) { // TODO: ??? dunno
            break;
        }

        // try to set it to 1
        unassigned_input->cvalue = '1';
        bool result = path_sensitization_helper(circ, first_run);
        if(result) {
            return true;
        } else {
            // try to set it to 0
            unassigned_input->cvalue = '0';
        }
    }

    return false;
}

bool CurrentCircuit::path_sensitization(ATPGCircuit circ, std::vector<std::string> path) {
    ///// SENSITIZATION THINGS BELOW
    printf("Trying to sensitize path: ");
    for(auto& e : path) {
        printf("%s ", e.c_str());
    }
    printf("\n");

    // sensitize path
    // initialize path wires
    for(auto& name : path) {
        auto& element = circ.get_element(name);
        if(element.type == ATPGCircuitElementType::WIRE) {
            element.cvalue = 'C';
        }
    }

    // C-drive
    auto c_frontiers = circ.get_c_frontiers();
    for(int i = 0; i < c_frontiers.size(); i++) {
        ATPGCircuitElement* gate = c_frontiers[i];

        // find non controlling value for gate
        char non_controlling_value;
        switch (gate->type)
        {
            case ATPGCircuitElementType::AND:
            case ATPGCircuitElementType::NAND:
                non_controlling_value = '1';
                break;
            case ATPGCircuitElementType::OR:
            case ATPGCircuitElementType::NOR:
                non_controlling_value = '0';
                break;
            default:
                printf("Gate type %d not supported\n", gate->type);
                return false;
        }

        // set all unassigned inputs of gate to non-controlling value
        auto gate_inputs = circ.get_inputs(gate->name);
        char fault_type;
        for(auto input : gate_inputs) {
            if(input->cvalue == 'x') {
                input->cvalue = non_controlling_value;
            } else {
                fault_type = input->cvalue;
            }
        }
        auto& gate_output = circ.get_element(gate->outputs[0]);
        fault_type = 'C';
        // if(gate->type == ATPGCircuitElementType::NAND || gate->type == ATPGCircuitElementType::NOR) {
        //     if(fault_type == 'D') {
        //         fault_type = 'E';
        //     } else {
        //         fault_type = 'D';
        //     }
        // }
        gate_output.cvalue = fault_type;
    }

    bool result = path_sensitization_helper(circ, true);
    if(!circ.has_conflict("")) {
        printf("Path is sensitizable!\n");
        printf("************************\n");
        for(auto& element : circ.elements) {
            printf("%s[%c]\n", element.name.c_str(), element.cvalue);
        }
        printf("************************\n");
        circ.print_pattern();
        return true;
    } else {
        printf("Path is NOT sensitizable!\n");
        return false;
    }
}

void topological_sort_helper(std::string element_name, bool visited[], std::stack<int>& stack) {
    Circuit& circ = CurrentCircuit::circ;
    auto& adj = circ.adjacencyList;
    bool found_element = false;
    int vertex;

    for(int i = 0; i < circ.elements.size(); i++) {
        if(circ.elements[i].elementName == element_name) {
            vertex = i;
            found_element = true;
            break;
        }
    }

    if(!found_element) {
        printf("Element name was incorrect!\n");
        return;
    }

    if(visited[vertex]) {
        return;
    }
    
    // Mark the current node as visited.
    visited[vertex] = true;

    auto& list = adj[vertex].second;
    for(int i = 0; i < list.size(); i++) {
        auto& curr = list[i];
        topological_sort_helper(curr.elementName, visited, stack);
    }

    stack.push(vertex);
}

ATPGCircuit CurrentCircuit::get_atpg_circuit() {
    Circuit& circ = CurrentCircuit::circ;
    auto& adj = circ.adjacencyList;
    std::stack<int> stack;
    int vertex_count = circ.elements.size();
    bool* visited = new bool[vertex_count];
    for (int i = 0; i < vertex_count; i++)
        visited[i] = false;
    
    for (int i = 0; i < vertex_count; i++)
        if (visited[i] == false)
            topological_sort_helper(circ.elements[i].elementName, visited, stack);
    
    std::vector<std::pair<std::string, int>> top_list {};
    std::vector<CircuitElement> top_list_circ {};

    while (stack.empty() == false) {
        int vertex = stack.top();
        auto& element = circ.elements[vertex];
        std::vector<CircuitElement> predecessor_elements;

        for(auto& pair : adj) {
            auto& second = pair.second;
            for(auto& connection : second) {
                if(connection.elementName == element.elementName) {
                    predecessor_elements.push_back(pair.first);
                }
            }
        }
        
        int max_arrival = 0;
        for(auto& predecessor : predecessor_elements) {
            int arrival_time = 0;
            for(int i = 0; i < circ.elements.size(); i++) {
                if(circ.elements[i].elementName == predecessor.elementName) {
                    arrival_time = circ.elements[i].arrivalTime;
                    break;
                }
            }
            if(arrival_time > max_arrival) {
                max_arrival = arrival_time;
            }
        }

        element.arrivalTime += max_arrival + element.delay;

        // if(element.isGate()) {
            top_list.push_back({element.elementName, element.arrivalTime});
            top_list_circ.push_back(element);
        // }
        // printf("%s(d:%d)(a:%d)\n", element.elementName.c_str(), element.delay, element.arrivalTime);
        stack.pop();
    }

    // printf("Topological order: ");
    // for(auto& element : top_list) {
    //     printf("%s[%d], ", element.first.c_str(), element.second);
    // }
    // printf("\n");

    ATPGCircuit atpg_circuit;
    for(auto& top_element : top_list) {
        CircuitElement element;
        for(auto& pair : adj) {
            if(pair.first.elementName == top_element.first) {
                element = pair.first;
                break;
            }
        }

        ATPGCircuitElement atpg_element;
        atpg_element.name = element.elementName;
        //TODO: only NAND for testing
        if(element.isGate()) {
            if(element.elementType == CircuitElementType::NAND) {
                atpg_element.type = ATPGCircuitElementType::NAND;
            }
            if(element.elementType == CircuitElementType::NOR) {
                atpg_element.type = ATPGCircuitElementType::NOR;
            }
            if(element.elementType == CircuitElementType::OR) {
                atpg_element.type = ATPGCircuitElementType::OR;
            }
            if(element.elementType == CircuitElementType::AND) {
                atpg_element.type = ATPGCircuitElementType::AND;
            }
        } else {
            atpg_element.type = ATPGCircuitElementType::WIRE;
        }
        atpg_element.delay = element.delay;
        
        // find outputs
        for(auto& pair : adj) {
            if(pair.first.elementName == top_element.first) {
                for(auto& output : pair.second) {
                    atpg_element.outputs.push_back(output.elementName);
                }
            }
        }

        // find inputs
        for(auto& pair : adj) {
            for(auto& pair_second : pair.second) {
                if(pair_second.elementName == top_element.first) {
                    atpg_element.inputs.push_back(pair.first.elementName);
                }
            }
        }
        atpg_circuit.elements.push_back(atpg_element);
    }

    // static analysis
    for(auto& element : atpg_circuit.elements) {
        std::vector<ATPGCircuitElement*> inputs = atpg_circuit.get_inputs(element.name);
        int max_arrival = 0;
        for(auto input : inputs) {
            if(input->arrival > max_arrival) {
                max_arrival = input->arrival;
            }
        }
        element.arrival = element.delay + max_arrival;
    }

    return atpg_circuit;
}

bool CurrentCircuit::d_algorithm(ATPGCircuit circ, std::string fault_element_name, char fault) {
    char gate_and_pdf[3][3] = {
        {'1', '1', 'D'},
        {'0', 'x', 'E'},
        {'x', '0', 'E'}
    };
    char gate_nand_pdf[3][3] = {
        {'1', '1', 'E'},
        {'0', 'x', 'D'},
        {'x', '0', 'D'}
    };
    char gate_or_pdf[3][3] = {
        {'0', '0', 'E'},
        {'x', '1', 'D'},
        {'1', 'x', 'D'}
    };
    char gate_nor_pdf[3][3] = {
        {'0', '0', 'D'},
        {'x', '1', 'E'},
        {'1', 'x', 'E'}
    };

    std::string activated_gate;

    for(auto& element : circ.elements) {
        if(element.name == fault_element_name) {
            element.cvalue = fault;

            auto& gate = circ.get_element(element.inputs[0]);

            std::vector<char> i0_vector;
            std::vector<char> i1_vector;

            char (*pdf_table)[3][3];
            switch (gate.type)
            {
            case ATPGCircuitElementType::AND: {
                pdf_table = &gate_and_pdf;
                break;
            }
            case ATPGCircuitElementType::NAND: {
                pdf_table = &gate_nand_pdf;
                break;
            }
            case ATPGCircuitElementType::OR: {
                pdf_table = &gate_or_pdf;
                break;
            }
            case ATPGCircuitElementType::NOR: {
                pdf_table = &gate_nor_pdf;
                break;
            }
            default:
                printf("Gate %d is not supported! 581\n", gate.type);
                return false;
            }

            for(auto row : *pdf_table) {
                if(row[2] == fault) {
                    i0_vector.push_back(row[0]);
                    i1_vector.push_back(row[1]);
                }
            }
            auto gate_inputs = circ.get_inputs(gate.name);
            // TODO: try with other possibilities?
            gate_inputs[0]->cvalue = i0_vector[0];
            gate_inputs[1]->cvalue = i1_vector[0];
            activated_gate = gate.name;
        }
    }

    bool result = d_algorithm_helper(circ, {}, activated_gate);
    printf("---- D ALG RESULT BELOW ----\n");
    printf("Tested for fault %c at %s\n", fault, fault_element_name.c_str());
    if(result) {
        // last implications
        for(auto& e : circ.elements) {
            if(e.type == ATPGCircuitElementType::WIRE) {
                continue;
            }
            std::vector<ATPGCircuitElement*> gate_inputs = circ.get_inputs(e.name);

            int unassigned_count = 0;
            for(auto input : gate_inputs) {
                if(input->cvalue == 'x') {
                    unassigned_count++;
                }
            }

            if(unassigned_count == 1) {
                std::map<std::string, std::string>* gate_map;
                switch (e.type)
                {
                case ATPGCircuitElementType::AND:
                    gate_map = &GateTables::gate_table_and;
                    break;
                case ATPGCircuitElementType::NAND:
                    gate_map = &GateTables::gate_table_nand;
                    break;
                case ATPGCircuitElementType::OR:
                    gate_map = &GateTables::gate_table_or;
                    break;
                case ATPGCircuitElementType::NOR:
                    gate_map = &GateTables::gate_table_nor;
                    break;
                default:
                    printf("Gate type %d is not supported!()\n", e.type);
                    return false;
                }
                char gate_output = circ.get_element(e.outputs[0]).cvalue;
                std::string map_output{gate_output};

                int unassigned_index = -1;
                int assigned_index = -1;
                for(int i = 0; i < gate_inputs.size(); i++) {
                    if(gate_inputs[i]->cvalue == 'x') {
                        unassigned_index = i;
                        break;
                    }
                }
                if(unassigned_index == 0) {
                    assigned_index = 1;
                } else {
                    assigned_index = 0;
                }

                // try to set it to 1
                std::string assigned_value{gate_inputs[assigned_index]->cvalue};
                std::string map_input = assigned_value + "1";
                if((*gate_map)[map_input] == map_output) {
                    gate_inputs[unassigned_index]->cvalue = '1';
                }
                // try to set it to 0
                map_input = assigned_value + "0";
                if((*gate_map)[map_input] == map_output) {
                    gate_inputs[unassigned_index]->cvalue = '0';
                }
            }
        }
        

        printf("Fault is testable!\n");
        printf("************************\n");
        for(auto& element : circ.elements) {
            printf("%s[%c]\n", element.name.c_str(), element.cvalue);
        }
        printf("************************\n");
        circ.print_pattern();
    } else {
        printf("Fault NOT testable!\n");
    }
}

bool d_algorithm_helper(ATPGCircuit& circ, std::vector<std::string> tried_d_frontiers, std::string activated_gate) {
    auto d_frontiers = circ.get_d_frontiers();
    auto j_frontiers = circ.get_j_frontiers();

    if(circ.has_conflict(activated_gate)) {
        return false;
    }

    bool d_reached_po = false;
    for(auto& element : circ.elements) {
        if(element.outputs.empty() && (element.cvalue == 'D' || element.cvalue == 'E')) {
            d_reached_po = true;
            break;
        }
    }

    if(!d_reached_po && d_frontiers.empty()) {
        return false;
    }

    if(!d_reached_po) {
        while(true) {
            ATPGCircuitElement* gate = nullptr;
            for(int i = 0; i < d_frontiers.size(); i++) {
                if(std::find(tried_d_frontiers.begin(), tried_d_frontiers.end(), d_frontiers[i]->name) != tried_d_frontiers.end()) {
                    // i is already tried
                } else {
                    gate = d_frontiers[i];
                    tried_d_frontiers.push_back(d_frontiers[i]->name);
                    break;
                }
            }

            // all d-frontiers are tried
            if(gate == nullptr) {
                break;
            }

            // find non controlling value for gate
            char non_controlling_value;
            switch (gate->type)
            {
                case ATPGCircuitElementType::AND:
                case ATPGCircuitElementType::NAND:
                    non_controlling_value = '1';
                    break;
                case ATPGCircuitElementType::OR:
                case ATPGCircuitElementType::NOR:
                    non_controlling_value = '0';
                    break;
                default:
                    printf("Gate type %d not supported\n", gate->type);
                    return false;
            }

            // set all unassigned inputs of gate to non-controlling value
            auto gate_inputs = circ.get_inputs(gate->name);
            char fault_type;
            for(auto input : gate_inputs) {
                if(input->cvalue == 'x') {
                    input->cvalue = non_controlling_value;
                } else {
                    fault_type = input->cvalue;
                }
            }
            auto& gate_output = circ.get_element(gate->outputs[0]);
            
            if(gate->type == ATPGCircuitElementType::NAND || gate->type == ATPGCircuitElementType::NOR) {
                if(fault_type == 'D') {
                    fault_type = 'E';
                } else {
                    fault_type = 'D';
                }
            }
            gate_output.cvalue = fault_type;


            bool result = d_algorithm_helper(circ, tried_d_frontiers, activated_gate);
            if(result) {
                return true;
            }
        }
        return false;
    }

    j_frontiers = circ.get_j_frontiers();
    // if fault effect reached at least one PO
    if(j_frontiers.size() == 0) {
        return true;
    }

    // select a gate in J frontier
    ATPGCircuitElement* gate = j_frontiers[0];

    auto gate_inputs = circ.get_inputs(gate->name);

    std::map<std::string, std::string>* gate_map;
    switch (gate->type)
    {
    case ATPGCircuitElementType::AND:
        gate_map = &GateTables::gate_table_and;
        break;
    case ATPGCircuitElementType::NAND:
        gate_map = &GateTables::gate_table_nand;
        break;
    case ATPGCircuitElementType::OR:
        gate_map = &GateTables::gate_table_or;
        break;
    case ATPGCircuitElementType::NOR:
        gate_map = &GateTables::gate_table_nor;
        break;
    default:
        printf("Gate type %d is not supported!()\n", gate->type);
        return false;
    }
    char gate_output = circ.get_element(gate->outputs[0]).cvalue;
    std::string map_output{gate_output};

    if((*gate_map)["11"] == map_output) {
        gate_inputs[0]->cvalue = '1';
        gate_inputs[1]->cvalue = '1';
    } else if((*gate_map)["10"] == map_output) {
        gate_inputs[0]->cvalue = '1';
        gate_inputs[1]->cvalue = '0';
    } else if((*gate_map)["01"] == map_output) {
        gate_inputs[0]->cvalue = '0';
        gate_inputs[1]->cvalue = '1';
    } else if((*gate_map)["00"] == map_output) {
        gate_inputs[0]->cvalue = '0';
        gate_inputs[1]->cvalue = '0';
    }
    return d_algorithm_helper(circ, tried_d_frontiers, activated_gate);
}
