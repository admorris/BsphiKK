#pragma once
// ROOT headers
#include "TFile.h"
#include "TTree.h"
// GSL
#include <gsl/gsl_randist.h>
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
RooDataSet* GetData(string name, TTree* tree, RooRealVar* massvar)
{
  return new RooDataSet((name+"data").c_str(),"",*massvar,RooFit::Import(*tree));
}
RooDataSet* GetData(string name, string filename, string cuts, RooRealVar* massvar)
{
  TTree* tree = GetTree(filename,cuts);
  return GetData(name, tree, massvar);
}
RooDataSet* GetSmearedData(string name, TTree* tree, RooRealVar* massvar, double sigma_res)
{
  double x;
  tree->SetBranchAddress(massvar->GetName(), &x);
  TTree temptree;
  temptree.Branch(massvar->GetName(), &x, (massvar->GetName()+std::string("/D")).c_str());
  std::cout << "Smearing " << massvar->GetName() << std::endl;
  gsl_rng* r = gsl_rng_alloc(gsl_rng_taus);
  for(unsigned i = 0; i < tree->GetEntries(); i++)
  {
    tree->GetEntry(i);
    x += gsl_ran_gaussian(r, sigma_res);
//    x *= gsl_ran_gaussian_pdf(1.0, sigma_res/x);
    temptree.Fill();
  }
  gsl_rng_free(r);
  return new RooDataSet((name+"data").c_str(),"",*massvar,RooFit::Import(temptree));
}
RooDataSet* GetSmearedData(string name, string filename, string cuts, RooRealVar* massvar, double sigma_res)
{
  TTree* tree = GetTree(filename,cuts);
  return GetSmearedData(name, tree, massvar, sigma_res);
}
