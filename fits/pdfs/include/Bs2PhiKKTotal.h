/** @class Bs2PhiKKTotal Bs2PhiKKTotal.h
 *
 *  RapidFit PDF for Bs2PhiKKTotal
 *
 *  @author Adam Morris
 *  @date Nov-Dec 2015
 */
#ifndef Bs2PhiKKTotal_H
#define Bs2PhiKKTotal_H

#ifndef __CINT__
#include "BasePDF.h"
#endif
#ifdef __CINT__
#include "framework/include/BasePDF.h"
#endif
#include "TMath.h"

class Bs2PhiKKTotal : public BasePDF
{
    public:
      Bs2PhiKKTotal(PDFConfigurator*);
      ~Bs2PhiKKComponent();
      virtual double Evaluate(DataPoint*);
      virtual double Normalisation(DataPoint*, PhaseSpaceBoundary*);
      virtual bool SetPhysicsParameters(ParameterSet*);
      virtual vector<string> GetDoNotIntegrateList();
    private:
      bool init;
      Bs2PhiKKTotal& operator=( const Bs2PhiKKTotal& );
      void MakePrototypes();
      double        mKK,     ctheta_1,     ctheta_2,     phi;
      ObservableRef mKKName, ctheta_1Name, ctheta_2Name, phiName;
      double        ASzero2,     APzero2,     APpara2,     ADzero2,     ADpara2;
      ObservableRef ASzero2Name, APzero2Name, APpara2Name, ADzero2Name, ADpara2Name;
      double APperp2() { return 1.0 - APzero2 - APpara2 ; }
      double ADperp2() { return 1.0 - ADzero2 - ADpara2 ; }
      double        nSwave,     nPwave,     nDwave;
      ObservableRef nSwaveName, nPwaveName, nDwaveName;
      Bs2PhiKKComponent* Swave;
      Bs2PhiKKComponent* Pwave;
      Bs2PhiKKComponent* Dwave;
};
#endif
