#ifndef __MASSFITTER_H__
#define __MASSFITTER_H__
// Standard C++ headers
#include <vector>
#include <string>
// RooFit headers
#include "RooAbsPdf.h"
#include "RooAbsReal.h"
#include "RooDataSet.h"
#include "RooFitResult.h"
#include "RooPlot.h"
#include "RooRealVar.h"
using namespace std;
class MassFitter
{
  public:
    // Constructor
    MassFitter(RooRealVar*);
    // Copy constructor
    MassFitter(const MassFitter&);
    // Destructor
    ~MassFitter();
    // Get and set private variables
    RooAbsPdf*          GetPDF()    { return _pdf;  }
    RooDataSet*         GetData()   { return _data; }
    void                SetPDF(RooAbsPdf*);
    void                SetPDF(string,string);
    void                ResetPDF();
    void                SetData(RooDataSet*);
    //
    RooFitResult*       Fit();
    RooFitResult*       Fit(RooDataSet*);
    void                Plot(RooPlot*);
  private:
    RooAbsPdf*          _pdf;
    RooDataSet*         _data;
    RooRealVar*         _mass;
    void                init();
    // Settings
    vector<string>      _builtins;
    // Flags
    bool                _haspdf;
    bool                _hasdata;
    // Signal models
    RooAbsPdf*          singleGaussian();
    RooAbsPdf*          doubleGaussian();
    RooAbsPdf*          tripleGaussian();
    // Background models
    RooAbsPdf*          flatfunction();
    // Combine function for total model
    RooAbsPdf*          combine(RooAbsPdf*,RooAbsPdf*);
    // Keep track of pointers
    vector<RooAbsReal*> _stuff;
};
#endif
