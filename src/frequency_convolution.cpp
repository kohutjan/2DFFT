#include "../include/frequency_convolution.hpp"

void FrequencyConvolution::FFTImg()
{
    optimalSize.width = cv::getOptimalDFTSize(this->src.cols + this->filter.cols);
    optimalSize.height = cv::getOptimalDFTSize(this->src.rows + this->filter.rows);

    this->srcPadded = cv::Mat::zeros(optimalSize, CV_32FC1);
    this->src.convertTo(this->srcPadded(cv::Rect(0,0, this->src.cols, this->src.rows)), this->srcPadded.type());
    cv::dft(this->srcPadded, this->spectrumImgCCS, cv::DFT_COMPLEX_OUTPUT, this->src.rows);
}

void FrequencyConvolution::FFTFilter()
{
    this->filterPadded = cv::Mat::zeros(optimalSize, CV_32FC1);
    this->filter.convertTo(this->filterPadded(cv::Rect(0,0, this->filter.cols, this->filter.rows)), this->filterPadded.type());
    cv::dft(this->filterPadded, this->spectrumFilterCCS, cv::DFT_COMPLEX_OUTPUT, this->filter.rows);
    return;
}

void FrequencyConvolution::MUL()
{
    cv::mulSpectrums(this->spectrumImgCCS, this->spectrumFilterCCS, this->spectrumImgCCS, 0, false);
    return;
}


cv::Mat FrequencyConvolution::SpectrumMagnitude(cv::Mat specCplx)
{
    cv::Mat specMag, planes[2];
    cv::split(specCplx, planes);
    cv::magnitude(planes[0], planes[1], planes[0]);
    cv::log( (planes[0] + cv::Scalar::all(1)), specMag );
    cv::normalize( specMag, specMag, 0, 1, CV_MINMAX );
    return specMag;
}

void FrequencyConvolution::rearrangeSpectrum( cv::Mat& s )
{
    int cx = s.cols/2;
    int cy = s.rows/2;

    cv::Mat q0(s, cv::Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    cv::Mat q1(s, cv::Rect(cx, 0, cx, cy));  // Top-Right
    cv::Mat q2(s, cv::Rect(0, cy, cx, cy));  // Bottom-Left
    cv::Mat q3(s, cv::Rect(cx, cy, cx, cy)); // Bottom-Right

    cv::Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

cv::Mat FrequencyConvolution::GetSpectrumImg(cv::Mat input)
{
    cv::Mat spectrum;
    cv::Size dftSize;
    dftSize.width = cv::getOptimalDFTSize(input.cols);
    dftSize.height = cv::getOptimalDFTSize(input.rows);
    cv::Mat inputPadded;
    inputPadded = cv::Mat::zeros(dftSize, CV_32FC1);
    input.convertTo(inputPadded(cv::Rect(0, 0, input.cols, input.rows)), inputPadded.type());
    cv::dft(inputPadded, spectrum, cv::DFT_COMPLEX_OUTPUT, input.rows);
    this->rearrangeSpectrum(spectrum);
    cv::Mat spectrumImg;
    this->SpectrumMagnitude(spectrum).convertTo(spectrumImg, CV_8UC1, 255);
    return spectrumImg;
}

void FrequencyConvolution::IFFT()
{
    cv::idft(this->spectrumImgCCS, this->srcPadded, cv::DFT_REAL_OUTPUT+cv::DFT_SCALE, this->src.rows + this->filter.rows);
    cv::normalize(this->srcPadded, this->srcPadded, 0, 1, CV_MINMAX);
    this->srcPadded(cv::Rect(this->filter.cols/2,this->filter.rows/2, this->src.cols, this->src.rows)).convertTo(this->dst, this->src.type(), 255, 0);

    cv::Mat resultImshow = this->GetSpectrumImg(this->srcPadded(cv::Rect(this->filter.cols/2,this->filter.rows/2, this->src.cols, this->src.rows)));
    imshow("result after idft", resultImshow);

    return;
}
