#ifndef CURRENTCIRCUIT_H
#define CURRENTCIRCUIT_H
#include "circuit.h"
#include "atpg_circuit.h"

//This class represents the currently processed circuit by PLODE to differantiate
//between submodules and the main circuit.
class CurrentCircuit
{
public:
    //No parameter constructor
    CurrentCircuit();

    static void topological_sort();

    static ATPGCircuit get_atpg_circuit();
    static bool d_algorithm(ATPGCircuit circ, std::string fault_element_name, char fault);
    static void path_sensitization(ATPGCircuit circ, std::vector<std::string> path);

    //Represents the currently processed circuit
    static Circuit circ;
};

#endif // CURRENTCIRCUIT_H
