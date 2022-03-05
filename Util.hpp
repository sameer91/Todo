#pragma once

#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

#include "ErrorCode.hpp"
namespace Todo {

    enum eDay { Mon, Tue, Wed, Thu, Fri, Sat, Sun };
    enum eMonth { Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec };

    class Day {
        private:
            eDay day;

        public:
            Day() { day = Mon; }
            Day(std::string dayStr) { *(this) = dayStr; }
            ~Day() {};

            int get() {
                return day;
            }
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
            Month(const std::string inStr) { *(this) = inStr; }
            ~Month() {};
            int get() {
                return month;
            }
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

    std::vector<std::string> getTocken(std::string inString,
            const char delim = ' ');
    class Time {
        private:
            uint32_t year;
            Month month;
            uint32_t day;
            Day weekDay;
            uint32_t hh, mm, ss;

        public:
            void dumpToTerminal() {
                std::printf("\nyear: %d,\nmonth: %d,\nday: %d,\nweekDay: %d,\nhh::mm::ss: %d::%d::%d\n", this->year, this->month.get(),
                        this->day, this->weekDay.get(), this->hh, this->mm, this->ss);
            }
            ~Time() {} ;
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
            Time() {};
            Time& operator=(Time* a) {
                this->year = a->year;
                this->month = a->month;
                this->weekDay = a->weekDay;
                this->day = a->day;
                this->hh = a->hh;
                this->mm = a->mm;
                this->ss = a->ss;
                return *(this);
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

}  // namespace Todo
