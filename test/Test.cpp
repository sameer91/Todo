#include "todo.hpp"

using namespace todo;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf(
        "Bad arguments\noptions can be one of these > -read, -write, -del\n");
    LOG_ERROR(-1, "bad args");
  }
  std::string testMode(argv[1]);
  std::transform(testMode.begin(), testMode.end(), testMode.begin(),
                 [](unsigned char c) { return std::tolower(c); });

  if (testMode == "-read") {
    Todo testObj;
    testObj.dumpToLog();

    return 0;
  }

  if (testMode == "-write") {
    Todo testObj;
    Time objTime = testObj.getTime();
    objTime.dumpToTerminal();
    testObj.addTask(objTime, "Task1");
    Time ob1(14, 10, 2001, 07, 35);
    Time ob2(22, 6, 2004, 12, 00);
    testObj.addTask(ob2, "That random thing");
    testObj.addTask(ob2, "Def not random");
    testObj.addTask(ob1, "That bday");
    testObj.addTask(ob1, "Task2");
    testObj.addTask(ob1, "Task3");
    testObj.dumpToFile();
    return 0;
  }

  if (testMode == "-del" || testMode == "-delete") {
    Todo testObj;
    // before delete.
    testObj.dumpToLog();

    testObj.removeTask(1);

    testObj.dumpToLog();

    return 0;
  }

  LOG_ERROR(-1, "malformed args.");
}
