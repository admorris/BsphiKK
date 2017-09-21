#include <fstream>
#include <map>
#include <algorithm>
#include <iostream>
#include "printsource.h"

void printsource(std::vector<std::string> resonances)
{
	std::map<std::string, bool> config;
	for(auto key: {"nophi", "altbarrier", "altflatte", "alt1680", "altresolution", "floatflatte", "notminLb", "splitbyyear", "splitbytrigger", "splitbymagnet", "toys", "nopeaking", "conssigfrac", "conspeaking", "alttrigger", "altlifetime", "PIDcalib"})
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
	std::vector<std::string> years = {""}, triggers = {""}, lifetimes = {""}, polarities = {""}, barriers = {"EvtGen"}, flatteparams = {""}, floatflatteoptions = {""}, resolutions = {""};
	if(config["splitbyyear"])
		years = {"2011", "2012"};
	if(config["splitbytrigger"])
	{
		if(!config["alttrigger"])
			triggers = {"TOS", "notTOS"};
		else
			triggers = {"TIS", "notTIS"};
	}
	if(config["splitbymagnet"])
		polarities = {"magup", "magdown"};
	if(config["altbarrier"])
		barriers = {"BsJpsiKK", "BaBar"};
	if(config["altflatte"])
		flatteparams = {"_BES", "_CLEO"};
	if(config["floatflatte"])
		floatflatteoptions = {"", "_both"};
	if(config["altresolution"])
		resolutions = {"_upper", "_lower"};
	if(config["altlifetime"])
		lifetimes = {"_14ps", "_17ps"};
	for(const auto& year: years)
	for(const auto& trigger: triggers)
	for(const auto& lifetime: lifetimes)
	for(const auto& polarity: polarities)
	for(const auto& barrier: barriers)
	for(const auto& flatte: flatteparams)
	for(const auto& floatflatteoption: floatflatteoptions)
	for(const auto& resolution: resolutions)
	{
		std::string tmpfilename = filename;
		for(const auto& suffix: {year, trigger, polarity})
			if(suffix != "")
				tmpfilename+="_"+suffix;
		if(config["altbarrier"])
			tmpfilename+="_"+barrier+"_barriers";
		if(config["altflatte"])
			tmpfilename+=flatte+"_flatte";
		if(config["floatflatte"])
			tmpfilename+=floatflatteoption;
		if(config["altresolution"])
			tmpfilename+=resolution+"resolution";
		std::cout << "Creating " << tmpfilename << std::endl;
		std::ofstream file(tmpfilename+".list", std::ios::out);
		file << "pdf/signal/Bs2PhiKK.xml\n";
		file << "pdf/signal/mKKresolution.xml\n";
		if(!config["alttrigger"])
		{
			if(config["altlifetime"])
				file << "pdf/signal/moments_acceptanceTOS" << lifetime << ".xml\n";
			else if(config["PIDcalib"])
				file << "pdf/signal/moments_acceptanceTOS_PIDcalib.xml\n";
			else
				file << "pdf/signal/moments_acceptanceTOS.xml\n";
		}
		else
			file << "pdf/signal/moments_acceptanceTIS.xml\n";
		file << "pdf/signal/PhiWindowWeight.xml\n";
		if(config["notminLb"])
			file << "pdf/signal/NotMinLb.xml\n";
		file << "pdf/background/Bs2PhiKK.xml\n";
		file << "pdf/background/combinatorial_nomva_moments.xml\n";
		file << "minimiser/minuit2.xml\n";
		file << "fitfunction/default.xml\n";
		if(config["toys"])
			file << "dataset/bestfit_toys.xml\n";
		else
		{
			file << "dataset/data_mva.xml\n";
			file << "dataset/Bs_LOKI_Mass_2sigma_cut.xml\n";
			if(config["splitbyyear"])
				file << "dataset/"+year+"_only.xml\n";
			if(config["splitbytrigger"])
				file << "dataset/"+trigger+"_only.xml\n";
			if(config["splitbymagnet"])
				file << "dataset/"+polarity+"_only.xml\n";
		}
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
					file << "parameterset/amplitudes/"+name+"_float_LHCbAmp_fixdeltazero_fixdeltaminus.xml\n";
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
				if(config["floatflatte"])
				{
					file << "parameterset/resonances/"+name+"_float"+flatte+floatflatteoption+".xml\n";
					file << "constraintfunction/"+name+"_constraint.xml\n";
				}
				else
					file << "parameterset/resonances/"+name+"_fixed"+flatte+".xml\n";
				file << "parameterset/amplitudes/"+name+"_float.xml\n";
			}
			else
			{
				file << "parameterset/fractions/"+name+"_float.xml\n";
				if(config["alt1680"] && name.find("1680") != std::string::npos)
				{
					file << "parameterset/resonances/"+name+"_float.xml\n";
					file << "constraintfunction/"+name+"_constraint.xml\n";
				}
				else
					file << "parameterset/resonances/"+name+"_fixed.xml\n";
				if(option == "transverse" || option == "longitudinal")
					file << "parameterset/amplitudes/"+name+"_float_"+option+".xml\n";
				else
					file << "parameterset/amplitudes/"+name+"_float.xml\n";
			}
		}
		file << "parameterset/deltaGammas_fixed.xml\n";
		file << "parameterset/mKKrespars_fixed"+resolution+".xml\n";
		file << "parameterset/thraccscale_fixed_TOS.xml\n";
		file << "parameterset/barrierfactorradii_fixed_"+barrier+".xml\n";
		if(config["conssigfrac"])
		{
			file << "parameterset/signal_fraction_1800_float.xml\n";
			file << "constraintfunction/signal_fraction_1800_constraint.xml\n";
		}
		else
			file << "parameterset/signal_fraction_1800_fixed.xml\n";
		file << "parameterset/backgrounds/combinatorial_hist.xml\n";
		if(!config["nopeaking"])
		{
			if(config["conspeaking"])
			{
				file << "parameterset/backgrounds/BdphiKstar_hist_float.xml\n";
				file << "parameterset/backgrounds/Lb2PhiKp_hist_float.xml\n";
				file << "constraintfunction/BdphiKstar_hist.xml\n";
				file << "constraintfunction/Lb2PhiKp_hist.xml\n";
			}
			else
			{
				file << "parameterset/backgrounds/BdphiKstar_hist.xml\n";
				file << "parameterset/backgrounds/Lb2PhiKp_hist.xml\n";
			}
		}
		if(config["toys"])
			file << "phasespaceboundary/variables_toys.xml\n";
		else
		{
			file << "phasespaceboundary/mKK_BCON_1800.xml\n";
			if(config["nophi"])
				file << "phasespaceboundary/angles_BCON_nophi.xml\n";
			else
				file << "phasespaceboundary/angles_BCON.xml\n";
			if(!config["alttrigger"])
				file << "phasespaceboundary/TOS.xml\n";
			else
				file << "phasespaceboundary/TIS.xml\n";
		}
		file << "output/lowrange.xml\n";
		file << "output/fullrange_ymax.xml\n";
		if(!config["nophi"])
			file << "output/phi.xml\n";
		file << "output/ctheta_1.xml\n";
		file << "output/ctheta_2.xml\n";
	}
}

