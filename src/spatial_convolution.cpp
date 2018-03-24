#include "spatial_convolution.hpp"

using namespace cv;

void SpatialConvolution::Regular()
{
  filter2D(this->src, this->dst, -1, this->filter);
  return;
}

void SpatialConvolution::Separable()
{
  sepFilter2D(this->src, this->dst, -1, this->kernelX, this->kernelY);
  return;
}
