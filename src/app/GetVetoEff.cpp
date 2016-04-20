#include <string>
#include <iostream>
#include <iomanip>
#include "CutEff.h"
using namespace std;
void GetVetoEff(string filename)
{
  cout << "Please make sure these cuts match the ones in cut.sh" << endl;
  string trigger = "(B_s0_L0HadronDecision_TOS||B_s0_L0Global_TIS)&&B_s0_Hlt1TrackAllL0Decision_TOS&&(B_s0_Hlt2Topo2BodyBBDTDecision_TOS||B_s0_Hlt2Topo3BodyBBDTDecision_TOS||B_s0_Hlt2Topo4BodyBBDTDecision_TOS||B_s0_Hlt2IncPhiDecision_TOS)";
//  if(filename.find("MC")!=string::npos)
//  {
//    trigger+="&&(B_s0_BKGCAT<20||B_s0_BKGCAT==50)";
//  }
//  if(filename.find("data")!=string::npos)
//  {
//    trigger+="&&(B_s0_LOKI_Mass>5600)";
//  }
//  else
//  {
//    trigger+="&&(B_s0_LOKI_Mass>5200&&B_s0_LOKI_Mass<5600)";
//  }
  string cut[] =
  {
    "TMath::Abs(phiKpiM-5279.58)>60||(TMath::Abs(phiKpiM-5279.58)<50&&Kplus0_ProbNNk>Kplus0_ProbNNpi&&Kminus0_ProbNNk>Kminus0_ProbNNpi)"
  , "TMath::Abs(phiKpM-5619.5)>60||(TMath::Abs(phiKpM-5619.5)<60&&Kplus0_ProbNNk>Kplus0_ProbNNp&&Kminus0_ProbNNk>Kminus0_ProbNNp)"
  , "TMath::Abs(phipM-2286.46)>24||(TMath::Abs(phipM-2286.46)<24&&Kplus0_ProbNNk>Kplus0_ProbNNp&&Kminus0_ProbNNk>Kminus0_ProbNNp)&&TMath::Abs(phipbarM-2286.46)>24||(TMath::Abs(phipbarM-2286.46)<24&&Kplus0_ProbNNk>Kplus0_ProbNNp&&Kminus0_ProbNNk>Kminus0_ProbNNp) "
  , "TMath::Abs(phiKplusM-1968.3)>24&&TMath::Abs(phiKminusM-1968.3)>24"
  , "TMath::Abs(phipiplusM-1869.61)>24&&TMath::Abs(phipiminusM-1869.61)>24"
  , "TMath::Abs(phipiplusM-1968.3)>24&&TMath::Abs(phipiminusM-1968.3)>24"
  };
  string totalcut = "B_s0_M>0";
  const unsigned int n = sizeof(cut)/sizeof(string);
  double eff[n];
  for(unsigned int i = 0; i < n; i++)
  {
    totalcut+="&&(" + cut[i] + ")";
    eff[i] = CutEff(filename,"B_s0_M",trigger,cut[i]);
  }
  double totaleff = CutEff(filename,"B_s0_M",trigger,totalcut);
  cout << "Cut & Efficiency \\\\" << endl;
  for(unsigned int i = 0; i < n; i++)
  {
    cout << cut[i] << " & " << eff[i]*100 << "\\% \\\\" << endl;
  }
  
  cout << "total" << " & " << totaleff*100 <<  "\\% \\\\" << endl;
}
int main(int argc, char* argv[])
{
  if(argc==2)
  {
    GetVetoEff((string)argv[1]);
  }
  else
  {
    cout << "Usage: " << argv[0] << " <filename>" << endl;
    return 1;
  }
  return 0;
}
