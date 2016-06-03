// C++ headers
#include <string>
#include <iostream>
#include <iomanip>
// BOOST headers
#include "boost/program_options.hpp"
// Custom headers
#include "CutEff.h"
#include "ResultDB.h"
#include "GetTree.h"
using namespace std;
void GetTrigEff(string filename, bool save, string DBfilename)
{
  string L0[]={"B_s0_L0HadronDecision_TOS", "B_s0_L0Global_TIS"};
  string Hlt1="B_s0_Hlt1TrackAllL0Decision_TOS";
  string Hlt2[]={"B_s0_Hlt2Topo3BodyBBDTDecision_TOS", "B_s0_Hlt2Topo4BodyBBDTDecision_TOS"};
  TTree* intree = GetTree(filename);
  new TCanvas;
  CutResult_t L0eff[]   = 
  {
    CutEff(intree,"B_s0_M","",L0[0])
  , CutEff(intree,"B_s0_M","",L0[1])
  };
  CutResult_t totL0eff  = CutEff(intree,"B_s0_M","",L0[0]+"||"+L0[1]);
  CutResult_t Hlt1eff   = CutEff(intree,"B_s0_M",L0[0]+"||"+L0[1],Hlt1);
  CutResult_t Hlt2eff[] = 
  {
    CutEff(intree,"B_s0_M","("+L0[0]+"||"+L0[1]+")&&("+Hlt1+")",Hlt2[0])
  , CutEff(intree,"B_s0_M","("+L0[0]+"||"+L0[1]+")&&("+Hlt1+")",Hlt2[1])
  };
  CutResult_t totHlt2eff = CutEff(intree,"B_s0_M",   "("+L0[0]+"||"+L0[1]+")&&("+Hlt1+")",   Hlt2[0]+"||"+Hlt2[1]    );
  CutResult_t toteff     = CutEff(intree,"B_s0_M","","("+L0[0]+"||"+L0[1]+")&&("+Hlt1+")&&("+Hlt2[0]+"||"+Hlt2[1]+")");
  cout << "Line & Efficiency \\\\" << endl
  << L0[0]   << " & " << L0eff[0].GetEff()*100   << "\\% \\\\" << endl
  << L0[1]   << " & " << L0eff[1].GetEff()*100   << "\\% \\\\" << endl
  << "L0"    << " & " << totL0eff.GetEff()*100   << "\\% \\\\" << endl
  << Hlt1    << " & " << Hlt1eff.GetEff()*100    << "\\% \\\\" << endl
  << Hlt2[0] << " & " << Hlt2eff[0].GetEff()*100 << "\\% \\\\" << endl
  << Hlt2[1] << " & " << Hlt2eff[1].GetEff()*100 << "\\% \\\\" << endl
  << "Hlt2"  << " & " << totHlt2eff.GetEff()*100 << "\\% \\\\" << endl
  << "Total" << " & " << toteff.GetEff()*100     << "\\% \\\\" << endl
  ;
  if(save)
  {
    ResultDB rdb(DBfilename);
    size_t mode_start = filename.find('/')==string::npos ? 0 : filename.find_last_of('/') + 1;
    size_t mode_end   = filename.find(".root");
    string mode = filename.substr(mode_start,mode_end-mode_start);// filename between final '/' and '.root'
    rdb.Update(mode+"Hadron","percent",L0eff[0].GetEff(),L0eff[0].GetEffErr());
    rdb.Update(mode+"Global","percent",L0eff[1].GetEff(),L0eff[1].GetEffErr());
    rdb.Update(mode+"Lzero","percent",totL0eff.GetEff(),totL0eff.GetEffErr());
    rdb.Update(mode+"HltOne","percent",Hlt1eff.GetEff(),Hlt1eff.GetEffErr());
    rdb.Update(mode+"ThreeBody","percent",Hlt2eff[0].GetEff(),Hlt2eff[0].GetEffErr());
    rdb.Update(mode+"FourBody","percent",Hlt2eff[1].GetEff(),Hlt2eff[1].GetEffErr());
    rdb.Update(mode+"HltTwo","percent",totHlt2eff.GetEff(),totHlt2eff.GetEffErr());
    rdb.Update(mode+"total","percent",toteff.GetEff(),toteff.GetEffErr());
    rdb.Save();
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
    ("output-file", value<string>(&dbf)->default_value("TrigEff.csv"), "output file"         )
    ("input-file" , value<string>(&filename                         ), "input file"          )
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
  GetTrigEff(filename,vmap.count("save"),dbf);
  return 0;
}
