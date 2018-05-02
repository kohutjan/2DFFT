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
    std::map<std::string, std::chrono::duration<double>> FFTImgDurations;
    std::map<std::string, std::chrono::duration<double>> FFTFilterDurations;
    std::map<std::string, std::chrono::duration<double>> MULDurations;
    std::map<std::string, std::chrono::duration<double>> IFFTDurations;
    std::map<std::string, std::chrono::duration<double>> frequencyDurations;
    std::map<std::string, std::chrono::duration<double>> openCVFilter2DDurations;
    std::map<std::string, std::chrono::duration<double>> openCVSeparableDurations;
    ~FilterStatistic(){}
};

#endif
