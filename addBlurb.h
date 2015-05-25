#include "TCanvas.h"
#include "TLatex.h"
void addBlurb(TCanvas* can, int year = 0,float xpos = 0.55,bool compact = false){

   can->cd(1);
   TLatex *myLatex = new TLatex(0.5,0.5,"");
   myLatex->SetTextFont(132);
   myLatex->SetTextColor(1);
   myLatex->SetNDC(kTRUE);
   myLatex->SetTextAlign(11);
   myLatex->SetTextSize(0.05);
   if(!compact)
   {
     if(year == 11)
       myLatex->DrawLatex(xpos, 0.85,"#splitline{LHCb Preliminary}{#scale[1]{#sqrt{s} = 7 TeV, L = 1 fb^{-1}}}");
     else if(year == 12)
       myLatex->DrawLatex(xpos, 0.85,"#splitline{LHCb Preliminary}{#scale[1]{#sqrt{s} = 8 TeV, L = 2 fb^{-1}}}");
     else if(year == 11+12)
       myLatex->DrawLatex(xpos, 0.85,"#splitline{LHCb Preliminary}{#scale[1]{#sqrt{s} = 7 + 8 TeV, L = 3 fb^{-1}}}");
     else
       myLatex->DrawLatex(xpos, 0.85,"LHCb Simulation");
    }
    else
    {
     if(year == 11)
       myLatex->DrawLatex(xpos, 0.85,"#splitline{LHCb Preliminary}{#splitline{#sqrt{s} = 7 TeV}{L = 1 fb^{-1}}}");
     else if(year == 12)
       myLatex->DrawLatex(xpos, 0.85,"#splitline{LHCb Preliminary}{#splitline{#sqrt{s} = 8 TeV}{L = 2 fb^{-1}}}");
     else if(year == 11+12)
       myLatex->DrawLatex(xpos, 0.85,"#splitline{LHCb Preliminary}{#splitline{#sqrt{s} = 7 + 8 TeV}{L = 3 fb^{-1}}}");
     else
       myLatex->DrawLatex(xpos, 0.85,"LHCb Simulation");
    
    }
}

void addPaperBlurb(TCanvas* can, float xpos = 0.55){

   can->cd(1);
   TLatex *myLatex = new TLatex(0.5,0.5,"");
   myLatex->SetTextFont(132);
   myLatex->SetTextColor(1);
   myLatex->SetNDC(kTRUE);
   myLatex->SetTextAlign(11);
   myLatex->SetTextSize(0.05);
   myLatex->DrawLatex(xpos, 0.85,"LHCb");
    
    
}
