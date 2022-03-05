#pragma once

#include <cstdint>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

#include "Util.hpp"

namespace Todo {
class todo {
 private:
  // Variables
  std::ifstream todoFile;
  Time currentTime;

 public:
  todo();
  ~todo(){};
  void addTask();
  void removeTask();
  void getNextTask();
  void markAsDone();
  void reOpen();
  void updateTask();
  Time getTime() { return currentTime; }
};
class task {
 private:
  task(){};
  ~task();
  uint32_t getId();

  std::string getTitle();
  std::string getDisc();

  void setId(const uint32_t taskId);
  void setTitle(const std::string taskTitle);
  void setDisc(const std::string taskDisc);

 public:
  uint32_t taskId;
  std::string taskDisc;
  std::string taskTitle;
  Time time;
};
}  // namespace Todo
