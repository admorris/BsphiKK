#include <string>
#include <vector>
#include <iostream>
#include "boost/program_options.hpp"

int main(int argc, char* argv[])
{
  std::vector<std::string> names;
  std::vector<double> masses;
  std::string PX, PY, PZ;
  using namespace boost::program_options;
  options_description desc("Allowed options",120);
  desc.add_options()
    ("help,H", "produce help message")
    ("particles", value<std::vector<std::string>>(&names)->multitoken(), "particles")
    ("masses", value<std::vector<double>>(&masses)->multitoken(), "masses")
    ("PXbranch",value<std::string>(&PX)->default_value("_PX"), "PX branch name")
    ("PYbranch",value<std::string>(&PY)->default_value("_PY"), "PY branch name")
    ("PZbranch",value<std::string>(&PZ)->default_value("_PZ"), "PZ branch name")
  ;
  variables_map vmap;
  store(parse_command_line(argc, argv, desc), vmap);
  notify(vmap);
  if (vmap.count("help"))
  {
    std::cout << desc << std::endl;
    return 1;
  }
  return 0;
}
