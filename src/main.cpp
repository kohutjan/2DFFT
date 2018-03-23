#include <iostream>
#include <string>
#include <getopt.h>

using namespace std;

int main(int argc, char **argv)
{
  static struct option long_options[] = {
  {"run", required_argument, 0, 'r'},
  {0, 0, 0, 0}};

  string runFilePath;

  cout << endl;
  cout << "Params" << endl;
  cout << "#############################################################" << endl;
  int option_index = 0;
  int opt = 0;
  while ((opt = getopt_long(argc, argv, "i:", long_options, &option_index)) != -1)
  {
    switch (opt)
    {
      case 'i':
        runFilePath = optarg;
        cout << "Run file path: " << optarg << endl;
        break;

      default:
        break;
    }
  }
  cout << "#############################################################" << endl;
  cout << endl;
  cout << endl;

  if (runFilePath.empty())
  {
    cout << "Path for run file have to be set." << endl;
    return -1;
  }

  return 0;
}
