// Standard headers
#include <stdexcept>
// ROOT headers
#include "TCanvas.h"
// RooFit headers
#include "RooAddPdf.h"
#include "RooBreitWigner.h"
#include "RooCBShape.h"
#include "RooExponential.h"
#include "RooFFTConvPdf.h"
#include "RooFormulaVar.h"
#include "RooGaussian.h"
#include "RooPolynomial.h"
#include "RooRealVar.h"
#include "RooUniform.h"
#include "RooVoigtian.h"
// Custom headers
#include "MassFitter.h"
using namespace std;
using namespace RooFit;
/******************************************************************************/
MassFitter::MassFitter(RooRealVar* mass) :
  _mass(mass)
{
  init();
}
MassFitter::MassFitter(const MassFitter& input) :
  _mass(input._mass)
{

}
MassFitter::~MassFitter()
{

}
/******************************************************************************/
void MassFitter::init()
{
  _haspdf  = false;
  _hasdata = false;
  cout << "MassFitter instance initialised" << endl;
}
/******************************************************************************/
void MassFitter::SetPDF(RooAbsPdf* pdf)
{
  _haspdf = true;
  _pdf    = pdf;
}
void MassFitter::SetPDF(string signame, string bkgname)
{
  RooAbsPdf* sigpdf, * bkgpdf;
  if(_haspdf)
  {
    ResetPDF();
  }
  _haspdf = false;
  cout << "Will attempt to construct PDF with name " << signame << endl;
  if(signame=="Single Gaussian")
  {
    sigpdf = singleGaussian();
  }
  else if(signame=="Double Gaussian")
  {
    sigpdf = doubleGaussian();
  }
  else if(signame=="Triple Gaussian")
  {
    sigpdf = tripleGaussian();
  }
  else if(signame=="Crystal Ball")
  {
    sigpdf = CrystalBall();
  }
  else if(signame=="Crystal Ball + 1 Gaussian")
  {
    sigpdf = CrystalBall1Gauss();
  }
  else if(signame=="Crystal Ball + 2 Gaussians")
  {
    sigpdf = CrystalBall2Gauss();
  }
  else if(signame=="Breit-Wigner" || signame=="Breit Wigner")
  {
    sigpdf = BreitWigner();
  }
  else if(signame=="Breit-Wigner * Gaussian" || signame=="Breit Wigner * Gaussian")
  {
    sigpdf = convolve(BreitWigner(),singleGaussian());
  }
  else if(signame=="Voigtian" || signame=="Voigt")
  {
    sigpdf = Voigtian();
  }
  else
  {
    throw invalid_argument(("No such PDF " + signame).c_str());
  }
  if(bkgname=="none" || bkgname=="None")
  {
    _pdf = sigpdf;
    _haspdf = true;
    return;
  }
  cout << "Will attempt to construct PDF with name " << bkgname << endl;
  if(bkgname=="Flat")
  {
    bkgpdf = flatfunction();
  }
  else if(bkgname=="Exponential")
  {
    bkgpdf = exponential();
  }
  else
  {
    throw invalid_argument(("No such PDF " + bkgname).c_str());
  }
  _pdf = combine(sigpdf,bkgpdf);
  _haspdf = true;
}
/******************************************************************************/
void MassFitter::ResetPDF()
{
  if(!_haspdf)
  {
    return;
  }
  for(unsigned int i = 0; i < _stuff.size(); i++)
  {
    delete _stuff[i];
  }
  _stuff.clear();
}
/******************************************************************************/
void MassFitter::SetData(RooDataSet* data)
{
  _hasdata = true;
  _data    = data;
}
/******************************************************************************/
RooFitResult* MassFitter::Fit()
{
  if(_hasdata && _haspdf)
  {
    return _pdf->fitTo(*_data);
  }
  else
  {
    throw runtime_error("Attempting to fit before setting PDF and/or DataSet.");
  }
}
RooFitResult* MassFitter::Fit(RooDataSet* data)
{
  if(_haspdf)
  {
    cout << "Saving passed RooDataSet in object." << endl;
    SetData(data);
    return Fit();
  }
  else
  {
    throw runtime_error("Attempting to fit before setting PDF.");
  }
}
/******************************************************************************/
RooAbsReal* MassFitter::GetThing(string name)
{
  for(unsigned int i = 0; i < _stuff.size(); i++)
  {
    if((string)_stuff[i]->GetName() == name)
    {
      return _stuff[i];
    }
  }
  throw invalid_argument(("No such parameter: "+name).c_str());
}
/******************************************************************************/
double MassFitter::GetValue(string name)
{
  return (double)((RooRealVar*)GetThing(name))->getValV();
}
/******************************************************************************/
void MassFitter::SetValue(string name, double value)
{
  RooRealVar* thing = (RooRealVar*)GetThing(name);
  thing->setVal(value);
  cout << "Setting " << name << " to " << value << endl;
}
/******************************************************************************/
void MassFitter::SetRange(string name, double valmin, double valmax)
{
  RooRealVar* thing = (RooRealVar*)GetThing(name);
  thing->setRange(valmin, valmax);
  cout << "Setting " << name << " range to [" << valmin << "," << valmax << "]" << endl;
}
/******************************************************************************/
void MassFitter::FixValue(string name, double value)
{
  RooRealVar* thing = (RooRealVar*)GetThing(name);
  thing->setVal(value);
  thing->setConstant();
  cout << "Fixing " << name << " to " << value << endl;
}
/******************************************************************************/
void MassFitter::Plot(RooPlot* frame)
{
  cout << "PDF name is " << _pdf->GetName() << endl;
  if(strcmp(_pdf->GetName(),"model")==0)
  {
    cout << "Plotting components sigmod and bkgmod." << endl;
    _pdf->plotOn(frame,Components("sigmod"),LineStyle(9),LineColor(kBlue));
    _pdf->plotOn(frame,Components("bkgmod"),LineStyle(kDotted),LineColor(kViolet));
  }
  cout << "Plotting PDF." << endl;
  _pdf->plotOn(frame,LineStyle(kSolid),LineColor(kRed));
}
/******************************************************************************/
SPlot* MassFitter::GetsPlot()
{
  return new SPlot("sData","An SPlot", *_data, _pdf, RooArgList(*GetThing("Nsig"),*GetThing("Nbkg")));
}
/******************************************************************************/
RooAbsPdf* MassFitter::combine(RooAbsPdf* sigmod, RooAbsPdf* bkgmod)
{
  RooRealVar* Nsig  = new RooRealVar("Nsig","Number of signal events",3.50443e+04,0,120000);
  RooRealVar* Nbkg  = new RooRealVar("Nbkg","Number of background events",2.52394e+02,0,20000);
  RooAddPdf*  model = new RooAddPdf("model","model",RooArgList(*bkgmod,*sigmod),RooArgList(*Nbkg,*Nsig));
  _stuff.push_back(Nsig);
  _stuff.push_back(Nbkg);
  _stuff.push_back(model);
  return (RooAbsPdf*)model;
}
/******************************************************************************/
RooAbsPdf* MassFitter::convolve(RooAbsPdf* first, RooAbsPdf* second)
{
  _mass->setBins(10000,"cache");
  RooFFTConvPdf* model = new RooFFTConvPdf("convshape","Convolved PDF",*_mass,*first,*second);
  _stuff.push_back(model);
  return (RooAbsPdf*)model;
}
/******************************************************************************/
RooAbsPdf* MassFitter::singleGaussian()
{
  RooRealVar*    mean      = new RooRealVar("mean","Mean \\phi \\phi mass",5.36815e+03,5360,5380);
  RooRealVar*    sigma1    = new RooRealVar("sigma1","Width of first gaussian",1.29312e+01,10,24);
  RooGaussian*   sigmod    = new RooGaussian("sigmod","First gaussian",*_mass,*mean,*sigma1);
  _stuff.push_back(mean);
  _stuff.push_back(sigma1);
  _stuff.push_back(sigmod);
  return (RooAbsPdf*)sigmod;
}
/******************************************************************************/
RooAbsPdf* MassFitter::doubleGaussian()
{
  RooRealVar*    mean      = new RooRealVar("mean","Mean \\phi \\phi mass",5.36815e+03,5360,5380);
  RooRealVar*    sigma1    = new RooRealVar("sigma1","Width of first gaussian",1.29312e+01,10,18);
  RooRealVar*    sigma2    = new RooRealVar("sigma2","Width of second gaussian",3.27034e+01,18,50);
  RooRealVar*    fgaus1    = new RooRealVar("fgaus1","Fraction of first gaussian",0.8,0.5,1.0);
  RooFormulaVar* ominf1    = new RooFormulaVar("ominf1","(1-@0)*(@0<1)",RooArgList(*fgaus1));
  RooGaussian*   gauss1    = new RooGaussian("gauss1","First gaussian",*_mass,*mean,*sigma1);
  RooGaussian*   gauss2    = new RooGaussian("gauss2","Second gaussian",*_mass,*mean,*sigma2);
  RooAddPdf*     sigmod    = new RooAddPdf("sigmod","sigmod",RooArgList(*gauss1,*gauss2),RooArgList(*fgaus1,*ominf1));
  _stuff.push_back(mean);
  _stuff.push_back(sigma1);
  _stuff.push_back(sigma2);
  _stuff.push_back(fgaus1);
  _stuff.push_back(ominf1);
  _stuff.push_back(gauss1);
  _stuff.push_back(gauss2);
  _stuff.push_back(sigmod);
  return (RooAbsPdf*)sigmod;
}
/******************************************************************************/
RooAbsPdf* MassFitter::tripleGaussian()
{
  RooRealVar*    mean      = new RooRealVar("mean","Mean \\phi \\phi mass",5.36815e+03,5360,5380);
  RooRealVar*    sigma1    = new RooRealVar("sigma1","Width of first gaussian",1.29312e+01,10,18);
  RooRealVar*    sigma2    = new RooRealVar("sigma2","Width of second gaussian",3.27034e+01,18,50);
  RooRealVar*    sigma3    = new RooRealVar("sigma3","Width of third gaussian",50,40,120);
  RooRealVar*    fgaus1    = new RooRealVar("fgaus1","Total fraction of first gaussian",0.5,0.3,1.0);
  RooFormulaVar* ominf1    = new RooFormulaVar("ominf1","(1-@0)*(@0<1)",*fgaus1);
  RooRealVar*    fgaus2    = new RooRealVar("fgaus2","Fraction of second gaussian relative to third",0.5,0.3,1.0);
  RooFormulaVar* ominf2    = new RooFormulaVar("ominf2","(1-@0)*(@0<1)",*fgaus2);
  RooGaussian*   gauss1    = new RooGaussian("gauss1","First gaussian",*_mass,*mean,*sigma1);
  RooGaussian*   gauss2    = new RooGaussian("gauss2","Second gaussian",*_mass,*mean,*sigma2);
  RooGaussian*   gauss3    = new RooGaussian("gauss3","Third gaussian",*_mass,*mean,*sigma3);
  RooAddPdf*     othbit    = new RooAddPdf("othbit","othbit",RooArgList(*gauss2,*gauss3),RooArgList(*fgaus2,*ominf2));
  RooAddPdf*     sigmod    = new RooAddPdf("sigmod","sigmod",RooArgList(*gauss1,*othbit),RooArgList(*fgaus1,*ominf1));
  _stuff.push_back(mean);
  _stuff.push_back(sigma1);
  _stuff.push_back(sigma2);
  _stuff.push_back(sigma3);
  _stuff.push_back(fgaus1);
  _stuff.push_back(fgaus2);
  _stuff.push_back(ominf1);
  _stuff.push_back(ominf2);
  _stuff.push_back(gauss1);
  _stuff.push_back(gauss2);
  _stuff.push_back(gauss3);
  _stuff.push_back(othbit);
  _stuff.push_back(sigmod);
  return (RooAbsPdf*)sigmod;
}
/******************************************************************************/
RooAbsPdf* MassFitter::CrystalBall()
{
  RooRealVar*    mean      = new RooRealVar("mean","Mean \\phi \\phi mass",5.36815e+03,5360,5380);
  RooRealVar*    alpha     = new RooRealVar("alpha","alpha",3,0,10);
	RooRealVar*    n         = new RooRealVar("n","n",1.0);
  RooRealVar*    sigma1    = new RooRealVar("sigma1","Width of first gaussian",1.29312e+01,10,24);
  RooCBShape*    sigmod    = new RooCBShape("sigmod","Crystal Ball",*_mass,*mean,*sigma1,*alpha,*n);
  _stuff.push_back(mean);
  _stuff.push_back(alpha);
  _stuff.push_back(n);
  _stuff.push_back(sigma1);
  _stuff.push_back(sigmod);
  return (RooAbsPdf*)sigmod;
}
/******************************************************************************/
RooAbsPdf* MassFitter::CrystalBall1Gauss()
{
  RooRealVar*    mean      = new RooRealVar("mean","Mean \\phi \\phi mass",5.36815e+03,5360,5380);
  RooRealVar*    alpha     = new RooRealVar("alpha","alpha",3,0,10);
	RooRealVar*    n         = new RooRealVar("n","n",1.0);
  RooRealVar*    sigma1    = new RooRealVar("sigma1","Width of first gaussian",1.29312e+01,10,18);
  RooRealVar*    sigma2    = new RooRealVar("sigma2","Width of second gaussian",3.27034e+01,18,50);
  RooRealVar*    fgaus1    = new RooRealVar("fgaus1","Fraction of first gaussian",0.8,0.5,1.0);
  RooFormulaVar* ominf1    = new RooFormulaVar("ominf1","(1-@0)*(@0<1)",RooArgList(*fgaus1));
  RooCBShape*    CBshape   = new RooCBShape("CBshape","Crystal Ball",*_mass,*mean,*sigma1,*alpha,*n);
  RooGaussian*   gauss2    = new RooGaussian("gauss2","First gaussian",*_mass,*mean,*sigma2);
  RooAddPdf*     sigmod    = new RooAddPdf("sigmod","sigmod",RooArgList(*CBshape,*gauss2),RooArgList(*fgaus1,*ominf1));
  _stuff.push_back(mean);
  _stuff.push_back(alpha);
  _stuff.push_back(n);
  _stuff.push_back(sigma1);
  _stuff.push_back(sigma2);
  _stuff.push_back(fgaus1);
  _stuff.push_back(ominf1);
  _stuff.push_back(CBshape);
  _stuff.push_back(gauss2);
  _stuff.push_back(sigmod);
  return (RooAbsPdf*)sigmod;
}
/******************************************************************************/
RooAbsPdf* MassFitter::CrystalBall2Gauss()
{
  RooRealVar*    mean      = new RooRealVar("mean","Mean \\phi \\phi mass",5.36815e+03,5360,5380);
  RooRealVar*    alpha     = new RooRealVar("alpha","alpha",3,0,10);
	RooRealVar*    n         = new RooRealVar("n","n",1.0);
  RooRealVar*    sigma1    = new RooRealVar("sigma1","Width of first gaussian",1.29312e+01,10,18);
  RooRealVar*    sigma2    = new RooRealVar("sigma2","Width of second gaussian",3.27034e+01,18,50);
  RooRealVar*    sigma3    = new RooRealVar("sigma3","Width of third gaussian",50,40,120);
  RooRealVar*    fgaus1    = new RooRealVar("fgaus1","Total fraction of first gaussian",0.5,0.3,1.0);
  RooFormulaVar* ominf1    = new RooFormulaVar("ominf1","(1-@0)*(@0<1)",*fgaus1);
  RooRealVar*    fgaus2    = new RooRealVar("fgaus2","Fraction of second gaussian relative to third",0.5,0.3,1.0);
  RooFormulaVar* ominf2    = new RooFormulaVar("ominf2","(1-@0)*(@0<1)",*fgaus2);
  RooCBShape*    CBshape   = new RooCBShape("CBshape","Crystal Ball",*_mass,*mean,*sigma1,*alpha,*n);
  RooGaussian*   gauss2    = new RooGaussian("gauss2","First gaussian ",*_mass,*mean,*sigma2);
  RooGaussian*   gauss3    = new RooGaussian("gauss3","Second gaussian",*_mass,*mean,*sigma3);
  RooAddPdf*     othbit    = new RooAddPdf("othbit","othbit",RooArgList(*gauss2,*gauss3),RooArgList(*fgaus2,*ominf2));
  RooAddPdf*     sigmod    = new RooAddPdf("sigmod","sigmod",RooArgList(*CBshape,*othbit),RooArgList(*fgaus1,*ominf1));
  _stuff.push_back(mean);
  _stuff.push_back(alpha);
  _stuff.push_back(n);
  _stuff.push_back(sigma1);
  _stuff.push_back(sigma2);
  _stuff.push_back(sigma3);
  _stuff.push_back(fgaus1);
  _stuff.push_back(fgaus2);
  _stuff.push_back(ominf1);
  _stuff.push_back(ominf2);
  _stuff.push_back(CBshape);
  _stuff.push_back(gauss2);
  _stuff.push_back(gauss3);
  _stuff.push_back(othbit);
  _stuff.push_back(sigmod);
  return (RooAbsPdf*)sigmod;
}
/******************************************************************************/
RooAbsPdf* MassFitter::BreitWigner()
{
  RooRealVar*     mean   = new RooRealVar("mass","Mean \\phi mass",1019.461,1018,1021);
  RooRealVar*     width  = new RooRealVar("width","Natural \\phi width",4.266,3.5,5);
  RooBreitWigner* sigmod = new RooBreitWigner("resonance","Breit-Wigner shape",*_mass,*mean,*width);
  _stuff.push_back(mean);
  _stuff.push_back(width);
  _stuff.push_back(sigmod);
  return (RooAbsPdf*)sigmod;
}
/******************************************************************************/
RooAbsPdf* MassFitter::Voigtian()
{
  RooRealVar*  mean   = new RooRealVar("mean","Mean \\phi mass", 1019.461,1018,1021);
  RooRealVar*  width  = new RooRealVar("width","Natural \\phi width",4.266,3.5,5);
  RooRealVar*  sigma1 = new RooRealVar("sigma1","Detector resolution",1,0,10);
  RooVoigtian* sigmod = new RooVoigtian("sigmod","sigmod",*_mass,*mean,*width,*sigma1);
  _stuff.push_back(mean);
  _stuff.push_back(width);
  _stuff.push_back(sigma1);
  _stuff.push_back(sigmod);
  return (RooAbsPdf*)sigmod;
}
/******************************************************************************/
RooAbsPdf* MassFitter::flatfunction()
{
  RooUniform* bkgmod = new RooUniform("bkgmod","Flat background",*_mass);
  _stuff.push_back(bkgmod);
  return (RooAbsPdf*)bkgmod;
}
/******************************************************************************/
RooAbsPdf* MassFitter::exponential()
{
  RooRealVar*     slope  = new RooRealVar("slope","Exponent",0,-1.0,1.0);
  RooExponential* bkgmod = new RooExponential("bkgmod","Flat background",*_mass,*slope);
  _stuff.push_back(slope);
  _stuff.push_back(bkgmod);
  return (RooAbsPdf*)bkgmod;
}
RooAbsPdf* MassFitter::straightline()
{
  RooRealVar*    slope  = new RooRealVar("slope","Gradient",0,-1.0,1.0);
  RooPolynomial* bkgmod = new RooPolynomial("bkgmod","Straight line background",*_mass,*slope);
  _stuff.push_back(slope);
  _stuff.push_back(bkgmod);
  return (RooAbsPdf*)bkgmod;
}
