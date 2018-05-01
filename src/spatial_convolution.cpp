#include "../include/spatial_convolution.hpp"

using namespace cv;

void SpatialConvolution::Regular()
{
    Size paddedSize;
    paddedSize.width = this->src.cols + this->filter.cols;
    paddedSize.height = this->src.rows + this->filter.rows;
    Mat paddedSrc = Mat::zeros(paddedSize, this->dst.type());
    Mat paddedDst(paddedSize, this->dst.type());
    this->src.copyTo(paddedSrc(Rect(this->filter.cols/2, this->filter.rows/2, this->src.cols, this->src.rows)));

    for(int row = (this->filter.rows / 2); row < paddedSrc.rows - (this->filter.rows / 2); ++row)
    {
        for(int col = (this->filter.cols / 2); col < paddedSrc.cols - (this->filter.cols / 2); ++col)
        {
            float convolutionSum = 0;
            for (int rowk = -(this->filter.rows / 2); rowk <= (this->filter.rows / 2); ++rowk)
            {
                for (int colk = -(this->filter.cols / 2); colk <= (this->filter.cols / 2); ++colk)
                {
                    convolutionSum += paddedSrc.at<float>(row + rowk, col + colk) * this->filter.at<float>(rowk + (this->filter.rows / 2), colk + (this->filter.cols / 2));
                }
            }
            paddedDst.at<float>(row, col) = convolutionSum;
        }
    }

    paddedDst(Rect(this->filter.cols/2, this->filter.rows/2, this->src.cols, this->src.rows)).copyTo(this->dst);
    return;
}

void SpatialConvolution::Separable()
{
    Size paddedSize;
    paddedSize.width = this->src.cols + this->kernelX.cols;
    paddedSize.height = this->src.rows + this->kernelY.rows;
    Mat paddedSrc = Mat::zeros(paddedSize, this->dst.type());
    Mat paddedDst(paddedSize, this->dst.type());
    this->src.copyTo(paddedSrc(Rect(this->kernelX.cols/2, this->kernelY.rows/2, this->src.cols, this->src.rows)));

    for(int row = (this->kernelY.rows / 2); row < paddedSrc.rows - (this->kernelY.rows / 2); ++row)
    {
        for(int col = (this->kernelX.cols / 2); col < paddedSrc.cols - (this->kernelX.cols / 2); ++col)
        {
            float convolutionSum = 0;
            for (int colk = -(this->kernelX.cols / 2); colk <= (this->kernelX.cols / 2); ++colk)
            {
                convolutionSum += paddedSrc.at<float>(row, col + colk) * this->kernelX.at<float>(0, colk + (this->kernelX.cols / 2));
            }
            paddedDst.at<float>(row, col) = convolutionSum;
        }
    }

    paddedDst.copyTo(paddedSrc);

    for(int col = (this->kernelX.cols / 2); col < paddedSrc.cols - (this->kernelX.cols / 2); ++col)
    {
        for(int row = (this->kernelY.rows / 2); row < paddedSrc.rows - (this->kernelY.rows / 2); ++row)
        {
            float convolutionSum = 0;
            for (int rowk = -(this->kernelY.rows / 2); rowk <= (this->kernelY.rows / 2); ++rowk)
            {
                convolutionSum += paddedSrc.at<float>(row + rowk, col) * this->kernelY.at<float>(rowk + (this->kernelY.rows / 2), 0);
            }
            paddedDst.at<float>(row, col) = convolutionSum;
        }
    }

    paddedDst(Rect(this->kernelX.cols/2, this->kernelY.rows/2, this->src.cols, this->src.rows)).copyTo(this->dst);
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
