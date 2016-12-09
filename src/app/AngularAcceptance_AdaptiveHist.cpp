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
//  TTree* gentree = GetTree(genfile,"KK_M<1800");
  TTree* gentree = GetTree(genfile);
//  TTree* seltree = GetTree(selfile,"BCON_KK_M<1800&&abs(KK_TRUEID)>500");
  TTree* seltree = GetTree(selfile);
  vector<string> branches = { "KK_M"
                            , "Phi_angle"
                            , "cos_theta1"
                            , "cos_theta2"
                            };
  vector<string> axis_title = {"m(K^{#plus}K^{#minus})"
                              ,"#Phi"
                              ,"cos(#theta_{1})"
                              ,"cos(#theta_{2})"
                              };
  vector<double> dimscale = { 1e-3
                            , 0.1
                            , 1.
                            , 1.
                            };
  const int ndims = branches.size();
  vector<double> x;
  for(int i = ndims; i-->0;)
    x.push_back(0);
  for(auto& branch : branches)
    gentree->SetBranchAddress(branch.c_str(), &x[&branch - &branches[0]]);
  // Begin adaptive binning stuff
  int nbins = 50;
  const int n = gentree->GetEntries() - gentree->GetEntries()%nbins;
  vector<double> data;
  for(int i = n*ndims; i-->0;)
    data.push_back(0); //lol
  for(int i = 0; i < n; i++)
  {
    gentree->GetEntry(i);
    for(int j = 0; j < ndims; j++)
      data[n*j+i] = dimscale[j] * (sym ? TMath::Abs(x[j]) : x[j]);
  }
  TKDTreeBinning binner(n,ndims,&data[0],nbins);
  // End adaptive binning stuff
  TFile output("Acceptance.root","RECREATE");
  NDHist_Adaptive genhist(&binner);
  genhist.SetDimScales(dimscale);
  Fill(branches, gentree, genhist, sym);
  NDHist_Adaptive selhist = genhist;
  selhist.Clear();
//  for(auto& branch : branches) branch = "BCON_"+branch;
  cout << "Renamed branches" << endl;
  Fill(branches, seltree, selhist, sym);
  NDHist_Adaptive acchist = selhist / genhist;
  acchist.SaveToTree()->Write();
  binner.GetTree()->Write();
  genhist.BinContentHist()->Draw();
  gPad->SaveAs("GenBinDist.pdf");
  selhist.BinContentHist()->Draw();
  gPad->SaveAs("SelBinDist.pdf");
  // Plot the distributions of bin edges
  vector<TGraph*> graphs;
  for(int idim = ndims; idim-->0;)
    graphs.push_back(new TGraph(nbins*2));
  for(int ibin = nbins; ibin-->0;)
    for(int idim = 0; idim < ndims; idim++)
    {
      graphs[idim]->SetPoint(ibin*2  ,binner.GetBinMinEdges(ibin)[idim]/dimscale[idim],acchist.GetBinContent(ibin));
      graphs[idim]->SetPoint(ibin*2+1,binner.GetBinMaxEdges(ibin)[idim]/dimscale[idim],acchist.GetBinContent(ibin));
    }

  TCanvas can;
  can.Divide(2,2);
  for(int idim = 0; idim < ndims; idim++)
  {
    can.cd(idim+1);
    graphs[idim]->SetName((branches[idim]+"BinEdgeDist").c_str());
    graphs[idim]->SetTitle("");
    graphs[idim]->GetXaxis()->SetTitle(axis_title[idim].c_str());
    graphs[idim]->SetMinimum(0.0);
    graphs[idim]->Draw("A*");
  }
  can.SaveAs("BinEdgeDist.pdf");
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

