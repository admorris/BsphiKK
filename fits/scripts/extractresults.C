double getvalue(TTree* tree, std::string name)
{
	tree->Draw(name.c_str(),"","goff");
	TH1* hist = static_cast<TH1*>(gDirectory->Get("htemp"));
	double value = hist->GetMean();
	delete hist;
	return value;
}

void extractresults(std::string filename)
{
	TTree* tree = static_cast<TTree*>(TFile::Open(filename.c_str())->Get("RapidFitResult"));
	for(auto branch: *tree->GetListOfBranches())
	{
		std::string name = branch->GetName();
		if(name.find("_value") != std::string::npos)
		{
			std::cout << endl;
			std::cout << std::left<< std::setw(40) << name.substr(0,name.find_last_of('_'))
			<< std::left<< std::setw(40) << getvalue(tree, name);
		}
		else if(name.find("_error") != std::string::npos)
		{
			std::cout << std::left<< std::setw(40) << getvalue(tree, name);
		}
	}
	std::cout << endl;
}

