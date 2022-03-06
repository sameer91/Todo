#include "Utils.hpp"

#include <cstdlib>

namespace todo {
std::vector<std::string> getTocken(std::string inString, const char delim) {
  std::vector<std::string> outArr;
  for (uint32_t i = 0; i < inString.size();) {
    std::string tmp = "";
    while (inString[i] != delim && i < inString.size()) {
      tmp += inString[i];
      ++i;
    }
    while (inString[i] == delim) {
      ++i;
    }
    // LOG_INFO("%s", tmp.c_str());
    outArr.push_back(tmp);
  }
  return outArr;
}

}  // namespace todo
