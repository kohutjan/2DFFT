#ifndef FILTER_CONTAINER_HPP
#define FILTER_CONTAINER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "filter.hpp"

class FilterContainer
{
  public:
    FilterContainer(){}
    bool Load(std::string filtersFilePath);
    void LoadFromStream(std::ifstream &filtersStream);
    cv::Mat LoadMean(int kernelSize, std::ifstream &filtersStream);
    cv::Mat LoadGauss(int kernelSize, std::ifstream &filtersStream);
    cv::Mat LoadCustom(int kernelSize, std::ifstream &filtersStream);
    void PrintFilter(Filter filter);
    std::vector<Filter> filters;
    ~FilterContainer(){}
};

#endif
