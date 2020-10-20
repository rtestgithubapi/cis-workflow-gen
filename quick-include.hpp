#ifndef RLIB_IMPL_QUICK_INCLUDE_XAML_GEN_HPP
#define RLIB_IMPL_QUICK_INCLUDE_XAML_GEN_HPP

#include <rlib/stdio.hpp>
#include <stdexcept>
#include "activity.hpp"
using namespace CIS;
using rlib::println;

#include <fstream>

inline auto &to_file(std::string filename) {
    auto outf = new std::ofstream (filename, std::ios_base::trunc);
    if(!*outf) {
        throw std::invalid_argument("Failed to open file: " + filename);
    }
    return *outf;
}

#endif