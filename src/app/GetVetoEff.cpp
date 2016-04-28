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
void GetVetoEff(string filename, bool save, string DBfilename)
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
    ("help,H",                                                         "produce help message")
    ("save"  ,                                                         "save the results"    )
    ("output-file", value<string>(&dbf)->default_value("VetoEff.csv"), "output file"         )
    ("input-file" , value<string>(                                  ), "input file"          )
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
  GetVetoEff(filename,vmap.count("save"),dbf);
  return 0;
}
