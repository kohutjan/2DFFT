#ifndef FREQUENCY_CONVOLUTION_HPP
#define FREQUENCY_CONVOLUTION_HPP

#include "convolution.hpp"

class FrequencyConvolution : public Convolution
{
  public:
    FrequencyConvolution(){}
    void FFTFilter();
    void FFTImg();
    void MUL();
    void IFFT();
    ~FrequencyConvolution(){}

  private:
    cv::Size optimalSize;
    cv::Mat srcPadded;
    cv::Mat filterPadded;
    cv::Mat spectrumImgCCS;
    cv::Mat spectrumFilterCCS;

    cv::Mat SpectrumMagnitude(cv::Mat specCplx);
    void rearrangeSpectrum( cv::Mat& s );
    cv::Mat GetSpectrumImg(cv::Mat input);
};

#endif
