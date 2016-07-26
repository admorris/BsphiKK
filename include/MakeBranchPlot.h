#ifndef __MakeBranchPlot_h__
#define __MakeBranchPlot_h__
#include <string>
#include "TH1.h"
using std::string;
TH1D* MakeBranchPlot(string, string, string, string, double, double, int);
#endif
