#include "spatial_convolution.hpp"

using namespace cv;

void SpatialConvolution::Regular()
{
  filter2D(this->src, this->dst, -1, this->filter);
  return;
}

void SpatialConvolution::Separable()
{
  return;
}
