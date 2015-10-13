#include <string>
#include <iostream>
#include <iomanip>
#include "CutEff.h"
using namespace std;
void GetTrigEff(string filename)
{
  string L0[]={"B_s0_L0HadronDecision_TOS", "B_s0_L0Global_TIS"};
  string Hlt1="B_s0_Hlt1TrackAllL0Decision_TOS";
  string Hlt2[]={"B_s0_Hlt2Topo2BodyBBDTDecision_TOS", "B_s0_Hlt2Topo3BodyBBDTDecision_TOS", "B_s0_Hlt2Topo4BodyBBDTDecision_TOS", "B_s0_Hlt2IncPhiDecision_TOS"};
  double L0eff[]   = 
  {
    CutEff(filename,"B_s0_M","",L0[0])
  , CutEff(filename,"B_s0_M","",L0[1])
  };
  double totL0eff  = CutEff(filename,"B_s0_M","",L0[0]+"||"+L0[1]);
  double Hlt1eff   = CutEff(filename,"B_s0_M",L0[0]+"||"+L0[1],Hlt1);
  double Hlt2eff[] = 
  {
    CutEff(filename,"B_s0_M","("+L0[0]+"||"+L0[1]+")&&("+Hlt1+")",Hlt2[0])
  , CutEff(filename,"B_s0_M","("+L0[0]+"||"+L0[1]+")&&("+Hlt1+")",Hlt2[1])
  , CutEff(filename,"B_s0_M","("+L0[0]+"||"+L0[1]+")&&("+Hlt1+")",Hlt2[2])
  , CutEff(filename,"B_s0_M","("+L0[0]+"||"+L0[1]+")&&("+Hlt1+")",Hlt2[3])
  };
  double totHlt2eff = CutEff(filename,"B_s0_M","("+L0[0]+"||"+L0[1]+")&&("+Hlt1+")",Hlt2[0]+"||"+Hlt2[1]+"||"+Hlt2[2]+"||"+Hlt2[3]);
  cout << "Line & Efficiency \\\\" << endl
  << L0[0]   << " & " << L0eff[0]*100   << "\\% \\\\" << endl
  << L0[1]   << " & " << L0eff[1]*100   << "\\% \\\\" << endl
  << "L0"    << " & " << totL0eff*100   << "\\% \\\\" << endl
  << Hlt1    << " & " << Hlt1eff*100    << "\\% \\\\" << endl
  << Hlt2[0] << " & " << Hlt2eff[0]*100 << "\\% \\\\" << endl
  << Hlt2[1] << " & " << Hlt2eff[1]*100 << "\\% \\\\" << endl
  << Hlt2[2] << " & " << Hlt2eff[2]*100 << "\\% \\\\" << endl
  << Hlt2[3] << " & " << Hlt2eff[3]*100 << "\\% \\\\" << endl
  << "Hlt2"  << " & " << totHlt2eff*100 << "\\% \\\\" << endl;
}
int main(int argc, char* argv[])
{
  if(argc==2)
  {
    GetTrigEff((string)argv[1]);
  }
  else
  {
    cout << "Usage: " << argv[0] << " <filename>" << endl;
    return 1;
  }
  return 0;
}
