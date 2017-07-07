#include <fstream>
#include <map>
#include <algorithm>
#include "printsource.h"

void printsource(std::vector<std::string> resonances)
{
	std::map<std::string, bool> config;
	for(auto key: {"nophi", "altbarrier", "altflatte", "splitbyyear", "splitbytrigger", "splitbymagnet"})
		config[key] = false;
	// Construct the filename
	std::string filename {""};
	for(const std::string& res: resonances)
	{
		filename += res;
		if(&res != &resonances.back())
			filename += "_";
		for(auto& conf: config)
			if(res == conf.first)
				conf.second = true;
	}
	resonances.erase(std::remove_if(resonances.begin(), resonances.end(), [&](const std::string res) -> bool
	{
		for(const auto& conf: config)
			if(res == conf.first)
				return true;
		return false;
	}), resonances.end());
	std::vector<std::string> years = {""}, triggers = {""}, polarities = {""}, barriers = {"BsJpsiKK"}, flatteparams = {""};
	if(config["splitbyyear"])
		years = {"2011", "2012"};
	if(config["splitbytrigger"])
		triggers = {"TOS", "notTOS"};
	if(config["splitbymagnet"])
		polarities = {"magup", "magdown"};
	if(config["altbarrier"])
		barriers = {"EvtGen", "BaBar"};
	if(config["altflatte"])
		flatteparams = {"_BES", "_CLEO"};
	for(const auto& year: years)
		for(const auto& trigger: triggers)
			for(const auto& polarity: polarities)
				for(const auto& barrier: barriers)
					for(const auto& flatte: flatteparams)
					{
						std::string tmpfilename = filename;
						for(const auto& suffix: {year, trigger, polarity})
							if(suffix != "")
								tmpfilename+="_"+suffix;
						if(config["altbarrier"])
							tmpfilename+="_"+barrier+"_barriers";
						if(config["altflatte"])
							tmpfilename+=flatte+"_flatte";
						std::ofstream file(tmpfilename+".list", std::ios::out);
						file << "pdf/signal/Bs2PhiKK.xml\n";
						file << "pdf/signal/mKKresolution.xml\n";
						file << "pdf/signal/moments_acceptanceTOS.xml\n";
						file << "pdf/background/Bs2PhiKK.xml\n";
						file << "pdf/background/combinatorial_nomva_moments.xml\n";
						file << "minimiser/minuit2.xml\n";
						file << "fitfunction/default.xml\n";
						file << "dataset/data_mva.xml\n";
						file << "dataset/Bs_LOKI_Mass_2sigma_cut.xml\n";
						if(config["splitbyyear"])
							file << "dataset/"+year+"_only.xml\n";
						if(config["splitbytrigger"])
							file << "dataset/"+trigger+"_only.xml\n";
						if(config["splitbymagnet"])
							file << "dataset/"+polarity+"_only.xml\n";
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
								if(config["nophi"])
									file << "parameterset/amplitudes/"+name+"_float_LHCbAmp_fixdeltazero.xml\n";
								else
									file << "parameterset/amplitudes/"+name+"_float_LHCbAmp_fixdeltazero_fixdeltaplus.xml\n";
								file << "constraintfunction/"+name+"_constraint.xml\n";
							}
							else if(res.find("ftwop1525") != std::string::npos)
							{
								file << "parameterset/fractions/"+name+"_float.xml\n";
								file << "parameterset/resonances/"+name+"_float.xml\n";
								file << "parameterset/amplitudes/"+name+"_float.xml\n";
								file << "constraintfunction/"+name+"_constraint.xml\n";
							}
							else if(res.find("fzero980") != std::string::npos)
							{
								file << "parameterset/fractions/"+name+"_float.xml\n";
								file << "parameterset/resonances/"+name+"_fixed"+flatte+".xml\n";
								file << "parameterset/amplitudes/"+name+"_float.xml\n";
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
						file << "parameterset/barrierfactorradii_fixed_"+barrier+".xml\n";
						file << "parameterset/signal_fraction_1800_fixed.xml\n";
						file << "parameterset/backgrounds/combinatorial_hist.xml\n";
						file << "phasespaceboundary/mKK_BCON_1800.xml\n";
						if(config["nophi"])
							file << "phasespaceboundary/angles_BCON_nophi.xml\n";
						else
							file << "phasespaceboundary/angles_BCON.xml\n";
						file << "output/lowrange.xml\n";
						file << "output/fullrange_ymax.xml\n";
						if(!config["nophi"])
							file << "output/phi.xml\n";
						file << "output/ctheta_1.xml\n";
						file << "output/ctheta_2.xml\n";
						file << "phasespaceboundary/TOS.xml\n";
					}
}
