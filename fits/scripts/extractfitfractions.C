double getvalue(TTree* tree, std::string name)
{
	tree->Draw(name.c_str(),"","goff");
	TH1* hist = static_cast<TH1*>(gDirectory->Get("htemp"));
	double value = hist->GetMean();
	delete hist;
	return value;
}

void extractfitfractions(std::string filename)
{
	TTree* tree = static_cast<TTree*>(TFile::Open(filename.c_str())->Get("combination_0"));
	for(auto branch: *tree->GetListOfBranches())
	{
		std::string name = branch->GetName();
		std::cout << endl;
		std::cout << std::left<< std::setw(40) << name.substr(name.find_first_of('_'))
		<< std::left<< std::setw(40) << getvalue(tree, name);
	}
	std::cout << endl;
}

