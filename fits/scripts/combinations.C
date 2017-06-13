{
	int ncomb = 0;
	std::cout << "ID \t# res\t# free params" << std::endl;
	for(const std::string& swave1 : {"", "f0(1370)", "f0(1500)"})
		for(const std::string& pwave1 : {"", "ϕ(1680)"})
			for(const std::string& swave2 : {"", "f0(1710)"})
			{
				std::vector<std::string> resonances = {"nonres", "f0(980)", "ϕ(1020)", swave1, "f2´(1525)", pwave1, swave2};
				int nres = 0;
				int nfps = 11; // nonres = 1 size, f0(980) = 1 phase, ϕ(1020) = 2 amp + 2 phase, f2´(1525) = 1 size + 2 amp + 3 phases
				for(const std::string& res: resonances)
					if(res != "")
						nres++;
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
			}
}
