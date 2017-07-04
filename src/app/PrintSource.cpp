#include "printsource.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

int main (int argc, char* argv[])
{
	std::vector<std::string> resonances;
	for(int i = 1; i < argc; i++)
		resonances.push_back(argv[i]);
	printsource(resonances);
	return 0;
}

