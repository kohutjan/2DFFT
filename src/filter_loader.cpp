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
    if (type == "SobelX")
    {
        return this->GetSobel(name, type, 'x');
    }
    if (type == "SobelY")
    {
        return this->GetSobel(name, type, 'y');
    }
    if (type == "DoG")
    {
        return this->GetDoG(name, type, kernelSize);
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
    if (type == "Low-gauss")
    {
        return this->GetLowGauss(name, type, radius, img);
    }
    if (type == "High-gauss")
    {
        return this->GetHighGauss(name, type , radius, img);
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

Filter FilterLoader::GetSobel(string name, string type, char dir)
{
    Mat values;
    if (dir == 'x')
    {
        values = (Mat_<float>(3,3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
    }
    if (dir == 'y')
    {
        values = (Mat_<float>(3,3) << 1, 2, 1, 0, 0, 0, -1, -2, -1);
    }
    return Filter(name, type, values);
}

Filter FilterLoader::GetDoG(string name, string type, int kernelSize)
{
    if (kernelSize == 3)
    {
        kernelSize = 5;
    }
    Mat gauss1 = this->GetGauss(name, type, kernelSize, -1).getValues();
    int middleKernelSize = kernelSize / 2;
    if (middleKernelSize % 2 == 0)
    {
        ++middleKernelSize;
    }
    Mat gauss2 = this->GetGauss(name, type, middleKernelSize, -1).getValues();
    int borderThickness = (kernelSize - middleKernelSize) / 2;
    copyMakeBorder(gauss2, gauss2, borderThickness, borderThickness, borderThickness, borderThickness,
                   BORDER_CONSTANT, 0);
    Mat values = gauss1 - gauss2;
    return Filter(name, type, values);
}

Filter FilterLoader::GetLowPass(string name, string type, int radius, Mat img)
{
    Mat filterMask = Mat::zeros(Size(img.cols, img.rows), CV_32FC1);
    circle(filterMask, Point(img.cols / 2, img.rows / 2), radius, 1, -1);
    Mat imgZeros = Mat::zeros(Size(img.cols, img.rows), CV_32FC1);
    vector<Mat> complexFilterVec;
    complexFilterVec.push_back(filterMask);
    complexFilterVec.push_back(imgZeros);
    Mat complexFilter;
    merge(complexFilterVec, complexFilter);
    this->rearrangeSpectrum(complexFilter);
    Mat filterValues;
    dft(complexFilter, filterValues, DFT_REAL_OUTPUT | DFT_SCALE | DFT_INVERSE);
    this->rearrangeSpectrum(filterValues);
    return Filter(name, type, filterValues);

}

Filter FilterLoader::GetHighPass(string name, string type, int radius, Mat img)
{
    Mat filterMask = Mat::ones(Size(img.cols, img.rows), CV_32FC1);
    circle(filterMask, Point(img.cols / 2, img.rows / 2), radius, 0, -1);
    Mat imgZeros = Mat::zeros(Size(img.cols, img.rows), CV_32FC1);
    vector<Mat> complexFilterVec;
    complexFilterVec.push_back(filterMask);
    complexFilterVec.push_back(imgZeros);
    Mat complexFilter;
    merge(complexFilterVec, complexFilter);
    this->rearrangeSpectrum(complexFilter);
    Mat filterValues;
    dft(complexFilter, filterValues, DFT_REAL_OUTPUT | DFT_SCALE | DFT_INVERSE);
    this->rearrangeSpectrum(filterValues);
    return Filter(name, type, filterValues);
}

Filter FilterLoader::GetLowGauss(string name, string type, int radius, Mat img)
{
    int topLeftRow = img.rows / 2 - radius;
    int topLeftCol = img.cols / 2 - radius;
    Mat filterMask = Mat::zeros(Size(img.cols, img.rows), CV_32FC1);
    Mat gauss = this->GetGauss(name, type, radius * 2, -1).getValues();
    normalize(gauss, gauss, 0, 1, CV_MINMAX);
    gauss.copyTo(filterMask(Rect(topLeftCol, topLeftRow, radius * 2, radius * 2)));
    Mat imgZeros = Mat::zeros(Size(img.cols, img.rows), CV_32FC1);
    vector<Mat> complexFilterVec;
    complexFilterVec.push_back(filterMask);
    complexFilterVec.push_back(imgZeros);
    Mat complexFilter;
    merge(complexFilterVec, complexFilter);
    this->rearrangeSpectrum(complexFilter);
    Mat filterValues;
    dft(complexFilter, filterValues, DFT_REAL_OUTPUT | DFT_SCALE | DFT_INVERSE);
    this->rearrangeSpectrum(filterValues);
    return Filter(name, type, filterValues);
}

Filter FilterLoader::GetHighGauss(string name, string type, int radius, Mat img)
{
    int topLeftRow = img.rows / 2 - radius;
    int topLeftCol = img.cols / 2 - radius;
    Mat filterMask = Mat::ones(Size(img.cols, img.rows), CV_32FC1);
    Mat gauss = -this->GetGauss(name, type, radius * 2, -1).getValues();
    normalize(gauss, gauss, 0, 1, CV_MINMAX);
    gauss.copyTo(filterMask(Rect(topLeftCol, topLeftRow, radius * 2, radius * 2)));
    Mat imgZeros = Mat::zeros(Size(img.cols, img.rows), CV_32FC1);
    vector<Mat> complexFilterVec;
    complexFilterVec.push_back(filterMask);
    complexFilterVec.push_back(imgZeros);
    Mat complexFilter;
    merge(complexFilterVec, complexFilter);
    this->rearrangeSpectrum(complexFilter);
    Mat filterValues;
    dft(complexFilter, filterValues, DFT_REAL_OUTPUT | DFT_SCALE | DFT_INVERSE);
    this->rearrangeSpectrum(filterValues);
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
