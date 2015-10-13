#include "CutEff.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <stdexcept>
/******************************************************************************/
int main(int argc, char* argv[])
{
  if(argc==6)
  {
    CutEff((string)argv[1], (string)argv[2], (string)argv[3], (string)argv[4], (string)argv[5]);
  }
  else if(argc==5)
  {
    cout << (string)argv[1] << " " << (string)argv[2] << " " << (string)argv[3] << " " << (string)argv[4] << endl;
    CutEff((string)argv[1], (string)argv[2], (string)argv[3], (string)argv[4]);
  }
  else if(argc>6)
  {
    cout << "Too many arguments." << endl;
    return 1;
  }
  else
  {
    cout << "Usage: " << argv[0] << " <filename> <branch to plot> <initial cut> <final cut> [<plot name>]" << endl;
    return 1;
  }
  return 0;
}
