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
void GetCutEff(string filename, string beforecut, string aftercut, bool save, string resname, string DBfilename)
{
  TTree* intree = GetTree(filename);
  new TCanvas;
  CutResult_t result = CutEff(intree, beforecut, aftercut);
  if(save)
  {
    ResultDB rdb(DBfilename);
    rdb.Update(resname,"percent",result.GetEff(),result.GetEffErr());
    rdb.Save();
  }
}
int main(int argc, char* argv[])
{
  string filename, beforecut, aftercut, dbf, res;
  using namespace boost::program_options;
  options_description desc("Allowed options",120);
  desc.add_options()
    ("help,H"      ,                                                       "produce help message")
    ("save"        ,                                                       "save the results"    )
    ("output-file" , value<string>(&dbf)->default_value("CutEff.csv"    ), "output file"         )
    ("result-name" , value<string>(&res)->default_value("efficiency"    ), "result name"         )
    ("input-file"  , value<string>(&filename                            ), "input file"          )
    ("initial-cut" , value<string>(&beforecut)->default_value("B_s0_M>0"), "initial cut"         )
    ("final-cut"   , value<string>(&aftercut                            ), "final cut"           )
  ;
  variables_map vmap;
  positional_options_description pd;
  pd.add("input-file", 1);
  pd.add("final-cut", 2);
  store(command_line_parser(argc, argv).options(desc).positional(pd).run(), vmap);
  notify(vmap);
  if (vmap.count("help"))
  {
    std::cout << desc << endl;
    return 1;
  }
  if(filename.empty())
  {
    cout << "Usage: " << argv[0] << " <filename> <cut>" << endl;
    return 1;
  }
  GetCutEff(filename,beforecut,aftercut,vmap.count("save"),res,dbf);
  return 0;
}
