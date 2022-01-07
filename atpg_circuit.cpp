#include "atpg_circuit.h"
#include "gate_tables.h"

ATPGCircuitElement& ATPGCircuit::get_element(std::string name) {
    for(auto& element : elements) {
        if(element.name == name) {
            return element;
        }
    }
    printf("shouldnt come here..\n");
}

std::vector<ATPGCircuitElement*> ATPGCircuit::get_inputs(std::string name) {
    ATPGCircuitElement& element = get_element(name);
    std::vector<ATPGCircuitElement*> inputs;
    for(std::string& input : element.inputs) {
        ATPGCircuitElement& input_element = get_element(input);
        inputs.push_back(&input_element);
    }
    return inputs;
}

// input has 1 or more D/D', output has X
std::vector<ATPGCircuitElement*> ATPGCircuit::get_d_frontiers() {
    std::vector<ATPGCircuitElement*> d_frontiers;
    for(auto& element : elements) {
        if(element.type == ATPGCircuitElementType::WIRE) {
            continue;
        }

        auto gate_inputs = get_inputs(element.name);
        auto& output = get_element(element.outputs[0]);
        if(output.cvalue != 'x') {
            continue;
        }

        for(auto input : gate_inputs) {
            if(input->cvalue == 'D' || input->cvalue == 'E') {
                d_frontiers.push_back(&get_element(element.name));
                break;
            }
        }
    }
    return d_frontiers;
}

// output value assigned, inputs not decided
std::vector<ATPGCircuitElement*> ATPGCircuit::get_j_frontiers() {
    std::vector<ATPGCircuitElement*> j_frontiers;
    for(auto& element : elements) {
        if(element.type == ATPGCircuitElementType::WIRE) {
            continue;
        }

        auto gate_inputs = get_inputs(element.name);
        auto& output = get_element(element.outputs[0]);
        if(output.cvalue == 'x') {
            continue;
        }

        bool all_x = true;
        for(auto input : gate_inputs) {
            if(input->cvalue != 'x') {
                all_x = false;
                break;
            }
        }

        if(all_x) {
            j_frontiers.push_back(&get_element(element.name));
        }
    }
    return j_frontiers;
}

// bool ATPGCircuit::justify_gate(std::string name) {
//     ATPGCircuitElement& gate = get_element(name);
//     if(gate.type == ATPGCircuitElementType::WIRE) {
//         return true;
//     }

//     // get input values
//     std::vector<ATPGCircuitElement*> inputs;
//     for(std::string& input : gate.inputs) {
//         ATPGCircuitElement& input_element = get_element(input);
//         inputs.push_back(&input_element);
//     }
//     ATPGCircuitElement& output = get_element(gate.outputs[0]);

//     switch (output.value)
//     {
//     case ATPGValue::ONE: {
//         switch (gate.type)
//         {
//         case ATPGCircuitElementType::AND: {
//             for(auto input : inputs) {
//                 if(input->value == ATPGValue::NOT_INITIALIZED) {
//                     input->value = ATPGValue::ONE;
//                 } else if(input->value != ATPGValue::ONE) {
//                     printf("fault 36\n");
//                     return false;
//                 }
//             }
//             break;
//         }
//         case ATPGCircuitElementType::NAND: {
//             bool got_zero = false;
//             int D_count = 0;
//             int D_not_count = 0;
//             for(auto input : inputs) {
//                 if(got_zero) {
//                     if(input->value == ATPGValue::NOT_INITIALIZED) {
//                         input->value = ATPGValue::DONT_CARE;
//                     }
//                 }

//                 if(input->value == ATPGValue::NOT_INITIALIZED) {
//                     input->value = ATPGValue::ZERO;
//                     got_zero = true;
//                 } else if(input->value == ATPGValue::D) {
//                     D_count++;
//                 } else if(input->value == ATPGValue::D_NOT) {
//                     D_not_count++;
//                 } else if(input->value == ATPGValue::ZERO) {
//                     got_zero = true;
//                 }
//             }
//             if(!got_zero) {
//                 if(!(D_count >= 1 && D_not_count >= 1)) { // case for D-D' inputs
//                     printf("fault 49\n");
//                     return false;    
//                 }
//             }
//             break;
//         }
//         case ATPGCircuitElementType::OR: {
//             bool got_one = false;
//             int D_count = 0;
//             int D_not_count = 0;
//             for(auto input : inputs) {
//                 if(got_one) {
//                     if(input->value == ATPGValue::NOT_INITIALIZED) {
//                         input->value = ATPGValue::DONT_CARE;
//                     }
//                 }

//                 if(input->value == ATPGValue::NOT_INITIALIZED) {
//                     input->value = ATPGValue::ONE;
//                     got_one = true;
//                 } else if(input->value == ATPGValue::D) {
//                     D_count++;
//                 } else if(input->value == ATPGValue::D_NOT) {
//                     D_not_count++;
//                 } else if(input->value == ATPGValue::ONE) {
//                     got_one = true;
//                 }
//             }
//             if(!got_one) {
//                 if(!(D_count >= 1 && D_not_count >= 1)) { // case for D-D' inputs
//                     printf("fault 94\n");
//                     return false;    
//                 }
//             }
//             break;
//         }
//         case ATPGCircuitElementType::NOR: {
//             for(auto input : inputs) {
//                 if(input->value == ATPGValue::NOT_INITIALIZED) {
//                     input->value = ATPGValue::ZERO;
//                 } else if(input->value != ATPGValue::ZERO) {
//                     printf("fault 104\n");
//                     return false;
//                 }
//             }
//             break;
//         }
//         default: {
//             printf("unsupported gate type\n");
//             break;
//         }
//         }
//         break;
//     }
    
//     case ATPGValue::ZERO: {
//         switch (gate.type)
//         {
//         case ATPGCircuitElementType::AND: {
//             bool got_zero = false;
//             int D_count = 0;
//             int D_not_count = 0;
//             for(auto input : inputs) {
//                 if(got_zero) {
//                     if(input->value == ATPGValue::NOT_INITIALIZED) {
//                         input->value = ATPGValue::DONT_CARE;
//                     }
//                 }

//                 if(input->value == ATPGValue::NOT_INITIALIZED) {
//                     input->value = ATPGValue::ZERO;
//                     got_zero = true;
//                 } else if(input->value == ATPGValue::D) {
//                     D_count++;
//                 } else if(input->value == ATPGValue::D_NOT) {
//                     D_not_count++;
//                 } else if(input->value == ATPGValue::ZERO) {
//                     got_zero = true;
//                 }
//             }
//             if(!got_zero) {
//                 if(!(D_count >= 1 && D_not_count >= 1)) { // case for D-D' inputs
//                     printf("fault 49\n");
//                     return false;    
//                 }
//             }
//             break;
//         }
//         case ATPGCircuitElementType::NAND: {
//             for(auto input : inputs) {
//                 if(input->value == ATPGValue::NOT_INITIALIZED) {
//                     input->value = ATPGValue::ONE;
//                 } else if(input->value != ATPGValue::ONE) {
//                     printf("fault 36\n");
//                     return false;
//                 }
//             }
//             break;
//         }
//         case ATPGCircuitElementType::OR: {
//             for(auto input : inputs) {
//                 if(input->value == ATPGValue::NOT_INITIALIZED) {
//                     input->value = ATPGValue::ZERO;
//                 } else if(input->value != ATPGValue::ZERO) {
//                     printf("fault 104\n");
//                     return false;
//                 }
//             }
//             break;
//         }
//         case ATPGCircuitElementType::NOR: {
//             bool got_one = false;
//             int D_count = 0;
//             int D_not_count = 0;
//             for(auto input : inputs) {
//                 if(got_one) {
//                     if(input->value == ATPGValue::NOT_INITIALIZED) {
//                         input->value = ATPGValue::DONT_CARE;
//                     }
//                 }

//                 if(input->value == ATPGValue::NOT_INITIALIZED) {
//                     input->value = ATPGValue::ONE;
//                     got_one = true;
//                 } else if(input->value == ATPGValue::D) {
//                     D_count++;
//                 } else if(input->value == ATPGValue::D_NOT) {
//                     D_not_count++;
//                 } else if(input->value == ATPGValue::ONE) {
//                     got_one = true;
//                 }
//             }
//             if(!got_one) {
//                 if(!(D_count >= 1 && D_not_count >= 1)) { // case for D-D' inputs
//                     printf("fault 94\n");
//                     return false;    
//                 }
//             }
//             break;
//         }
//         default: {
//             printf("unsupported gate type\n");
//             break;
//         }
//         }
//         break;
//     }
//     case ATPGValue::D:
//     case ATPGValue::D_NOT:
//     case ATPGValue::DONT_CARE:
//     case ATPGValue::NOT_INITIALIZED:
//     case ATPGValue::CRIT_PATH:
//     break;
//     }

//     switch (gate.type)
//     {
//     case ATPGCircuitElementType::AND:
//     case ATPGCircuitElementType::NAND:
//     case ATPGCircuitElementType::OR:
//     case ATPGCircuitElementType::NOR:
//     }
// }

bool ATPGCircuit::has_conflict(std::string exclude_gate) {
    for(auto& element : elements) {
        if(element.type == ATPGCircuitElementType::WIRE) {
            continue;
        }
        if(element.name == exclude_gate) {
            continue;
        }

        auto inputs = get_inputs(element.name);
        auto& output = get_element(element.outputs[0]);

        std::string i0{inputs[0]->cvalue};
        std::string i1{inputs[1]->cvalue};
        std::string map_output{output.cvalue};
        std::string map_input = i0 + i1;

        std::string map_value;
        switch (element.type)
        {
        case ATPGCircuitElementType::AND:
            map_value = GateTables::gate_table_and[map_input];
            break;
        case ATPGCircuitElementType::NAND:
            // todo change this
            map_value = GateTables::gate_table_and[map_input];
            break;
        case ATPGCircuitElementType::OR:
        case ATPGCircuitElementType::NOR:
        default:
            printf("Gate type %d is not supported! has_conflict()\n", element.type);
            return true;
        }

        if(map_value != map_output) {
            return true;
        }
    }
    return false;
}
