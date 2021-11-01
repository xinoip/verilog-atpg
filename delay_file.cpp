#include "delay_file.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "Utils.h"

DelayFile::DelayFile(std::string file_name) : m_file_name(file_name) {
    using namespace std;
    ifstream in_file;
    in_file.open(file_name);
    if (!in_file) {
        cerr << "Unable to open delay file \"" << file_name << "\"" << endl;
        return;
    }

    string line;
    vector<string> fileLines = Utils::getLinesFromDelayFile(file_name);
    for (auto line : fileLines) {
        std::vector<std::string> split = Utils::splitStringByDelimiter(line, ' ');
        std::string& name = split[0];
        int delay = std::stoi(split[1]);
        m_delay_map.insert({name, delay});
    }
    in_file.close();
}

int DelayFile::delay_of_element(std::string name) const {
    try {
        return m_delay_map.at(name);
    } catch (...) {
        return 0; // assume unspecified delays are 0
    }
}
