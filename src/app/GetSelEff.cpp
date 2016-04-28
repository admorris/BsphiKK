// C++ headers
#include <string>
#include <iostream>
#include <iomanip>
// BOOST headers
#include "boost/program_options.hpp"
// Custom headers
#include "CutEff.h"
#include "ResultDB.h"
using namespace std;
void GetSelEff(string filename, bool save, string DBfilename)
{
  cout << "Please make sure these cuts match the ones in cut.sh" << endl;
  string trigger = "(B_s0_L0HadronDecision_TOS||B_s0_L0Global_TIS)&&B_s0_Hlt1TrackAllL0Decision_TOS&&(B_s0_Hlt2Topo2BodyBBDTDecision_TOS||B_s0_Hlt2Topo3BodyBBDTDecision_TOS||B_s0_Hlt2Topo4BodyBBDTDecision_TOS||B_s0_Hlt2IncPhiDecision_TOS)";
  if(filename.find("MC")!=string::npos)
  {
    trigger+="&&(B_s0_BKGCAT<20||B_s0_BKGCAT==50)";
  }
  if(filename.find("data")!=string::npos)
  {
    trigger+="&&(B_s0_LOKI_Mass>5600)";
  }
  else
  {
    trigger+="&&(B_s0_LOKI_Mass>5200&&B_s0_LOKI_Mass<5600)";
  }
  string cut[] =
  {
    "Kminus_TRACK_GhostProb<0.3&&Kplus_TRACK_GhostProb<0.3&&Kminus0_TRACK_GhostProb<0.3&&Kplus0_TRACK_GhostProb<0.3"
  , "Kminus_isMuon==0&&Kplus_isMuon==0&&Kminus0_isMuon==0&&Kplus0_isMuon==0"
  , "Kminus_PT>500&&Kplus_PT>500&&Kminus0_PT>500&&Kplus0_PT>500"
  , "TMath::Abs(phi_1020_MM-1019.461)<15"
  , "B_s0_FDCHI2_OWNPV>250"
  , "B_s0_IPCHI2_OWNPV<20"
  , "Kminus_ProbNNk*(1-Kminus_ProbNNpi)>0.025&&Kplus_ProbNNk*(1-Kplus_ProbNNpi)>0.025&&Kminus0_ProbNNk*(1-Kminus0_ProbNNpi)>0.025&&Kplus0_ProbNNk*(1-Kplus0_ProbNNpi)>0.025"
  , "Kplus_ProbNNk*(1-Kplus_ProbNNp)>0.01&&Kminus_ProbNNk*(1-Kminus_ProbNNp)>0.01&&Kplus0_ProbNNk*(1-Kplus0_ProbNNp)>0.01&&Kminus0_ProbNNk*(1-Kminus0_ProbNNp)>0.01"
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
  if(save)
  {
    ResultDB rdb(DBfilename);
  }
}
int main(int argc, char* argv[])
{
  string filename, dbf;
  using namespace boost::program_options;
  options_description desc("Allowed options",120);
  desc.add_options()
    ("help,H",                                                        "produce help message")
    ("save"  ,                                                        "save the results"    )
    ("output-file", value<string>(&dbf)->default_value("SelEff.csv"), "output file"         )
    ("input-file" , value<string>(                                 ), "input file"          )
  ;
  variables_map vmap;
  positional_options_description pd;
  pd.add("input-file", 1);
  store(command_line_parser(argc, argv).options(desc).positional(pd).run(), vmap);
  notify(vmap);
  if (vmap.count("help"))
  {
    std::cout << desc << endl;
    return 1;
  }
  if(filename.empty())
  {
    cout << "Usage: " << argv[0] << " <filename>" << endl;
    return 1;
  }
  GetSelEff(filename,vmap.count("save"),dbf);
  return 0;
}
