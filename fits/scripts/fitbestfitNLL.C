void fitbestfitNLL(std::string filename, double dataNLL)
{
	TTree* tree = static_cast<TTree*>(TFile::Open(filename.c_str())->Get("RapidFitResult"));
	TCanvas can;
	TH1D hist("hist","",12,2800,4000);
	tree->Draw("NLL>>hist","","E1");
	hist.Fit("gaus");
	hist.SetLineColor(kBlack);
	TLine line;
	line.SetLineStyle(kDotted);
	line.DrawLine(dataNLL,0,dataNLL,hist.GetMaximum());
	gStyle->SetOptStat(0);
	hist.GetXaxis()->SetTitle("-ln(#it{L})");
	// Style
	for(TAxis* axis : {hist.GetXaxis(),hist.GetYaxis()})
	{
		axis->SetTitleSize(0.05);
		axis->SetLabelSize(0.05);
		axis->SetTitleFont(132);
		axis->SetLabelFont(132);
	}
	can.SaveAs("NLL.pdf");
}

