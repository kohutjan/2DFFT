#include "run.hpp"


using namespace std;
using namespace cv;


bool Run::Start()
{
  SpatialConvolution spatialConvolution;
  FrequencyConvolution frequencyConvolution;

  for (auto& imagePath: this->imagePaths)
  {
    Mat src = imread(imagePath, CV_LOAD_IMAGE_GRAYSCALE);
    if(!src.data)
    {
       cout <<  "Could not open or find the image" << endl;
       return false;
    }
    Mat dst = src.clone();

    for (auto& filter: this->filters)
    {
      //OpenCV Mat for filter: filter.second

      //Prepare sptial convolution
      Mat flipedFilter = filter.second.getValues().clone();
      flip(filter.second.getValues(), flipedFilter, -1);
      spatialConvolution.setData(src, dst, flipedFilter);

      //Measure time
      spatialConvolution.Regular();

      //Statistic for given filter: this->statistics[filter.first]

      //Save statistics

      imshow(filter.first, dst);
      waitKey(0);
    }
  }
  return true;
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
  }
  cout << "#############################################################" << endl;
  cout << endl;
  cout << endl;
  return;
}

void Run::InitFilterStatistics()
{
  for (const auto& filter: this->filters)
  {
    this->statistics[filter.first] = FilterStatistic(filter.first);
  }
}
