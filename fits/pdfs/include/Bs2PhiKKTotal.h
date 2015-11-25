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
// Self
#include "Bs2PhiKKComponent.h"

class Bs2PhiKKTotal : public BasePDF
{
    public:
      // *structors
      Bs2PhiKKTotal(PDFConfigurator*);
      ~Bs2PhiKKTotal();
      // Required methods
      virtual double Evaluate(DataPoint*);
      virtual double Normalisation(DataPoint*, PhaseSpaceBoundary*);
      virtual bool SetPhysicsParameters(ParameterSet*);
      virtual vector<string> GetDoNotIntegrateList();
    private:
      bool init;
      Bs2PhiKKTotal& operator=( const Bs2PhiKKTotal& );
      void MakePrototypes();
      // K+K− mass and helicity angles
      double        mKK,     ctheta_1,     ctheta_2,     phi;
      ObservableRef mKKName, ctheta_1Name, ctheta_2Name, phiName;
      // Magnitude-squared of helicity amplitudes
      double        ASsq,     APsq[3],     ADsq[3];
      ObservableRef ASsqName, APsqName[3], ADsqName[3];
      // Phase of helicity amplitudes
      double        deltaS,     deltaP[3],     deltaD[3];
      ObservableRef deltaSName, deltaPName[3], deltaDName[3];
      // The m(KK) components
      Bs2PhiKKComponent* Swave;
      Bs2PhiKKComponent* Pwave;
      Bs2PhiKKComponent* Dwave;
};
#endif
