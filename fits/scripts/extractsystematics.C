double getstddev(TTree* tree, std::string name)
{
	tree->Draw(name.c_str(),"Fit_Status==3","goff");
	TH1* hist = static_cast<TH1*>(gDirectory->Get("htemp"));
	double value = hist->GetStdDev();
	delete hist;
	return value;
}

void extractsystematics(std::string filename)
{
	TTree* tree = static_cast<TTree*>(TFile::Open(filename.c_str())->Get("RapidFitResult"));
	int ndp = 3;
	for(auto branch: *tree->GetListOfBranches())
	{
		std::string name = branch->GetName();
		if(name.find("_value") != std::string::npos)
		{
			double syst = getstddev(tree, name);
			std::cout << std::left<< std::setw(40) << name.substr(0,name.find_last_of('_'))
			<< std::left<< std::setw(40) << std::round(std::pow(10,ndp)*syst)/std::pow(10,ndp)
			<< std::left<< std::setw(40) << syst << std::endl;
		}
	}
}

