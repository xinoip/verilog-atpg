#ifndef GATE_TABLES_H
#define GATE_TABLES_H

#include "atpg_circuit_element.h"
#include <map>

class GateTables {
public:
    static std::map<std::string, std::string> gate_table_and;
    static std::map<std::string, std::string> gate_table_nand;
    static std::map<std::string, std::string> gate_table_or;
    static std::map<std::string, std::string> gate_table_nor;
};

#endif
