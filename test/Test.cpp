#include "ErrorCode.hpp"
#include "Utils.hpp"
#include "todo.hpp"

using namespace todo;
int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf(
        "Bad arguments\noptions can be one of these > -read, -write, -del, "
        "-mark, -get\n");
    LOG_ERROR(-1, "bad args...");
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

    testObj.removeTask(99);

    testObj.dumpToLog();

    return 0;
  }

  if (testMode == "-mark") {
    Todo testObj;
    testObj.dumpToLog();
    testObj.updateTaskState(1, DONE);
    testObj.updateTaskPriority(1, URGENT);
    testObj.dumpToLog();
    testObj.updateTaskState(99, DONE);
    testObj.updateTaskPriority(99, LOW);
    testObj.dumpToLog();

    return 0;
  }

  if (testMode == "-get") {
    Todo testObj;
    Time time(15, 05, 2022, 10, 00);
    Time t2(21, 05, 2022, 16, 00);
    testObj.addTask(time, "Upcoming Task 1", "Lorel Ipsum");
    testObj.addTask(t2, "Upcoming 22", "GGEZ");

    testObj.dumpToLog();
    Time temp = getCurrentTime();
    auto tList = testObj.getUpcomingTasks(temp, 5);
    for (auto& t : tList) {
      LOG_INFO("Task [%u], '%s' '%s' s[%d] p[%d]", t.getId(),
               t.getTitle().c_str(), t.getDisc().c_str(), t.getState(),
               t.getPriority());
    }
    testObj.dumpToLog();
    testObj.removeTask(7);

    return 0;
  }

  LOG_ERROR(-1, "malformed args.");
}
