std::string substitute(std::string source, const std::string& find, const std::string& replace)
{
	for(std::string::size_type i = 0; (i = source.find(find, i)) != std::string::npos;)
	{
		source.replace(i, find.length(), replace);
		i += replace.length();
	}
	return source;
}

double plotbranch(TTree* tree, std::string valuename, std::string genname, std::string plotname)
{
	TCanvas can;
	double min = tree->GetMinimum(valuename.c_str()) - tree->GetMaximum(genname.c_str());
	double max = tree->GetMaximum(valuename.c_str()) - tree->GetMinimum(genname.c_str());
	double lim = std::max(std::abs(max), std::abs(min));
	gStyle->SetOptStat(0);
	TH1D htemp("htemp", "", 20, -lim, lim);
	tree->Draw((valuename+"-"+genname+">>htemp").c_str(),"","hist");
	TFitResultPtr fit = htemp.Fit("gaus","S");
	double mean = fit->GetParams()[1];
	double width = fit->GetParams()[2];
	std::cout << mean/width << " sigma" << std::endl;
	can.SaveAs(plotname.c_str());
	return  mean/width;
}

void plotvaluebranches(std::string filename)
{
	TTree* tree = static_cast<TTree*>(TFile::Open(filename.c_str())->Get("RapidFitResult"));
	int ndp = 3;
	TH1D hist("hist","",20, -2, +2);
	for(auto branch: *tree->GetListOfBranches())
	{
		std::string name = branch->GetName();
		if(name.find("_gen") != std::string::npos)
		{
			std::string valuename = substitute(name, "gen", "value");
			double pull = plotbranch(tree, valuename, name, substitute(name, "_gen", ".pdf"));
			hist.Fill(pull);
		}
	}
	TCanvas can;
	hist.Draw();
	can.SaveAs("pull.pdf");
}

