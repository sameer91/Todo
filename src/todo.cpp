#include "todo.hpp"

#include <cstdint>
#include <ctime>
#include <memory>
#include <string>
#include <utility>

#include "ErrorCode.hpp"
#include "Utils.hpp"

namespace todo {
uint32_t Todo::nextTaskId = 1;

Todo::Todo() {
  // Get CurentTime
  time_t t = time(NULL);
  std::string timeStr = std::ctime(&t);
  LOG_INFO("%s", timeStr.c_str());
  Time cTime(timeStr);
  currentTime = cTime;

  // Path to $HOME/Todo.txt
  std::string todoPath;
  todoPath = getenv("HOME");
  todoPath += "/.todo.txt";
  LOG_INFO("Using todo.txt: %s", todoPath.c_str());
  filePath = todoPath;
  todoFile.open(filePath.c_str(),
                std::ios::in | std::ios::out | std::ios::binary);

  if (!todoFile) {
    LOG_INFO("File does not exist: %s", todoPath.c_str());
    todoFile.open(todoPath.c_str(), std::ios::in | std::ios::out |
                                        std::ios::trunc | std::ios::binary);
    if (!todoFile) {
      LOG_ERROR(-1, "Unable to create file.");
    }
    todoFile.close();
  } else {
    // File exists read data from file.
    loadFile();
    todoFile.close();
  }
}

void Todo::addTask(Hash hash, std::string title, std::string disc,
                   TaskState state, TaskPriority prio) {
  auto tempTask = std::make_shared<Task>(nextTaskId, title, disc, state, prio);
  taskList.push_back(tempTask);
  taskMap[hash].push_back(tempTask);
  taskIDToTimeHash.insert(std::make_pair(nextTaskId, hash));
  nextTaskId++;
  tempTask.reset();
}

void Todo::addTask(Time taskTime, std::string title, std::string disc,
                   TaskState state, TaskPriority prio) {
  uint64_t hash = taskTime.getHash();
  addTask(hash, title, disc, state, prio);
}

void Todo::removeTask(uint32_t taskId) {
  Hash hash = taskIDToTimeHash[taskId];
  auto& tList = taskMap[hash];
  uint32_t idx = 0;
  for (auto it = tList.begin(); it != tList.end(); ++it, idx++) {
    if (taskId == it->lock()->getId()) {
      taskIDToTimeHash.erase(taskId);
      tList[idx].reset();
      tList.erase(it);
    }
  }
  idx = 0;
  for (auto it : taskList) {
    if (taskId == it->getId()) {
      taskList[idx].reset();
      taskList.erase(taskList.begin() + idx);
      return;
    }
    idx++;
  }
}
void Todo::dumpToFile() {
  // Trucate file before writing.
  todoFile.open(filePath.c_str(),
                std::ios::out | std::ios::trunc | std::ios::binary);

  LOG_INFO("Todo object state");
  LOG_INFO("nextTaskId: %d", nextTaskId);
  LOG_INFO("taskMap:");
  for (auto it = taskMap.begin(); it != taskMap.end(); ++it) {
    LOG_INFO("  hash: %lu", it->first);

    todoFile << "HASH " << it->first << std::endl;
    todoFile << "START_TASK" << std::endl;

    for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
      const auto& task = it2->lock();

      todoFile << "TASK\n" << *task.get();
      LOG_INFO("    taskId: %d, Title: %s", task->getId(),
               task->getTitle().c_str());
    }
    todoFile << "END_TASK" << std::endl;
  }
  LOG_INFO("id-hash map");
  for (auto it : taskIDToTimeHash) {
    LOG_INFO("  Id %u, hash: %lu", it.first, it.second);
  }
  todoFile << "EOF" << std::endl;
  todoFile.close();
}

void Todo::dumpToLog() {
  LOG_INFO("Todo object state");
  LOG_INFO("nextTaskId: %d", nextTaskId);
  LOG_INFO("taskMap:");
  for (auto it = taskMap.begin(); it != taskMap.end(); ++it) {
    LOG_INFO("  hash: %lu", it->first);
    for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
      const auto& task = it2->lock();
      LOG_INFO("    taskId: %d, Title: %s", task->getId(),
               task->getTitle().c_str());
    }
  }
  LOG_INFO("id-hash map");
  for (auto it : taskIDToTimeHash) {
    LOG_INFO("  Id %u, hash: %lu", it.first, it.second);
  }
}

void Todo::loadFile() {
  std::string flag = "STR";
  todoFile.seekg(0, std::ios::beg);
  while (flag != "EOF" && !todoFile.eof()) {
    if (flag == "HASH") {
      Hash taskHash;
      todoFile >> taskHash;
      todoFile >> flag;
      if (flag != "START_TASK") {
        LOG_ERROR(-1, "task list block not found -- %s");
      }
      todoFile >> flag;
      while (flag != "END_TASK" && !todoFile.eof()) {
        if (flag != "TASK") {
          LOG_INFO("%s", flag.c_str());
          LOG_ERROR(-1, "TASK block not found");
        }
        todoFile >> flag;
        std::string title = "";
        while (flag != "TITLE_E" && !todoFile.eof()) {
          if (!title.empty()) title += ' ';
          title += flag;
          todoFile >> flag;
        }
        todoFile >> flag;
        std::string disc = "";
        while (flag != "DISC_E" && !todoFile.eof()) {
          if (!disc.empty()) disc += ' ';
          disc += flag;
          todoFile >> flag;
        }
        uint32_t sNo, pNo;
        todoFile >> sNo;
        todoFile >> pNo;
        TaskState state = TaskState(sNo);
        TaskPriority prio = TaskPriority(pNo);

        addTask(taskHash, title, disc, state, prio);

        todoFile >> flag;
      }
    } else if (flag == "STR") {
      todoFile >> flag;
      continue;
    } else {
      LOG_ERROR(-1, "no HASH");
    }
    todoFile >> flag;
  }
  if (flag == "STR") {
    LOG_INFO("Empty ./todo.txt");
  } else if (flag != "EOF") {
    LOG_ERROR(-1, "Malformed todo.txt");
  }
}

// -------------------------- LIST -------------------
std::vector<uint32_t> Task::idList;
}  // namespace todo
