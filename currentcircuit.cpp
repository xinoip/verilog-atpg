#include "currentcircuit.h"

#include <stack>
#include <vector>
#include "atpg_circuit.h"

Circuit CurrentCircuit::circ;

CurrentCircuit::CurrentCircuit()
{

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
            atpg_element.type = ATPGCircuitElementType::NAND;
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
    for(auto& name : crit_path_names) {
        auto& element = atpg_circuit.get_element(name);
        if(element.type == ATPGCircuitElementType::WIRE) {
            element.value = ATPGValue::CRIT_PATH;
        }
    }

    // set offset to non-controlling values on crit path
    for(auto& name : crit_path_names) {
        auto& element = atpg_circuit.get_element(name);
        if(element.type == ATPGCircuitElementType::WIRE) {
            continue;
        }
        // get input values
        std::vector<ATPGCircuitElement*> inputs;
        for(auto& input : element.inputs) {
            auto& input_element = atpg_circuit.get_element(input);
            inputs.push_back(&input_element);
        }

        if(inputs.size() != 2) {
            printf("Only 2 input gates supported!\n");
            break;
        }

        switch (element.type)
        {
        case ATPGCircuitElementType::NAND:
        case ATPGCircuitElementType::AND:
            if(inputs[0]->value == ATPGValue::CRIT_PATH) {
                if(inputs[1]->value == ATPGValue::NOT_INITIALIZED) {
                    inputs[1]->value = ATPGValue::ONE;
                } else if(inputs[1]->value != ATPGValue::ONE) {
                    printf("FAULT 248\n");
                }
            } else if(inputs[1]->value == ATPGValue::CRIT_PATH) {
                if(inputs[0]->value == ATPGValue::NOT_INITIALIZED) {
                    inputs[0]->value = ATPGValue::ONE;
                } else if(inputs[0]->value != ATPGValue::ONE) {
                    printf("FAULT 255\n");
                }
            }
            break;
        case ATPGCircuitElementType::OR:
        case ATPGCircuitElementType::NOR:
            if(inputs[0]->value == ATPGValue::CRIT_PATH) {
                if(inputs[1]->value == ATPGValue::NOT_INITIALIZED) {
                    inputs[1]->value = ATPGValue::ZERO;
                } else if(inputs[1]->value != ATPGValue::ZERO) {
                    printf("FAULT 265\n");
                }
            } else if(inputs[1]->value == ATPGValue::CRIT_PATH) {
                if(inputs[0]->value == ATPGValue::NOT_INITIALIZED) {
                    inputs[0]->value = ATPGValue::ZERO;
                } else if(inputs[0]->value != ATPGValue::ZERO) {
                    printf("FAULT 271\n");
                }
            }
            break;
        default:
            printf("Not supported gate: %d\n", element.type);
            break;
        }
    }

    // justify rest
    for(auto& element : atpg_circuit.elements) {
        if(element.type == ATPGCircuitElementType::WIRE) {
            continue;
        }

        // get input values
        std::vector<ATPGCircuitElement*> inputs;
        bool has_not_initialized = false;
        for(auto& input : element.inputs) {
            auto& input_element = atpg_circuit.get_element(input);
            if(input_element.value == ATPGValue::NOT_INITIALIZED) {
                has_not_initialized = true;
            }
            inputs.push_back(&input_element);
        }
        ATPGCircuitElement& output = atpg_circuit.get_element(element.outputs[0]);
        if(output.value == ATPGValue::NOT_INITIALIZED) {
            has_not_initialized = true;
        }

        // this gate is already done
        if(!has_not_initialized) {
            continue;
        }

        if(inputs.size() != 2) {
            printf("Only 2 input gates supported!\n");
            break;
        }

        if(output.value == ATPGValue::NOT_INITIALIZED) {
            switch (element.type)
            {
            case ATPGCircuitElementType::NAND:
                if(inputs[0]->value == ATPGValue::CRIT_PATH || inputs[1]->value == ATPGValue::CRIT_PATH) {
                    if(inputs[0]->value == ATPGValue::NOT_INITIALIZED) {
                        inputs[0]->value = ATPGValue::ZERO;
                    }
                    if(inputs[1]->value == ATPGValue::NOT_INITIALIZED) {
                        inputs[1]->value = ATPGValue::ZERO;
                    }
                    output.value = ATPGValue::ONE;
                } else if(inputs[0]->value == ATPGValue::ZERO || inputs[1]->value == ATPGValue::ZERO) {
                    if(inputs[0]->value == ATPGValue::NOT_INITIALIZED) {
                        inputs[0]->value = ATPGValue::DONT_CARE;
                    }
                    if(inputs[1]->value == ATPGValue::NOT_INITIALIZED) {
                        inputs[1]->value = ATPGValue::DONT_CARE;
                    }
                    output.value = ATPGValue::ONE;
                } else if(inputs[0]->value == ATPGValue::ONE && inputs[1]->value == ATPGValue::ONE) {
                    output.value = ATPGValue::ZERO;
                } else {
                    output.value = ATPGValue::DONT_CARE;
                }
                break;
            case ATPGCircuitElementType::NOR:
                if(inputs[0]->value == ATPGValue::CRIT_PATH || inputs[1]->value == ATPGValue::CRIT_PATH) {
                    if(inputs[0]->value == ATPGValue::NOT_INITIALIZED) {
                        inputs[0]->value = ATPGValue::ONE;
                    }
                    if(inputs[1]->value == ATPGValue::NOT_INITIALIZED) {
                        inputs[1]->value = ATPGValue::ONE;
                    }
                    output.value = ATPGValue::ZERO;
                } else if(inputs[0]->value == ATPGValue::ONE || inputs[1]->value == ATPGValue::ONE) {
                    if(inputs[0]->value == ATPGValue::NOT_INITIALIZED) {
                        inputs[0]->value = ATPGValue::DONT_CARE;
                    }
                    if(inputs[1]->value == ATPGValue::NOT_INITIALIZED) {
                        inputs[1]->value = ATPGValue::DONT_CARE;
                    }
                    output.value = ATPGValue::ZERO;
                } else if(inputs[0]->value == ATPGValue::ZERO && inputs[1]->value == ATPGValue::ZERO) {
                    output.value = ATPGValue::ONE;
                } else {
                    output.value = ATPGValue::DONT_CARE;
                }
                break;
            default:
                printf("Not supported gate: %d\n", element.type);
                break;
            }
        } else if(output.value == ATPGValue::DONT_CARE) {
            // for(auto& value : input_values) {
            //     if(value == ATPGValue::NOT_INITIALIZED) {
            //         value = ATPGValue::DONT_CARE;
            //     }
            // }
        } else if(output.value == ATPGValue::ONE) {
            switch (element.type)
            {
            case ATPGCircuitElementType::NAND:
                if(inputs[0]->value == ATPGValue::ZERO || inputs[1]->value == ATPGValue::ZERO) {
                    if(inputs[0]->value == ATPGValue::NOT_INITIALIZED) {
                        inputs[0]->value = ATPGValue::DONT_CARE;
                    }
                    if(inputs[1]->value == ATPGValue::NOT_INITIALIZED) {
                        inputs[1]->value = ATPGValue::DONT_CARE;
                    }
                } else if(inputs[0]->value == ATPGValue::NOT_INITIALIZED && inputs[1]->value == ATPGValue::NOT_INITIALIZED) {
                    inputs[0]->value = ATPGValue::ZERO;
                    inputs[1]->value = ATPGValue::DONT_CARE;
                } else if(inputs[0]->value == ATPGValue::NOT_INITIALIZED) {
                    inputs[0]->value = ATPGValue::ZERO;
                } else if(inputs[1]->value == ATPGValue::NOT_INITIALIZED) {
                    inputs[1]->value = ATPGValue::ZERO;
                }
                break;
            case ATPGCircuitElementType::NOR:
                inputs[0]->value = ATPGValue::ZERO;
                inputs[1]->value = ATPGValue::ZERO;
                break;
            default:
                printf("Not supported gate: %d\n", element.type);
                break;
            }
        } else if(output.value == ATPGValue::ZERO) {
            switch (element.type)
            {
            case ATPGCircuitElementType::NAND:
                inputs[0]->value = ATPGValue::ONE;
                inputs[1]->value = ATPGValue::ONE;
                break;
            case ATPGCircuitElementType::NOR:
                if(inputs[0]->value == ATPGValue::ONE || inputs[1]->value == ATPGValue::ONE) {
                    if(inputs[0]->value == ATPGValue::NOT_INITIALIZED) {
                        inputs[0]->value = ATPGValue::DONT_CARE;
                    }
                    if(inputs[1]->value == ATPGValue::NOT_INITIALIZED) {
                        inputs[1]->value = ATPGValue::DONT_CARE;
                    }
                } else if(inputs[0]->value == ATPGValue::NOT_INITIALIZED && inputs[1]->value == ATPGValue::NOT_INITIALIZED) {
                    inputs[0]->value = ATPGValue::ONE;
                    inputs[1]->value = ATPGValue::DONT_CARE;
                } else if(inputs[0]->value == ATPGValue::NOT_INITIALIZED) {
                    inputs[0]->value = ATPGValue::ONE;
                } else if(inputs[1]->value == ATPGValue::NOT_INITIALIZED) {
                    inputs[1]->value = ATPGValue::ONE;
                }
                break;
            default:
                printf("Not supported gate: %d\n", element.type);
                break;
            }
        }
    }

    printf("************************\n");
    for(auto& element : atpg_circuit.elements) {
        printf("%s[%d]\n", element.name.c_str(), element.value);
    }
    printf("************************\n");
}
