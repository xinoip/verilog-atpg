#ifndef CIRCUIT_H
#define CIRCUIT_H
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <utility>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <unordered_set>
#include <unistd.h>
#include <algorithm>
#include <string>
#include <QtCharts>
#include <stack>
#include "Utils.h"
#include "circuitelement.h"

//This class represents a circuit with its elements and other features such as the input-output calling order,
//submodule connection, circuit name. It essentially contains a graph data structure with additional parameters.
class Circuit
{
public:
    //This enum represents the submodule connection method that PLODE will use.
    //If graphJoiningMethod is used, the graph structures of the main circuit and submodules of the main circuit
    //are interconnected to each other. If spiceSubcircuitMethod is used, the main circuit's Spice deck contains
    //subckt definitions and required lines for the submodules of the main circuit.
    enum SubmoduleConnectionMethod{
        graphJoiningMethod,
        spiceSubcircuitMethod
    };

    //This map represents all the possible logic gate types that PLODE can convert.
    std::map<std::string, CircuitElementType> mapOfNodeTypes = {
        {"or",CircuitElementType::OR},
        {"and",CircuitElementType::AND},
        {"nand",CircuitElementType::NAND},
        {"nor",CircuitElementType::NOR},
        {"xor",CircuitElementType::XOR},
        {"xnor",CircuitElementType::XNOR},
        {"not",CircuitElementType::NOT},
        {"buf",CircuitElementType::BUF},
        {"oai3",CircuitElementType::OAI3},
        {"&",CircuitElementType::AND},
        {"|",CircuitElementType::OR},
        {"^",CircuitElementType::XOR},
        {"^~",CircuitElementType::XNOR},
        {"~^",CircuitElementType::XNOR},
        {"~",CircuitElementType::NOT},

    };
    //Intentionally empty no parameter constructor
    Circuit() {}

    //Copy constructor definition
    Circuit (const Circuit & circuit);

    //Holds the adjacency list for the graph structure of the Circuit
    std::vector< std::pair< CircuitElement,std::vector<CircuitElement> > > adjacencyList;

    //Holds all the elements in the circuit
    std::vector<CircuitElement> elements;

    //Holds the name of the circuit
    std::string circuitName;

    //Holds the names of the output nodes of the circuit
    std::vector<std::string> outputs;

    //Holds the names of the input nodes of the circuit
    std::vector<std::string> inputs;

    //Holds the submodules of the circuit
    std::vector<Circuit> submodules;

    //Holds the submodule counts for each submodule in the circuit
    static std::map<std::string, int> submoduleCounts;

    //Holds the call order of the circuit. It holds node names in the call order
    std::vector<std::string> callOrder;

    //Holds the submodule connection method. It can be either Graph Joining or Spice Subcircuit method.
    SubmoduleConnectionMethod submoduleConnectionMethod;

    //Temporary wire count for wire name generation
    static int gen_temp_wire_count;

    //Temporary gate count for gate name generation
    static int gen_temp_gate_count;

    //Temporary wire count for submodule wire name generation
    static int gen_submodule_temp_wire_count;

    //Temporary gate count for submodule gate name generation
    static int gen_submodule_temp_gate_count;

    //Adds an edge to the circuit graph structure containing element1 and element2 circuit elements
    void addEdge(CircuitElement & element1, CircuitElement & element2);

    //Prints every node and its connected nodes
    void print();

    //Reads a Structural Verilog file and builds the Circuit graph structure
    bool fillFromVerilogFile(std::string fileName);

    //Sets submodule connection method of the circuit. It can be either Graph Joining or Spice subcircuit method
    void setSubmoduleConnectionMethod(SubmoduleConnectionMethod method) {submoduleConnectionMethod = method;}

    //Sorts the outputs using the call order
    void sortOutputsWithCallOrder();

    //Sorts the inputs using the call order
    void sortInputsWithCallOrder();

    //Applies the GraphJoining method between circuit and submodule graphs
    void connectSubmoduleByGraphJoining(Circuit * submodule, std::vector<std::string> parameters, std::map<std::string, CircuitElementType> isInputOutputOrWire);

    //Changes the circuit element's name with the new name
    void changeElementName(std::string previousName, std::string newName);

    //Changes the circuit element's type with the new type
    void changeElementType(std::string elementName, CircuitElementType newType);

    //Connects another circuit's graph with
    void connectOtherCircuit(Circuit * circuit);
private:

    //Checks if the element with the given name exists in the circuit
    bool doesElementWithNameExist(std::string name);
};

#endif // CIRCUIT_H
