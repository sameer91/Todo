#include "todo.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "ErrorCode.hpp"
#include "Utils.hpp"

namespace todo {

hashComparator hashAlessThanB;

uint32_t Todo::nextTaskId = 1;

Todo::Todo() {
  // Get CurentTime
  syncCurrentTime();

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

void Todo::addTask(Time taskTime, const std::string& title, std::string disc,
                   TaskState state, TaskPriority prio) {
  Hash hash = taskTime.getHash();
  addTask(hash, title, disc, state, prio);
}

void Todo::addTask(Hash hash, const std::string& title, std::string disc,
                   TaskState state, TaskPriority prio) {
  addTask(nextTaskId++, hash, title, disc, state, prio);
}

void Todo::addTask(uint32_t id, Time taskTime, const std::string& title,
                   std::string disc, TaskState state, TaskPriority prio) {
  addTask(id, taskTime.getHash(), title, disc, state, prio);
}

void Todo::addTask(uint32_t id, Hash hash, const std::string& title,
                   std::string disc, TaskState state, TaskPriority prio) {
  if (idList.find(id) != idList.end()) {
    LOG_INFO("Task id [%u] already in use.", id);
    return;
  }
  auto tempTask = std::make_shared<Task>(id, title, disc, state, prio);
  taskList.push_back(tempTask);
  taskMap[hash].push_back(tempTask->getId());
  idToTask[tempTask->getId()] = tempTask;
  taskIDToTimeHash.insert(std::make_pair(id, hash));
  idList.insert(id);
  tempTask.reset();
}

void Todo::removeTask(const uint32_t& taskId) {
  Hash hash = taskIDToTimeHash[taskId];
  auto& tList = taskMap[hash];
  uint32_t idx = 0;
  for (auto it = tList.begin(); it != tList.end(); ++it, idx++) {
    if (taskId == *it) {
      taskIDToTimeHash.erase(taskId);

      // tList[idx].reset();
      idToTask[taskId].reset();
      idToTask.erase(taskId);

      taskMap[hash].erase(it);
      if (tList.empty()) {
        taskMap.erase(hash);
      }
      break;
    }
  }
  idx = 0;
  for (auto it : taskList) {
    if (taskId == it->getId()) {
      taskList[idx].reset();
      taskList.erase(taskList.begin() + idx);
      idList.erase(taskId);
      LOG_INFO("Removed Task [%u]", taskId);
      return;
    }
    idx++;
  }
  LOG_INFO("Could not find Task(%u) -- unable to remove from obj.", taskId);
}

std::vector<Task> Todo::getUpcomingTasks(Time& fromTime, uint32_t count) {
  std::vector<Task> returnList;
  Hash startTime = fromTime.getHash();
  syncCurrentTime();
  if (hashAlessThanB(startTime, currentTime.getHash())) {
    LOG_INFO("startTime less than current time");
    return returnList;
  }
  auto itr = taskMap.begin();
  while (hashAlessThanB(itr->first, currentTime.getHash())) {
    itr++;
  }
  for (; itr != taskMap.end() && count > 0; itr++) {
    LOG_INFO("tashHash %lu", itr->first);
    auto& tList = itr->second;
    for (auto id : tList) {
      LOG_INFO("  TaskID: %u", id);
      auto task = idToTask[id].lock();
      LOG_INFO("Owner %lu", task.use_count());
      if (task->getState() != DONE && count > 0) {
        Task tmp(*task);
        // tmp.deep_copy(task);
        returnList.push_back(tmp);
        count--;
      }
    }
  }
  return returnList;
}

void Todo::updateTaskState(const uint32_t& id, const TaskState& state) {
  if (idToTask.find(id) != idToTask.end()) {
    idToTask[id].lock()->setState(state);
    return;
  }
  LOG_INFO("Could not find Task(%u) -- failed to mark as s[%d]", id, state);
}

void Todo::updateTaskPriority(const uint32_t& id, const TaskPriority& prio) {
  if (idToTask.find(id) != idToTask.end()) {
    idToTask[id].lock()->setPriority(prio);
  }
  LOG_INFO("Could not find Task(%u) -- failed to mark as p[%d]", id, prio);
}

void Todo::syncCurrentTime() { currentTime = getCurrentTime(); }

void Todo::dumpToFile() {
  // Trucate file before writing.
  todoFile.open(filePath.c_str(),
                std::ios::out | std::ios::trunc | std::ios::binary);

  LOG_INFO("Todo object state(To file)");
  LOG_INFO("nextTaskId: %u", nextTaskId);
  LOG_INFO("taskMap:");
  for (auto it = taskMap.begin(); it != taskMap.end(); ++it) {
    LOG_INFO("  hash: %lu", it->first);

    todoFile << "HASH " << it->first << std::endl;
    todoFile << "START_TASK" << std::endl;

    for (auto it2 : it->second) {
      const auto& task = idToTask[it2].lock();

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
  LOG_INFO("nextTaskId: %u", nextTaskId);
  LOG_INFO("taskMap:");
  for (auto it = taskMap.begin(); it != taskMap.end(); ++it) {
    LOG_INFO("  hash: %lu", it->first);
    for (auto it2 : it->second) {
      const auto& task = idToTask[it2].lock();
      LOG_INFO("    taskId: %d, Title: '%s' s[%d] p[%d] Disc: '%s', ",
               task->getId(), task->getTitle().c_str(), task->getState(),
               task->getPriority(), task->getDisc().c_str());
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
        LOG_ERROR(-1, "task list block not found -- %s", flag.c_str());
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

// --------------------------
Time getCurrentTime() {
  time_t t = time(NULL);
  std::string timeStr = std::ctime(&t);
  LOG_INFO("Current system time: %s", timeStr.c_str());

  return Time(timeStr);
}

}  // namespace todo
