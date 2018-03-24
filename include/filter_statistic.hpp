#ifndef FILTER_STATISTIC_HPP
#define FILTER_STATISTIC_HPP

#include <string>

class FilterStatistic
{
  public:
    FilterStatistic(){}
    FilterStatistic(std::string _name) : name(_name){}
    ~FilterStatistic(){}
  private:
    std::string name;
};

#endif
