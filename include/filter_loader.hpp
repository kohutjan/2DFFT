#ifndef FILTER_LOADER_HPP
#define FILTER_LOADER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include "filter.hpp"

class FilterLoader
{
  public:
    FilterLoader(){}
    bool Load(std::string filtersFilePath);
    void LoadFromStream(std::ifstream &filtersStream);
    cv::Mat LoadMean(int kernelSize, std::ifstream &filtersStream);
    cv::Mat LoadGauss(int kernelSize, std::ifstream &filtersStream);
    cv::Mat LoadCustom(int kernelSize, std::ifstream &filtersStream);
    void PrintFilter(Filter filter);
    std::map<std::string, Filter> getFilters() { return filters; }
    ~FilterLoader(){}
  private:
    std::map<std::string, Filter> filters;
};

#endif
