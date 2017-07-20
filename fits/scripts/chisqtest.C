#include <Math/SpecFuncMathCore.h>
// grep -h "chi2:" toyjob_*/*.o* | sed 's/chi2:\s*//g' > chisq.txt
// ROOT::Math::chisquared_pdf
double chisq_pdf(double* x, double* par)
{
	return par[0]*ROOT::Math::chisquared_pdf(x[0],par[1]);
}

void chisqtest(std::string filename, double data_chisq)
{
	std::ifstream data(filename);
	double xmin = 80, xmax = 200;
	TH1D hist("chisq","",24,xmin,xmax);
	double chisq;
	while(data >> chisq)
	{
		hist.Fill(chisq);
	}
	TF1 pdf("pdf",&chisq_pdf,xmin, xmax, 2, 1);
	pdf.SetParameter(0,10);
	pdf.SetParameter(1,125);
	TCanvas can;
	hist.Fit(&pdf,"","",xmin,xmax);
	hist.Draw("E1");
	hist.SetLineColor(kBlack);
	TLine line;
	line.SetLineStyle(kDotted);
	line.DrawLine(data_chisq,0,data_chisq,hist.GetMaximum());
	gStyle->SetOptStat(0);
	hist.GetXaxis()->SetTitle("#chi^{2}");
	for(TAxis* axis : {hist.GetXaxis(),hist.GetYaxis()})
	{
		axis->SetTitleSize(0.05);
		axis->SetLabelSize(0.05);
		axis->SetTitleFont(132);
		axis->SetLabelFont(132);
	}
	std::cout << "Probability of χ²: " << TMath::Prob(data_chisq, pdf.GetParameter(1)) << std::endl;
	can.SaveAs("chisq.pdf");
}

