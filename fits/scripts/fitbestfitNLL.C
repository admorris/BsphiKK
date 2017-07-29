void setytitle(TAxis* axis,double binw)
{
	axis->CenterTitle();
	stringstream ytitle;
	ytitle << "#font[132]{}Results / (";
	if(binw > 10)
		ytitle << TMath::Nint(binw);
	else if (binw > 0.1)
		ytitle << TMath::Nint(binw*10.0)/10.0;
	else
		ytitle << TMath::Nint(binw*pow(10.0,ceil(-log10(binw))))*pow(10.0,floor(log10(binw)));
	ytitle << ")";
	axis->SetTitle(ytitle.str().c_str());
}
void fitbestfitNLL(std::string filename, double dataNLL)
{
	TTree* tree = static_cast<TTree*>(TFile::Open(filename.c_str())->Get("RapidFitResult"));
	TH1D hist("hist","",12,2800,4000);
	tree->Draw("NLL>>hist");
	double NLL; tree->SetBranchAddress("NLL", &NLL);
	for(int i = 0; i < tree->GetEntries(); i++)
	{
		tree->GetEntry(i);
		hist.Fill(NLL);
	}
	TCanvas can;
	gPad->SetMargin(0.15,0.05,0.15,0.02);
	hist.Draw("E1");
	hist.Fit("gaus");
	hist.SetLineColor(kBlack);
	TLine line;
	line.SetLineStyle(kDashed);
	line.DrawLine(dataNLL,0,dataNLL,hist.GetMaximum());
	gStyle->SetOptStat(0);
	hist.GetXaxis()->SetTitle("-ln(#it{L})");
	// Style
	for(TAxis* axis : {hist.GetXaxis(),hist.GetYaxis()})
	{
		axis->SetTitleSize(0.07);
		axis->SetLabelSize(0.07);
		axis->SetTitleFont(132);
		axis->SetLabelFont(132);
		axis->SetLabelOffset(0.02);
	}
	setytitle(hist.GetYaxis(),hist.GetXaxis()->GetBinWidth(1));
	can.SetTicks(1,1);
	can.SaveAs("NLL.pdf");
}

