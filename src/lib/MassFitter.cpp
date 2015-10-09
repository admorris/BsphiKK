#include <stdexcept>
#include "MassFitter.h"
using namespace std;
/******************************************************************************/
MassFitter::MassFitter()
{
  init();
}
MassFitter::MassFitter(const MassFitter& input)
{

}
MassFitter::~MassFitter()
{

}
/******************************************************************************/
void MassFitter::init()
{
  haspdf  = false;
  hasdata = false;
}
/******************************************************************************/
void MassFitter::SetPDF(RooAbsPdf* newpdf)
{
  haspdf = true;
  pdf    = newpdf;
}
void MassFitter::SetData(RooDataSet* newdata)
{
  hasdata = true;
  data    = newdata;
}
/******************************************************************************/
RooFitResult* MassFitter::Fit()
{
  if(hasdata && haspdf)
  {
    return pdf->fitTo(*data);
  }
  else
  {
    throw runtime_error("Attempting to fit before setting PDF and/or DataSet.");
  }
}
RooFitResult* MassFitter::Fit(RooDataSet* newdata)
{
  if(haspdf)
  {
    return pdf->fitTo(*newdata);
  }
  else
  {
    throw runtime_error("Attempting to fit before setting PDF.");
  }
}
/******************************************************************************/
