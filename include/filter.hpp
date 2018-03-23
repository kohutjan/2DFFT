#ifndef FILTER_HPP
#define FILTER_HPP

#include <string>
#include <opencv2/imgproc/imgproc.hpp>

class Filter
{
  public:
    Filter(std::string _name, std::string _type, int _stride, cv::Mat _values) :
           name(_name), type(_type), stride(_stride), values(_values){}
    std::string getName() const { return name; }
    std::string getType() const { return type; }
    int getKernelSize() const { return values.rows; }
    int getStride() const { return stride; }
    cv::Mat getValues() const { return values; }
    ~Filter(){}
  private:
    const std::string name;
    const std::string type;
    const int stride;
    const cv::Mat values;
};

#endif
