#ifndef DELAY_FILE_H
#define DELAY_FILE_H

#include <string>
#include <map>

class DelayFile {
public:
    DelayFile(std::string file_name);

    int delay_of_element(std::string name) const;

private:
    std::string m_file_name;
    std::map<std::string, int> m_delay_map;
};

#endif
