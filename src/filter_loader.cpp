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

    Rect centerRect(static_cast<int>((optimalSize.width / 2) - radius), static_cast<int>((optimalSize.height / 2) - radius), radius * 2, radius * 2);
    Mat filterMask = Mat::zeros(Size(optimalSize.width, optimalSize.height), CV_32FC2);
    filterMask(centerRect).setTo(Scalar(1.0f, 1.0f));

    this->rearrangeSpectrum(filterMask);

    Mat complexValuesBeforeIdft[2];
    split(filterMask, complexValuesBeforeIdft);
    //imshow("low_pass_before_idft", complexValuesBeforeIdft[0]);

    Mat filterValues;
    idft(filterMask, filterValues, DFT_COMPLEX_OUTPUT+DFT_SCALE, img.rows);
    //normalize(filterValues, filterValues, 0, 1, CV_MINMAX);

    Mat complexValues[2];
    split(filterValues, complexValues);
    Mat realValues = complexValues[0];

    //realValues.convertTo(realValues, CV_8U, 255, 0);
    this->rearrangeSpectrum(realValues);

    return Filter(name, type, realValues);
}

Filter FilterLoader::GetHighPass(string name, string type, int radius, Mat img)
{
    Size optimalSize;

    // velikost podle obrazku nebo optimalni velikosti obrazku? davaji rozdilne vysledky...
    optimalSize.width = img.cols; //cv::getOptimalDFTSize(img.cols);
    optimalSize.height = img.rows; //cv::getOptimalDFTSize(img.rows);

    Rect centerRect(static_cast<int>((optimalSize.width / 2) - radius), static_cast<int>((optimalSize.height / 2) - radius), radius * 2, radius * 2);
    Mat filterMask = Mat::ones(Size(optimalSize.width, optimalSize.height), CV_32FC2);
    filterMask(centerRect).setTo(Scalar(0.0f, 0.0f));

    this->rearrangeSpectrum(filterMask);

    Mat complexValuesBeforeIdft[2];
    split(filterMask, complexValuesBeforeIdft);
    //imshow("high_pass_before_idft", complexValuesBeforeIdft[0]);

    Mat filterValues;
    idft(filterMask, filterValues, DFT_COMPLEX_OUTPUT+DFT_SCALE, img.rows);
    //normalize(filterValues, filterValues, 0, 1, CV_MINMAX);

    Mat complexValues[2];
    split(filterValues, complexValues);
    Mat realValues = complexValues[0];

    //realValues.convertTo(realValues, CV_8U, 255, 0);
    this->rearrangeSpectrum(realValues);

    return Filter(name, type, realValues);
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

void FilterLoader::rearrangeSpectrum( Mat& s )
{
    int cx = s.cols/2;
    int cy = s.rows/2;

    Mat q0(s, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(s, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(s, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(s, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);
}
