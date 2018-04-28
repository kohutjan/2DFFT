#include "../include/frequency_convolution.hpp"


void FrequencyConvolution::FFTImg()
{
    optimalSize.width = cv::getOptimalDFTSize(this->src.cols + this->filter.cols);
    optimalSize.height = cv::getOptimalDFTSize(this->src.rows + this->filter.rows);

    this->srcPadded = cv::Mat::zeros(optimalSize, CV_32FC1);
    this->src.convertTo(this->srcPadded(cv::Rect(0,0, this->src.cols, this->src.rows)), this->srcPadded.type());
    cv::dft(this->srcPadded, this->spectrumImgCCS, 0, this->src.rows);
}

void FrequencyConvolution::FFTFilter()
{
    this->filterPadded = cv::Mat::zeros(optimalSize, CV_32FC1);
    this->filter.convertTo(this->filterPadded(cv::Rect(0,0, this->filter.cols, this->filter.rows)), this->filterPadded.type());
    cv::dft(this->filterPadded, this->spectrumFilterCCS, 0, this->filter.rows);
    return;
}

void FrequencyConvolution::MUL()
{
    cv::mulSpectrums(this->spectrumImgCCS, this->spectrumFilterCCS, this->spectrumImgCCS, 0, false);
    return;
}

void FrequencyConvolution::IFFT()
{
    cv::idft(this->spectrumImgCCS, this->srcPadded, cv::DFT_SCALE, this->src.rows + this->filter.rows);
    cv::normalize(this->srcPadded, this->srcPadded, 0, 1, CV_MINMAX);
    this->srcPadded(cv::Rect(this->filter.cols/2,this->filter.rows/2, this->src.cols, this->src.rows)).convertTo(this->dst, this->src.type(), 255, 0);
    return;
}
