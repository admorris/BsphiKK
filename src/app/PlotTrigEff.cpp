#include "CutEff.h"
#include "TFile.h"
#include "GetTree.h"
#include "TrigEffPlot.h"
void PlotTrigEff(std::string filename,std::string plotname)
{
  std::string L0[]={"B_s0_L0HadronDecision_TOS", "B_s0_L0Global_TIS"};
  std::string Hlt1="B_s0_Hlt1TrackAllL0Decision_TOS";
  std::string Hlt2[]={"B_s0_Hlt2Topo3BodyBBDTDecision_TOS", "B_s0_Hlt2Topo4BodyBBDTDecision_TOS", "B_s0_Hlt2Topo2BodyBBDTDecision_TOS", "B_s0_Hlt2IncPhiDecision_TOS"};
  const unsigned int nbins = 20;
  double mKKlo = 990, mKKhi = 1800, binw = (mKKhi-mKKlo)/nbins;
  double mKK[nbins], mKKE[nbins];
  TTree* intree = GetTree(filename);
  TFile outputfile("TrigEffPlots.root","RECREATE");
  std::vector<TrigEffPlot> Plots;
  Plots.push_back(TrigEffPlot("HadronTOS",nbins,"",    L0[0]                                                       ));
  Plots.push_back(TrigEffPlot("GlobalTIS",nbins,"",               L0[1]                                            ));
  Plots.push_back(TrigEffPlot("Level0Tot",nbins,"","("+L0[0]+"||"+L0[1]+")"                                        ));
  Plots.push_back(TrigEffPlot("Hlt1Total",nbins,"&& ("+L0[0]+"||"+L0[1]+")",   Hlt1                                ));
  Plots.push_back(TrigEffPlot("Topo3Body",nbins,"&& ("+L0[0]+"||"+L0[1]+")&&("+Hlt1+")",   Hlt2[0]                 ));
  Plots.push_back(TrigEffPlot("Topo4Body",nbins,"&& ("+L0[0]+"||"+L0[1]+")&&("+Hlt1+")",                Hlt2[1]    ));
  Plots.push_back(TrigEffPlot("Topo2Body",nbins,"&& ("+L0[0]+"||"+L0[1]+")&&("+Hlt1+")",   Hlt2[2]                 ));
  Plots.push_back(TrigEffPlot("IncPhiDec",nbins,"&& ("+L0[0]+"||"+L0[1]+")&&("+Hlt1+")",                Hlt2[3]    ));
  Plots.push_back(TrigEffPlot("Hlt2Total",nbins,"&& ("+L0[0]+"||"+L0[1]+")&&("+Hlt1+")",   Hlt2[0]+"||"+Hlt2[1]    ));
  Plots.push_back(TrigEffPlot("TrigTotal",nbins,"","("+L0[0]+"||"+L0[1]+")&&("+Hlt1+")&&("+Hlt2[0]+"||"+Hlt2[1]+")"));
  for(auto Plot : Plots)
  {
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
    graph.SetMinimum(0);
    DrawGraph(plotname+Plot.name,graph)->Write();
  }
  outputfile.Close();
}
int main(int argc, char* argv[])
{
  if(argc!=3)
  {
    std::cout << "Usage: " << argv[0] << " <filename> <plot name prefix>" << std::endl;
    return 1;
  }
  PlotTrigEff(argv[1],argv[2]);
  return 0;
}

