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
// RooStats headers
#include "RooStats/SPlot.h"
using namespace std;
using namespace RooFit;
using namespace RooStats;
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
    void                SetPDF(RooAbsPdf*);
    void                SetPDF(string,string);
    void                ResetPDF();
    RooAbsReal*         GetThing(string);
    RooDataSet*         GetData()   { return _data; }
    void                SetData(RooDataSet*);
    // Parameter values
    double              GetValue(string);
    void                SetValue(string, double);
    void                SetRange(string, double, double);
    void                FixValue(string, double);
    // Command functions
    RooFitResult*       Fit();
    RooFitResult*       Fit(RooDataSet*);
    void                Plot(RooPlot*);
    SPlot*              GetsPlot();
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
    RooAbsPdf*          CrystalBall();
    RooAbsPdf*          CrystalBall1Gauss();
    RooAbsPdf*          CrystalBall2Gauss();
    RooAbsPdf*          BreitWigner();
    RooAbsPdf*          Voigtian();
    // Background models
    RooAbsPdf*          flatfunction();
    RooAbsPdf*          exponential();
    RooAbsPdf*          straightline();
    // Combine function for total model
    RooAbsPdf*          combine(RooAbsPdf*,RooAbsPdf*);
    RooAbsPdf*          convolve(RooAbsPdf*,RooAbsPdf*);
    // Keep track of pointers
    vector<RooAbsReal*> _stuff;
};
#endif
