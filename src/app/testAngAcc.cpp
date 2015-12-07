#include "AutoAngAcc.h"
#include "TH1D.h"
#include "TH3D.h"
#include "TPad.h"
#include "TFile.h"
#include "progbar.h"
#include <iostream>
#include "TStyle.h"
#include "TCanvas.h"
#include <string>

using namespace std;
void testAngAcc()
{

  int n = 100;
  double v[3];
  
  double* lo = AutoAngAcc::gXMin;
  double* hi = AutoAngAcc::gXMax;
  //
  TH3D* hist = new TH3D("hist","",n,lo[0],hi[0]
                                 ,n,lo[1],hi[1]
                                 ,n,lo[2],hi[2]);
  //
  for(int i = 0; i < n; i++)
  {
    v[0] = i*(hi[0]-lo[0])/(double)n+lo[0];
    for(int j = 0; j < n; j++)
    {
      v[1] = j*(hi[1]-lo[1])/(double)n+lo[1];
      for(int k = 0; k < n; k++)
      {
        v[2] = k*(hi[2]-lo[2])/(double)n+lo[2];
        double a = AutoAngAcc::Eval(v);
        hist->SetBinContent(i+1,j+1,k+1,a);
      }
    }
  }
  TFile* file = new TFile("AngAcc.root");
  TH1D* data[3] = {(TH1D*)file->Get("x_0_orig")
                  ,(TH1D*)file->Get("x_1_orig")
                  ,(TH1D*)file->Get("x_2_orig")};
  TH1D* fit[3] = {hist->ProjectionX()
                 ,hist->ProjectionY()
                 ,hist->ProjectionZ()};
  string fname[3] = {"projX","projY","projZ"};
  string title[3] = {"#Phi","cos #theta_{1}","cos #theta_{2}"};
  double scale;
  gStyle->SetOptStat(0);
  for(int i = 0; i < 3; i++)
  {
    new TCanvas();
    gPad->SetBottomMargin(0.15);
    scale = data[i]->Integral()/fit[i]->Integral();
    fit[i]->Scale(scale);
    data[i]->GetXaxis()->SetTitle(title[i].c_str());
    data[i]->GetXaxis()->SetTitleSize(0.05);
    data[i]->SetTitle("");
    data[i]->Draw("PE");
    fit[i]->SetLineColor(kRed);
    data[i]->SetLineColor(kBlack);
    fit[i]->Draw("Lsame");
    gPad->SaveAs((fname[i]+".pdf").c_str());
  }
}
int main()
{
  testAngAcc();
  return 0;
}
