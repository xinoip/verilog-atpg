#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <regex>
class Utils{
    public:
        static int getWireCount(std::vector<std::string> tokens);

        //Checks if the given string is a valid number
        static bool validNumberString(std::string str);

        //Checks if the given string is a Spice option
        static bool isStringValueSpiceOption(std::string str);

        //Checks if the given string is a boolean valued Spice option
        static bool isBoolValueSpiceOption(std::string str);

        //Checks if the given string is a numeric valued Spice option
        static bool isNumericSpiceOption(std::string str);

        //Checks if the given string is a valid string value for the given Spice option name
        static bool isValidStringValueForSpiceOption(std::string optionName, std::string str);

        //Reads lines from a Verilog file and creates a line vector
        static std::vector<std::string> getLinesFromVerilogFile(std::string fileName);
};

#endif // UTILS_H
