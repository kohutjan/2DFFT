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

      if (this->spatial)
      {
        //Prepare spatial convolution
        Mat flipedFilter = filter.second.getValues().clone();
        flip(filter.second.getValues(), flipedFilter, -1);
        spatialConvolution.setData(src, dst, flipedFilter);

        //Measure time
        spatialConvolution.Regular();
      }

      if (this->separable)
      {
        if (isSeparable(filter.second.getValues()))
        {
          spatialConvolution.Separable();
        }
      }

      if (this->frequency)
      {
        frequencyConvolution.FFT();
        frequencyConvolution.MUL();
        frequencyConvolution.IFFT();
      }
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

bool Run::isSeparable(Mat kernel)
{
  Mat sigma;
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
  SVD::compute(kernel, sigma);
  for (auto it = sigma.begin<float>(); it != sigma.end<float>(); it++) {
    if (*it != 0) rank++;
  }

  if (rank == 1) {
    return true;
  }
  else {
    return false;
  }
}
