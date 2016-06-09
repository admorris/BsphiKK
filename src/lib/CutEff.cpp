#include "TTree.h"
#include "TFile.h"
#include "CutEff.h"
#include "GetTree.h"
#include <string>
#include <iostream>
#include <iomanip>
Cut_t::Cut_t(string n, string c) : name(n), cut(c) {}
CutResult_t::CutResult_t(double b, double a) : before(b), after(a) {}
CutResult_t CutEff(string filename, string beforecut, string cut)
{
/*Input************************************************************************/
  // Open the input file
  TTree* intree = GetTree(filename);
  return CutEff(intree, beforecut, cut);
}
CutResult_t CutEff(TTree* intree, string beforecut, string cut)
{
  // Draw "before" and fetch the yield
  Long64_t nocutyield = intree->GetEntries(beforecut.c_str());
  string aftercut;
  if(beforecut=="")
    aftercut=cut;
  else if(cut!="")
    aftercut="("+beforecut+")&&("+cut+")";
  else
    aftercut=beforecut;
  // Draw "after" and fetch the yield
  Long64_t cutyield   = intree->GetEntries(aftercut.c_str());
  cout << "         Kept\tCut" << endl
       << "#events: " << cutyield << "\t" << nocutyield-cutyield << "\t" << endl
       << "percent: " << setprecision(3) << 100.0*cutyield/nocutyield << "%\t" << 100.0*(nocutyield-cutyield)/nocutyield << "%" << endl;
  return CutResult_t(nocutyield,cutyield);
}
