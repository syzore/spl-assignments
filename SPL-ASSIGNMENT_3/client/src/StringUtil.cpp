
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

#include "../include/StringUtil.h"
#include <stdlib.h>

std::vector<std::string> StringUtil::split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}