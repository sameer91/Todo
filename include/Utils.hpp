#pragma once

#include <sys/types.h>

#include <cstdint>
#include <cstdio>
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
struct timeComparator {
  bool operator()(const uint64_t& a, const uint64_t& b) {
    return (a / 10) < (b / 10);
  }
};

class Day {
 private:
  eDay day;

 public:
  Day() { day = Mon; }
  Day(std::string dayStr) { *(this) = dayStr; }
  Day(const uint8_t a) { day = eDay(a - 1); }
  ~Day(){};
  int get() { return day + 1; }
  void operator=(const std::string inStr) {
    switch (inStr[0]) {
      case 'M':
      case 'm':
        day = Mon;
        break;
      case 'T':
      case 't': {
        switch (inStr[1]) {
          case 'U':
          case 'u':
            day = Tue;
            break;
          case 'H':
          case 'h':
            day = Thu;
            break;
        }
        break;
      }
      case 'W':
      case 'w':
        day = Wed;
        break;
      case 'F':
      case 'f':
        day = Fri;
        break;
      case 'S':
      case 's':
        switch (inStr[1]) {
          case 'A':
          case 'a':
            day = Sat;
            break;
          case 'U':
          case 'u':
            day = Sun;
            break;
        }
        break;
      default:
        LOG_ERROR(0, "Day");
    }
  }
};

class Month {
 private:
  eMonth month;

 public:
  Month() { month = Jan; }
  Month(uint8_t a) { month = eMonth(a - 1); }
  Month(const std::string inStr) { *(this) = inStr; }
  ~Month(){};
  int get() { return month + 1; }
  // void operator=(const uint8_t a) {
  //   month = eMonth(a-1);
  // }
  void operator=(const std::string inStr) {
    LOG_INFO("%s", inStr.c_str());
    switch (inStr[0]) {
      case 'J':
      case 'j':
        if (inStr[1] == 'a' || inStr[1] == 'A') {
          month = Jan;
        } else if (inStr[2] == 'n' || inStr[1] == 'N') {
          month = Jun;
        } else if (inStr[2] == 'l' || inStr[2] == 'L') {
          month = Jul;
        } else {
          LOG_ERROR(-1, "Jan-Jun-Jul");
        }
        break;
      case 'F':
      case 'f':
        month = Feb;
        break;
      case 'M':
      case 'm':
        if (inStr[2] == 'r' || inStr[2] == 'R') {
          month = Mar;
        } else if (inStr[2] == 'y' || inStr[2] == 'Y') {
          month = May;
        } else {
          LOG_ERROR(-1, "Mar-May");
        }
        break;
      case 'A':
      case 'a':
        if (inStr[1] == 'p' || inStr[1] == 'P') {
          month = Apr;
        } else if (inStr[1] == 'u' || inStr[1] == 'U') {
          month = Aug;
        } else {
          LOG_ERROR(-1, "apr-aug");
        }
        break;
      case 'S':
      case 's':
        month = Sep;
        break;
      case 'O':
      case 'o':
        month = Oct;
        break;
      case 'N':
      case 'n':
        month = Nov;
        break;
      case 'D':
      case 'd':
        month = Dec;
        break;
      default:
        LOG_ERROR(-1, "Month");
    }
  }
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
    printf("%d %d %d %d %d %d %d\n", this->year, this->month.get(), this->day,
           this->weekDay.get(), this->hh, this->mm, this->ss);
  }
  ~Time(){};
  Time(uint8_t DD, uint8_t MM, uint16_t YYYY, uint8_t hh, uint8_t mm,
       uint8_t ss = 0) {
    day = DD;
    month = MM;
    year = YYYY;
    this->hh = hh;
    this->mm = mm;
    this->ss = ss;
    updateWeekDay();
  }
  void updateWeekDay() {
    static int tempYear[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    int m = month.get();
    int y = year - (m < 3);
    int w = (y + y / 4 - y / 100 + y / 400 + tempYear[m - 1] + day) % 7;
    weekDay = w;
  }
  Time(std::string inString) {
    // inString format: <weekday> <mon> <day> <HH:MM:SS> <year>
    std::vector<std::string> split1 = getTocken(inString);
    weekDay = split1[0];
    month = split1[1];
    day = std::stoi(split1[2]);

    auto hrs = getTocken(split1[3], ':');
    hh = std::stoi(hrs[0]);
    mm = std::stoi(hrs[1]);
    ss = std::stoi(hrs[2]);

    year = std::stoi(split1[4]);
  }
  Time(){};
  uint64_t getHash() {
    // Generate the formate
    uint64_t hashValue = 0;

    hashValue += year;

    hashValue *= 100;
    hashValue += month.get();

    hashValue *= 100;
    hashValue += day;

    hashValue *= 100;
    hashValue += hh;

    hashValue *= 100;
    hashValue += mm;

    hashValue *= 100;
    hashValue += ss;

    hashValue *= 10;
    hashValue += weekDay.get();

    return hashValue;
  }
  Time& operator=(Time* a) {
    *(this) = *(a);
    return *this;
  }
  Time& operator=(Time& a) {
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
