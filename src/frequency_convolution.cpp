#include "../include/frequency_convolution.hpp"

void FrequencyConvolution::FFTImg()
{
    optimalSize.width = cv::getOptimalDFTSize(this->src.cols + this->filter.cols);
    optimalSize.height = cv::getOptimalDFTSize(this->src.rows + this->filter.rows);

    this->srcPadded = cv::Mat::zeros(optimalSize, CV_32FC1);
    this->src.convertTo(this->srcPadded(cv::Rect(0,0, this->src.cols, this->src.rows)), this->srcPadded.type());
    cv::dft(this->srcPadded, this->spectrumImgCplx, cv::DFT_COMPLEX_OUTPUT, this->src.rows);
}

void FrequencyConvolution::FFTFilter()
{
    this->filterPadded = cv::Mat::zeros(optimalSize, CV_32FC1);
    this->filter.convertTo(this->filterPadded(cv::Rect(0,0, this->filter.cols, this->filter.rows)), this->filterPadded.type());
    cv::dft(this->filterPadded, this->spectrumFilterCplx, cv::DFT_COMPLEX_OUTPUT, this->filter.rows);
    return;
}

void FrequencyConvolution::MUL()
{
    cv::mulSpectrums(this->spectrumImgCplx, this->spectrumFilterCplx, this->spectrumImgCplx, 0, false);
    return;
}

void FrequencyConvolution::IFFT()
{
    cv::dft(this->spectrumImgCplx, this->srcPadded, cv::DFT_REAL_OUTPUT | cv::DFT_SCALE | cv::DFT_INVERSE, this->src.rows + this->filter.rows);
    this->dst = this->srcPadded(cv::Rect(this->filter.cols/2,this->filter.rows/2, this->src.cols, this->src.rows));
    return;
}
