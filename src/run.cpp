#include "run.hpp"


using namespace std;

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
    if (paramName == "images_folder")
    {
      runStream >> this->imagesFolderPath;
      cout << paramName << ": " << this->imagesFolderPath << endl;
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
