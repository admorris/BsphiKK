#include "CutEff.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <stdexcept>
/******************************************************************************/
int main(int argc, char* argv[])
{
  if(argc==4)
  {
    CutEff((string)argv[1], (string)argv[2], (string)argv[3]);
  }
  else
  {
    std::cout << "Usage: " << argv[0] << " <filename> <initial cut> <final cut>" << std::endl;
    return 1;
  }
  return 0;
}
