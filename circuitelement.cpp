#include "circuitelement.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include "delay_file.h"
#include "currentcircuit.h"

CircuitElement::CircuitElement(){
    elementName = "";
    delay = 0;
    arrivalTime = 0;
}
CircuitElement::CircuitElement(std::string name, CircuitElementType type, std::string subcircuitType)
{
    DelayFile delay_file{CurrentCircuit::delay_file_name};
    elementName = name;
    elementType = type;
    this->subcircuitType = subcircuitType;
    delay = delay_file.delay_of_element(name);
    arrivalTime = 0;
}
bool CircuitElement::operator==(const CircuitElement& other)
{
    if (this->elementName == other.elementName && this->elementType == other.elementType)
        return true;
    return false;
}

bool CircuitElement::isGate(){
    if(this->elementType == CircuitElementType::WIRE || this->elementType == CircuitElementType::INPUT || this->elementType == CircuitElementType::OUTPUT || this->elementType == CircuitElementType::SUBCIRCUIT){
        return false;
    }
    else{
        return true;
    }
}
