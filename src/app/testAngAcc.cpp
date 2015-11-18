#include "AutoAngAcc.h"
#include "TH1D.h"
#include "TH3D.h"
#include "TPad.h"
#include "TFile.h"
#include "progbar.h"
#include <iostream>
#include "TStyle.h"

using namespace std;
void testAngAcc()
{

  int n = 100;
  double v[3];
  double xlow = AutoAngAcc::gXMin[0];
  double xup  = AutoAngAcc::gXMax[0];
  double ylow = AutoAngAcc::gXMin[1];
  double yup  = AutoAngAcc::gXMax[1];
  double zlow = AutoAngAcc::gXMin[2];
  double zup  = AutoAngAcc::gXMax[2];
  //
  TH3D* hist = new TH3D("hist","",n,xlow,xup,n,ylow,yup,n,zlow,zup);
  //
  for(int i = 0; i < n; i++)
  {
    v[0] = i*(xup-xlow)/(double)n+xlow;
    for(int j = 0; j < n; j++)
    {
      v[1] = j*(yup-ylow)/(double)n+ylow;
      for(int k = 0; k < n; k++)
      {
        v[2] = k*(zup-zlow)/(double)n+zlow;
        double a = AutoAngAcc::Eval(v);
        hist->SetBinContent(i+1,j+1,k+1,a);
      }
    }
  }
  gStyle->SetOptStat(0);
  hist->ProjectionX()->Draw();
  gPad->SaveAs("projX.pdf");
  hist->ProjectionY()->Draw();
  gPad->SaveAs("projY.pdf");
  hist->ProjectionZ()->Draw();
  gPad->SaveAs("projZ.pdf");
}
int main()
{
  testAngAcc();
  return 0;
}
