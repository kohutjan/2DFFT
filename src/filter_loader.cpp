#include "../include/filter_loader.hpp"


using namespace std;
using namespace cv;

Filter FilterLoader::Get2DFilter(string name, string type, int kernelSize)
{
    if (type == "Mean")
    {
        return this->GetMean(name, type, kernelSize);
    }
    if (type == "Gaussian")
    {
        return this->GetGauss(name, type, kernelSize, -1);
    }
    Filter filter;
    return filter;
}

Filter FilterLoader::GetSpecFilter(string name, string type, int radius, Mat img)
{
    if (type == "Low-pass")
    {
        return this->GetLowPass(name, type, radius, img);
    }
    if (type == "High-pass")
    {
        return this->GetHighPass(name, type, radius, img);
    }
    Filter filter;
    return filter;
}

Filter FilterLoader::GetMean(string name, string type, int kernelSize)
{
    return Filter(name, type, this->LoadMean(kernelSize));
}

Filter FilterLoader::GetGauss(string name, string type, int kernelSize, float sigma)
{
    Mat values1D = getGaussianKernel(kernelSize, sigma, CV_32F);
    Mat values2D = values1D * values1D.t();
    return Filter(name, type, values2D);
}

Filter FilterLoader::GetLowPass(string name, string type, int radius, Mat img)
{
    /*int topLeftRow = img.rows / 2 - radius;
    int topLeftCol = img.cols / 2 - radius;
    Mat filterSpec = Mat::zeros(Size(img.cols, img.rows), CV_32FC2);
    filterSpec.setTo(1.0);
    Mat square = filterSpec(Rect(Point(topLeftCol, topLeftRow), Point(topLeftCol + radius, topLeftRow + radius)));
    square.setTo(0.0);*/
    Size optimalSize;

    // velikost podle obrazku nebo optimalni velikosti obrazku? davaji rozdilne vysledky...
    optimalSize.width = img.cols; //cv::getOptimalDFTSize(img.cols);
    optimalSize.height = img.rows; //cv::getOptimalDFTSize(img.rows);

    Mat filterSpec = Mat::zeros(Size(optimalSize.width, optimalSize.height), CV_32FC2);
    Rect centerRect(static_cast<int>((optimalSize.width / 2) - radius), static_cast<int>((optimalSize.height / 2) - radius), radius * 2, radius * 2);
    Mat filterMask(filterSpec.rows, filterSpec.cols, CV_32FC2, cv::Scalar(0.0f, 0.0f));
    filterMask(centerRect).setTo(Scalar(1.0f, 1.0f));
    //imshow("low_pass_before_idft", filterMask);

    Mat filterValues;
    idft(filterMask, filterValues, DFT_REAL_OUTPUT+DFT_SCALE, img.rows);
    normalize(filterValues, filterValues, 0, 1, CV_MINMAX);
    filterValues.convertTo(filterValues, CV_8U, 255, 0);
    return Filter(name, type, filterValues);
}

Filter FilterLoader::GetHighPass(string name, string type, int radius, Mat img)
{
    Size optimalSize;

    // velikost podle obrazku nebo optimalni velikosti obrazku? davaji rozdilne vysledky...
    optimalSize.width = img.cols; //cv::getOptimalDFTSize(img.cols);
    optimalSize.height = img.rows; //cv::getOptimalDFTSize(img.rows);

    Mat filterSpec = Mat::ones(Size(optimalSize.width, optimalSize.height), CV_32FC2);
    Rect centerRect(static_cast<int>((optimalSize.width / 2) - radius), static_cast<int>((optimalSize.height / 2) - radius), radius * 2, radius * 2);
    Mat filterMask(filterSpec.rows, filterSpec.cols, CV_32FC2, cv::Scalar(1.0f, 1.0f));
    filterMask(centerRect).setTo(Scalar(0.0f, 0.0f));
    //imshow("high_pass_before_idft", filterMask);

    Mat filterValues;
    idft(filterMask, filterValues, DFT_REAL_OUTPUT+DFT_SCALE, img.rows);
    normalize(filterValues, filterValues, 0, 1, CV_MINMAX);
    filterValues.convertTo(filterValues, CV_8U, 255, 0);
    return Filter(name, type, filterValues);
}

bool FilterLoader::Load(string filtersFilePath)
{
  cout << "Filters file path: " << filtersFilePath << endl;
  ifstream filtersStream(filtersFilePath);
  if (filtersStream.is_open())
  {
    this->LoadFromStream(filtersStream);
    filtersStream.close();
    return true;
  }
  else
  {
    cerr << "Unable to open filters file." << endl;
    return false;
  }
}

void FilterLoader::LoadFromStream(ifstream &filtersStream)
{
  cout << "Filters params:" << endl;
  cout << "#############################################################" << endl;
  while (!filtersStream.eof())
  {
    string name;
    filtersStream >> name;
    string type;
    filtersStream >> type;
    int kernelSize;
    filtersStream >> kernelSize;
    if (type == "mean")
    {
      Filter mean = Filter(name, type, this->LoadMean(kernelSize));
      this->PrintFilter(mean);
      this->filters[name] = mean;
      this->filtersInsertOrder.push_back(name);
      continue;
    }
    if (type == "gauss")
    {
      Filter gauss = Filter(name, type, this->LoadGauss(kernelSize, filtersStream));
      this->PrintFilter(gauss);
      this->filters[name] = gauss;
      this->filtersInsertOrder.push_back(name);
      continue;
    }
    if (type == "custom")
    {
      Filter custom  = Filter(name, type, this->LoadCustom(kernelSize, filtersStream));
      this->PrintFilter(custom);
      this->filters[name] = custom;
      this->filtersInsertOrder.push_back(name);
      continue;
    }
  }
  cout << "#############################################################" << endl;
  cout << endl;
  cout << endl;
  return;
}

Mat FilterLoader::LoadMean(int kernelSize)
{
  Mat values = Mat::zeros(Size(kernelSize, kernelSize), CV_32F);
  float mean = 1.0 / (kernelSize * kernelSize);
  values.setTo(mean);
  return values;
}

Mat FilterLoader::LoadGauss(int kernelSize, ifstream &filtersStream)
{
  float sigma;
  filtersStream >> sigma;
  Mat values = getGaussianKernel(kernelSize, sigma, CV_32F);
  return values;
}

Mat FilterLoader::LoadCustom(int kernelSize, ifstream &filtersStream)
{
  Mat values = Mat::zeros(Size(kernelSize, kernelSize), CV_32F);
  for (int i = 0; i < values.rows; i++)
  {
    float * rowPtr = values.ptr<float>(i);
    for (int j = 0; j < values.cols; j++)
    {
      filtersStream >> rowPtr[j];
    }
  }
  return values;
}

void FilterLoader::PrintFilter(Filter filter)
{
  cout << filter.getName() << " ";
  cout << filter.getType() << " ";
  cout << filter.getKernelSize() << " ";
  if (filter.getType() == "custom")
  {
    Mat values = filter.getValues();
    for (int i = 0; i < values.rows; i++)
    {
      float * rowPtr = values.ptr<float>(i);
      for (int j = 0; j < values.cols; j++)
      {
        cout << rowPtr[j] << " ";
      }
    }
  }
  cout << endl;
}
