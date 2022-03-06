#include "todo.hpp"

#include <cstdint>
#include <ctime>
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
  currentTime = new Time(timeStr);

  // Path to $HOME/Todo.txt
  std::string todoPath;
  todoPath = getenv("HOME");
  todoPath += "/.todo.txt";
  LOG_INFO("Using todo.txt: %s", todoPath.c_str());
  filePath = todoPath.data();
  todoFile.open(filePath.c_str(), std::ios::in | std::ios::out);

  if (!todoFile) {
    LOG_INFO("File does not exist: %s", todoPath.c_str());
    todoFile.open(todoPath.c_str(),
                  std::ios::in | std::ios::out | std::ios::trunc);
    if (!todoFile) {
      LOG_ERROR(-1, "Unable to create file.");
    }
    todoFile.close();
  } else {
    // File exists read data from file.
    loadFile();
    todoFile.close();
  }
  // TODO: populate taskList
}
void Todo::addTask(Hash hash, std::string title, std::string disc,
                   TaskState state, TaskPriority prio) {
  auto task = Task(nextTaskId++, title, disc, state, prio);
  taskMap[hash].push_back(task);
  taskIDToTimeHash.insert(std::make_pair(task.getId(), hash));
}
void Todo::addTask(Time taskTime, std::string title, std::string desc,
                   TaskState state, TaskPriority prio) {
  auto thisTask = Task(nextTaskId++, title, desc, state, prio);
  uint64_t hash = taskTime.getHash();
  taskMap[hash].push_back(thisTask);
  taskIDToTimeHash.insert(std::make_pair(thisTask.getId(), hash));
}
void Todo::dumpTasks() {
  // TODO: Trucate file before writing.
  todoFile.open(filePath.c_str(), std::ios::out | std::ios::trunc);

  LOG_INFO("Todo object state");
  LOG_INFO("nextTaskId: %d", nextTaskId);
  LOG_INFO("taskMap:");
  for (auto it = taskMap.begin(); it != taskMap.end(); ++it) {
    LOG_INFO("  hash: %lu", it->first);

    todoFile << "HASH " << it->first << std::endl;
    todoFile << "START_TASK" << std::endl;

    for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
      todoFile << "TASK\n" << *it2;
      LOG_INFO("    taskId: %d, Title: %s", it2->getId(),
               it2->getTitle().c_str());
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
void Todo::loadFile() {
  std::string flag = "STR";
  todoFile.seekg(0, std::ios::beg);
  while (flag != "EOF") {
    if (flag == "HASH") {
      Hash taskHash;
      todoFile >> taskHash;
      todoFile >> flag;
      if (flag != "START_TASK") {
        LOG_ERROR(-1, "task list block not found -- %s");
      }
      todoFile >> flag;
      while (flag != "END_TASK") {
        if (flag != "TASK") {
          LOG_INFO("%s", flag.c_str());
          LOG_ERROR(-1, "TASK block not found");
        }
        todoFile >> flag;
        std::string title = "";
        while (flag != "TITLE_E") {
          if (!title.empty()) title += ' ';
          title += flag;
          todoFile >> flag;
        }
        todoFile >> flag;
        std::string disc = "";
        while (flag != "DISC_E") {
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
}
std::list<uint32_t> Task::idList;
}  // namespace todo
