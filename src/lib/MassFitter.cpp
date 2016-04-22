// Standard headers
#include <stdexcept>
// ROOT headers
#include "TCanvas.h"
// RooFit headers
#include "RooAddPdf.h"
#include "RooBreitWigner.h"
#include "RooCBShape.h"
#include "RooDstD0BG.h"
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
Component::Component(string name, RooAbsPdf* pdf) :
  _name(name)
, _pdf(pdf)
, _style(kDashed)
, _colour(kBlack)
, _hasyieldvar(false)
{

}
/******************************************************************************/
Component::~Component()
{
  for(auto thing : _stuff)
    delete thing;
}
/******************************************************************************/
RooRealVar* Component::GetYieldVar()
{
  if(!_hasyieldvar)
  {
    _yield = new RooRealVar(("N_"+_name).c_str(),("Auto-generated yield for component "+_name).c_str(),1);
    _stuff.push_back(_yield);
  }
  return _yield;
}
/******************************************************************************/
void Component::SetYieldVar(RooRealVar* yield)
{
  _yield = yield;
  _stuff.push_back(_yield);
  _hasyieldvar = true;
}
/******************************************************************************/
RooAbsReal* Component::GetThing(string name)
{
  for(auto thing : _stuff)
    if((string)thing->GetName() == name)
      return thing;
  throw invalid_argument(("No such parameter: "+name).c_str());
}
/******************************************************************************/
void Component::SetValue(string name, double value)
{
  RooRealVar* thing = (RooRealVar*)GetThing(name);
  thing->setVal(value);
  cout << "Setting " << name << " to " << value << endl;
}
/******************************************************************************/
void Component::SetRange(string name, double valmin, double valmax)
{
  RooRealVar* thing = (RooRealVar*)GetThing(name);
  thing->setRange(valmin, valmax);
  cout << "Setting " << name << " range to [" << valmin << "," << valmax << "]" << endl;
}
/******************************************************************************/
void Component::FixValue(string name, double value)
{
  RooRealVar* thing = (RooRealVar*)GetThing(name);
  thing->setVal(value);
  thing->setConstant();
  cout << "Fixing " << name << " to " << value << endl;
}
/******************************************************************************/
/******************************************************************************/
MassFitter::MassFitter(RooRealVar* mass) :
  _mass(mass)
{
  init();
}
MassFitter::MassFitter(const MassFitter& input) :
  _mass(input._mass)
{
  init();
}
MassFitter::~MassFitter()
{
  if(_components.size()>1)
    delete _pdf;
  for(auto component : _components)
    delete component;
}
/******************************************************************************/
void MassFitter::init()
{
  _haspdf  = false;
  _hasdata = false;
  _useyieldvars = false;
  cout << "MassFitter instance initialised" << endl;
}
/******************************************************************************/
Component* MassFitter::AddComponent(string name, RooAbsPdf* thepdf, RooRealVar* yield)
{
  Component* newpdf = AddComponent(name,thepdf);
  newpdf->SetYieldVar(yield);
  _useyieldvars = true;
  return newpdf;
}
Component* MassFitter::AddComponent(string name, RooAbsPdf* thepdf)
{
  Component* newpdf = new Component(name, thepdf);
  _components.push_back(newpdf);
  return newpdf;
}
Component* MassFitter::AddComponent(string name, string type, RooRealVar* yield)
{
  Component* newpdf = AddComponent(name, type);
  newpdf->SetYieldVar(yield);
  _useyieldvars = true;
  return newpdf;
}
Component* MassFitter::AddComponent(string name, string type)
{
  Component* newpdf;
  cout << "Will attempt to construct PDF with name " << type << endl;
  if(type=="Single Gaussian")
  {
    newpdf = singleGaussian(name);
  }
  else if(type=="Double Gaussian")
  {
    newpdf = doubleGaussian(name);
  }
  else if(type=="Triple Gaussian")
  {
    newpdf = tripleGaussian(name);
  }
  else if(type=="Crystal Ball")
  {
    newpdf = CrystalBall(name);
  }
  else if(type=="Crystal Ball + 1 Gaussian")
  {
    newpdf = CrystalBall1Gauss(name);
  }
  else if(type=="Crystal Ball + 2 Gaussians")
  {
    newpdf = CrystalBall2Gauss(name);
  }
  else if(type=="Breit-Wigner" || type=="Breit Wigner")
  {
    newpdf = BreitWigner(name);
  }
//  else if(type=="Breit-Wigner * Gaussian" || type=="Breit Wigner * Gaussian")
//  {
//    newpdf = convolve(BreitWigner(),singleGaussian());
//  }
  else if(type=="Voigtian" || type=="Voigt")
  {
    newpdf = Voigtian(name);
  }
  else if(type=="RooDstD0BG" || type=="Threshold" || type=="ThresholdShape")
  {
    newpdf = ThresholdShape(name);
  }
  else if(type=="Flat")
  {
    newpdf = flatfunction(name);
  }
  else if(type=="Exponential")
  {
    newpdf = exponential(name);
  }
  else
  {
    throw invalid_argument(("No such PDF " + type).c_str());
  }
  _components.push_back(newpdf);
  return newpdf;
}
/******************************************************************************/
void MassFitter::assemble()
{
  RooArgList pdflist;
  RooArgList coeflist;
  switch(_components.size())
  {
    case 0:
      throw runtime_error("Trying to assemble a PDF with no components.");
      break;
    case 1:
      _pdf = _components[0]->GetPDF();
      break;
    default:
      for(auto pdf : _components)
      {
        pdflist.add(*(pdf->GetPDF()));
        if(_useyieldvars)
          coeflist.add(*(pdf->GetYieldVar()));
      }
      _pdf = _useyieldvars ? new RooAddPdf("model","Total PDF",pdflist,coeflist) : new RooAddPdf("model","Total PDF",pdflist) ;
      break;
  }
  _haspdf = true;
}
/******************************************************************************/
Component* MassFitter::GetComponent(string name)
{
  for(auto pdf : _components)
    if((string)pdf->GetName() == name)
      return pdf;
  throw invalid_argument(("No such component: "+name+".").c_str());
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
  if(!_haspdf)
    assemble();
  if(_hasdata)
    return _pdf->fitTo(*_data);
  else
    throw runtime_error("Attempting to fit without a DataSet.");
}
RooFitResult* MassFitter::Fit(RooDataSet* data)
{
  if(!_haspdf)
    assemble();
  cout << "Saving passed RooDataSet in object." << endl;
  SetData(data);
  return Fit();
}
/******************************************************************************/
void MassFitter::Plot(RooPlot* frame)
{
  if(_components.size()>1)
    for(auto comp : _components)
      _pdf->plotOn(frame,Components(comp->GetName().c_str()),LineStyle(comp->GetStyle()),LineColor(comp->GetColour()));
  _pdf->plotOn(frame,LineStyle(kSolid),LineColor(kRed));
}
/******************************************************************************/
SPlot* MassFitter::GetsPlot(RooRealVar* Nsig, RooRealVar* Nbkg)
{
  return new SPlot("sData","An SPlot", *_data, _pdf, RooArgList(*Nsig,*Nbkg));
}
/******************************************************************************/
Component* MassFitter::singleGaussian(string name)
{
  RooRealVar*    mean      = new RooRealVar("mean","Mean \\phi \\phi mass",5.36815e+03,5360,5380);
  RooRealVar*    sigma1    = new RooRealVar("sigma1","Width of first gaussian",1.29312e+01,10,24);
  RooGaussian*   thepdf    = new RooGaussian(name.c_str(),"Gaussian",*_mass,*mean,*sigma1);
  Component* pdf = new Component(name,thepdf);
  pdf->AddThing(mean);
  pdf->AddThing(sigma1);
  pdf->AddThing(thepdf);
  return pdf;
}
/******************************************************************************/
Component* MassFitter::doubleGaussian(string name)
{
  RooRealVar*    mean      = new RooRealVar("mean","Mean \\phi \\phi mass",5.36815e+03,5360,5380);
  RooRealVar*    sigma1    = new RooRealVar("sigma1","Width of first gaussian",1.29312e+01,10,18);
  RooRealVar*    sigma2    = new RooRealVar("sigma2","Width of second gaussian",3.27034e+01,18,50);
  RooRealVar*    fgaus1    = new RooRealVar("fgaus1","Fraction of first gaussian",0.8,0.5,1.0);
  RooFormulaVar* ominf1    = new RooFormulaVar("ominf1","(1-@0)*(@0<1)",RooArgList(*fgaus1));
  RooGaussian*   gauss1    = new RooGaussian("gauss1","First gaussian",*_mass,*mean,*sigma1);
  RooGaussian*   gauss2    = new RooGaussian("gauss2","Second gaussian",*_mass,*mean,*sigma2);
  RooAddPdf*     thepdf    = new RooAddPdf(name.c_str(),"Double Gaussian",RooArgList(*gauss1,*gauss2),RooArgList(*fgaus1,*ominf1));
  Component* pdf = new Component(name,thepdf);
  pdf->AddThing(mean);
  pdf->AddThing(sigma1);
  pdf->AddThing(sigma2);
  pdf->AddThing(fgaus1);
  pdf->AddThing(ominf1);
  pdf->AddThing(gauss1);
  pdf->AddThing(gauss2);
  pdf->AddThing(thepdf);
  return pdf;
}
/******************************************************************************/
Component* MassFitter::tripleGaussian(string name)
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
  RooAddPdf*     thepdf    = new RooAddPdf(name.c_str(),"Triple Gaussian",RooArgList(*gauss1,*othbit),RooArgList(*fgaus1,*ominf1));
  Component* pdf = new Component(name,thepdf);
  pdf->AddThing(mean);
  pdf->AddThing(sigma1);
  pdf->AddThing(sigma2);
  pdf->AddThing(sigma3);
  pdf->AddThing(fgaus1);
  pdf->AddThing(fgaus2);
  pdf->AddThing(ominf1);
  pdf->AddThing(ominf2);
  pdf->AddThing(gauss1);
  pdf->AddThing(gauss2);
  pdf->AddThing(gauss3);
  pdf->AddThing(othbit);
  pdf->AddThing(thepdf);
  return pdf;
}
/******************************************************************************/
Component* MassFitter::CrystalBall(string name)
{
  RooRealVar*    mean      = new RooRealVar("mean","Mean \\phi \\phi mass",5.36815e+03,5360,5380);
  RooRealVar*    alpha     = new RooRealVar("alpha","alpha",3,0,10);
	RooRealVar*    n         = new RooRealVar("n","n",1.0);
  RooRealVar*    sigma1    = new RooRealVar("sigma1","Width of first gaussian",1.29312e+01,10,24);
  RooCBShape*    thepdf    = new RooCBShape(name.c_str(),"Crystal Ball",*_mass,*mean,*sigma1,*alpha,*n);
  Component* pdf = new Component(name,thepdf);
  pdf->AddThing(mean);
  pdf->AddThing(alpha);
  pdf->AddThing(n);
  pdf->AddThing(sigma1);
  pdf->AddThing(thepdf);
  return pdf;
}
/******************************************************************************/
Component* MassFitter::CrystalBall1Gauss(string name)
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
  RooAddPdf*     thepdf    = new RooAddPdf(name.c_str(),"Crystal Ball + Gaussian",RooArgList(*CBshape,*gauss2),RooArgList(*fgaus1,*ominf1));
  Component* pdf = new Component(name,thepdf);
  pdf->AddThing(mean);
  pdf->AddThing(alpha);
  pdf->AddThing(n);
  pdf->AddThing(sigma1);
  pdf->AddThing(sigma2);
  pdf->AddThing(fgaus1);
  pdf->AddThing(ominf1);
  pdf->AddThing(CBshape);
  pdf->AddThing(gauss2);
  pdf->AddThing(thepdf);
  return pdf;
}
/******************************************************************************/
Component* MassFitter::CrystalBall2Gauss(string name)
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
  RooAddPdf*     thepdf    = new RooAddPdf(name.c_str(),"Crystal Ball + 2 Gaussians",RooArgList(*CBshape,*othbit),RooArgList(*fgaus1,*ominf1));
  Component* pdf = new Component(name,thepdf);
  pdf->AddThing(mean);
  pdf->AddThing(alpha);
  pdf->AddThing(n);
  pdf->AddThing(sigma1);
  pdf->AddThing(sigma2);
  pdf->AddThing(sigma3);
  pdf->AddThing(fgaus1);
  pdf->AddThing(fgaus2);
  pdf->AddThing(ominf1);
  pdf->AddThing(ominf2);
  pdf->AddThing(CBshape);
  pdf->AddThing(gauss2);
  pdf->AddThing(gauss3);
  pdf->AddThing(othbit);
  pdf->AddThing(thepdf);
  return pdf;
}
/******************************************************************************/
Component* MassFitter::BreitWigner(string name)
{
  RooRealVar*     mean   = new RooRealVar("mass","Mean \\phi mass",1019.461,1018,1021);
  RooRealVar*     width  = new RooRealVar("width","Natural \\phi width",4.266,3.5,5);
  RooBreitWigner* thepdf = new RooBreitWigner(name.c_str(),"Breit Wigner",*_mass,*mean,*width);
  Component* pdf = new Component(name,thepdf);
  pdf->AddThing(mean);
  pdf->AddThing(width);
  pdf->AddThing(thepdf);
  return pdf;
}
/******************************************************************************/
Component* MassFitter::Voigtian(string name)
{
  RooRealVar*  mean   = new RooRealVar("mean","Mean \\phi mass", 1019.461,1018,1021);
  RooRealVar*  width  = new RooRealVar("width","Natural \\phi width",4.266,3.5,5);
  RooRealVar*  sigma1 = new RooRealVar("sigma1","Detector resolution",1,0,10);
  RooVoigtian* thepdf = new RooVoigtian(name.c_str(),"Voigtian",*_mass,*mean,*width,*sigma1);
  Component* pdf = new Component(name,thepdf);
  pdf->AddThing(mean);
  pdf->AddThing(width);
  pdf->AddThing(sigma1);
  pdf->AddThing(thepdf);
  return pdf;
}
/******************************************************************************/
Component* MassFitter::ThresholdShape(string name)
{
  RooRealVar* dm0    = new RooRealVar("dm0","dm0",2*493,0,4000);
  RooRealVar* a      = new RooRealVar("a","A",2,-100,100);
  RooRealVar* b      = new RooRealVar("b","B",-3.5,-100,100);
  RooRealVar* c      = new RooRealVar("c","C",10,-100,100);
  RooDstD0BG* thepdf = new RooDstD0BG(name.c_str(),"Threshold shape",*_mass,*dm0,*c,*a,*b);
  Component* pdf = new Component(name,thepdf);
  pdf->AddThing(dm0);
  pdf->AddThing(a);
  pdf->AddThing(b);
  pdf->AddThing(c);
  pdf->AddThing(thepdf);
  return pdf;
}
/******************************************************************************/
Component* MassFitter::flatfunction(string name)
{
  RooUniform* thepdf = new RooUniform(name.c_str(),"Flat background",*_mass);
  Component* pdf = new Component(name,thepdf);
  pdf->AddThing(thepdf);
  return pdf;
}
/******************************************************************************/
Component* MassFitter::exponential(string name)
{
  RooRealVar*     slope  = new RooRealVar("slope","Exponent",0,-1.0,1.0);
  RooExponential* thepdf = new RooExponential(name.c_str(),"Exponential background",*_mass,*slope);
  Component* pdf = new Component(name,thepdf);
  pdf->AddThing(slope);
  pdf->AddThing(thepdf);
  return pdf;
}
/******************************************************************************/
Component* MassFitter::straightline(string name)
{
  RooRealVar*    slope  = new RooRealVar("slope","Gradient",0,-1.0,1.0);
  RooPolynomial* thepdf = new RooPolynomial(name.c_str(),"Straight line background",*_mass,*slope);
  Component* pdf = new Component(name,thepdf);
  pdf->AddThing(slope);
  pdf->AddThing(thepdf);
  return pdf;
}
