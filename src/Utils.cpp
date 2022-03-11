#include "Utils.hpp"

#include <cstdlib>

namespace todo {

// -------- TIME --------
Time::Time(uint8_t DD, uint8_t MM, uint16_t YYYY, uint8_t hours,
           uint8_t minutes, uint8_t seconds)
    : year(YYYY), month(MM), day(DD), hh(hours), mm(minutes), ss(seconds) {
  updateWeekDay();
}

Time::Time(const std::string& inString) {
  // inString format: <weekday> <mon> <day> <HH:MM:SS> <year>
  std::vector<std::string> split1 = getTocken(inString);
  weekDay = split1[0];
  month = split1[1];
  day = (uint32_t)std::stoi(split1[2]);

  auto hrs = getTocken(split1[3], ':');
  hh = (uint32_t)std::stoi(hrs[0]);
  mm = (uint32_t)std::stoi(hrs[1]);
  ss = (uint32_t)std::stoi(hrs[2]);

  year = (uint32_t)std::stoi(split1[4]);
}

void Time::updateWeekDay() {
  const static uint tempYear[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
  uint32_t m = month.get();
  uint32_t y = year - (m < 3);
  uint32_t w = (y + y / 4 - y / 100 + y / 400 + tempYear[m - 1] + day) % 7;
  weekDay = (Day)w;
}

  Hash Time::getHash() const {
  // Generate hash value
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
// ---------------- DAY ------------------
void Day::operator=(const std::string& inStr) {
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

// ----------------- MONTH -------------------
void Month::operator=(const std::string inStr) {
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

std::vector<std::string> getTocken(std::string inString, const char delim) {
  std::vector<std::string> outArr;
  for (uint32_t i = 0; i < inString.size();) {
    std::string tmp = "";
    while (inString[i] != delim && i < inString.size()) {
      tmp += inString[i];
      ++i;
    }
    while (inString[i] == delim) {
      ++i;
    }
    // LOG_INFO("%s", tmp.c_str());
    outArr.push_back(tmp);
  }
  return outArr;
}

}  // namespace todo
