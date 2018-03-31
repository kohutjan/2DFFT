#include <iostream>
#include <string>
#include <getopt.h>

#include "run.hpp"
#include "filter_loader.hpp"

using namespace std;

int main(int argc, char **argv)
{
  static struct option long_options[] = {
  {"run", required_argument, 0, 'r'},
  {"filters", required_argument, 0, 'f'},
  {"out", required_argument, 0, 'o'},
  {"show", no_argument, 0, 's'},
  {0, 0, 0, 0}};

  string runFilePath;
  string filtersFilePath;
  string statisticsFilePath;
  bool show = false;

  cout << endl;
  cout << "Params" << endl;
  cout << "#############################################################" << endl;
  int option_index = 0;
  int opt = 0;
  while ((opt = getopt_long(argc, argv, "r:f:o:s", long_options, &option_index)) != -1)
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

      case 'o':
        statisticsFilePath = optarg;
        cout << "Statistics file path: " << optarg << endl;
        break;

      case 's':
        show = true;
        cout << "Show: true" << endl;
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

  FilterLoader filterLoader;
  if (filterLoader.Load(filtersFilePath))
  {
    run.setFilters(filterLoader.getFilters(), filterLoader.getFiltersInsertOrder());
    run.InitFilterStatistics();
    run.Start(show);
    if (statisticsFilePath.empty())
    {
      run.Print();
    }
    else
    {
      run.Print(statisticsFilePath);
    }
  }

  return 0;
}
