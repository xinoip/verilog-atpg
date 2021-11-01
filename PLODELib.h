// VETSOLIB.H
#ifndef PLODELIB_H
#define PLODELIB_H

#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <utility>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unordered_set>
#include <unistd.h>
#include <algorithm>
#include <string>
#include <QtCharts>
#include "circuit.h"
#include <experimental/filesystem>

#include "delayparser.h"
class PLODELib
{
public:
    //Default constructor
    PLODELib();

    //Converts a given Circuit graph structure to Spice deck with the given standard cell library
    static std::string convertFromVerilogToSpice(Circuit & circuit, std::string library_path);

    //Creates a QT usable voltage graph with given file name of the voltage simulation results, node name and input changing interval
    static QChartView * createVoltageGraph(std::string file_name, std::string nodename, double inputChangeInterval);

    //Creates a QT usable delay graph with given file name of the voltage simulation results and node name
    static QChartView * createDelayGraph(std::string file_name, std::string nodename);

    //Creates a QT usable power graph with given file name of the current simulation results and supply voltage
    static QChartView* createPowerGraph(std::string file_name, double supplyvoltage);

    //Runs ngspice to do Spice simulations on the converted Spice file to get voltage and current simulation results.
    static void runSpiceSimulation(std::string file_path);

    //Adds .option .control and input lines to the converted Spice file
    static std::string addSimulationParameters(Circuit circuit, std::vector<std::vector<int> > inputValues, double inputChangeTime, double supplyvoltage, std::vector<std::pair<std::string,std::string> > options,  double clockPeriod,
                                               std::string clockName);

    //Getter for delay parser
    static DelayParser getDelayParser() { return delayParser; }

    //Creates a file name with the current spice simulation time to keep track of different simulation results
    static std::string createFilename(std::string main_circuit_file_name);

    //Adds subckt lines to the converted Spice file if the submodule connection method is chosen to be Spice subcircuiting
    static void addSubcircuitsToSpiceFile(Circuit & circuit, std::string filename);

    //Generates logic and delay results using the delay parser of PLODE
    static void generateLogicAndDelayResults(Circuit circuit, double supplyVoltage, double inputChangeTime, std::string voltageSimulationFileHeader, std::string clockName);

    //Total gate and subcircuit counts for naming the gate calls in the Spice
    static int andcount;
    static int orcount;
    static int nandcount;
    static int norcount;
    static int xorcount;
    static int xnorcount;
    static int notcount;
    static int bufcount;
    static int oaicount;
    static int dffcount;
    static int subcircuitCount;

    //Holds the spice simulation duration
    static double simulationDuration;

    //Holds the total simulation time
    static double totalSimulationTime;

    //Holds the voltage time pairs for inputs as a list
    static std::map<std::string, std::vector<std::pair<double,double>>> inputsVoltageTimeList;

    //Holds the voltage time pairs for outputs as a list
    static std::map<std::string, std::vector<std::pair<double,double>>> outputsVoltageTimeList;

private:

    //Holds the delay parser of PLODE
    static DelayParser delayParser;
};

#endif // PLODELIB_H
