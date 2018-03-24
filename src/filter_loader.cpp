#include "filter_loader.hpp"


using namespace std;
using namespace cv;

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
      continue;
    }
    if (type == "gauss")
    {
      Filter gauss = Filter(name, type, this->LoadGauss(kernelSize, filtersStream));
      this->PrintFilter(gauss);
      this->filters[name] = gauss;
      continue;
    }
    if (type == "custom")
    {
      Filter custom  = Filter(name, type, this->LoadCustom(kernelSize, filtersStream));
      this->PrintFilter(custom);
      this->filters[name] = custom;
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
