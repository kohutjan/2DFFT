#include "../include/spatial_convolution.hpp"

using namespace cv;

void SpatialConvolution::Regular()
{
    for(int y = this->filter.rows / 2; y < this->src.rows - (this->filter.rows - 2); ++y)
	{
        for(int x = this->filter.cols / 2; x < this->src.cols - (this->filter.cols - 2); ++x)
		{
			double convolutionSum = 0;
            for (int yk = -(this->filter.rows / 2); yk < (this->filter.rows / 2); ++yk)
			{
                for (int xk = -(this->filter.cols / 2); xk < (this->filter.cols / 2); ++xk)
				{
                    convolutionSum += this->src.at<float>(y + yk, x + xk) * this->filter.at<float>(yk + (this->filter.rows / 2), xk + (this->filter.cols / 2));
				}
			}
            this->dst.at<float>(y, x) = convolutionSum;
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
