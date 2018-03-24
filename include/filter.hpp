#ifndef FILTER_HPP
#define FILTER_HPP

#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>


class Filter
{
  public:
    Filter(){};
    Filter(std::string _name, std::string _type, int _stride, cv::Mat _values) :
           name(_name), type(_type), stride(_stride), values(_values){}
    std::string getName() const { return name; }
    std::string getType() const { return type; }
    int getKernelSize() const { return values.rows; }
    int getStride() const { return stride; }
    cv::Mat getValues() const { return values; }
    ~Filter(){}
  private:
    std::string name;
    std::string type;
    int stride;
    cv::Mat values;
};

#endif
