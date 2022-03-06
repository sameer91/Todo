#include "todo.hpp"

using namespace todo;

int main() {
  Todo testObj;
  Time objTime = testObj.getTime();
  objTime.dumpToTerminal();
  // testObj.addTask(objTime, "Task1");
  // Time ob1(14, 10, 2001, 07, 35);
  // Time ob2(22, 6, 2004, 12, 00);
  // testObj.addTask(ob2, "That random thing");
  // testObj.addTask(ob2, "Def not random");
  // testObj.addTask(ob1, "That bday");
  // testObj.addTask(ob1, "Task2");
  // testObj.addTask(ob1, "Task3");
  testObj.dumpTasks();

  return 0;
}
