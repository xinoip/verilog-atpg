#ifndef SINGLEPATH_H
#define SINGLEPATH_H
#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include<string>
#include<algorithm>
#include<unordered_set>

class SinglePath
{
    private:
        //Contains the path connections as a string vector.
        std::vector<std::string> path;

        //The logic gate list
        const std::vector<std::string>  cirElements = {"OR", "AND", "NAND", "NOR", "XOR", "XNOR", "NOT", "OAI3", "BUF"};

        //Insert wire ...; line to Verilog file
        void InsertWireLine(std::ifstream& inVerilogFile, std::ofstream& outVerilogFile);

    public:
        //Constructor
        SinglePath(std::vector<std::string> pathStr);

        //Takes a Verilog file and a path converts it to a new Verilog with only path
        int ConvertToSinglePathVerilog(std::string inputVerilogLoc, std::string outputVerilogLoc);

};



#endif // SINGLEPATH_H
