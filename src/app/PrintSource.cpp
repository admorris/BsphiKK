#include "printsource.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

int main (int argc, char* argv[])
{
	std::vector<std::string> resonances;
	bool phi = true;
	for(int i = 1; i < argc; i++)
	{
		std::string res = argv[i];
		resonances.push_back(res);
		if(resonances.back() == "nophi")
			phi = false;
	}
	resonances.erase(std::remove_if(resonances.begin(), resonances.end(), [&](const std::string res) -> bool {return res == "nophi";}), resonances.end());
	printsource(resonances, phi);
	return 0;
}

