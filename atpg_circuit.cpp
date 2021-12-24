#include "atpg_circuit.h"

ATPGCircuitElement& ATPGCircuit::get_element(std::string name) {
    for(auto& element : elements) {
        if(element.name == name) {
            return element;
        }
    }
    printf("shouldnt come here..\n");
}
