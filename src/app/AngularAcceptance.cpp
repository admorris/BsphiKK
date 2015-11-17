#include <iostream>
#include <string>

#include "TFile.h"
#include "TTree.h"

#include "TCanvas.h"
#include "TH3.h"
#include "TMultiDimFit.h"
#include "TAxis.h"

#include "progbar.h"

using namespace std;

double D(TH3D* hist, double x, double y, double z) // Don't need this any more, since Interpolate() now works
{
  int bin_x = hist->GetXaxis()->FindBin(x);
  int bin_y = hist->GetYaxis()->FindBin(y);
  int bin_z = hist->GetZaxis()->FindBin(z);
  return hist->GetBinContent(bin_x,bin_y,bin_z);
}

void AngularAcceptance(string filename)
{
  // Input
  TFile* file = new TFile(filename.c_str());
  TTree* tree = (TTree*)file->Get("DecayTree");
  int n = tree->GetEntries();
  double x[3],d;
  tree->SetBranchAddress("Phi_angle", &x[0]);
  tree->SetBranchAddress("cos_theta1",&x[1]);
  tree->SetBranchAddress("cos_theta2",&x[2]);
  // Configure and make the 3D histogram
  int nbins = 10;
  double xlow = -3.14159;
  double xup  = +3.14159;
  double xrange = xup - xlow;
  double ylow = -1;
  double yup  = +1;
  double yrange = yup - ylow;
  double zlow = -1;
  double zup  = +1;
  double zrange = zup - zlow;
  // For TH3::Interpolate() to work properly, the points have to be within the centres of the edge bins
  TH3D* hist = new TH3D("hist","",nbins,xlow-(xrange/(nbins)),xup+(xrange/(nbins))  // Phi range
                                 ,nbins,ylow-(yrange/(nbins)),yup+(yrange/(nbins))  // cos_theta1 range
                                 ,nbins,zlow-(zrange/(nbins)),zup+(zrange/(nbins)));// cos_theta2 range
  // Fill the histogram
  cout << "Filling a 3D histogram with " << n << " events." << endl;
  progbar bar(n);
  for(int i = 0; i < n; i++)
  {
    tree->GetEntry(i);
    hist->Fill(x[0],x[1],x[2]);
    if(i%(n/100)==0)
    {
      bar.print(i);
    }
  }
  bar.terminate();
//  cout << "Normalising the histogram." << endl;
//  hist->Scale(1.0/hist->Integral());
  // Output
  TFile* outfile = new TFile("AngAcc.root", "RECREATE");
  TMultiDimFit* fit = new TMultiDimFit(3, TMultiDimFit::kLegendre);
  // Configuration
  int maxpowers[3] = {4,4,6};
  fit->SetMaxPowers(maxpowers);
  fit->SetMaxFunctions(1000);
  fit->SetMaxStudy(10000);
  fit->SetMaxTerms(30);
  fit->SetPowerLimit(1);
  fit->SetMinAngle(10);
  fit->SetMaxAngle(10);
  fit->SetMinRelativeError(.01);
  // Event loop
  for(int i = 0; i < n; i++)
  {
    tree->GetEntry(i);
    d = hist->Interpolate(x[0],x[1],x[2]);
//    d = D(hist,x[0],x[1],x[2]);
    if(i < n/2)
    {
      fit->AddRow(x,d); // Training sample
    }
    else
    {
      fit->AddTestRow(x,d); // Test sample
    }
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
  fit->MakeCode();
  outfile->Write();
  outfile->Close();
}

int main(int argc, char* argv[])
{
  AngularAcceptance(argv[1]);
  return 0;
}
