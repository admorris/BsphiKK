#include <iostream>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TKDTreeBinning.h"

#include "GetTree.h"
#include "NDHist_Adaptive.h"
#include "AngularAcceptanceFill.h"
#include "ResultDB.h"

using namespace std;
void AngularAcceptance(string selfile, string genfile)
{
  bool sym = true;
  TTree* gentree = GetTree(genfile,"KK_M<1800");
  TTree* seltree = GetTree(selfile,"BCON_KK_M<1800&&abs(KK_TRUEID)>500");
  vector<string> branches = { "KK_M"
                            , "cos_theta1"
                            , "cos_theta2"
                            , "Phi_angle"
                            };
  vector<double> x;
  for(auto branch : branches)
  {
    x.push_back(0);
    gentree->SetBranchAddress(branch.c_str(), &x.back());
  }
  const int ndims = branches.size();
  // Begin adaptive binning stuff
  int nbins = 50;
  const int n = gentree->GetEntries() - gentree->GetEntries()%nbins;
  cout << "Attempting to make an array with " << n*ndims << " elements" << endl;
  vector<double> data;
  for(int i = n*ndims; i-->0;)
    data.push_back(0); //lol
  for(int i = 0; i < n; i++)
  {
    gentree->GetEntry(i);
    x[0]/=1000; // MeV to GeV
    for(int j = 0; j < ndims; j++)
      data[n*j+i] = sym ? TMath::Abs(x[j]) : x[j];
  }
  TKDTreeBinning binner(n,ndims,&data[0],nbins);
  // End adaptive binning stuff
  TFile output("Acceptance.root","RECREATE");
  NDHist_Adaptive genhist(&binner);
  Fill(branches, gentree, genhist, sym);
  genhist.BinContentHist("genhistcontent")->Write();
  NDHist_Adaptive selhist = genhist;
  selhist.Clear();
  branches[0] = "BCON_KK_M"; // rename the mass branch for the selected MC
  Fill(branches, seltree, selhist, sym);
  selhist.BinContentHist("selhistcontent")->Write();
  NDHist_Adaptive acchist = selhist / genhist;
  acchist.SaveToTree()->Write();
  binner.GetTree()->Write();
  // Plot the distributions of bin edges
  vector<TGraph*> graphs;
  for(int idim = ndims; idim-->0;)
    graphs.push_back(new TGraph(nbins*2));
  for(int ibin = nbins; ibin-->0;)
    for(int idim = ndims; idim-->0;)
    {
      graphs[idim]->SetPoint(ibin*2  ,binner.GetBinMinEdges(ibin)[idim],acchist.GetBinContent(ibin));
      graphs[idim]->SetPoint(ibin*2+1,binner.GetBinMaxEdges(ibin)[idim],acchist.GetBinContent(ibin));
    }
  vector<string> axis_title = {"m(KK) [GeV]"
                              ,"cos(#theta_{1})"
                              ,"cos(#theta_{2})"
                              ,"#Phi"};
  for(int idim = 0; idim < ndims; idim++)
  {
    graphs[idim]->SetName((branches[idim]+"BinEdgeDist").c_str());
    graphs[idim]->SetTitle("");
    graphs[idim]->GetXaxis()->SetTitle(axis_title[idim].c_str());
    graphs[idim]->SetMinimum(0.0);
    graphs[idim]->Write();
  }
  output.Close();
  cout << "Saving results" << endl;
  ResultDB table("Acceptance.csv");
  table.Update("accNgen","N",gentree->GetEntries(),0);
  table.Update("accNsel","N",seltree->GetEntries(),0);
  table.Update("accNbins","N",binner.GetNBins(),0);
  table.Save();
  cout << "End" << endl;
  return;
}
int main(int argc, char* argv[])
{
  AngularAcceptance(argv[1], argv[2]);
  return 0;
}

