#include <iostream>
#include <string>
#include <getopt.h>

#include "run.hpp"

using namespace std;

int main(int argc, char **argv)
{
  static struct option long_options[] = {
  {"run", required_argument, 0, 'r'},
  {"filters", required_argument, 0, 'f'},
  {0, 0, 0, 0}};

  string runFilePath;
  string filtersFilePath;

  cout << endl;
  cout << "Params" << endl;
  cout << "#############################################################" << endl;
  int option_index = 0;
  int opt = 0;
  while ((opt = getopt_long(argc, argv, "r:f:", long_options, &option_index)) != -1)
  {
    switch (opt)
    {
      case 'r':
        runFilePath = optarg;
        cout << "Run file path: " << optarg << endl;
        break;

      case 'f':
        filtersFilePath = optarg;
        cout << "Filters file path: " << optarg << endl;
        break;

      default:
        break;
    }
  }
  cout << "#############################################################" << endl;
  cout << endl;
  cout << endl;

  if (runFilePath.empty() || filtersFilePath.empty())
  {
    cout << "Paths to run and filters files have to be set." << endl;
    return -1;
  }

  Run run;
  run.Load(runFilePath);

  return 0;
}
