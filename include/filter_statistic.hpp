#ifndef FILTER_STATISTIC_HPP
#define FILTER_STATISTIC_HPP

#include <string>
#include <map>

class FilterStatistic
{
  public:
    FilterStatistic(){}
    FilterStatistic(std::string _name) : name(_name){}
    std::string name;
    std::map<std::string, std::chrono::duration<double>> spatialDurations;
    std::map<std::string, std::chrono::duration<double>> separableDurations;
    ~FilterStatistic(){}
};

#endif
