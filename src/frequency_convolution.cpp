#include "frequency_convolution.hpp"

void FrequencyConvolution::FFT()
{
  cv::Size optimalSize;

  optimalSize.width = cv::getOptimalDFTSize(this->src.cols);
  optimalSize.height = cv::getOptimalDFTSize(this->src.rows);

  this->srcPadded = cv::Mat::zeros(optimalSize, CV_32FC1);
  this->src.convertTo(srcPadded(cv::Rect(0,0, this->src.cols, this->src.rows)), this->srcPadded.type());

  this->filterPadded = cv::Mat::zeros(optimalSize, CV_32FC1);
  this->filter.convertTo(srcPadded(cv::Rect(0,0, this->filter.cols, this->filter.rows)), this->filterPadded.type());

  cv::dft(this->srcPadded, this->spectrumImgCCS, 0, this->src.rows);
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
  cv::idft(this->spectrumImgCCS, this->srcPadded, cv::DFT_SCALE, this->src.rows);
  cv::normalize(this->srcPadded, this->srcPadded, 0, 1, CV_MINMAX);

  this->srcPadded(cv::Rect(0,0, this->src.cols, this->src.rows)).convertTo(this->dst, this->src.type(), 255, 0);
  return;
}
