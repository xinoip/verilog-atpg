#ifndef CIRCUITELEMENT_H
#define CIRCUITELEMENT_H

#include <QGraphicsItem>
#include <string>

//This enum represents all the circuit element types. A circuit element can be either an input, output, wire
//a logic gate or a subcircuit.
enum CircuitElementType {
    OR,
    AND,
    NAND,
    NOR,
    XOR,
    XNOR,
    NOT,
    INPUT,
    OUTPUT,
    WIRE,
    OAI3,
    BUF,
    SUBCIRCUIT,
    DFF
};

//This class represents a circuit element in a circuit
class CircuitElement
{
public:
    //No parameter constructor for a circuit element
    CircuitElement();

    //Constructs a circuit element with the given name, type and subcircuit type

    CircuitElement(std::string name, CircuitElementType type, std::string subcircuitType = "");

    //Holds the element name of the circuit element
    std::string elementName;

    //Holds the element type of the circuit element
    CircuitElementType elementType;

    //Holds the type of the subcircuit if the circuit element is a subcircuit
    std::string subcircuitType;

    //Overloading of the == operator for circuit element
    bool operator==(const CircuitElement& other);

    //Checks if the circuit element is a gate
    bool isGate();

    //Checks if the circuit element is a subcircuit
    bool isSubcircuit() { return elementType==CircuitElementType::SUBCIRCUIT;}

    //The input order of the circuit element if its a gate or a subcircuit
    std::vector<std::string> inputOrder;

    //The output order of the circuit element if its a gate or a subcircuit
    std::vector<std::string> outputOrder;

    //Adds the given node name to the input order of the circuit element
    void addToInputOrder(std::string input) {
        if(isGate() || isSubcircuit()){
            inputOrder.push_back(input);
        }
    }

    //Adds the given node name to the output order of the circuit element
    void addToOutputOrder(std::string output) {
        if(isGate() || isSubcircuit()){
            outputOrder.push_back(output);
        }
    }

    //Returns the input order of the circuit element
    std::vector<std::string> getInputOrder(){return inputOrder;}

    //Returns the output order of the circuit element
    std::vector<std::string> getOutputOrder(){return outputOrder;}

};

#endif // CIRCUITELEMENT_H
