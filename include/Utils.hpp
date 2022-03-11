#pragma once

#include <sys/types.h>

#include <cstdint>
#include <cstdio>
#include <ctime>
#include <string>
#include <type_traits>
#include <vector>

#include "ErrorCode.hpp"

namespace todo {
enum eDay { Mon, Tue, Wed, Thu, Fri, Sat, Sun };
enum eMonth { Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec };
enum TaskState { TODO, OPEN, DONE };
enum TaskPriority { LOW, MEDIUM, HIGH, URGENT };

typedef uint64_t Hash;

std::vector<std::string> getTocken(std::string inString,
                                   const char delim = ' ');
class hashComparator {
 public:
  bool operator()(const Hash& a, const Hash& b) {
    return (a / 10) < (b / 10);
  }
};

class Day {
 private:
  eDay day;

 public:
  Day() { day = Mon; }
  explicit Day(const std::string& dayStr) { *(this) = dayStr; }
  explicit Day(const uint& a) { day = eDay(a - 1); }
  ~Day(){}
  uint get() const { return day + 1; }
  void operator=(const std::string& inStr);

};

class Month {
 private:
  eMonth month;

 public:
  Month() { month = Jan; }
   explicit Month(const uint& a) { month = eMonth(a - 1); }
   explicit Month(const std::string inStr) { *(this) = inStr; }
   ~Month() {}
   uint get() const { return month + 1; }
   // void operator=(const uint8_t a) {
   //   month = eMonth(a-1);
   // }
   void operator=(const std::string inStr);
};

class Time {
 private:
  uint32_t year;
  Month month;
  uint32_t day;
  Day weekDay;
  uint32_t hh, mm, ss;

 public:
  void dumpToTerminal() {
    // YYYYMMDDWHHMMSS
    LOG_INFO("%d %d %d %d %d %d %d\n", this->year, this->month.get(), this->day,
           this->weekDay.get(), this->hh, this->mm, this->ss);
  }
  ~Time(){}
  Time(uint8_t DD, uint8_t MM, uint16_t YYYY, uint8_t hours, uint8_t minutes,
       uint8_t seconds = 0);
  void updateWeekDay();
  Time(const std::string& inString);
  Time(){}
  Hash getHash() const;

  Time& operator=(const Time a) {
    this->year = a.year;
    this->month = a.month;
    this->weekDay = a.weekDay;
    this->day = a.day;
    this->hh = a.hh;
    this->mm = a.mm;
    this->ss = a.ss;
    return *(this);
  }
};

}  // namespace todo
