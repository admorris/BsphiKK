#include <cstdlib>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "datum.h"
#define expected_num_args 6
#define mKK_lower_lim 1020
#define mKK_upper_lim 2000
#define minfrac 90

int main(int argc, char* argv[])
{
	if(argc != expected_num_args)
	{
		std::cerr << argc-1 << " arguments provided. " << expected_num_args << " expected." << std::endl;
		std::cerr << "Usage: " << argv[0] << " <upper mKK limit> <signal yield> <sig yield error> <background yield> <bkg yield error>" << std::endl;
		return 1;
	}
	int m = std::atoi(argv[1]);
	if(m < mKK_lower_lim || m > mKK_upper_lim)
			std::cerr << "Warning: mKK limit is not between 1.02 and 2 GeV. This seems unlikely." << std::endl;
	datum S  = {std::atof(argv[2]), std::atof(argv[3])};
	datum B  = {std::atof(argv[4]), std::atof(argv[5])};
	for(auto& N : {S,B})
		if(N.val() <= 0 || N.err() <= 0)
		{
			std::cerr << "One argument is <= 0. Will not continue." << std::endl;
			return 1;
		}
	datum f = {S.val()/(S.val()+B.val()), S.val() * B.err()/((S.val()+B.val())*(S.val()+B.val()))};
	std::cout << " With m(KK) < " << std::setprecision(4) << m << " GeV, the signal fraction is " << f << std::endl;
	if(f.val() <= 0 || std::isnan(f.val()))
	{
		std::cerr << "Impossible fraction. Will not continue." << std::endl;
		return 1;
	}
	else if (f.val() < minfrac/100.)
		std::cerr << "Warning: fraction is below " << minfrac << "%. This seems unlikely." << std::endl;
	// Write the XML file
	std::ofstream XML("signal_fraction_" + std::to_string(m) + "_fixed.xml", std::ios::out);
	XML << "# S: " << S << "\n"
	    << "# B: " << B << "\n"
	    << "<PhysicsParameter>\n"
	    << "\t<Name>signal_fraction</Name>\n"
	    << "\t<Value>" << f.val() << "</Value> # ± " << f.err() << "\n"
	    << "\t<Type>Fixed</Type>\n"
	    << "</PhysicsParameter>" << std::endl;
	XML.close();
	return 0;
}
