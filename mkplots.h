#include "TCanvas.h"
#include "TSystem.h"
#include "addBlurb.h"
#include <string>
#include <sstream>
using namespace std;
void mkplots(TCanvas* canv, string fname)
{
    canv->cd();
    if(fname.find("data") != string::npos || fname.find("Data") != string::npos || fname.find("DATA") != string::npos)
    {
      addBlurb(canv, 11+12, 0.65, true);
    }
    else
    {
      addBlurb(canv, 0, 0.65, true);
    }
    stringstream filename;
    filename << "./figs/" << fname;
    canv->Print((filename.str()+".png").c_str());
    canv->Print((filename.str()+".eps").c_str());
    canv->Print((filename.str()+".C").c_str());
    gSystem->Exec(("epstopdf "+filename.str()+".eps").c_str());
    gSystem->Exec(("rm "+filename.str()+".eps").c_str());
    gSystem->Exec(("convert "+filename.str()+".pdf "+filename.str()+".eps").c_str());
}

void mkpaperplots(TCanvas* canv, string fname)
{
    canv->cd();
    addPaperBlurb(canv, 0.65);
    stringstream filename;
    filename << "./paperfigs/" << fname;
    canv->Print((filename.str()+".png").c_str());
    canv->Print((filename.str()+".eps").c_str());
    canv->Print((filename.str()+".C").c_str());
    gSystem->Exec(("epstopdf "+filename.str()+".eps").c_str());
    gSystem->Exec(("rm "+filename.str()+".eps").c_str());
    gSystem->Exec(("convert "+filename.str()+".pdf "+filename.str()+".eps").c_str());
    TFile* file = new TFile((""+filename.str()+".root").c_str(),"RECREATE");
    canv->Write();
    file->Close();
}
