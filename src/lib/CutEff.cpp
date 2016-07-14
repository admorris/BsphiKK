#include "TTree.h"
#include "TFile.h"
#include "TStopwatch.h"
#include "CutEff.h"
#include "GetTree.h"
#include <string>
#include <iostream>
#include <iomanip>
Cut_t::Cut_t(string n, string c) : name(n), cut(c) {}
CutResult_t::CutResult_t(double b, double a) : before(b), after(a) {}
CutResult_t CutEff(string filename, string beforecut, string cut)
{
  TTree* intree = GetTree(filename);
  return CutEff(intree, beforecut, cut);
}
CutResult_t CutEff(TTree* intree, string beforecut, string cut)
{
  TStopwatch timer;
  timer.Start();
  Long64_t nocutyield = intree->GetEntries(beforecut.c_str());
  string aftercut;
  if(beforecut=="")
    aftercut=cut;
  else if(cut!="")
    aftercut="("+beforecut+")&&("+cut+")";
  else
    aftercut=beforecut;
  Long64_t cutyield   = intree->GetEntries(aftercut.c_str());
  timer.Stop();
  cout << "         Kept\tCut\n"
       << "#events: " << cutyield << "\t" << nocutyield-cutyield << "\n"
       << "percent: " << setprecision(3) << 100.0*cutyield/nocutyield << "%\t" << 100.0*(nocutyield-cutyield)/nocutyield << "%\n"
       << "time taken: " << timer.RealTime() << " s" << endl;
  return CutResult_t(nocutyield,cutyield);
}
