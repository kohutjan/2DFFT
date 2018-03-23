#ifndef RUN_HPP
#define RUN_HPP

#include <iostream>
#include <fstream>
#include <string>

class Run
{
  public:
    Run() : imagesFolderPath(""), iterations(0){}
    Run(std::string _imagesFolderPath, int _iterations) :
        imagesFolderPath(_imagesFolderPath), iterations(_iterations){}
    bool Load(std::string runFilePath);
    void LoadFromStream(std::ifstream &runStream);
    bool Start();
    ~Run(){}
  private:
    std::string imagesFolderPath;
    int iterations;
};

#endif
