#include <iostream>
#include <string>
#include <vector>

#include "TFile.h"
#include "TTree.h"

using namespace std;

class FourDHist
{
  public:
    FourDHist(int,double,double,
              int,double,double,
              int,double,double,
              int,double,double); // nbins, low, high ×4
    Fill(double,double,double,double);
  private:
    int nbinsw,
    double wlo,whi,xlo,xhi,ylo,yhi,zlo,zhi; // Limits
    double**** bincontent;
};
FourDHist::FourDHist( int,double,double,
                      int,double,double,
                      int,double,double,
                      int,double,double)
: 
{

}
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
  double xlo = imposesymmetry ? 0 : -TMath::Pi();
  double xup = +TMath::Pi();
  double xrange = xup - xlo;
  int nbinsy = 10;
  double ylo = imposesymmetry ? 0 : -1;
  double yup = +1;
  double yrange = yup - ylo;
  int nbinsz = 20;
  double zlo = imposesymmetry ? 0 : -1;
  double zup = +1;
  double zrange = zup - zlo;
  // For TH3::Interpolate() to work properly, the points have to be within the centres of the edge bins
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
}

int main(int argc, char* argv[])
{
  AngularAcceptance(argv[1]);
  return 0;
}

