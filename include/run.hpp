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
    Run(){}
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
    std::vector<std::string> GetImagePaths()
    {
      return imagePaths;
    }
    void SetIterations(int _iterations)
    {
      iterations = _iterations;
    }
    void SetConvolutions(bool _spatial, bool _separable, bool _frequency)
    {
      spatial = _spatial;
      separable = _separable;
      frequency = _frequency;
    }
    void SetOpenCVConvolutions(bool _openCVFilter2D, bool _openCVSeparable)
    {
      openCVFilter2D = _openCVFilter2D;
      openCVSeparable = _openCVSeparable;
    }
    void SetShow()
    {
      show = true;
    }
    void SetVerbose()
    {
      verbose = true;
    }
    bool Start();
    bool isSeparable(cv::Mat kernel, cv::Mat &kernelX, cv::Mat &kernelY);
    std::map<std::string, FilterStatistic> statistics;
    ~Run(){}
  private:
    std::vector<std::string> imagePaths;
    int iterations = 0;
    bool show = false;
    bool verbose = false;
    bool spatial = false;
    bool separable = false;
    bool frequency = false;
    bool frequencyImage = false;
    bool frequencyFilter = false;
    bool frequencyMul = false;
    bool frequencyInverse = false;
    bool openCVFilter2D = false;
    bool openCVSeparable = false;
    std::map<std::string, Filter> filters;
    std::vector<std::string> filtersInsertOrder;
};

#endif
