#include "gate_tables.h"

std::map<std::string, std::string> GateTables::gate_table_and = {
    {"00", "0"},
    {"01", "0"},
    {"0x", "0"},
    {"0D", "0"},
    {"0E", "0"},
    {"10", "0"},
    {"11", "1"},
    {"1x", "x"},
    {"1D", "D"},
    {"1E", "E"},
    {"x0", "0"},
    {"x1", "x"},
    {"xx", "x"},
    {"xD", "x"},
    {"xE", "x"},
    {"D0", "0"},
    {"D1", "D"},
    {"Dx", "x"},
    {"DD", "D"},
    {"DE", "0"},
    {"E0", "0"},
    {"E1", "E"},
    {"Ex", "x"},
    {"ED", "0"},
    {"EE", "E"},
    {"C0", "0"},
    {"C1", "C"},
    {"Cx", "x"},
    {"0C", "0"},
    {"1C", "C"},
    {"xC", "x"},
};

std::map<std::string, std::string> GateTables::gate_table_nand = {
    {"00", "1"},
    {"01", "1"},
    {"0x", "1"},
    {"0D", "1"},
    {"0E", "1"},
    {"10", "1"},
    {"11", "0"},
    {"1x", "x"},
    {"1D", "E"},
    {"1E", "D"},
    {"x0", "1"},
    {"x1", "x"},
    {"xx", "x"},
    {"xD", "x"},
    {"xE", "x"},
    {"D0", "1"},
    {"D1", "E"},
    {"Dx", "x"},
    {"DD", "E"},
    {"DE", "1"},
    {"E0", "1"},
    {"E1", "D"},
    {"Ex", "x"},
    {"ED", "1"},
    {"EE", "D"},
    {"C0", "1"},
    {"C1", "C"},
    {"Cx", "x"},
    {"0C", "1"},
    {"1C", "C"},
    {"xC", "x"},
};

std::map<std::string, std::string> GateTables::gate_table_or = {
    {"00", "0"},
    {"01", "0"},
    {"0x", "x"},
    {"0D", "D"},
    {"0E", "E"},
    {"10", "1"},
    {"11", "1"},
    {"1x", "1"},
    {"1D", "1"},
    {"1E", "1"},
    {"x0", "x"},
    {"x1", "1"},
    {"xx", "x"},
    {"xD", "x"},
    {"xE", "x"},
    {"D0", "D"},
    {"D1", "1"},
    {"Dx", "x"},
    {"DD", "D"},
    {"DE", "1"},
    {"E0", "E"},
    {"E1", "1"},
    {"Ex", "x"},
    {"ED", "1"},
    {"EE", "E"},
    {"C0", "C"},
    {"C1", "1"},
    {"Cx", "x"},
    {"0C", "C"},
    {"1C", "1"},
    {"xC", "x"},
};

std::map<std::string, std::string> GateTables::gate_table_nor = {
    {"00", "1"},
    {"01", "1"},
    {"0x", "x"},
    {"0D", "E"},
    {"0E", "D"},
    {"10", "0"},
    {"11", "0"},
    {"1x", "0"},
    {"1D", "0"},
    {"1E", "0"},
    {"x0", "x"},
    {"x1", "0"},
    {"xx", "x"},
    {"xD", "x"},
    {"xE", "x"},
    {"D0", "E"},
    {"D1", "0"},
    {"Dx", "x"},
    {"DD", "E"},
    {"DE", "0"},
    {"E0", "D"},
    {"E1", "0"},
    {"Ex", "x"},
    {"ED", "0"},
    {"EE", "D"},
    {"C0", "C"},
    {"C1", "0"},
    {"Cx", "x"},
    {"0C", "C"},
    {"1C", "0"},
    {"xC", "x"},
};
