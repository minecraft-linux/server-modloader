#pragma once

#include <vector>
#include <string>

namespace modloader {

class ElfHelper {

public:
    static std::vector<std::string> getDependencies(std::string const &path);

};

}