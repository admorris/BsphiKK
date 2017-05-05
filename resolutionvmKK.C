{
int n=9;
double x[9], y[9];
x[0] = 2*0.493677; y[0] = 0;
x[1] = 1.058;	y[1] = 1.16473;//e-03;
x[2] = 1.152;	y[2] = 1.72031;//e-03;
x[3] = 1.252;	y[3] = 2.13986;//e-03;
x[4] = 1.350;	y[4] = 2.42880;//e-03;
x[5] = 1.450;	y[5] = 2.78261;//e-03;
x[6] = 1.551;	y[6] = 3.05661;//e-03;
x[7] = 1.650;	y[7] = 3.21820;//e-03;
x[8] = 1.750;	y[8] = 3.44150;//e-03;
std::vector<double> xe(n,(x[n-1]-x[0])/(2.*n));
std::vector<double> ye(n,0);
TGraphErrors graph(n,x,y,xe.data(),ye.data());
graph.SetMarkerStyle(20);
graph.Draw("AP");

TF1 *fun = new TF1("fun","pow([0]*TMath::Abs((x-2*0.493677)),0.5)", 0.1, 100e3);
fun->SetLineStyle(2);
fun->SetLineColor(2);
fun->SetParameters(1,333);
graph.Fit(fun, "","",x[0], 1.8);
graph.SetTitle("");
graph.GetXaxis()->SetTitle("#it{m}(#it{K}^{+}#it{K}^{#minus}) [GeV/#it{c}^{2}]");
graph.GetYaxis()->SetTitle("#it{#Delta m}(#it{K}^{+}#it{K}^{#minus}) [MeV/#it{c}^{2}]");
graph.GetXaxis()->SetTitleSize(0.05);
graph.GetYaxis()->SetTitleSize(0.05);
graph.GetXaxis()->SetLabelSize(0.05);
graph.GetYaxis()->SetLabelSize(0.05);
graph.GetXaxis()->SetTitleFont(132);
graph.GetYaxis()->SetTitleFont(132);
graph.GetXaxis()->SetLabelFont(132);
graph.GetYaxis()->SetLabelFont(132);

gPad->SaveAs("/home/adam/documents/thesis/body/BsPhiKK/figs/resolution.pdf");
}
