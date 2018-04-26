#ifndef RUN_HPP
#define RUN_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <chrono>

#include "filter.hpp"
#include "filter_statistic.hpp"
#include "spatial_convolution.hpp"
#include "frequency_convolution.hpp"

class Run
{
  public:
    Run() : iterations(0), spatial(false), separable(false), frequency(false){}
    bool Load(std::string runFilePath);
    void LoadFromStream(std::ifstream &runStream);
    void Print();
    bool Print(std::string statisticsFilePath);
    void PrintToStream(std::ostream &statisticsStream);
    void setFilters(std::map<std::string, Filter> _filters,
                    std::vector<std::string> _filtersInsertOrder)
                    { filters = _filters; filtersInsertOrder = _filtersInsertOrder; }
    void InitFilterStatistics();
    void AddImagePath(std::string path);
    std::vector<std::string> GetImagePaths();
    void SetIterations(int iterations);
    void SetConvolutions(bool regular_spatial, bool separable_spatial, bool frequency);
    bool Start(bool show);
    bool isSeparable(cv::Mat kernel, cv::Mat &kernelX, cv::Mat &kernelY);
    std::map<std::string, FilterStatistic> statistics;
    ~Run(){}
  private:
    std::vector<std::string> imagePaths;
    int iterations;
    bool spatial;
    bool separable;
    bool frequency;
    bool dirSelected;
    std::map<std::string, Filter> filters;
    std::vector<std::string> filtersInsertOrder;
};

#endif
