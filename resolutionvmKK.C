{
const int n=9;
double x[n], y[n];
// x in GeV, y in MeV
x[0] = 2*0.493677; y[0] = 0;
x[1] = 1.058;	y[1] = 1.16473;
x[2] = 1.152;	y[2] = 1.72031;
x[3] = 1.252;	y[3] = 2.13986;
x[4] = 1.350;	y[4] = 2.42880;
x[5] = 1.450;	y[5] = 2.78261;
x[6] = 1.551;	y[6] = 3.05661;
x[7] = 1.650;	y[7] = 3.21820;
x[8] = 1.750;	y[8] = 3.44150;
double xerr = (x[n-1]-x[0])/(2.*n);
double xe[n]; fill_n(xe,n,xerr);
double ye[n]; fill_n(ye,n,0);
TGraphErrors graph(n,x,y,xe,ye);
graph.SetMarkerStyle(20);
graph.Draw("AP");

TF1 fun("fun",Form("x>%f?pow([0]*TMath::Abs((x-%f)),0.5):0",x[0],x[0]));
fun.SetLineStyle(2);
fun.SetLineColor(2);
fun.SetParameters(1,333);
graph.Fit(&fun, "","",x[0]-xerr/10, x[8]+xerr);
graph.SetTitle("");
graph.GetXaxis()->SetTitle("#it{m}(#it{K}^{+}#it{K}^{#minus}) [GeV/#it{c}^{2}]");
graph.GetYaxis()->SetTitle("#it{#Delta m}(#it{K}^{+}#it{K}^{#minus}) [MeV/#it{c}^{2}]");
for(TAxis* axis : {graph.GetXaxis(),graph.GetYaxis()})
{
  axis->SetTitleSize(0.05);
  axis->SetLabelSize(0.05);
  axis->SetTitleFont(132);
  axis->SetLabelFont(132);
}
graph.GetXaxis()->SetRangeUser(x[0]-xerr,x[n-1]+xerr);
gPad->SetMargin(0.1,0.02,0.11,0.02);
gPad->SaveAs("latex/figs/resolution.pdf");
}
