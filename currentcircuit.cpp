#include "currentcircuit.h"

#include "gate_tables.h"
#include <stack>
#include <vector>
#include "atpg_circuit.h"

Circuit CurrentCircuit::circ;

CurrentCircuit::CurrentCircuit()
{

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

void CurrentCircuit::path_sensitization(ATPGCircuit circ, std::vector<std::string> path) {
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
                return;
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
    } else {
        printf("Path is NOT sensitizable!\n");
    }


    printf("************************\n");
    for(auto& element : circ.elements) {
        printf("%s[%c]\n", element.name.c_str(), element.cvalue);
    }
    printf("************************\n");
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
        printf("Fault is testable!\n");
    } else {
        printf("Fault NOT testable!\n");
    }

    printf("************************\n");
    for(auto& element : circ.elements) {
        printf("%s[%c]\n", element.name.c_str(), element.cvalue);
    }
    printf("************************\n");
}

bool d_algorithm_helper(ATPGCircuit& circ, std::vector<std::string> tried_d_frontiers, std::string activated_gate) {
    auto d_frontiers = circ.get_d_frontiers();
    auto j_frontiers = circ.get_j_frontiers();

    if(circ.has_conflict(activated_gate) || d_frontiers.empty()) {
        return false;
    }

    bool d_reached_po = false;
    for(auto& element : circ.elements) {
        if(element.outputs.empty() && (element.cvalue == 'D' || element.cvalue == 'E')) {
            d_reached_po = true;
            break;
        }
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

    // if fault effect reached at least one PO
    if(j_frontiers.size() == 0) {
        return true;
    }

    // select a gate in J frontier
    ATPGCircuitElement* gate = j_frontiers[0];

    while(true) { // gate not justified
        auto gate_inputs = circ.get_inputs(gate->name);

        // select an unassigned input of gate
        ATPGCircuitElement* unassigned_input;
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
        bool result = d_algorithm_helper(circ, tried_d_frontiers, activated_gate);
        if(result) {
            return true;
        } else {
            // try to set it to 0
            unassigned_input->cvalue = '0';
        }
    }

    return false;
}
