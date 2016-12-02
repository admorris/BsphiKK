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
  double x[4];
  // Begin adaptive binning stuff
  int nbins = 50;
  const int n = gentree->GetEntries() - gentree->GetEntries()%nbins;
  cout << "Attempting to make an array with " << n*4 << " elements" << endl;
  vector<double> data;
  for(int i = 0; i < n*4; i++)
    data.push_back(0); //lol
  gentree->SetBranchAddress("Phi_angle" ,&x[0]);
  gentree->SetBranchAddress("cos_theta1",&x[1]);
  gentree->SetBranchAddress("cos_theta2",&x[2]);
  gentree->SetBranchAddress("KK_M"      ,&x[3]);
  for(int i = 0; i < n; i++)
  {
    gentree->GetEntry(i);
    x[3]/=1000; // MeV to GeV
    for(int j = 0; j < 4; j++)
      data[n*j+i] = sym ? TMath::Abs(x[j]) : x[j];
  }
  TKDTreeBinning binner(n,4,&data[0],nbins);
  // End adaptive binning stuff
  TFile output("Acceptance.root","RECREATE");
  NDHist_Adaptive genhist(&binner);
  Fill(x, gentree, genhist, "KK_M", sym);
  new TCanvas;
  genhist.BinContentHist()->Draw();
  gPad->SaveAs("GenBinDist.pdf");
  NDHist_Adaptive selhist = genhist;
  selhist.Clear();
  Fill(x, seltree, selhist, "BCON_KK_M", sym);
  new TCanvas;
  selhist.BinContentHist()->Draw();
  gPad->SaveAs("SelBinDist.pdf");
  NDHist_Adaptive acchist = selhist / genhist;
  acchist.SaveToTree()->Write();
  binner.GetTree()->Write();
  output.Close();
  
  
  // Plot the distributions of bin edges
  vector<TGraph*> graphs;
  for(int idim = 4; idim-->0;)
    graphs.push_back(new TGraph(nbins*2));
  cout << "Initialised graphs for bin edges" << endl;
  for(int ibin = 0; ibin < nbins; ibin++)
  {
    for(int idim = 0; idim < 4; idim++)
    {
      graphs[idim]->SetPoint(ibin*2  ,binner.GetBinMinEdges(ibin)[idim],acchist.GetBinContent(ibin));
      graphs[idim]->SetPoint(ibin*2+1,binner.GetBinMaxEdges(ibin)[idim],acchist.GetBinContent(ibin));
    }
  }
  cout << "Drawing graphs" << endl;
  TCanvas can;
  can.Divide(2,2);
  vector<string> axis_title = {"#Phi"
                              ,"cos(#theta_{1})"
                              ,"cos(#theta_{2})"
                              ,"m(KK) [GeV]"};
  for(int idim = 0; idim < 4; idim++)
  {
    can.cd(idim+1);
    graphs[idim]->SetTitle("");
    graphs[idim]->Draw("A*");
    graphs[idim]->GetXaxis()->SetTitle(axis_title[idim].c_str());
    graphs[idim]->SetMinimum(0.0);
  }
  can.SaveAs("BinEdgeDist.pdf");
  
  
  ResultDB table("Acceptance.csv");
  table.Update("accNgen","N",gentree->GetEntries(),0);
  table.Update("accNsel","N",seltree->GetEntries(),0);
  table.Update("accNbins","N",binner.GetNBins(),0);
  table.Save();
}
int main(int argc, char* argv[])
{
  AngularAcceptance(argv[1], argv[2]);
  return 0;
}

