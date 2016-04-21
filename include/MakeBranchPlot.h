#ifndef __MakeBranchPlot_h__
#define __MakeBranchPlot_h__
#include <string>
#include "RooPlot.h"
using std::string;
RooPlot* MakeBranchPlot(string, string, string, string, string, string, string, double, double, int, string, double);
#endif
