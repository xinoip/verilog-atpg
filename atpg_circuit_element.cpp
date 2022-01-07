#include "atpg_circuit_element.h"

std::string ATPGCircuitElement::value_to_string() {
    switch (value)
    {
    case ATPGValue::D:
        return "D";
    case ATPGValue::D_NOT:
        return "E";
    case ATPGValue::DONT_CARE:
        return "X";
    case ATPGValue::NOT_INITIALIZED:
        return "?";
    case ATPGValue::ONE:
        return "1";
    case ATPGValue::ZERO:
        return "0";
    default:
        printf("should not reach 19 value\n");
        return "F";
    }
}

void ATPGCircuitElement::string_to_value(std::string str) {
    if(str == "D") {
        value = ATPGValue::D;
    } else if(str == "E") {
        value = ATPGValue::D_NOT;
    } else if(str == "X") {
        value = ATPGValue::DONT_CARE;
    } else if(str == "?") {
        value = ATPGValue::NOT_INITIALIZED;
    } else if(str == "1") {
        value = ATPGValue::ONE;
    } else if(str == "0") {
        value = ATPGValue::ZERO;
    } else {
        printf("should not reach 38 value\n");
    }
}
