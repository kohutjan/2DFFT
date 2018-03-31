#ifndef SPATIAL_CONVOLUTION_HPP
#define SPATIAL_CONVOLUTION_HPP

#include "convolution.hpp"

class SpatialConvolution : public Convolution
{
  public:
    SpatialConvolution(){};
    void Regular();
    void OpenCVRegular();
    void Separable();
    using Convolution::setData;
    void setData(cv::Mat _src, cv::Mat _dst, cv::Mat _kernelX, cv::Mat _kernelY)
    {
      src = _src;
      dst = _dst;
      kernelX = _kernelX;
      kernelY = _kernelY;
    }
    ~SpatialConvolution(){}

  protected:
    cv::Mat kernelX;
    cv::Mat kernelY;
};

#endif
