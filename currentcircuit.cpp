#include "currentcircuit.h"

#include "gate_tables.h"
#include <stack>
#include <vector>
#include "atpg_circuit.h"

Circuit CurrentCircuit::circ;

CurrentCircuit::CurrentCircuit()
{

}

bool d_algorithm(ATPGCircuit circ, std::string fault_element_name, char fault);

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

void CurrentCircuit::topological_sort() {
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

    printf("Topological order: ");
    for(auto& element : top_list) {
        printf("%s[%d], ", element.first.c_str(), element.second);
    }
    printf("\n");

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

    printf("Critical path:\n");
    int slowest_output_index = 0;
    for(int i = 0, current_max = 0; i < top_list.size(); i++) {
        auto& element = top_list[i];
        if(element.second > current_max) {
            current_max = element.second;
            slowest_output_index = i;
        }
    }
    std::vector<std::string> crit_path_names;
    crit_path_names.push_back(top_list[slowest_output_index].first);
    printf("%s[%d]\n", top_list[slowest_output_index].first.c_str(), top_list[slowest_output_index].second);
    std::vector<CircuitElement> crit_path_list {};
    while(true) {
        std::vector<CircuitElement> predecessor_elements;
        // get pred elements
        for(auto& pair : adj) {
            auto& second = pair.second;
            for(auto& connection : second) {
                if(connection.elementName == top_list[slowest_output_index].first) {
                    predecessor_elements.push_back(pair.first);
                }
            }
        }
        if(predecessor_elements.empty()) {
            break;
        }
        // hack for dirty ugly datastructure to get arrival times..
        for(auto& pred : predecessor_elements) {
            for(auto& el : circ.elements) {
                if(el.elementName == pred.elementName) {
                    pred.arrivalTime = el.arrivalTime;
                    break;
                }
            }
        }
        // determine slowest pred
        int slowest_pred_index = 0;
        int slowest_pred_arrival_time = 0;
        for(int i = 0; i < predecessor_elements.size(); i++) {
            auto& pred = predecessor_elements[i];
            if(pred.arrivalTime > slowest_pred_arrival_time) {
                slowest_pred_arrival_time = pred.arrivalTime;
                slowest_pred_index = i;
            }
        }
        // find slowest pred in topological sorted list
        CircuitElement& slowest_pred = predecessor_elements[slowest_pred_index];
        int found_index = -1;
        for(int i = 0; i < top_list.size(); i++) {
            if(top_list[i].first == slowest_pred.elementName) {
                found_index = i;
                break; 
            }
        }
        crit_path_list.push_back(slowest_pred);
        // printf("%s[%d] \n", slowest_pred.elementName.c_str(), slowest_pred.arrivalTime);
        slowest_output_index = found_index;
    }
    // print critical path
    for(auto& element : crit_path_list) {
        printf("%s[%d] \n", element.elementName.c_str(), element.arrivalTime);
        crit_path_names.push_back(element.elementName);
    }
    printf("\n");

    ///// SENSITIZATION THINGS BELOW

    // sensitize path
    // initialize crit path wires
    // for(auto& name : crit_path_names) {
    //     auto& element = atpg_circuit.get_element(name);
    //     if(element.type == ATPGCircuitElementType::WIRE) {
    //         element.value = ATPGValue::CRIT_PATH;
    //     }
    // }

    // set offset to non-controlling values on crit path
    // for(auto& name : crit_path_names) {
    //     auto& element = atpg_circuit.get_element(name);
    //     if(element.type == ATPGCircuitElementType::WIRE) {
    //         continue;
    //     }
    //     // get input values
    //     std::vector<ATPGCircuitElement*> inputs;
    //     for(auto& input : element.inputs) {
    //         auto& input_element = atpg_circuit.get_element(input);
    //         inputs.push_back(&input_element);
    //     }

    //     if(inputs.size() != 2) {
    //         printf("Only 2 input gates supported!\n");
    //         break;
    //     }

    //     switch (element.type)
    //     {
    //     case ATPGCircuitElementType::NAND:
    //     case ATPGCircuitElementType::AND:
    //         if(inputs[0]->value == ATPGValue::CRIT_PATH) {
    //             if(inputs[1]->value == ATPGValue::NOT_INITIALIZED) {
    //                 inputs[1]->value = ATPGValue::ONE;
    //             } else if(inputs[1]->value != ATPGValue::ONE) {
    //                 printf("FAULT 248\n");
    //             }
    //         } else if(inputs[1]->value == ATPGValue::CRIT_PATH) {
    //             if(inputs[0]->value == ATPGValue::NOT_INITIALIZED) {
    //                 inputs[0]->value = ATPGValue::ONE;
    //             } else if(inputs[0]->value != ATPGValue::ONE) {
    //                 printf("FAULT 255\n");
    //             }
    //         }
    //         break;
    //     case ATPGCircuitElementType::OR:
    //     case ATPGCircuitElementType::NOR:
    //         if(inputs[0]->value == ATPGValue::CRIT_PATH) {
    //             if(inputs[1]->value == ATPGValue::NOT_INITIALIZED) {
    //                 inputs[1]->value = ATPGValue::ZERO;
    //             } else if(inputs[1]->value != ATPGValue::ZERO) {
    //                 printf("FAULT 265\n");
    //             }
    //         } else if(inputs[1]->value == ATPGValue::CRIT_PATH) {
    //             if(inputs[0]->value == ATPGValue::NOT_INITIALIZED) {
    //                 inputs[0]->value = ATPGValue::ZERO;
    //             } else if(inputs[0]->value != ATPGValue::ZERO) {
    //                 printf("FAULT 271\n");
    //             }
    //         }
    //         break;
    //     default:
    //         printf("Not supported gate: %d\n", element.type);
    //         break;
    //     }
    // }

    // // justify rest
    // for(auto& element : atpg_circuit.elements) {
    //     if(element.type == ATPGCircuitElementType::WIRE) {
    //         continue;
    //     }

    //     // get input values
    //     std::vector<ATPGCircuitElement*> inputs;
    //     bool has_not_initialized = false;
    //     for(auto& input : element.inputs) {
    //         auto& input_element = atpg_circuit.get_element(input);
    //         if(input_element.value == ATPGValue::NOT_INITIALIZED) {
    //             has_not_initialized = true;
    //         }
    //         inputs.push_back(&input_element);
    //     }
    //     ATPGCircuitElement& output = atpg_circuit.get_element(element.outputs[0]);
    //     if(output.value == ATPGValue::NOT_INITIALIZED) {
    //         has_not_initialized = true;
    //     }

    //     // this gate is already done
    //     if(!has_not_initialized) {
    //         continue;
    //     }

    //     if(inputs.size() != 2) {
    //         printf("Only 2 input gates supported!\n");
    //         break;
    //     }

    //     if(output.value == ATPGValue::NOT_INITIALIZED) {
    //         switch (element.type)
    //         {
    //         case ATPGCircuitElementType::NAND:
    //             if(inputs[0]->value == ATPGValue::CRIT_PATH || inputs[1]->value == ATPGValue::CRIT_PATH) {
    //                 if(inputs[0]->value == ATPGValue::NOT_INITIALIZED) {
    //                     inputs[0]->value = ATPGValue::ZERO;
    //                 }
    //                 if(inputs[1]->value == ATPGValue::NOT_INITIALIZED) {
    //                     inputs[1]->value = ATPGValue::ZERO;
    //                 }
    //                 output.value = ATPGValue::ONE;
    //             } else if(inputs[0]->value == ATPGValue::ZERO || inputs[1]->value == ATPGValue::ZERO) {
    //                 if(inputs[0]->value == ATPGValue::NOT_INITIALIZED) {
    //                     inputs[0]->value = ATPGValue::DONT_CARE;
    //                 }
    //                 if(inputs[1]->value == ATPGValue::NOT_INITIALIZED) {
    //                     inputs[1]->value = ATPGValue::DONT_CARE;
    //                 }
    //                 output.value = ATPGValue::ONE;
    //             } else if(inputs[0]->value == ATPGValue::ONE && inputs[1]->value == ATPGValue::ONE) {
    //                 output.value = ATPGValue::ZERO;
    //             } else {
    //                 output.value = ATPGValue::DONT_CARE;
    //             }
    //             break;
    //         case ATPGCircuitElementType::NOR:
    //             if(inputs[0]->value == ATPGValue::CRIT_PATH || inputs[1]->value == ATPGValue::CRIT_PATH) {
    //                 if(inputs[0]->value == ATPGValue::NOT_INITIALIZED) {
    //                     inputs[0]->value = ATPGValue::ONE;
    //                 }
    //                 if(inputs[1]->value == ATPGValue::NOT_INITIALIZED) {
    //                     inputs[1]->value = ATPGValue::ONE;
    //                 }
    //                 output.value = ATPGValue::ZERO;
    //             } else if(inputs[0]->value == ATPGValue::ONE || inputs[1]->value == ATPGValue::ONE) {
    //                 if(inputs[0]->value == ATPGValue::NOT_INITIALIZED) {
    //                     inputs[0]->value = ATPGValue::DONT_CARE;
    //                 }
    //                 if(inputs[1]->value == ATPGValue::NOT_INITIALIZED) {
    //                     inputs[1]->value = ATPGValue::DONT_CARE;
    //                 }
    //                 output.value = ATPGValue::ZERO;
    //             } else if(inputs[0]->value == ATPGValue::ZERO && inputs[1]->value == ATPGValue::ZERO) {
    //                 output.value = ATPGValue::ONE;
    //             } else {
    //                 output.value = ATPGValue::DONT_CARE;
    //             }
    //             break;
    //         default:
    //             printf("Not supported gate: %d\n", element.type);
    //             break;
    //         }
    //     } else if(output.value == ATPGValue::DONT_CARE) {
    //         // for(auto& value : input_values) {
    //         //     if(value == ATPGValue::NOT_INITIALIZED) {
    //         //         value = ATPGValue::DONT_CARE;
    //         //     }
    //         // }
    //     } else if(output.value == ATPGValue::ONE) {
    //         switch (element.type)
    //         {
    //         case ATPGCircuitElementType::NAND:
    //             if(inputs[0]->value == ATPGValue::ZERO || inputs[1]->value == ATPGValue::ZERO) {
    //                 if(inputs[0]->value == ATPGValue::NOT_INITIALIZED) {
    //                     inputs[0]->value = ATPGValue::DONT_CARE;
    //                 }
    //                 if(inputs[1]->value == ATPGValue::NOT_INITIALIZED) {
    //                     inputs[1]->value = ATPGValue::DONT_CARE;
    //                 }
    //             } else if(inputs[0]->value == ATPGValue::NOT_INITIALIZED && inputs[1]->value == ATPGValue::NOT_INITIALIZED) {
    //                 inputs[0]->value = ATPGValue::ZERO;
    //                 inputs[1]->value = ATPGValue::DONT_CARE;
    //             } else if(inputs[0]->value == ATPGValue::NOT_INITIALIZED) {
    //                 inputs[0]->value = ATPGValue::ZERO;
    //             } else if(inputs[1]->value == ATPGValue::NOT_INITIALIZED) {
    //                 inputs[1]->value = ATPGValue::ZERO;
    //             }
    //             break;
    //         case ATPGCircuitElementType::NOR:
    //             inputs[0]->value = ATPGValue::ZERO;
    //             inputs[1]->value = ATPGValue::ZERO;
    //             break;
    //         default:
    //             printf("Not supported gate: %d\n", element.type);
    //             break;
    //         }
    //     } else if(output.value == ATPGValue::ZERO) {
    //         switch (element.type)
    //         {
    //         case ATPGCircuitElementType::NAND:
    //             inputs[0]->value = ATPGValue::ONE;
    //             inputs[1]->value = ATPGValue::ONE;
    //             break;
    //         case ATPGCircuitElementType::NOR:
    //             if(inputs[0]->value == ATPGValue::ONE || inputs[1]->value == ATPGValue::ONE) {
    //                 if(inputs[0]->value == ATPGValue::NOT_INITIALIZED) {
    //                     inputs[0]->value = ATPGValue::DONT_CARE;
    //                 }
    //                 if(inputs[1]->value == ATPGValue::NOT_INITIALIZED) {
    //                     inputs[1]->value = ATPGValue::DONT_CARE;
    //                 }
    //             } else if(inputs[0]->value == ATPGValue::NOT_INITIALIZED && inputs[1]->value == ATPGValue::NOT_INITIALIZED) {
    //                 inputs[0]->value = ATPGValue::ONE;
    //                 inputs[1]->value = ATPGValue::DONT_CARE;
    //             } else if(inputs[0]->value == ATPGValue::NOT_INITIALIZED) {
    //                 inputs[0]->value = ATPGValue::ONE;
    //             } else if(inputs[1]->value == ATPGValue::NOT_INITIALIZED) {
    //                 inputs[1]->value = ATPGValue::ONE;
    //             }
    //             break;
    //         default:
    //             printf("Not supported gate: %d\n", element.type);
    //             break;
    //         }
    //     }
    // }

    // printf("************************\n");
    // for(auto& element : atpg_circuit.elements) {
    //     printf("%s[%d]\n", element.name.c_str(), element.value);
    // }
    // printf("************************\n");


    //////////////// run D algorithm

    // d frontiers: has input D-D'
    // j frontiers: output known, input unknown

    d_algorithm(atpg_circuit, "N30", 'D');

    // std::string fault_element_name = "N11";
    // std::stack<ATPGStep> atpg_step_stack;

    // auto d_frontier_names = atpg_circuit.get_d_frontiers();

    // end condition
    // if(d_frontier_names.empty()) {
    //     bool d_reached_po = false;
    //     for(auto& element : atpg_circuit.elements) {
    //         if(element.outputs.empty() && (element.cvalue == 'D' || element.cvalue == 'E')) {
    //             d_reached_po = true;
    //             break;
    //         }
    //     }
    //     if(d_reached_po) {
    //         printf("Fault is tested, printing inputs...\n");
    //     } else {
    //         printf("Fault not testable!\n");
    //     }
    // }

    // // PDF
    // for(auto& element : atpg_circuit.elements) {
    //     if(element.name == fault_element_name) {
    //         element.cvalue = 'D';
    //         atpg_step_stack.push({"SET", element.name, 'D'});

    //         auto& gate = atpg_circuit.get_element(element.inputs[0]);

    //         // TODO: gate types
    //         std::vector<char> i0_vector;
    //         std::vector<char> i1_vector;
    //         if(gate.type == ATPGCircuitElementType::NAND) {
    //             for(auto row : gate_and_pdf) {
    //                 if(row[2] == 'D') {
    //                     i0_vector.push_back(row[0]);
    //                     i1_vector.push_back(row[1]);
    //                 }
    //             }
    //         }
    //         auto gate_inputs = atpg_circuit.get_inputs(gate.name);
    //         if(i0_vector.size() == 1) {
    //             gate_inputs[0]->cvalue = i0_vector[0];
    //             gate_inputs[1]->cvalue = i1_vector[0];
    //             atpg_step_stack.push({"SET", gate_inputs[0]->name, i0_vector[0]});
    //             atpg_step_stack.push({"SET", gate_inputs[1]->name, i1_vector[0]});
    //         } else {

    //         }
    //     }
    // }

    // // d-drive PDC
    // // get d-frontiers
    // std::vector<std::string> d_frontiers = atpg_circuit.get_d_frontiers();

    // if(d_frontiers.size() == 1) {
    //     auto& d_propagate = atpg_circuit.get_element(d_frontiers[0]);
    //     auto d_propagate_inputs = atpg_circuit.get_inputs(d_propagate.name);
    //     // TODO: gate types
    //     if(d_propagate.type == ATPGCircuitElementType::NAND) {
    //         char i0 = d_propagate_inputs[0]->cvalue; // D
    //         char i1 = d_propagate_inputs[1]->cvalue; // x
    //         for(auto row : gate_and_pdc) {
    //             if(row[2] == 'D') {
    //                 if(i0 == row[0]) {
    //                     i1 = row[1];
    //                     break;
    //                 } else if(i1 == row[1]) {
    //                     i0 = row[0];
    //                     break;
    //                 }
    //             }
    //         }
    //         d_propagate_inputs[0]->cvalue = i0;
    //         d_propagate_inputs[1]->cvalue = i1;
    //         implication_stack.push({d_propagate_inputs[0]->name, i0, false});
    //         implication_stack.push({d_propagate_inputs[1]->name, i1, true});
    //     }
    // }

    // fault activation


}

bool d_algorithm_helper(ATPGCircuit& circ, std::vector<std::string> tried_d_frontiers, std::string activated_gate);
bool d_algorithm(ATPGCircuit circ, std::string fault_element_name, char fault) {
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
    printf("Tested for fault %c at %s\n", 'D', fault_element_name.c_str());
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

    // if(d_frontiers.empty()) {
    //     if(d_reached_po) {
    //         return true;
    //     } else {
    //         return false;
    //     }
    // }

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
