#include <iostream>
#include <string>

#include "TFile.h"
#include "TTree.h"

#include "TCanvas.h"
#include "TH3.h"
#include "TMultiDimFit.h"
#include "TAxis.h"
#include "TMath.h"

#include "progbar.h"

using namespace std;

//double D(TH3D* hist, double x, double y, double z) // Don't need this any more, since Interpolate() now works
//{
//  int bin_x = hist->GetXaxis()->FindBin(x);
//  int bin_y = hist->GetYaxis()->FindBin(y);
//  int bin_z = hist->GetZaxis()->FindBin(z);
//  return hist->GetBinContent(bin_x,bin_y,bin_z);
//}

void AngularAcceptance(string filename)
{
  bool imposesymmetry = true;
  // Input
  TFile* file = new TFile(filename.c_str());
  TTree* tree = (TTree*)file->Get("DecayTree");
  int n = tree->GetEntries();
  double x[3],d;
  tree->SetBranchAddress("Phi_angle", &x[0]);
  tree->SetBranchAddress("cos_theta1",&x[1]);
  tree->SetBranchAddress("cos_theta2",&x[2]);
  // Configure and make the 3D histogram
  int nbinsx = 10;
  double xlow = imposesymmetry ? 0 : -TMath::Pi();
  double xup  = +TMath::Pi();
  double xrange = xup - xlow;
  int nbinsy = 10;
  double ylow = imposesymmetry ? 0 : -1;
  double yup  = +1;
  double yrange = yup - ylow;
  int nbinsz = 20;
  double zlow = imposesymmetry ? 0 : -1;
  double zup  = +1;
  double zrange = zup - zlow;
  // For TH3::Interpolate() to work properly, the points have to be within the centres of the edge bins
  TH3D* hist = new TH3D("hist","",nbinsx,xlow-(xrange/(nbinsx)),xup+(xrange/(nbinsx))  // Phi range
                                 ,nbinsy,ylow-(yrange/(nbinsy)),yup+(yrange/(nbinsy))  // cos_theta1 range
                                 ,nbinsz,zlow-(zrange/(nbinsz)),zup+(zrange/(nbinsz)));// cos_theta2 range
  // Fill the histogram
  cout << "Filling a 3D histogram with " << n << " events." << endl;
  progbar bar(n);
  for(int i = 0; i < n; i++)
  {
    tree->GetEntry(i);
    imposesymmetry ?
      hist->Fill(TMath::Abs(x[0]),TMath::Abs(x[1]),TMath::Abs(x[2]))
      :
      hist->Fill(x[0],x[1],x[2]);
    if(i%(n/100)==0)
    {
      bar.print(i);
    }
  }
  bar.terminate();
  // Output
  TFile* outfile = new TFile("AngAcc.root", "RECREATE");
  TMultiDimFit* fit = new TMultiDimFit(3, TMultiDimFit::kLegendre,"KV");
  // Configuration
  int maxpowers[3] = {2,2,6};
  fit->SetMaxPowers(maxpowers);
  fit->SetMaxFunctions(100000);
  fit->SetMaxStudy(1000000);
  fit->SetMaxTerms(30);
  fit->SetPowerLimit(1);
  fit->SetMinAngle(10);
  fit->SetMaxAngle(10);
  fit->SetMinRelativeError(.01);
  // Event loop
  for(int i = 0; i < n; i++)
  {
    tree->GetEntry(i);
    d = imposesymmetry ?
      hist->Interpolate(TMath::Abs(x[0]),TMath::Abs(x[1]),TMath::Abs(x[2]))
      :
      hist->Interpolate(x[0],x[1],x[2]);
    (i < n/2) ?
      fit->AddRow(x,d) // Training sample
      :
      fit->AddTestRow(x,d); // Test sample
  }
  // Print starting parameters
  fit->Print("p");
  // Print out the statistics
  fit->Print("s");
  // Book histograms
  fit->MakeHistograms();
  // Find the parameterization
  fit->FindParameterization();
  // Print coefficents
  fit->Print("rc");
  // Do the fit
  fit->Fit("M");
  // Print result
  fit->Print("fc");
  // Write code to file
  fit->MakeMethod("AutoAngAcc");
  hist->Write();
  outfile->Write();
  outfile->Close();
}

int main(int argc, char* argv[])
{
  AngularAcceptance(argv[1]);
  return 0;
}

