#ifndef GATE_TABLES_H
#define GATE_TABLES_H

#include "atpg_circuit_element.h"
#include <map>

// singular cover
// char gate_and_sc[3][3] = {
//     {'0', 'x', '0'},
//     {'x', '0', '0'},
//     {'1', '1', '1'}
// };
// // fault activation

// // propagation d-cube
// char gate_and_pdc[6][3] = {
//     {'1', 'D', 'D'},
//     {'D', '1', 'D'},
//     {'D', 'D', 'D'},
//     {'E', '1', 'E'},
//     {'1', 'E', 'E'},
//     {'E', 'E', 'E'}
// };


// SC: singular cover, outputs are 1/0
// std::map<std::string, std::string> gate_table_sc_and = {
//     {"0x", "0"},
//     {"x0", "0"},
//     {"11", "1"},
// };

// // PDC: propagation d-cube, outputs are D/D'
// std::map<std::string, std::string> gate_table_pdc_and = {
//     {"D1", "D"},
//     {"1D", "D"},
//     {"DD", "D"},
//     {"E1", "E"},
//     {"1E", "E"},
//     {"EE", "E"},
// };

// // PDCF: primitive d-cubes for a fault (fault activation)
// std::map<std::string, std::string> gate_table_pdcf_and = {
//     {"11", "D"},
//     {"0x", "E"},
//     {"x0", "E"},
// };

// std::map<std::string, std::string> gate_table_and = {
//     {"00", "0"},
//     {"01", "0"},
//     {"0x", "0"},
//     {"0D", "0"},
//     {"0E", "0"},
//     {"10", "0"},
//     {"11", "1"},
//     {"1x", "x"},
//     {"1D", "D"},
//     {"1E", "E"},
//     {"x0", "0"},
//     {"x1", "x"},
//     {"xx", "x"},
//     {"xD", "x"},
//     {"xE", "x"},
//     {"D0", "0"},
//     {"D1", "D"},
//     {"Dx", "x"},
//     {"DD", "D"},
//     {"DE", "0"},
//     {"E0", "0"},
//     {"E1", "E"},
//     {"Ex", "x"},
//     {"ED", "0"},
//     {"EE", "E"},
// };

// std::map<std::string, std::string> gate_table_nand = {
//     {"00", "1"},
//     {"01", "1"},
//     {"0x", "1"},
//     {"0D", "1"},
//     {"0E", "1"},
//     {"10", "1"},
//     {"11", "0"},
//     {"1x", "x"},
//     {"1D", "E"},
//     {"1E", "D"},
//     {"x0", "1"},
//     {"x1", "x"},
//     {"xx", "x"},
//     {"xD", "x"},
//     {"xE", "x"},
//     {"D0", "1"},
//     {"D1", "E"},
//     {"Dx", "x"},
//     {"DD", "E"},
//     {"DE", "1"},
//     {"E0", "1"},
//     {"E1", "D"},
//     {"Ex", "x"},
//     {"ED", "1"},
//     {"EE", "D"},
// };

class GateTables {
public:
    static std::map<std::string, std::string> gate_table_and;
    static std::map<std::string, std::string> gate_table_nand;
};

#endif
