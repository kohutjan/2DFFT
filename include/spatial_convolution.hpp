#ifndef SPATIAL_CONVOLUTION_HPP
#define SPATIAL_CONVOLUTION_HPP

#include "convolution.hpp"

class SpatialConvolution : public Convolution
{
  public:
    SpatialConvolution(){};
    void Regular();
    void Separable();
    ~SpatialConvolution(){}
};

#endif
