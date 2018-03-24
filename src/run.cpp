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
        imshow(filter.first + " spatial regular", dst);
      }

      if (this->separable)
      {
        Mat kernelX, kernelY;
        Mat flipedFilter = filter.second.getValues().clone();
        flip(filter.second.getValues(), flipedFilter, -1);
        if (isSeparable(flipedFilter, kernelX, kernelY))
        {
          spatialConvolution.setData(src, dst, kernelX, kernelY);
          spatialConvolution.Separable();
        }
        imshow(filter.first + " spatial separable", dst);
      }

      if (this->frequency)
      {
        frequencyConvolution.FFT();
        frequencyConvolution.MUL();
        frequencyConvolution.IFFT();
        imshow(filter.first + " frequency", dst);
      }
      //Statistic for given filter: this->statistics[filter.first]

      //Save statistics
    }
    waitKey(0);
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

bool Run::isSeparable(Mat kernel, Mat &kernelX, Mat &kernelY)
{
  Mat u, sigma, vt;
  Mat first_col_of_U(3, 1, CV_32F);
  Mat first_row_of_VT(1, 3, CV_32F);
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
    if (*it > 0.00001)
    {
       rank++; //threshold for calculation errors, sigma values are never negative
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
    u.col(0).copyTo(first_col_of_U.col(0));
    vt.row(0).copyTo(first_row_of_VT.row(0));

    first_col_of_U *= sigma.at<float>(0,0);

    kernelX = first_col_of_U;
    kernelY = first_row_of_VT;
    return true;
  }
  else { //non-separable
    return false;
  }
}
