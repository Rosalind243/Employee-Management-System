
#include "Date.h"

Date::Date(int d, int m, int y)
{
  day = d;
  month = m;
  year = y; 
}

std::string Date::toString() const
{
  std::ostringstream oss;

  oss << std::setw(2) << std::setfill('0') << day << "/"
      << std::setw(2) << std::setfill('0') << month << "/"
      << year;

  return oss.str();
}
