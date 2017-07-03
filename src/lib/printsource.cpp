#include <fstream>
#include "printsource.h"

void printsource(std::vector<std::string> resonances, bool phi)
{
	// Construct the filename
	std::string filename {""};
	for(const std::string& res: resonances)
	{
		filename += res;
		if(&res != &resonances.back())
			filename += "_";
		else
			filename += ".list";
	}
	std::ofstream file(filename, std::ios::out);
	file << "pdf/signal/Bs2PhiKK.xml\n";
	file << "pdf/signal/mKKresolution.xml\n";
	file << "pdf/signal/moments_acceptanceTOS.xml\n";
	file << "pdf/background/Bs2PhiKK.xml\n";
	file << "pdf/background/combinatorial_nomva_moments.xml\n";
	file << "minimiser/minuit2.xml\n";
	file << "fitfunction/default.xml\n";
	file << "dataset/data_mva.xml\n";
	file << "dataset/Bs_LOKI_Mass_2sigma_cut.xml\n";
	for(const std::string& res: resonances)
	{
		std::string option = "";
		std::string name = res;
		size_t optpos = res.find("_");
		if(optpos != std::string::npos)
		{
			name = res.substr(0, optpos);
			option = res.substr(optpos + 1);
		}
		if(res.find("nonres") != std::string::npos)
		{
			file << "parameterset/fractions/nonres_float.xml\n";
			file << "parameterset/amplitudes/nonres_float.xml\n";
		}
		else if(res.find("phi1020") != std::string::npos)
		{
			file << "parameterset/fractions/"+name+"_fixed.xml\n";
			file << "parameterset/resonances/"+name+"_float.xml\n";
			if(phi)
				file << "parameterset/amplitudes/"+name+"_float_LHCbAmp_fixdeltazero_fixdeltaplus.xml\n";
			else
				file << "parameterset/amplitudes/"+name+"_float_LHCbAmp_fixdeltazero.xml\n";
			file << "constraintfunction/"+name+"_constraint.xml\n";
		}
		else if(res.find("ftwop1525") != std::string::npos)
		{
			file << "parameterset/fractions/"+name+"_float.xml\n";
			file << "parameterset/resonances/"+name+"_float.xml\n";
			file << "parameterset/amplitudes/"+name+"_float.xml\n";
			file << "constraintfunction/"+name+"_constraint.xml\n";
		}
		else
		{
			file << "parameterset/fractions/"+name+"_float.xml\n";
			file << "parameterset/resonances/"+name+"_fixed.xml\n";
			if(option == "transverse" || option == "longitudinal")
				file << "parameterset/amplitudes/"+name+"_float_"+option+".xml\n";
			else
				file << "parameterset/amplitudes/"+name+"_float.xml\n";
		}
	}
	file << "parameterset/deltaGammas_fixed.xml\n";
	file << "parameterset/mKKrespars_fixed.xml\n";
	file << "parameterset/thraccscale_fixed_TOS.xml\n";
	file << "parameterset/barrierfactorradii_fixed_EvtGen.xml\n";
	file << "parameterset/signal_fraction_1800_fixed.xml\n";
	file << "parameterset/backgrounds/combinatorial_hist.xml\n";
	file << "phasespaceboundary/mKK_BCON_1800.xml\n";
	if(phi)
		file << "phasespaceboundary/angles_BCON.xml\n";
	else
		file << "phasespaceboundary/angles_BCON_nophi.xml\n";
	file << "output/lowrange.xml\n";
	file << "output/fullrange_ymax.xml\n";
	if(phi)
		file << "output/phi.xml\n";
	file << "output/ctheta_1.xml\n";
	file << "output/ctheta_2.xml\n";
	file << "phasespaceboundary/TOS.xml\n";
}

