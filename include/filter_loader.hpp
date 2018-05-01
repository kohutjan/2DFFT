#ifndef FILTER_LOADER_HPP
#define FILTER_LOADER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#include "filter.hpp"

class FilterLoader
{
  public:
    FilterLoader(){}
    Filter Get2DFilter(std::string name, std::string type, int kernelSize);
    Filter GetSpecFilter(std::string name, std::string type, int radius, cv::Mat img);
    Filter GetMean(std::string name, std::string type, int kernelSize);
    Filter GetGauss(std::string name, std::string type, int kernelSize, float sigma);
    Filter GetSobel(std::string name, std::string type, char dir);
    Filter GetDoG(std::string name, std::string type, int kernelSize);
    Filter GetLowPass(std::string name, std::string type, int radius, cv::Mat img);
    Filter GetHighPass(std::string name, std::string type, int radius, cv::Mat img);
    Filter GetLowGauss(std::string name, std::string type, int radius, cv::Mat img);
    Filter GetHighGauss(std::string name, std::string type, int radius, cv::Mat img);
    bool Load(std::string filtersFilePath);
    void LoadFromStream(std::ifstream &filtersStream);
    cv::Mat LoadMean(int kernelSize);
    cv::Mat LoadGauss(int kernelSize, std::ifstream &filtersStream);
    cv::Mat LoadCustom(int kernelSize, std::ifstream &filtersStream);
    void PrintFilter(Filter filter);
    std::map<std::string, Filter> getFilters() { return filters; }
    std::vector<std::string> getFiltersInsertOrder() {return filtersInsertOrder; }
    ~FilterLoader(){}
  private:
    std::map<std::string, Filter> filters;
    std::vector<std::string> filtersInsertOrder;

    void rearrangeSpectrum(cv::Mat& s);
};

#endif
