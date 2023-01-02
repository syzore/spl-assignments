#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <iterator>

class StringUtil
{
private:
public:
    static std::vector<std::string> split(const std::string &s, char delim);
};
