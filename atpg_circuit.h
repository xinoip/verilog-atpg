#ifndef ATPG_CIRCUIT_H
#define ATPG_CIRCUIT_H

#include <vector>
#include "atpg_circuit_element.h"

struct ATPGStep {
    std::string name;
    std::string subject;
    char value;
    bool alternative_tried;
};

class ATPGCircuit {
public:
    std::vector<ATPGCircuitElement> elements;

    ATPGCircuitElement& get_element(std::string name);
    std::vector<ATPGCircuitElement*> get_inputs(std::string name);

    bool justify_gate(std::string name);

    std::vector<ATPGCircuitElement*> get_d_frontiers();
    std::vector<ATPGCircuitElement*> get_j_frontiers();
    std::vector<ATPGCircuitElement*> get_c_frontiers();
    std::vector<ATPGCircuitElement*> get_cj_frontiers();

    bool has_conflict(std::string exclude_gate);
};

#endif
