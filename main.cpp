#include "todo.hpp"

using namespace Todo;

int main() {
  todo testObj;
  Time objTime = testObj.getTime();
  objTime.dumpToTerminal();

  return 0;
}
