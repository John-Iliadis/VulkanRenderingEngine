//
// Created by Gianni on 2/01/2025.
//

#include "utils.hpp"

void check(bool result, const char* msg, std::source_location location)
{
    if (!result)
    {
        std::stringstream errorMSG;
        errorMSG << '`' << location.function_name() << "`: " << msg << '\n';

        throw std::runtime_error(errorMSG.str());
    }
}