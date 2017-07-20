void extractphitransversity(std::string filename)
{
	TTree* tree = static_cast<TTree*>(TFile::Open(filename.c_str())->Get("RapidFitResult"));
	double phi1020_Aplus_value; tree->SetBranchAddress("phi1020_Aplus_value",&phi1020_Aplus_value);
	double phi1020_deltaplus_value; tree->SetBranchAddress("phi1020_deltaplus_value",&phi1020_deltaplus_value);
	double phi1020_Azero_value; tree->SetBranchAddress("phi1020_Azero_value",&phi1020_Azero_value);
	double phi1020_deltazero_value; tree->SetBranchAddress("phi1020_deltazero_value",&phi1020_deltazero_value);
	double phi1020_deltaminus_value; tree->SetBranchAddress("phi1020_deltaminus_value",&phi1020_deltaminus_value);
	int Fit_Status; tree->SetBranchAddress("Fit_Status",&Fit_Status);
	TH1D Aperpsq_hist("Aperpsq_hist","|A⟂|²",100, 0, 1);
	TH1D Azerosq_hist("Azerosq_hist","|A0|²",100, 0, 1);
	TH1D deltaparasq_hist("deltaparasq_hist"," δ∥  ",100, -2*M_PI, 2*M_PI);
	
	for(unsigned i = 0; tree->GetEntry(i), i < tree->GetEntries(); i++)
	{
		if(Fit_Status!=3)
			continue;
		std::complex<double> Aplus = std::polar<double>(phi1020_Aplus_value, phi1020_deltaplus_value);
		std::complex<double> Azero = std::polar<double>(phi1020_Azero_value, phi1020_deltazero_value);
		std::complex<double> Aminus = std::polar<double>(std::sqrt(1. - std::pow(phi1020_Aplus_value,2) - std::pow(phi1020_Azero_value,2)),phi1020_deltaminus_value);
		std::complex<double> Apara = (Aplus + Aminus) / std::sqrt(2.);
		std::complex<double> Aperp = (Aplus - Aminus) / std::sqrt(2.);
		Aperpsq_hist.Fill(std::pow(std::abs(Aperp),2));
		Azerosq_hist.Fill(std::pow(std::abs(Azero),2));
		deltaparasq_hist.Fill(std::arg(Apara));
	}
	for(auto* hist: {&Aperpsq_hist, &Azerosq_hist, &deltaparasq_hist})
	{
		std::cout << hist->GetTitle() << " : "
		<< std::right << std::setw(10) << std::setprecision(3) << hist->GetMean() << " ± "
		<< std::left << std::setw(10) << std::setprecision(3) << hist->GetStdDev()
		<< std::endl;
	}
}

