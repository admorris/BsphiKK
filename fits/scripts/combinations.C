#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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
		{
			file << "parameterset/fractions/nonres_float.xml\n";
			file << "parameterset/amplitudes/nonres_float.xml\n";
		}
		else if(res == "phi1020")
		{
			file << "parameterset/fractions/"+res+"_fixed.xml\n";
			file << "parameterset/resonances/"+res+"_float.xml\n";
			if(phi)
				file << "parameterset/amplitudes/"+res+"_float_LHCbAmp_fixdeltazero_fixdeltaplus.xml\n";
			else
				file << "parameterset/amplitudes/"+res+"_float_LHCbAmp_fixdeltazero.xml\n";
			file << "constraintfunction/"+res+"_constraint.xml\n";
		}
		else if(res == "ftwop1525LHCb" || res == "ftwop1525")
		{
			file << "parameterset/fractions/"+res+"_float.xml\n";
			file << "parameterset/resonances/"+res+"_float.xml\n";
			file << "parameterset/amplitudes/"+res+"_float.xml\n";
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
void combinations(bool phi = true)
{
	int ncomb = 0;
	std::cout << "ID \t# res\t# free params\n";
	for(const std::string& swave1 : {""/*, "fzero1370LHCb", "fzero1500LHCb"*/})
		for(const std::string& pwave1 : {"", "phi1680"})
			for(const std::string& dwave1 : {"", "ftwo1640"})
				for(const std::string& swave2 : {"", "fzero1710"})
					for(const std::string& dwave2 : {"", "ftwo1750"})
					{
						std::vector<std::string> resonances {};
						int nres = 0;
						int nfps;
						if(phi)
							nfps = 17; // nonres = 2, f0(980) = 2, ϕ(1020) = 4, f2´(1525) = 8
						else
							nfps = 13; // nonres = 2, f0(980) = 2, ϕ(1020) = 3, f2´(1525) = 6
						for(const std::string& res: {std::string("nonres"), std::string("fzero980"), std::string("phi1020"), swave1, std::string("ftwop1525LHCb"), dwave1, pwave1, swave2, dwave2})
							if(res != "")
							{
								nres++;
								resonances.push_back(res);
							}
						for(const auto& comp: {swave1, swave2})
							if(comp != "")
								nfps += 2; // 1 size + 1 phase
						for(const auto& comp: {pwave1, dwave1, dwave2})
							if(comp != "")
							{
								if(phi)
									nfps += 6; // 1 size + 2 amp + 3 phases
								else
									nfps += 4; // 1 size + 1 amp + 2 phases
							}
						ncomb++;
						std::cout << ncomb << "\t" << nres << "\t" << nfps << "\t";
						for(const std::string& res: resonances)
							if(res != "")
								std::cout << res + " ";
						std::cout << std::endl;
						printsource(resonances, phi);
					}
}

