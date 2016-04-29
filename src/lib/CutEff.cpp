#include "TTree.h"
#include "TFile.h"
#include "CutEff.h"
#include <string>
#include <iostream>
Cut_t::Cut_t(string n, string c) : name(n), cut(c) {}
CutResult_t::CutResult_t(double b, double a) : before(b), after(a) {}
CutResult_t CutEff(string filename, string branchtoplot, string beforecut, string cut)
{
  return CutEff(filename, branchtoplot, beforecut, cut,"");
}
CutResult_t CutEff(string filename, string branchtoplot, string beforecut, string cut, string plotname)
{
/*Input************************************************************************/
  // Open the input file
  TFile* infile  = new TFile(filename.c_str());
  // Get the input tree
  TTree* intree;
  if((TTree*)infile->Get("DecayTree") == (TTree*)0x0)
  {
    cout << "Tree DecayTree not found. Trying DecayTreeTuple/DecayTree" << endl;
    intree  = (TTree*)infile->Get("DecayTreeTuple/DecayTree");
  }
  else
  {
    intree  = (TTree*)infile->Get("DecayTree");
  }
  // Draw "before" and fetch the yield
  Long64_t nocutyield = intree->Draw(branchtoplot.c_str(),beforecut.c_str());
  string aftercut;
  if(beforecut=="")
    aftercut=cut;
  else if(cut!="")
    aftercut="("+beforecut+")&&("+cut+")";
  else
    aftercut=beforecut;
  // Draw "after" and fetch the yield
  Long64_t cutyield   = intree->Draw(branchtoplot.c_str(),aftercut.c_str());
  cout << "         Kept\tCut" << endl
       << "#events: " << cutyield << "\t" << nocutyield-cutyield << "\t" << endl
       << "percent: " << setprecision(3) << 100.0*cutyield/nocutyield << "%\t" << 100.0*(nocutyield-cutyield)/nocutyield << "%" << endl;
  return CutResult_t(nocutyield,cutyield);
}
