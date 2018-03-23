#include "filter_container.hpp"


using namespace std;
using namespace cv;

bool FilterContainer::Load(string filtersFilePath)
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

void FilterContainer::LoadFromStream(ifstream &filtersStream)
{
  cout << "Filters params:" << endl;
  cout << "#############################################################" << endl;
  while (!filtersStream.eof())
  {
    string name;
    filtersStream >> name;
    string type;
    filtersStream >> type;
    int stride;
    filtersStream >> stride;
    int kernelSize;
    filtersStream >> kernelSize;
    if (type == "mean")
    {
      //Filter mean = Filter(name, type, stride, this->LoadMean(kernelSize, filtersStream))
      //this->filters.push_back(mean);
      continue;
    }
    if (type == "gauss")
    {
      //Filter gauss = Filter(name, type, stride, this->LoadGauss(kernelSize, filtersStream))
      //this->filters.push_back(gauss);
      continue;
    }
    if (type == "custom")
    {
      Filter custom = Filter(name, type, stride, this->LoadCustom(kernelSize, filtersStream));
      this->PrintFilter(custom);
      this->filters.push_back(custom);
      continue;
    }
  }
  cout << "#############################################################" << endl;
  cout << endl;
  cout << endl;
  return;
}

Mat FilterContainer::LoadCustom(int kernelSize, ifstream &filtersStream)
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

void FilterContainer::PrintFilter(Filter filter)
{
  cout << filter.getName() << " ";
  cout << filter.getType() << " ";
  cout << filter.getStride() << " ";
  cout << filter.getKernelSize() << " ";
  Mat values = filter.getValues();
  for (int i = 0; i < values.rows; i++)
  {
    float * rowPtr = values.ptr<float>(i);
    for (int j = 0; j < values.cols; j++)
    {
      cout << rowPtr[j] << " ";
    }
  }
  cout << endl;
}
