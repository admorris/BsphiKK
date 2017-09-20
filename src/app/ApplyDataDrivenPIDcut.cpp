#include "TFile.h"
#include "TH2.h"
#include <iostream>
#include <string>
#include "substitute.h"
#include "GetTree.h"

int main(int argc, char* argv[])
{
	if(argc != 6)
	{
		std::cerr << "Usage: " << argv[0] << " <input ntuple> <output ntuple> <mag up file> <mag down file> <hist name>" << std::endl;
		return 1;
	}
	TTree* inputtree = GetTree(argv[1]);
	std::map<short, TH2*> hist;
	std::cout << "Opening " << argv[3] << std::endl;
	hist[-1] = static_cast<TH2*>(TFile::Open(argv[3])->Get(argv[5]));
	std::cout << "Opening " << argv[4] << std::endl;
	hist[+1] = static_cast<TH2*>(TFile::Open(argv[4])->Get(argv[5]));
	TFile outputfile(argv[2], "RECREATE");
	TTree* outputtree = inputtree->CloneTree(0);
	short polarity; inputtree->SetBranchAddress("Polarity", &polarity);
	double K_P[4], K_PT[4];
	unsigned i(0);
	for(const std::string kaon: {"Kminus", "Kplus", "Kminus0", "Kplus0"})
	{
		inputtree->SetBranchAddress((kaon+"_P").c_str(), &K_P[i]);
		inputtree->SetBranchAddress((kaon+"_PT").c_str(), &K_PT[i]);
		i++;
	}
	unsigned n_bef(inputtree->GetEntries()), n_aft(0);
	for(i = 0; i < n_bef; ++i)
	{
		inputtree->GetEntry(i);
		bool accept(true);
		for(unsigned j(0); j < 4; ++j)
		{
			double p = K_P[j];
			double eta = std::acosh(K_P[j]/K_PT[j]);
			int bin = hist[polarity]->FindBin(p, eta);
			double eff = hist[polarity]->GetBinContent(bin);
			accept = accept && std::rand()/(double)RAND_MAX <= eff;
		}
		if(accept)
		{
			outputtree->Fill();
			n_aft++;
		}
	}
	std::cout << "Kept " << n_aft << " of " << n_bef << " events." << std::endl;
	outputtree->Write();
	outputfile.Close();
	return 0;
}

