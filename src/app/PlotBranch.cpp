// Standard headers
#include <string>
// BOOST headers
#include "boost/program_options.hpp"
// ROOT headers
#include "TFile.h"
#include "TTree.h"
// Custom headers
#include "plotmaker.h"


void PlotBranch(string filename, string branchname, string xtitle, string unit, double xlow, double, xup)
{

}

int main(int argc, int argv[])
{
  using namespace boost::program_options;
  options_description desc("Allowed options");
  std::string file, branch, xtitle, unit;
  desc.add_options()
    ("help,H"  ,                                                                                                    "produce help message"                      )
    ("file,F"  , value<std::string>(&file  )->default_value("ntuples/BsphiKK_data_mva.root"                      ), "set data file"                             )
    ("branch,B", value<std::string>(&branch)->default_value("B_s0_LOKI_Mass"                                     ), "set branch to plot"                        )
    ("title,T" , value<std::string>(&xtitle)->default_value("#it{m}(#it{K^{#plus}K^{#minus}K^{#plus}K^{#minus}})"), "set x-axis title (takes ROOT LaTeX format)")
    ("unit,U"  , value<std::string>(&unit  )->default_value("MeV/#it{c}^{2}"                                     ), "set unit (takes ROOT LaTeX format)"        )
  ;
  variables_map vmap;
  store(parse_command_line(argc, argv, desc), vmap);
  notify(vmap);
  if (vmap.count("help"))
  {
    std::cout << desc << endl;
    return 1;
  }
  
  return 0;
}
