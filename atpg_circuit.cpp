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

// input and output has C
std::vector<ATPGCircuitElement*> ATPGCircuit::get_c_frontiers() {
    std::vector<ATPGCircuitElement*> c_frontiers;
    for(auto& element : elements) {
        if(element.type == ATPGCircuitElementType::WIRE) {
            continue;
        }

        auto gate_inputs = get_inputs(element.name);
        auto& output = get_element(element.outputs[0]);
        if(output.cvalue != 'C') {
            continue;
        }

        for(auto input : gate_inputs) {
            if(input->cvalue == 'C') {
                c_frontiers.push_back(&get_element(element.name));
                break;
            }
        }
    }
    return c_frontiers;
}

// output known, 1 or more inputs unknown
std::vector<ATPGCircuitElement*> ATPGCircuit::get_cj_frontiers() {
    std::vector<ATPGCircuitElement*> cj_frontiers;
    for(auto& element : elements) {
        if(element.type == ATPGCircuitElementType::WIRE) {
            continue;
        }

        auto gate_inputs = get_inputs(element.name);
        auto& output = get_element(element.outputs[0]);
        if(output.cvalue == 'x') {
            continue;
        }

        bool one_x = false;
        for(auto input : gate_inputs) {
            if(input->cvalue == 'x') {
                one_x = true;
                break;
            }
        }

        if(one_x) {
            cj_frontiers.push_back(&get_element(element.name));
        }
    }
    return cj_frontiers;
}

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
            map_value = GateTables::gate_table_nand[map_input];
            break;
        case ATPGCircuitElementType::OR:
            map_value = GateTables::gate_table_or[map_input];
            break;
        case ATPGCircuitElementType::NOR:
            map_value = GateTables::gate_table_nor[map_input];
            break;
        default:
            printf("Gate type %d is not supported! has_conflict()\n", element.type);
            return true;
        }

        if(map_input != "xx") {
            if(map_output != "x" && map_value != map_output) {
                return true;
            }
        }
    }
    return false;
}

std::vector<ATPGCircuitElement*> ATPGCircuit::get_pos() {
    std::vector<ATPGCircuitElement*> pos;
    for(auto& e : elements) {
        if(e.outputs.size() == 0) {
            pos.push_back(&e);
        }
    }
    return pos;
}
std::vector<ATPGCircuitElement*> ATPGCircuit::get_pis() {
    std::vector<ATPGCircuitElement*> pis;
    for(auto& e : elements) {
        if(e.inputs.size() == 0) {
            pis.push_back(&e);
        }
    }
    return pis;
}
