#include "../include/spatial_convolution.hpp"

using namespace cv;

void SpatialConvolution::Regular()
{
    for(int row = this->filter.rows / 2; row < this->src.rows - (this->filter.rows / 2); ++row)
	{
        for(int col = this->filter.cols / 2; col < this->src.cols - (this->filter.cols / 2); ++col)
		{
			double convolutionSum = 0;
            for (int rowk = -(this->filter.rows / 2); rowk < (this->filter.rows / 2); ++rowk)
			{
                for (int colk = -(this->filter.cols / 2); colk < (this->filter.cols / 2); ++colk)
				{
                    convolutionSum += this->src.at<float>(row + rowk, col + colk) * this->filter.at<float>(rowk + (this->filter.rows / 2), colk + (this->filter.cols / 2));
				}
			}
            this->dst.at<float>(row, col) = convolutionSum;
		}
	}
    return;
}

void SpatialConvolution::Separable()
{
    for(int row = (this->kernelY.rows / 2); row < this->src.rows - (this->kernelY.rows / 2); ++row)
    {
        for(int col = (this->kernelX.cols / 2); col < this->src.cols - (this->kernelX.cols / 2); ++col)
        {
            double convolutionSum = 0;
            for (int colk = -(this->kernelX.cols / 2); colk < (this->kernelX.cols / 2); ++colk)
            {
                convolutionSum += this->src.at<float>(row, col + colk) * this->kernelX.at<float>(0, colk + (this->filter.cols / 2));
            }
            this->dst.at<float>(row, col) = convolutionSum;
        }
    }

    for(int col = (this->kernelX.cols / 2); col < this->dst.cols - (this->kernelX.cols / 2); ++col)
    {
        for(int row = (this->kernelY.rows / 2); row < this->dst.rows - (this->kernelY.rows / 2); ++row)
        {
            double convolutionSum = 0;
            for (int rowk = -(this->kernelY.rows / 2); rowk < (this->kernelY.rows / 2); ++rowk)
            {
                convolutionSum += this->dst.at<float>(row + rowk, col) * this->kernelY.at<float>(rowk + (this->filter.rows / 2), 0);
            }
            this->dst.at<float>(row, col) = convolutionSum;
        }
    }

    return;
}

void SpatialConvolution::OpenCVRegular()
{
  filter2D(this->src, this->dst, -1, this->filter);
  return;
}

void SpatialConvolution::OpenCVSeparable()
{
  sepFilter2D(this->src, this->dst, -1, this->kernelX, this->kernelY);
  return;
}
