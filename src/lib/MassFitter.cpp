// Standard headers
#include <stdexcept>
// RooFit headers
#include "RooAddPdf.h"
#include "RooFormulaVar.h"
#include "RooGaussian.h"
#include "RooRealVar.h"
#include "RooUniform.h"
// Custom headers
#include "MassFitter.h"
using namespace std;
/******************************************************************************/
MassFitter::MassFitter(RooRealVar mass) :
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
}
/******************************************************************************/
void MassFitter::SetPDF(RooAbsPdf* pdf)
{
  _haspdf = true;
  _pdf    = pdf;
}
void MassFitter::SetPDF(string name)
{
  if(_haspdf)
  {
    ResetPDF();
  }
  _haspdf = false;
  cout << "Will attempt to construct PDF with name " << name << endl;
  if(name=="Single Gaussian")
  {
    _pdf = singleGaussian();
  }
  else if(name=="Double Gaussian")
  {
    _pdf = doubleGaussian();
  }
  else if(name=="Triple Gaussian")
  {
    _pdf = tripleGaussian();
  }
  else
  {
    throw invalid_argument(("No such PDF " + name).c_str());
  }
  cout << name << " constructed." << endl;
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
    return _pdf->fitTo(*data);
  }
  else
  {
    throw runtime_error("Attempting to fit before setting PDF.");
  }
}
/******************************************************************************/
RooAbsPdf* MassFitter::singleGaussian()
{
  RooRealVar*    mean      = new RooRealVar("mean","Mean \\phi \\phi mass",5.36815e+03,5360,5380);
  RooRealVar*    sigma1    = new RooRealVar("sigma1","Width of first gaussian",1.29312e+01,10,18);
  RooGaussian*   sigmod    = new RooGaussian("sigmod","First gaussian",_mass,*mean,*sigma1);
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
  RooRealVar*    fgaus1    = new RooRealVar("fgaus1","Fraction of first gaussian",0.8,0.7,0.9);
  RooFormulaVar* fgaus2    = new RooFormulaVar("fgaus2","(1-@0)*(@0<1)",RooArgList(*fgaus1));
  RooGaussian*   gauss1    = new RooGaussian("gauss1","First gaussian",_mass,*mean,*sigma1);
  RooGaussian*   gauss2    = new RooGaussian("gauss2","Second gaussian",_mass,*mean,*sigma2);
  RooAddPdf*     sigmod    = new RooAddPdf("sigmod","sigmod",RooArgList(*gauss1,*gauss2),RooArgList(*fgaus1,*fgaus2));
  _stuff.push_back(mean);
  _stuff.push_back(sigma1);
  _stuff.push_back(sigma2);
  _stuff.push_back(fgaus1);
  _stuff.push_back(fgaus2);
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
  RooRealVar*    fgaus1    = new RooRealVar("fgaus1","Fraction of first gaussian",0.8,0.7,0.9);
  RooRealVar*    fgaus2    = new RooRealVar("fgaus2","Fraction of second gaussian",0.1,0.0,0.3);
  RooFormulaVar* fgaus3    = new RooFormulaVar("fgaus3","(1-@0-@1)*(@0+@1<1)",RooArgList(*fgaus1,*fgaus2));
  RooGaussian*   gauss1    = new RooGaussian("gauss1","First gaussian",_mass,*mean,*sigma1);
  RooGaussian*   gauss2    = new RooGaussian("gauss2","Second gaussian",_mass,*mean,*sigma2);
  RooGaussian*   gauss3    = new RooGaussian("gauss3","Third gaussian",_mass,*mean,*sigma3);
  RooAddPdf*     sigmod    = new RooAddPdf("sigmod","sigmod",RooArgList(*gauss1,*gauss2,*gauss3),RooArgList(*fgaus1,*fgaus2,*fgaus3));
  _stuff.push_back(mean);
  _stuff.push_back(sigma1);
  _stuff.push_back(sigma2);
  _stuff.push_back(sigma3);
  _stuff.push_back(fgaus1);
  _stuff.push_back(fgaus2);
  _stuff.push_back(fgaus3);
  _stuff.push_back(gauss1);
  _stuff.push_back(gauss2);
  _stuff.push_back(gauss3);
  _stuff.push_back(sigmod);
  return (RooAbsPdf*)sigmod;
}
