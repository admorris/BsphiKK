#include "CutEff.h"
#include "TFile.h"
#include "GetTree.h"
#include "TrigEffPlot.h"
void PlotEffvsmKK(std::string filename,std::string plotname,std::string cutstring)
{
  const unsigned int nbins = 20;
  double mKKlo = 990, mKKhi = 1800, binw = (mKKhi-mKKlo)/nbins;
  double mKK[nbins], mKKE[nbins];
  TTree* intree = GetTree(filename);
  TFile outputfile("TrigEffPlots.root","RECREATE");
  TrigEffPlot Plot("vsmKK",nbins,"",cutstring);
  for(unsigned int ibin = 0; ibin < nbins; ibin++)
  {
    double binlo = mKKlo+ibin*binw, binhi = mKKlo+(ibin+1)*binw;
    mKK[ibin] = (binhi+binlo)/2.0;
    mKKE[ibin] = binw/2.0;
    std::string mKKcut = "(KK_M>"+tostr(binlo)+"&&KK_M<"+tostr(binhi)+")";
    CutResult_t Result = CutEff(intree,mKKcut+Plot.befcut,Plot.aftcut);
    Plot.y [ibin] = Result.GetEff()   *100.0;
    Plot.ye[ibin] = Result.GetEffErr()*100.0;
  }
  TGraphErrors graph(nbins,mKK,Plot.y.data(),mKKE,Plot.ye.data());
  graph.SetMinimum(80);
  graph.SetMaximum(100);
  graph.Fit("pol0");
  DrawGraph(plotname+Plot.name,graph)->Write();
}
int main(int argc, char* argv[])
{
  if(argc!=4)
  {
    std::cout << "Usage: " << argv[0] << " <filename> <plot name prefix> <cutstring>" << std::endl;
    return 1;
  }
  PlotEffvsmKK(argv[1],argv[2],argv[3]);
  return 0;
}

