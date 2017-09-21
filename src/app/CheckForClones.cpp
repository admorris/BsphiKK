#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

std::vector<int> GetSortedKeys(std::string line)
{
  std::vector<int> keys;
  size_t ltab = line.find('\t',0); // skip event number
  for(int i = 0; i < 4; i++)
  {
    ltab = line.find('\t',ltab+1); // skip run number on i = 0
    keys.push_back(atoi(line.substr(ltab,line.find('\t',ltab+1)).c_str()));
  }
  std::sort(keys.begin(),keys.end());
  return keys;
}

int main(int argc, char* argv[])
{
  std::ifstream file(argv[1]);
  std::string thisline(""),lastline(""),thisevtno,lastevtno;
  while(file.good())
  {
    lastline = thisline;
    std::getline(file,thisline,'\n');
    lastevtno = lastline.substr(0,lastline.find('\t'));
    thisevtno = thisline.substr(0,thisline.find('\t'));
    if(lastevtno == thisevtno)
    {
      std::cout << lastline << std::endl << thisline;
      std::vector<int> lastkeys = GetSortedKeys(lastline);
      std::vector<int> thiskeys = GetSortedKeys(thisline);
      if(lastkeys[0] == thiskeys[0]
      && lastkeys[1] == thiskeys[1]
      && lastkeys[2] == thiskeys[2]
      && lastkeys[3] == thiskeys[3])
        std::cout << "\tCLONE";
      std::cout << std::endl;
    }
  }
  return 0;
}
