#ifndef FREQUENCY_CONVOLUTION_HPP
#define FREQUENCY_CONVOLUTION_HPP

#include "convolution.hpp"

class FrequencyConvolution : public Convolution
{
  public:
    FrequencyConvolution(){};
    void FFT();
    void MUL();
    void IFFT();
    ~FrequencyConvolution(){}
};

#endif
