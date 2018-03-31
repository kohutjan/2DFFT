#include "spatial_convolution.hpp"

using namespace cv;

void SpatialConvolution::Regular()
{
	for(int y = 0; y < this->src.rows - this->filter.cols; ++y)
	{
		float * dstRowPtr = this->dst.ptr<float>(y + 1);
		for(int x = 0; x < this->src.cols - this->filter.cols; ++x)
		{
			double convolutionSum = 0;
			for (int yk = 0; yk < this->filter.rows; ++yk)
			{
				const float * srcRowPtr = this->src.ptr<float>(y + yk);
				const float * kernelRowPtr = this->src.ptr<float>(yk);
				for (int xk = 0; xk < this->filter.cols; ++xk)
				{
					convolutionSum += ((float) srcRowPtr[x + xk]) * kernelRowPtr[xk];
				}
			}
			dstRowPtr[x + 1] = convolutionSum;
		}
	}
  return;
}

void SpatialConvolution::OpenCVRegular()
{
  filter2D(this->src, this->dst, -1, this->filter);
  return;
}

void SpatialConvolution::Separable()
{
  sepFilter2D(this->src, this->dst, -1, this->kernelX, this->kernelY);
  return;
}
