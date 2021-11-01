#ifndef CURRENTCIRCUIT_H
#define CURRENTCIRCUIT_H
#include "circuit.h"

//This class represents the currently processed circuit by PLODE to differantiate
//between submodules and the main circuit.
class CurrentCircuit
{
public:
    //No parameter constructor
    CurrentCircuit();

    static void topological_sort();

    //Represents the currently processed circuit
    static Circuit circ;
};

#endif // CURRENTCIRCUIT_H
