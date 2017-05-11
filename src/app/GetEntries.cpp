#include "TFile.h"
#include "TTree.h"

#include <string>
#include <iostream>
void GetEntries(std::string filename, std::string path, std::string cut)
{
  int nentries = static_cast<TTree*>(TFile::Open(filename.c_str())->Get(path.c_str()))->GetEntries(cut.c_str());
  std::cout << nentries << std::endl;
}
int main(int argc, char* argv[])
{
  if(argc==3)
    GetEntries((std::string)argv[1], (std::string)argv[2], "");
  else if(argc==4)
    GetEntries((std::string)argv[1], (std::string)argv[2], (std::string)argv[3]);
  else
  {
    std::cout << "Usage: " << argv[0] << " <filename> <path/to/tuple> <cut>" << std::endl;
    return 1;
  }
  return 0;
}
