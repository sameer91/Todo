#pragma once

#include <stdlib.h>

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <ostream>
#include <set>
#include <string>

#include "ErrorCode.hpp"
#include "Utils.hpp"

namespace todo {

Time getCurrentTime();

class Task {
 public:
  Task() : id(0), title(""), disc(""), state(DONE), prio(LOW) {}
  Task(uint32_t Id, std::string title, std::string disc = "",
       TaskState state = TODO, TaskPriority prio = MEDIUM)
      : id(Id), title(title), disc(disc), state(state), prio(prio) {
    if (std::find(idList.begin(), idList.end(), Id) != idList.end()) {
      LOG_ERROR(-1, "taskId collision.");
    }
    //    idList.push_back(Id);
  }
  Task(const Task& a)
      : id(a.getId()),
        title(a.getTitle()),
        disc(a.getDisc()),
        state(a.getState()),
        prio(a.getPriority()) {}
  void deep_copy(const std::shared_ptr<Task>& a) {
    this->id = a->getId();
    this->title = a->getTitle();
    this->disc = a->getDisc();
    this->state = a->getState();
    this->prio = a->getPriority();
  }
  ~Task() { LOG_INFO("Task [%u] destroyed.", id); };
  std::string getTitle() const { return title; }
  std::string getDisc() const { return disc; }
  uint32_t getId() const { return id; }
  TaskState getState() const { return state; }
  TaskPriority getPriority() const { return prio; }

  void setId(const uint32_t Id);
  void setTitle(const std::string taskTitle) { this->title = taskTitle; }
  void setDisc(const std::string taskDisc) { this->disc = taskDisc; }
  void setState(const TaskState& taskState) { this->state = taskState; }
  void setPriority(const TaskPriority& taskPrio) { this->prio = taskPrio; }

  friend std::ostream& operator<<(std::ostream& os, const Task& a) {
    os << a.title << " TITLE_E\n"
       << a.disc << " DISC_E\n"
       << a.state << ' ' << a.prio << '\n';
    return os;
  }

 private:
  // TODO: move this to Todo?
  static std::vector<uint32_t> idList;
  uint32_t id;
  std::string title;
  std::string disc;
  TaskState state;
  TaskPriority prio;
};

class Todo {
 public:
  Todo();
  ~Todo() { todoFile.close(); }

  void dumpToFile();
  void dumpToLog();
  void loadFile();

  void addTask(Hash taskHash, const std::string& title, std::string disc = "",
               TaskState state = TODO, TaskPriority prio = MEDIUM);
  void addTask(Time taskTime, const std::string& title, std::string disc = "",
               TaskState state = TODO, TaskPriority prio = MEDIUM);
  void addTask(uint32_t id, Hash taskHash, const std::string& title,
               std::string disc = "", TaskState state = TODO,
               TaskPriority prio = MEDIUM);
  void addTask(uint32_t id, Time taskTime, const std::string& title,
               std::string disc = "", TaskState state = TODO,
               TaskPriority prio = MEDIUM);
  void removeTask(const uint32_t& taskId);

  std::vector<Task> getUpcomingTasks(Time& fromTime, uint32_t count);

  void updateTaskState(const uint32_t& id, const TaskState& state);
  void updateTaskPriority(const uint32_t& id, const TaskPriority& prio);

  void syncCurrentTime();
  Time getTime() { return currentTime; }

 private:
  // Variables
  static uint32_t nextTaskId;
  std::string filePath;
  std::fstream todoFile;
  Time currentTime;
  std::map<Hash, std::vector<uint32_t>, hashComparator> taskMap;
  std::map<uint32_t, uint64_t> taskIDToTimeHash;

  std::map<uint32_t, std::weak_ptr<Task>> idToTask;
  std::vector<std::shared_ptr<Task>> taskList;
  std::set<uint32_t> idList;
};

}  // namespace todo
