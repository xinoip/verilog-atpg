#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<map>
#include<cmath>
#include<iomanip>
#include<algorithm>

#define MARGINRATIO 0.2F

//Used to parse all input and output spice voltage vs time text files
//It can extract all the required delays for multiple outputs and multiple transitions
class DelayParser{

    private:
        //These two matrices will hold the
        //delayMatrix: delay for each output and transition
        //logicMatrix: logic values of each output after each transition including the initial values
        std::vector<std::vector<double>> delayMatrix;
        std::vector<std::vector<bool>> logicMatrix;

        //Holds the input 50% transition times
        //The size will be equal to the number of transitions
        std::vector<std::vector<double>> inputTransitionTimes;

        //Holds the all transition times of all inputs in one vector
        std::vector<double> allInputTransitionTimes;

        //Holds output node names
        std::vector<std::string> outputNames;

        //Holds the output 50% transition times
        //outputTransitiontimes(i, j) where i is the output node
        //and j is the jth transition time
        std::vector<std::vector<double>> outputTransitionTimes;

        //Holds output node logic values for ith output after jth transition
        std::vector<std::vector<bool>> outputTransitionValues;

        //Finds the transition times looking at times array and voltage array
        //and performs linear interpolation for exact time
        std::vector<double> ReturnTransitionTimesAndValues(const std::vector<double>& times, const std::vector<double>& voltages, double transitionVoltage, bool isInput);

    public:

        //Clear all vectors for a new simulation
        void ClearThemAll();

        //Combines the input Transition times to have all transition times for any input
        //It uses transitionInterval to guarantee that the transition times of inputs belong to distinct transitions
        void CombineInputTransitionTimes(double transitionInterval);

        //Compare two text files containing the output vectors for each transition
        //List down the different vectors than expected on a separate file
        //Returns number of mismatchs (errors)
        static std::vector<int> CompareActualWithExpected(std::string actualFile, std::string expectedFile, std::string resultFile);

        //Extract delays and fill the two private transition time vectors
        //according to the given Spice file at fileLocation
        //If the node is a primary input then isInput is true
        //else it is primary output
        void ExtractTransitionTimes(std::string fileLocation, double transitionVoltage, bool isInput);

        //Fills the delayMatrix(i, j) where
        //i is the ith output and
        //j is jth transition
        //Fills the logicMatrix(i, j) where
        //i is the ith output and
        //j is the logic value at that output for jth transition
        //It  uses the both the input and output transition times vectors to compute that
        void FillLogicDelayMatrices();

        //@Gökçe
        std::vector<double> GetAllInputTransitionTimes();

        //Return the output node names
        std::vector<std::string> GetOutputNames();

        //Display the delay and the logic values for each output and transition
        void WriteDelayAndValuesToConsole();

        //Transfer the delay for each output and transition to file
        void WriteDelayToFile(std::string delayFileLoc, std::string delimiter);

        //Transfer the logic values for each output and transition to file
        void WriteLogicToFile(std::string logicFileLoc, std::string delimiter);

        //Writes a particular bus which is a subset of the output to a file in hex format
        //Output[start:end] is converted and written in hex format to hexFileLoc
        void WriteParticularOutputToHexFile(std::string hexFileLoc, int start, int end);
};



