#ifndef RUN_HPP
#define RUN_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#include "filter.hpp"
#include "filter_statistic.hpp"

class Run
{
  public:
    Run() : iterations(0){}
    bool Load(std::string runFilePath);
    void LoadFromStream(std::ifstream &runStream);
    void setFilters(std::map<std::string, Filter> _filters) { filters = _filters; }
    void InitFilterStatistics();
    bool Start();
    ~Run(){}
  private:
    std::vector<std::string> imagePaths;
    int iterations;
    std::map<std::string, Filter> filters;
    std::map<std::string, FilterStatistic> statistics;
};

#endif
