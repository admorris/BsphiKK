#ifndef ROO_THREEBODYPHASESPACE
#define ROO_THREEBODYPHASESPACE

#include "RooAbsPdf.h"
#include "RooRealProxy.h"

class RooRealVar;

class RooThreeBodyPhaseSpace : public RooAbsPdf
{
  public:
    RooThreeBodyPhaseSpace() {} ;
    RooThreeBodyPhaseSpace(const char* name, const char* title, RooAbsReal& _m12, RooAbsReal& _M, RooAbsReal& _m1, RooAbsReal& _m2, RooAbsReal& _m3);
    RooThreeBodyPhaseSpace(const RooThreeBodyPhaseSpace& other, const char* name=0);
    virtual TObject* clone(const char* newname) const { return new RooThreeBodyPhaseSpace(*this,newname); }
    inline virtual ~RooThreeBodyPhaseSpace() {}
  protected:
    RooRealProxy m12;
    RooRealProxy M;
    RooRealProxy m1;
    RooRealProxy m2;
    RooRealProxy m3;
    Double_t evaluate() const ;
  private:
    Double_t p(Double_t,Double_t,Double_t) const ;
    ClassDef(RooThreeBodyPhaseSpace,0) // Three-body phase space function
};

#endif

