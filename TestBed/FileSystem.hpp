#ifndef TESTBED_FILE_SYSTEM_HPP
#define TESTBED_FILE_SYSTEM_HPP

#include <string>
#include <vector>

namespace TestBed {

std::vector<std::string> readFiles(const std::string& path, const std::string& ext);

} /* namespace TestBed */

#endif
