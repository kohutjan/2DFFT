#ifndef CONVOLUTION_HPP
#define CONVOLUTION_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


class Convolution
{
  public:
    void setData(cv::Mat _src, cv::Mat _dst, cv::Mat _filter)
    {
      src = _src;
      dst = _dst;
      filter = _filter;
    }
    ~Convolution(){}
  protected:
    Convolution(){}
    cv::Mat src;
    cv::Mat dst;
    cv::Mat filter;
};

#endif
