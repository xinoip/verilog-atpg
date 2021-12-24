#ifndef ATPG_CIRCUIT_H
#define ATPG_CIRCUIT_H

#include <vector>
#include "atpg_circuit_element.h"

class ATPGCircuit {
public:
    std::vector<ATPGCircuitElement> elements;

    ATPGCircuitElement& get_element(std::string name);
};

#endif
