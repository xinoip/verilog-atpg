#ifndef ATPG_CIRCUIT_ELEMENT_H
#define ATPG_CIRCUIT_ELEMENT_H

#include <vector>
#include <string>

enum class ATPGCircuitElementType {
    WIRE,
    AND,
    NAND,
    OR,
    NOR
};

enum class ATPGValue {
    ZERO,
    ONE,
    DONT_CARE,
    CRIT_PATH,
    NOT_INITIALIZED
};

class ATPGCircuitElement {
public:
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;
    ATPGCircuitElementType type;
    std::string name;
    ATPGValue value = ATPGValue::NOT_INITIALIZED;
};

#endif
