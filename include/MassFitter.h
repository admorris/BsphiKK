#ifndef __MASSFITTER_H__
#define __MASSFITTER_H__
// Standard C++ libraries
#include <vector>
// RooFit libraries
#include "RooAbsPdf.h"
#include "RooDataSet.h"
#include "RooFitResult.h"

class MassFitter
{
  public:
    // Constructor
    MassFitter();
    // Copy constructor
    MassFitter(const MassFitter&);
    // Destructor
    ~MassFitter();
    // Get and set private variables
    RooAbsPdf*         GetPDF() { return pdf;  }
    RooDataSet*        GetData(){ return data; }
    void               SetPDF(RooAbsPdf*);
    void               SetData(RooDataSet*);
    // 
    RooFitResult*      Fit();
    RooFitResult*      Fit(RooDataSet*);
  private:
    RooAbsPdf*         pdf;
    RooDataSet*        data;
    void               init();
    // Settings
    vector<RooAbsPdf*> builtins;
    // Flags
    bool               haspdf;
    bool               hasdata;
};
#endif
