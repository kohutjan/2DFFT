#include "../include/run.hpp"


using namespace std;
using namespace cv;


bool Run::Start()
{
  SpatialConvolution spatialConvolution;
  FrequencyConvolution frequencyConvolution;

  for (auto& imagePath: this->imagePaths)
  {
    if (this->verbose)
    {
      cout << "Measuring time on " << imagePath << endl;
      cout << "-------------------------------------------------------------" << endl;
    }
    Mat src = imread(imagePath, CV_LOAD_IMAGE_GRAYSCALE);
    if(!src.data)
    {
       cout <<  "Could not open or find the image" << endl;
       return false;
    }
    Mat dst = src.clone();

    for (auto& filterName: this->filtersInsertOrder)
    {
      if (this->verbose)
      {
        cout << "Measuring time for " << filterName << endl;
        cout << "-------------------------------------------------------------" << endl;
      }
      //OpenCV Mat for filter: filter.second

      if (this->spatial)
      {
        //Prepare spatial convolution
        Mat srcF;
        Mat dstF;
        src.convertTo(srcF, CV_32FC1);
        dst.convertTo(dstF, CV_32FC1);
        spatialConvolution.setData(srcF, dstF, this->filters[filterName].getValues());

        //Measure time
        chrono::duration<double, std::milli> duration = std::chrono::milliseconds::zero();
        for (int i = 0; i < this->iterations; ++i)
        {
          auto beginSpatial = chrono::high_resolution_clock::now();
          spatialConvolution.Regular();
          auto endSpatial = chrono::high_resolution_clock::now();
          duration += endSpatial - beginSpatial;
          if (this->verbose)
          {
            if ((i + 1) % 100 == 0)
            {
              cout << (i + 1) << " spatial iterations DONE." << endl;
            }
          }
        }
        if (this->verbose)
        {
          cout << "Spatial iterations DONE." << endl;
        }
        this->statistics[filterName].spatialDurations[imagePath] = duration;

        if (this->show)
        {
          dstF.convertTo(dst, CV_8UC1);
          imshow(filterName + " spatial regular", dst);
        }
      }
      if (this->separable)
      {
        Mat kernelX, kernelY;
        if (isSeparable(this->filters[filterName].getValues(), kernelX, kernelY))
        {
          Mat srcF;
          Mat dstF;
          src.convertTo(srcF, CV_32FC1);
          dst.convertTo(dstF, CV_32FC1);
          spatialConvolution.setData(srcF, dstF, kernelX, kernelY);

          chrono::duration<double, std::milli> duration = std::chrono::milliseconds::zero();
          for (int i = 0; i < this->iterations; ++i)
          {
            auto beginSeparable = chrono::high_resolution_clock::now();
            spatialConvolution.Separable();
            auto endSeparable = chrono::high_resolution_clock::now();
            duration += endSeparable - beginSeparable;
            if (this->verbose)
            {
              if ((i + 1) % 100 == 0)
              {
                cout << (i + 1) << " separable iterations DONE." << endl;
              }
            }
          }
          if (this->verbose)
          {
            cout << "Separable iterations DONE." << endl;
          }

          this->statistics[filterName].separableDurations[imagePath] = duration;

          if (this->show)
          {
            dstF.convertTo(dst, CV_8UC1);
            imshow(filterName + " spatial separable", dst);
          }
        }
      }
      if (this->frequency)
      {
        Mat flipedFilter = this->filters[filterName].getValues().clone();
        frequencyConvolution.setData(src, dst, flipedFilter);

        chrono::duration<double, std::milli> durationFFTImg = std::chrono::milliseconds::zero();
        chrono::duration<double, std::milli> durationFFTFilter = std::chrono::milliseconds::zero();
        chrono::duration<double, std::milli> durationMUL = std::chrono::milliseconds::zero();
        chrono::duration<double, std::milli> durationIFFT = std::chrono::milliseconds::zero();
        chrono::duration<double, std::milli> duration = std::chrono::milliseconds::zero();
        for (int i = 0; i < this->iterations; ++i)
        {
          auto beginFFTImg = chrono::high_resolution_clock::now();
          frequencyConvolution.FFTImg();
          auto endFFTImg = chrono::high_resolution_clock::now();
          auto beginFFTFilter = chrono::high_resolution_clock::now();
          frequencyConvolution.FFTFilter();
          auto endFFTFilter = chrono::high_resolution_clock::now();
          auto beginMUL = chrono::high_resolution_clock::now();
          frequencyConvolution.MUL();
          auto endMUL = chrono::high_resolution_clock::now();
          auto beginIFFT = chrono::high_resolution_clock::now();
          frequencyConvolution.IFFT();
          auto endIFFT = chrono::high_resolution_clock::now();
          durationFFTImg += endFFTImg - beginFFTImg;
          durationFFTFilter += endFFTFilter - beginFFTFilter;
          durationMUL += endMUL - beginMUL;
          durationIFFT += endIFFT - beginIFFT;
          if (this->verbose)
          {
            if ((i + 1) % 100 == 0)
            {
              cout << (i + 1) << " frequency iterations DONE." << endl;
            }
          }
        }
        if (this->verbose)
        {
          cout << "Frequency iterations DONE." << endl;
        }
        duration = durationFFTImg + durationFFTFilter + durationMUL + durationIFFT;
        this->statistics[filterName].FFTImgDurations[imagePath] = durationFFTImg;
        this->statistics[filterName].FFTFilterDurations[imagePath] = durationFFTFilter;
        this->statistics[filterName].MULDurations[imagePath] = durationMUL;
        this->statistics[filterName].IFFTDurations[imagePath] = durationIFFT;
        this->statistics[filterName].frequencyDurations[imagePath] = duration;

        if (this->show)
        {
          imshow(filterName + " frequency", dst);
        }
      }
      if (this->openCVFilter2D)
      {
        //Prepare spatial convolution
        Mat flipedFilter = this->filters[filterName].getValues().clone();
        flip(this->filters[filterName].getValues(), flipedFilter, -1);
        Mat srcF;
        Mat dstF;
        src.convertTo(srcF, CV_32FC1);
        dst.convertTo(dstF, CV_32FC1);
        spatialConvolution.setData(srcF, dstF, flipedFilter);

        //Measure time
        chrono::duration<double, std::milli> duration = std::chrono::milliseconds::zero();
        for (int i = 0; i < this->iterations; ++i)
        {
          auto beginSpatial = chrono::high_resolution_clock::now();
          spatialConvolution.OpenCVRegular();
          auto endSpatial = chrono::high_resolution_clock::now();
          duration += endSpatial - beginSpatial;
          if (this->verbose)
          {
            if ((i + 1) % 100 == 0)
            {
              cout << (i + 1) << " OpenCV_filter2D iterations DONE." << endl;
            }
          }
        }
        if (this->verbose)
        {
          cout << "OpenCV_filter2D iterations DONE." << endl;
        }
        this->statistics[filterName].openCVFilter2DDurations[imagePath] = duration;

        if (this->show)
        {
          dstF.convertTo(dst, CV_8UC1);
          imshow(filterName + " OpenCV filter2D ", dst);
        }
      }
      if (this->openCVSeparable)
      {
        Mat kernelX, kernelY;
        if (isSeparable(this->filters[filterName].getValues(), kernelX, kernelY))
        {
          Mat srcF;
          Mat dstF;
          src.convertTo(srcF, CV_32FC1);
          dst.convertTo(dstF, CV_32FC1);
          spatialConvolution.setData(srcF, dstF, kernelX, kernelY);

          chrono::duration<double, std::milli> duration = std::chrono::milliseconds::zero();
          for (int i = 0; i < this->iterations; ++i)
          {
            auto beginSeparable = chrono::high_resolution_clock::now();
            spatialConvolution.OpenCVSeparable();
            auto endSeparable = chrono::high_resolution_clock::now();
            duration += endSeparable - beginSeparable;
            if (this->verbose)
            {
              if ((i + 1) % 100 == 0)
              {
                cout << (i + 1) << " OpenCV_separable iterations DONE." << endl;
              }
            }
          }
          if (this->verbose)
          {
            cout << "OpenCV_separable iterations DONE." << endl;
          }

          this->statistics[filterName].openCVSeparableDurations[imagePath] = duration;

          if (this->show)
          {
            dstF.convertTo(dst, CV_8UC1);
            imshow(filterName + " OpenCV separable", dst);
          }
        }
      }

      if (this->verbose)
      {
        cout << "-------------------------------------------------------------" << endl;
        cout << endl;
      }
    }
    if (this->show)
    {
      waitKey(0);
    }
  }
  return true;
}

void Run::Print()
{
  cout << "Statistics" << endl;
  cout << "#############################################################" << endl;
  this->PrintToStream(cout);
  cout << "#############################################################" << endl;
  cout << endl;
  cout << endl;
  return;
}

bool Run::Print(string statisticsFilePath)
{
  cout << "Statics file path: " << statisticsFilePath << endl;
  filebuf fb;
  fb.open(statisticsFilePath, ios::out);
  if (fb.is_open())
  {
    ostream statisticsStream(&fb);
    this->PrintToStream(statisticsStream);
    fb.close();
    return true;
  }
  else
  {
    cerr << "Unable to open statistics file." << endl;
    return false;
  }
}

void Run::PrintToStream(ostream &statisticsStream)
{
  statisticsStream.precision(3);
  statisticsStream << scientific;
  statisticsStream << "_____________________________________________________________" << endl;
  statisticsStream << endl;
  statisticsStream << "iterations per image: " << this->iterations << endl;
  statisticsStream << "format: imagePath [spatial_duration] [separable_duration] [frequency_duration]" << endl;
  statisticsStream << "                  [frequency_image_duration] [frequency_filter_duration]" << endl;
  statisticsStream << "                  [frequency_mul_duration] [frequency_inverse_duration]" << endl;
  statisticsStream << "                  [OpenCV_filter2D_duration] [OpenCV_separable_duration]" << endl;
  statisticsStream << "_____________________________________________________________" << endl;
  statisticsStream << endl;

  for (auto& filterName: this->filtersInsertOrder)
  {
    statisticsStream << filterName << " ";
    statisticsStream << this->filters[filterName].getType() << " ";
    statisticsStream << this->filters[filterName].getKernelSize() << endl;
    statisticsStream << "-------------------------------------------------------------" << endl;
    for (auto& imagePath: this->imagePaths)
    {
      statisticsStream << imagePath << " ";
      if (this->spatial)
      {
        double meanDuration = this->statistics[filterName].spatialDurations[imagePath].count() / this->iterations;
        statisticsStream <<  meanDuration << " ";
      }
      if (this->separable)
      {
        //Some filters doesn't have to be separable
        if (!this->statistics[filterName].separableDurations.empty())
        {
          double meanDuration = this->statistics[filterName].separableDurations[imagePath].count() / this->iterations;
          statisticsStream << meanDuration << " ";
        }
        else
        {
          statisticsStream << "unsep ";
        }
      }
      if (this->frequency)
      {
        double meanDuration = this->statistics[filterName].frequencyDurations[imagePath].count() / this->iterations;
        statisticsStream <<  meanDuration << " ";
      }
      if (this->frequencyImage)
      {
        double meanDuration = this->statistics[filterName].FFTImgDurations[imagePath].count() / this->iterations;
        statisticsStream << meanDuration << " ";
      }
      if (this->frequencyFilter)
      {
        double meanDuration = this->statistics[filterName].FFTFilterDurations[imagePath].count() / this->iterations;
        statisticsStream << meanDuration << " ";
      }
      if (this->frequencyMul)
      {
        double meanDuration = this->statistics[filterName].MULDurations[imagePath].count() / this->iterations;
        statisticsStream << meanDuration << " ";
      }
      if (this->frequencyInverse)
      {
        double meanDuration = this->statistics[filterName].IFFTDurations[imagePath].count() / this->iterations;
        statisticsStream << meanDuration << " ";
      }
      if (this->openCVFilter2D)
      {
        double meanDuration = this->statistics[filterName].openCVFilter2DDurations[imagePath].count() / this->iterations;
        statisticsStream << meanDuration << " ";
      }
      if (this->openCVSeparable)
      {
        if (!this->statistics[filterName].openCVSeparableDurations.empty())
        {
          double meanDuration = this->statistics[filterName].openCVSeparableDurations[imagePath].count() / this->iterations;
          statisticsStream << meanDuration << " ";
        }
        else
        {
          statisticsStream << "unsep ";
        }
      }
      statisticsStream << endl;
    }
    statisticsStream << "-------------------------------------------------------------" << endl;
    statisticsStream << endl;
  }
  return;
}

bool Run::Load(string runFilePath)
{
  cout << "Run file path: " << runFilePath << endl;
  ifstream runStream(runFilePath);
  if (runStream.is_open())
  {
    this->LoadFromStream(runStream);
    runStream.close();
    return true;
  }
  else
  {
    cerr << "Unable to open run file." << endl;
    return false;
  }
}

void Run::LoadFromStream(ifstream &runStream)
{
  cout << "Run params:" << endl;
  cout << "#############################################################" << endl;
  while (!runStream.eof())
  {
    string paramName;
    runStream >> paramName;
    if (paramName == "images")
    {
      int numberOfImages;
      runStream >> numberOfImages;
      vector<string> imagePaths(numberOfImages);
      cout << paramName << ": " << numberOfImages << endl;
      for (auto& imagePath: imagePaths)
      {
        runStream >> imagePath;
        cout << imagePath << endl;
      }
      this->imagePaths = imagePaths;
      continue;
    }
    if (paramName == "iterations")
    {
      runStream >> this->iterations;
      cout << paramName << ": " << this->iterations << endl;
      continue;
    }
    if (paramName == "spatial")
    {
      this->spatial = true;
      cout << paramName << endl;
      continue;
    }
    if (paramName == "separable")
    {
      this->separable = true;
      cout << paramName << endl;
      continue;
    }
    if (paramName == "frequency")
    {
      this->frequency = true;
      cout << paramName << endl;
      continue;
    }
    if (paramName == "frequency_image")
    {
      this->frequencyImage = true;
      cout << paramName << endl;
      continue;
    }
    if (paramName == "frequency_filter")
    {
      this->frequencyFilter = true;
      cout << paramName << endl;
      continue;
    }
    if (paramName == "frequency_mul")
    {
      this->frequencyMul = true;
      cout << paramName << endl;
      continue;
    }
    if (paramName == "frequency_inverse")
    {
      this->frequencyInverse = true;
      cout << paramName << endl;
      continue;
    }
    if (paramName == "OpenCV_filter2D")
    {
      this->openCVFilter2D = true;
      cout << paramName << endl;
      continue;
    }
    if (paramName == "OpenCV_separable")
    {
      this->openCVSeparable = true;
      cout << paramName << endl;
      continue;
    }
  }
  cout << "#############################################################" << endl;
  cout << endl;
  cout << endl;
  return;
}

void Run::InitFilterStatistics()
{
  for (auto& filterName: this->filtersInsertOrder)
  {
    this->statistics[filterName] = FilterStatistic(filterName);
  }
}

bool Run::isSeparable(Mat kernel, Mat &kernelX, Mat &kernelY)
{
  Mat u, sigma, vt;
  int rank = 0;

  /*
  Computes the Single-value decomposition of the matrix kernel, which
  separates the input matrix into three matrices U, Sigma and V transposed.
  By multiplying these three matrices we get the original kernel again.
  The sigma matrix is a diagonal matrix the size of kernel, which has
  non-zero values on the diagonal only and zeros everywhere else.
  By counting how many elements of the diagonal are non-zero, we
  can determine the rank of the original matrix kernel. This way we can
  decide whether the kernel is separable (rank == 1) or non-separable
  (rank != 0) and thus perform the appropriate convolution.
  */
  SVD::compute(kernel, sigma, u, vt);
  for (auto it = sigma.begin<float>(); it != sigma.end<float>(); it++) {
    if (*it > 0.00001) //threshold for calculation errors, sigma values are never negative
    {
       rank++;
    }

    if (rank > 1)
    {
        break;
    }
  }

  /*
  Since SVD gives us three matrices which when multiplied create the original matrix,
  we can multiplay U with Sigma and end up with two matrices: UxSigma and VT. These
  two matrices when multiplied give the original matrix. If the original matrix is
  separable then Sigma only has a single value larger than zero and that value is
  at the coordinates of (0,0). Because of that, When we multiply U x Sigma, we get
  a matrix which can only have non-zero values in the first column. Due to this,
  when we multiply USigmaxVT, only the first row of VT has any significance.
  Thanks to these facts, we can simly extract the first column of U and multiply it
  by Sigma and then extract the first row of VT and we end up with two vectors:
  first_col_of_U multiplied by Sigma (mx1) and first_row_of_VT (1xn).
  These two vectors then form the decomposition of the original separable 2D matrix
  into two 1D vectors.
  */
  if (rank == 1) { //separable
    Mat first_col_of_U(u.rows, 1, CV_32F);
    Mat first_row_of_VT(1, vt.cols, CV_32F);
    u.col(0).copyTo(first_col_of_U.col(0));
    vt.row(0).copyTo(first_row_of_VT.row(0));
    first_col_of_U *= sigma.at<float>(0,0);
    kernelX = first_row_of_VT;
    kernelY = first_col_of_U;
    return true;
  }
  else { //non-separable
    return false;
  }
}

void Run::AddImagePath(string path)
{
    this->imagePaths.push_back(path);
}
