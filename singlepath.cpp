#include "singlepath.h"

SinglePath::SinglePath(std::vector<std::string> pathStr)
{
    path = pathStr;
}

void SinglePath::InsertWireLine(std::ifstream& inVerilogFile, std::ofstream& outVerilogFile)
{

    std::string input = path[0];
    std::string output = path[path.size() - 1];

    std::string line;
    //For only writing wire line
    std::unordered_set<std::string> wireSet;
    while(getline(inVerilogFile, line))
    {
        std::replace(line.begin(), line.end(), ';', ' ');
        std::replace(line.begin(), line.end(), ',', ' ');
        std::replace(line.begin(), line.end(), '(', ' ');
        std::replace(line.begin(), line.end(), ')', ' ');

        std::istringstream stm(line);
        std::string word;
        std::vector<std::string> tokens;
        std::vector<std::string> pathGates;


        while (stm >> word)
        {
            if(path.end() != std::find(path.begin(), path.end(), word))
            {
                pathGates.push_back(word);
            }
            tokens.push_back(word);
        }


        if(tokens.size() > 0 &&  isalpha(tokens.front().at(0)))
        {
            std::string gate = tokens.front();
            for(size_t i = 0; i < gate.size(); ++i)
            {
                gate[i] = toupper(gate[i]);
            }

            if(cirElements.end() != std::find(cirElements.begin(), cirElements.end(), gate))
            {
                bool insidePath = false;
                for(std::string& a : tokens)
                {
                    if(path.end() != std::find(path.begin(), path.end(), a))
                    {
                        insidePath = true;
                    }
                }

                if(insidePath)
                {
                    std::string gateOutput = tokens.at(2);

                    if(path.end() == std::find(path.begin(), path.end(), gateOutput))
                    {
                        if(gateOutput != input && gateOutput != output)
                        {
                            wireSet.insert(gateOutput);
                        }
                    }

                    for(size_t i = 0; i < pathGates.size(); ++i)
                    {
                        if(pathGates[i] != input && pathGates[i] != output)
                        {
                            wireSet.insert(pathGates[i]);
                        }
                    }
                }
            }
        }
    }

    //Wire components colelcted
    //Insert wire line
    outVerilogFile << "wire ";
    for(auto& a : wireSet)
    {
        outVerilogFile << a <<", ";
    }
    outVerilogFile << "Vcc, gnd;" << std::endl;

    inVerilogFile.clear();
    inVerilogFile.seekg(0);

}

int SinglePath::ConvertToSinglePathVerilog(std::string inputVerilogLoc, std::string outputVerilogLoc)
{

    std::ifstream inVerilogFile;
    std::ofstream outVerilogFile;

    inVerilogFile.open(inputVerilogLoc);
    outVerilogFile.open(outputVerilogLoc, std::ofstream::trunc);
    outVerilogFile.clear();

    if (!inVerilogFile) {
        std::cout << inputVerilogLoc << std::endl;
        std::cerr << "Unable to open file"<<std::endl;
        return -1;
    }

    std::string input = path[0];
    std::string output = path[path.size() - 1];

    outVerilogFile << "module singlepath (" << input << ", " << output << ");" << std::endl;
    outVerilogFile << "input " << input << ";" << std::endl;
    outVerilogFile << "output " << output << ";" << std::endl;

    std::string line;
    InsertWireLine(inVerilogFile, outVerilogFile);

    while(getline(inVerilogFile, line))
    {
        std::replace(line.begin(), line.end(), ';', ' ');
        std::replace(line.begin(), line.end(), ',', ' ');
        std::replace(line.begin(), line.end(), '(', ' ');
        std::replace(line.begin(), line.end(), ')', ' ');

        std::istringstream stm(line);
        std::string word;
        std::vector<std::string> tokens;
        std::vector<std::string> pathGates;


        while (stm >> word)
        {
            if(path.end() != std::find(path.begin(), path.end(), word))
            {
                pathGates.push_back(word);
            }
            tokens.push_back(word);            
        }


        if(tokens.size() > 0 &&  isalpha(tokens.front().at(0)))
        {
            std::string gate = tokens.front();
            std::string gateLower = tokens.front();
            for(size_t i = 0; i < gate.size(); ++i)
            {
                gate[i] = toupper(gate[i]);
                gateLower[i] = tolower(gate[i]);
            }

            if(cirElements.end() != std::find(cirElements.begin(), cirElements.end(), gate))
            {
                bool insidePath = false;
                size_t sizeArguments;
                size_t countArguments = 0;

                for(std::string& a : tokens)
                {
                    if(path.end() != std::find(path.begin(), path.end(), a))
                    {
                        insidePath = true;
                    }
                }

                if(insidePath)
                {
                    sizeArguments = tokens.size() - 2;
                    std::string gateOutput = tokens.at(2);

                    outVerilogFile << gateLower << " " << tokens.at(1) << "(" ;

                    if(path.end() == std::find(path.begin(), path.end(), gateOutput))
                    {
                        outVerilogFile << gateOutput << ", ";
                        ++countArguments;
                    }

                    for(size_t i = 0; i < pathGates.size(); ++i)
                    {
                        outVerilogFile << pathGates[i];
                        ++countArguments;
                        if(i < pathGates.size() - 1)
                        {
                            outVerilogFile << ", ";
                        }
                        else if(i == pathGates.size() - 1)
                        {
                            std::string sideinput;
                            if(gate == "AND" || gate == "NAND")
                            {
                                sideinput = "Vcc";
                            }
                            else
                            {
                                sideinput = "gnd";
                            }

                            while(countArguments < sizeArguments)
                            {
                                outVerilogFile << ", " << sideinput;
                                ++countArguments;
                            }
                            outVerilogFile << ");" << std::endl;;
                        }
                    }
                }
            }
        }
    }
    outVerilogFile << std::endl << "endmodule" << std::endl;

    inVerilogFile.close();
    outVerilogFile.close();

    return 0;
}
