#include "Utils.h"


bool Utils::validNumberString(std::string str){

    bool containsE = false;
    if(str.empty()){
        return false;
    }
    for(size_t i = 0; i < str.length();i++){
        if(std::isdigit(str[i]) == 0 && str[i] != '.' && str[i] != 'e' && str[i] != '+' && str[i] != '-'){
            return false;
        }
        if(str[i] == 'e'){
            containsE = true;
            if(str[i+1] != '+' && str[i+1] != '-' && !std::isdigit(str[i+1])){
                return false;
            }
            if(!std::isdigit(str[i-1])){
                return false;
            }
            if(i+1 >= str.length()){
                return false;
            }
        }
        if(str[i] == '.'){
            if(containsE){
                return false;
            }
            if(i+1 >= str.length()){
                return false;
            }
            if(!std::isdigit(str[i+1])){
                return false;
            }
        }

    }

    return true;
}

bool Utils::isNumericSpiceOption(std::string str){
    if(str == "XMU" || str == "TRTOL" || str == "SRCSTEPS" || str == "RAMPTIME" ||
       str == "MAXORD" || str == "MAXOPALTER" || str == "MAXEVITER" || str == "ITL6" ||
       str == "ITL5" || str == "ITL4" || str == "ITL3" || str == "CONVABSSTEP" ||
       str == "CONVSTEP" || str == "CHGTOL" || str == "VNTOL" || str == "RSHUNT" ||
       str == "RELTOL" || str == "PIVTOL" || str == "PIVREL" || str == "ITL2" ||
       str == "ITL1" || str == "GMINSTEPS" || str == "GMIN" || str == "ABSTOL" ||
       str == "MAXWARNS" || str == "TNOM" || str == "TEMP"){

        return true;
    }
    else{
        return false;
    }
}

bool Utils::isStringValueSpiceOption(std::string str){
    if(str == "SEED" || str == "WARN" || str == "METHOD" || str == "NOOPALTER"){
        return true;
    }
    else{
        return false;
    }
}

bool Utils::isBoolValueSpiceOption(std::string str){
    if(str == "ACCT" || str == "NOACCT" || str == "NOINIT" || str == "LIST" ||
            str == "NOMOD" || str == "NOPAGE" || str == "NODE" || str == "NOREFVALUE" ||
            str == "OPTS" || str == "SEEDINFO" || str == "SAVECURRENTS" || str == "KEEPOPINFO" ||
            str == "NOOPITER" || str == "AUTOSTOP" || str == "INTERP"){
        return true;
    }
    else{
        return false;
    }
}

bool isStringContainsInteger(std::string str){
    for(size_t i = 0 ; i < str.length(); i++){
        if(!isdigit(str[i])){
            return false;
        }
    }
    return true;
}
bool Utils::isValidStringValueForSpiceOption(std::string optionName, std::string str){
    if(!isStringValueSpiceOption(optionName)){
        return false;
    }
    else{
        std::cout << "STRING VALID CHECK: String is:" << str << " " << (str != "random") << " " << isStringContainsInteger(str) << std::endl;
        if(optionName == "SEED" && (str != "random" && !isStringContainsInteger(str))){
            std::cout <<"WHY DID I ENDED UP IN HERE!?!?!?!?!?!?" << std::endl;
           return false;
        }
        else if(optionName == "WARN" && (str != "1" && str != "0")){
            return false;
        }
        else if(optionName == "METHOD" && (str != "gear" && str != "trapezoidal" && str != "trap")){
            return false;
        }
        else if(optionName == "NOOPALTER" && (str != "true" && str != "false")){
            return false;
        }
        return true;
    }
}

std::vector<std::string> Utils::getLinesFromVerilogFile(std::string fileName){
    std::vector<std::string> lines;
    std::ifstream inFile(fileName);
    if(!inFile){
        std::cerr << "File could not be opened!" << std::endl;
    }
    std::string line;
    std::string fullLine = "";
    while(std::getline(inFile, line)){
        if(line.substr(0,2) == "//" || line == " "){
            continue;
        }
        else{
            line.erase(std::remove_if(line.begin(), line.end(), [](char chr) { return chr == '\r' || chr == '\n' || chr == '\t'; }), line.end());
            std::string::iterator new_end = std::unique(line.begin(), line.end(), [](auto lhs, auto rhs){ return (lhs == rhs) && (lhs == ' '); });
            line.erase(new_end, line.end());

            fullLine += line;

            if(line.back() == ';' || line == "endmodule"){
                fullLine = std::regex_replace(fullLine, std::regex(", "), ",");

                lines.push_back(fullLine);
                fullLine = "";
            }
        }

    }
    std::cout << "*********************************" << std::endl;
    for(const auto& line : lines){
        std::cout << line << std::endl << std::endl;
    }
    std::cout << "*********************************" << std::endl;

    inFile.close();

    return lines;
}
