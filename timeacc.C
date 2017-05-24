{
	auto file = std::unique_ptr<TFile>(TFile::Open("ntuples/BsphiKK_MC_mvacut.root"));
	TTree* tree = static_cast<TTree*>(file->Get("DecayTree"));
	TH1D hist("hist","",100,0,0.01);
	tree->Draw("B_s0_TAU>>hist","exp(B_s0_TAU/0.00153)/10000");
	TF1 fitfunc;
	fitfunc = TF1("fitfunc","x>[2]?[0]*((([1]*(x-[2]))^[3])/(1+([1]*(x-[2]))^[3])):0");
	fitfunc.SetParNames("A","b","t_0","n");
	fitfunc.SetParameters(1,1e3,1e-4,3);
	/*
	fitfunc.SetParameter(0,1);
	fitfunc.SetParameter(1,1e-3);
	fitfunc.SetParameter(2,1e-4);
	fitfunc.SetParameter(3,3);
	*/
	hist.Fit(&fitfunc);
}
