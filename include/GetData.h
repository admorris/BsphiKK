#ifndef __GetDataFunctions_h__
#define __GetDataFunctions_h__
// ROOT headers
#include "TFile.h"
#include "TTree.h"
// Custom headers
#include "GetTree.h"
// RooFit headers
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
RooHistPdf* GetDataHist(string name, string filename, string cuts, RooRealVar* massvar)
{
  TTree* tree = GetTree(filename,cuts);
  RooDataSet* data = new RooDataSet((name+"data").c_str(),"",*massvar,RooFit::Import(*tree));
  RooDataHist* hist = new RooDataHist((name+"hist").c_str(),"",*massvar,*data);
  return new RooHistPdf("shape","",*massvar,*hist);
}
RooDataSet* GetData(string name, string filename, string cuts, RooRealVar* massvar)
{
  TTree* tree = GetTree(filename,cuts);
  return new RooDataSet((name+"data").c_str(),"",*massvar,RooFit::Import(*tree));
}
#endif
