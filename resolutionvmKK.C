{
int n=9;
double x[9], y[9];
x[0] = 2*0.493677; y[0] = 0;
x[1] = 1.058;	y[1] = 1.16473e-03;
x[2] = 1.152;	y[2] = 1.72031e-03;
x[3] = 1.252;	y[3] = 2.13986e-03;
x[4] = 1.350;	y[4] = 2.42880e-03;
x[5] = 1.450;	y[5] = 2.78261e-03;
x[6] = 1.551;	y[6] = 3.05661e-03;
x[7] = 1.650;	y[7] = 3.21820e-03;
x[8] = 1.750;	y[8] = 3.44150e-03;
TGraph graph(n,x,y);
graph.SetMarkerStyle(20);
graph.Draw("AP");

TF1 *fun = new TF1("fun","pow([0]*TMath::Abs((x-2*0.493677)),0.5)", 0.1, 100e3);
fun->SetLineStyle(2);
fun->SetLineColor(2);
fun->SetParameters(1,333);
graph.Fit(fun, "","",x[0], 1.8);


}
