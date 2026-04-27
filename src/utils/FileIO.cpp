// ============================================================
// FileIO.cpp
// ============================================================

#include "utils/FileIO.hpp"

#include <fstream>
#include <sstream>

namespace utils {

bool FileIO::readText(const std::string& path, std::string& out) {
    std::ifstream f(path);
    if (!f.is_open()) return false;
    std::stringstream ss;
    ss << f.rdbuf();
    out = ss.str();
    return true;
}

} // namespace utils
