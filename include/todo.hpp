#pragma once

#include <stdlib.h>

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <ostream>
#include <string>

#include "ErrorCode.hpp"
#include "Utils.hpp"

namespace todo {

class Task {
 public:
  Task() : id(0), title(""), disc(""), state(DONE), prio(LOW) {}
  Task(uint32_t Id, std::string title, std::string disc = "",
       TaskState state = TODO, TaskPriority prio = MEDIUM)
      : id(Id), title(title), disc(disc), state(state), prio(prio) {
    if (std::find(idList.begin(), idList.end(), Id) != idList.end()) {
      LOG_ERROR(-1, "taskId collision.");
    }
    idList.push_back(Id);
  }
  ~Task(){};
  std::string getTitle() { return title; }
  std::string getDisc() { return disc; }
  uint32_t getId() { return id; }

  void setId(const uint32_t Id);
  void setTitle(const std::string taskTitle) { this->title = taskTitle; }
  void setDisc(const std::string taskDisc) { this->disc = taskDisc; }

  friend std::ostream& operator<<(std::ostream& os, const Task& a) {
    static bool list = false;
    os << a.title << " TITLE_E\n"
       << a.disc << " DISC_E\n"
       << a.state << ' ' << a.prio << '\n';
    return os;
  }
  // friend std::istream& operator>>(std::istream& is, Task& a) {
  //   is >> a.id;
  //   idList.push_back(a.id);
  //   is >> a.title;
  //   is >> a.disc;
  //   uint32_t state, prio;
  //   is >> state;
  //   is >> prio;
  //   a.state = TaskState(state);
  //   a.prio = TaskPriority(prio);
  //   return is;
  // }

 private:
  static std::list<uint32_t> idList;
  std::string disc;
  std::string title;
  uint32_t id;
  TaskState state;
  TaskPriority prio;
};

class Todo {
 public:
  Todo();
  ~Todo() { todoFile.close(); }

  void dumpTasks();
  void loadFile();
  void addTask(Hash taskHash, std::string title, std::string disc = "",
               TaskState state = TODO, TaskPriority prio = MEDIUM);

  void addTask(Time taskTime, std::string title, std::string disc = "",
               TaskState state = TODO, TaskPriority prio = MEDIUM);
  void removeTask(uint32_t taskId);
  void getNextTask();
  void markAsDone();
  void reOpen();
  void updateTask();
  Time getTime() { return currentTime; }

 private:
  // Variables
  static uint32_t nextTaskId;
  std::string filePath;
  std::fstream todoFile;
  Time currentTime;
  std::map<Hash, std::vector<Task>, timeComparator> taskMap;
  std::map<uint32_t, uint64_t> taskIDToTimeHash;
};

}  // namespace todo
