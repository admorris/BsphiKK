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
  Cut_t cuts[] =
  {
    Cut_t("BdPhiKstar","TMath::Abs(phiKpiM-5279.58)>60||(TMath::Abs(phiKpiM-5279.58)<50&&Kplus0_ProbNNk>Kplus0_ProbNNpi&&Kminus0_ProbNNk>Kminus0_ProbNNpi)")
  , Cut_t("LbPhiKp","TMath::Abs(phiKpM-5619.5)>60||(TMath::Abs(phiKpM-5619.5)<60&&Kplus0_ProbNNk>Kplus0_ProbNNp&&Kminus0_ProbNNk>Kminus0_ProbNNp)")
  , Cut_t("LcPhip","TMath::Abs(phipM-2286.46)>24||(TMath::Abs(phipM-2286.46)<24&&Kplus0_ProbNNk>Kplus0_ProbNNp&&Kminus0_ProbNNk>Kminus0_ProbNNp)&&TMath::Abs(phipbarM-2286.46)>24||(TMath::Abs(phipbarM-2286.46)<24&&Kplus0_ProbNNk>Kplus0_ProbNNp&&Kminus0_ProbNNk>Kminus0_ProbNNp) ")
  , Cut_t("DPhiK","TMath::Abs(phiKplusM-1968.3)>24&&TMath::Abs(phiKminusM-1968.3)>24")
  , Cut_t("DPhipi","TMath::Abs(phipiplusM-1869.61)>24&&TMath::Abs(phipiminusM-1869.61)>24")
  , Cut_t("DsPhipi","TMath::Abs(phipiplusM-1968.3)>24&&TMath::Abs(phipiminusM-1968.3)>24")
  };
  string totalcut = "B_s0_M>0";// Something true for all events so the others can be appended with &&
  const unsigned int n = sizeof(cuts)/sizeof(Cut_t);
  // Calulate efficiency for each cut
  for(unsigned int i = 0; i < n; i++)
  {
    totalcut+="&&(" + cuts[i].cut + ")";
    cuts[i].eff = CutEff(filename,"B_s0_M",trigger,cuts[i].cut);
  }
  double totaleff = CutEff(filename,"B_s0_M",trigger,totalcut);
  // Print table
  cout << "Cut & Efficiency \\\\" << endl;
  for(unsigned int i = 0; i < n; i++)
  {
    cout << cuts[i].name << " & " << cuts[i].eff*100 << "\\% \\\\" << endl;
  }
  cout << "total" << " & " << totaleff*100 <<  "\\% \\\\" << endl;
  // Save to database
  if(save)
  {
    ResultDB rdb(DBfilename);
    size_t mode_start = filename.find('/')==string::npos ? 0 : filename.find_last_of('/');
    size_t mode_end   = filename.find(".root");
    string mode = filename.substr(mode_start+1,mode_end);// filename between final '/' and '.root'
    for(unsigned int i = 0; i < n; i++)
    {
      rdb.Update(mode+cuts[i].name,"percent",cuts[i].eff,0);
    }
    rdb.Update(mode+"total","percent",totaleff,0);
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
