#include <iostream>
#include <fstream>
#include <string>
#include <vector>

void printsource(std::vector<std::string> resonances)
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
	ofstream file(filename, std::ios::out);
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
		if(res == "nonres")
			file << "parameterset/fractions/nonres_float.xml\n";
		else if(res == "phi1020")
		{
			file << "parameterset/fractions/"+res+"_fixed.xml\n";
			file << "parameterset/resonances/"+res+"_float_mass.xml\n";
			file << "parameterset/amplitudes/"+res+"_float_LHCbAmp_onephase.xml\n";
		}
		else if(res == "ftwop1525LHCb" || res == "ftwop1525")
		{
			file << "parameterset/fractions/"+res+"_float.xml\n";
			file << "parameterset/resonances/"+res+"_float.xml\n";
			file << "parameterset/amplitudes/"+res+"_fixed_zero.xml\n";
			file << "constraintfunction/"+res+"_constraint.xml\n";
		}
		else
		{
			file << "parameterset/fractions/"+res+"_float.xml\n";
			file << "parameterset/resonances/"+res+"_fixed.xml\n";
			file << "parameterset/amplitudes/"+res+"_float.xml\n";
		}
	}
	file << "parameterset/deltaGammas_fixed.xml\n";
	file << "parameterset/mKKrespars_fixed.xml\n";
	file << "parameterset/thraccscale_fixed_TOS.xml\n";
	file << "parameterset/barrierfactorradii_fixed_EvtGen.xml\n";
	file << "parameterset/signal_fraction_1800_fixed.xml\n";
	file << "parameterset/backgrounds/combinatorial_hist.xml\n";
	file << "phasespaceboundary/mKK_BCON_1800.xml\n";
	file << "phasespaceboundary/angles_BCON.xml\n";
	file << "output/lowrange.xml\n";
	file << "output/fullrange_ymax.xml\n";
	file << "output/phi.xml\n";
	file << "output/ctheta_1.xml\n";
	file << "output/ctheta_2.xml\n";
	file << "phasespaceboundary/TOS.xml\n";
}
int main()
{
	int ncomb = 0;
	std::cout << "ID \t# res\t# free params" << std::endl;
	for(const std::string& swave1 : {"", "fzero1370LHCb", "fzero1500LHCb"})
		for(const std::string& pwave1 : {"", "phi1680"})
			for(const std::string& swave2 : {"", "fzero1710"})
			{
				std::vector<std::string> resonances {};
				int nres = 0;
				int nfps = 11; // nonres = 1 size, f0(980) = 1 phase, ϕ(1020) = 2 amp + 2 phase + mass, f2´(1525) = 1 size + 2 amp + mass + width
				for(const std::string& res: {std::string("nonres"), std::string("fzero980"), std::string("phi1020"), swave1, std::string("ftwop1525LHCb"), pwave1, swave2})
					if(res != "")
					{
						nres++;
						resonances.push_back(res);
					}
				if(swave1 != "")
					nfps += 2; // 1 size + 1 phase
				if(swave2 != "")
					nfps += 2; // 1 size + 1 phase
				if(pwave1 != "")
					nfps += 6; // 1 size + 2 amp + 3 phases
				ncomb++;
				std::cout << ncomb << "\t" << nres << "\t" << nfps << "\t";
				for(const std::string& res: resonances)
					if(res != "")
						std::cout << res + " ";
				std::cout << std::endl;
				printsource(resonances);
			}
	return 0;
}
void combinations()
{
	main();
}

