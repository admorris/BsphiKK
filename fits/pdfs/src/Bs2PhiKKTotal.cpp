/** @class Bs2PhiKKTotal Bs2PhiKKTotal.cpp
 *
 *  RapidFit PDF for Bs2PhiKKTotal
 *
 *  @author Adam Morris
 *  @date Nov-Dec 2015
 */

#include "Bs2PhiKKTotal.h"
#include "Bs2PhiKKComponent.h"
#include "TComplex.h"
#include <iostream>
#include "math.h"
#include "TMath.h"
#include "PDFConfigurator.h"
#define DEBUGFLAG true

PDF_CREATOR( Bs2PhiKKTotal )

//Constructor
Bs2PhiKKTotal::Bs2PhiKKTotal(PDFConfigurator* config) :
  // Physics parameters
    ASzero2(0.0)
  , APzero2(0.0)
  , APpara2(0.0)
  , ADzero2(0.0)
  , ADpara2(0.0)
  , nSwave(0.0)
  , nPwave(0.0)
  , nDwave(0.0)
  // Observables
  , mKK(0.0)
  , phi(0.0)
  , ctheta_1(0.0)
  , ctheta_2(0.0)
  // Physics parameters
  , ASzero2Name   ( config->getName("ASzero2" ) )
  , APzero2Name   ( config->getName("APzero2" ) )
  , APpara2Name   ( config->getName("APpara2" ) )
  , ADzero2Name   ( config->getName("ADzero2" ) )
  , ADpara2Name   ( config->getName("ADpara2" ) )
  , nSwaveName    ( config->getName("nSwave"  ) )
  , nPwaveName    ( config->getName("nPwave"  ) )
  , nDwaveName    ( config->getName("nDwave"  ) )
  // Observables
  , mKKName       ( config->getName("mKK"     ) )
  , ctheta_1Name  ( config->getName("ctheta_1") )
  , ctheta_2Name  ( config->getName("ctheta_2") )
  , phiName       ( config->getName("phi"     ) )
  // Options
  , init(true)
{
    MakePrototypes();
    cout << "Making PhiKK" << endl;
    Bs2PhiKKComponent* Swave = new Bs2PhiKKComponent(0, 980    ,100    ,"FT");
    Bs2PhiKKComponent* Pwave = new Bs2PhiKKComponent(1,1019.461,  4.266,"BW");
    Bs2PhiKKComponent* Dwave = new Bs2PhiKKComponent(2,1525    , 73    ,"BW");
}
//Destructor
Bs2PhiKKComponent::~Bs2PhiKKComponent()
{
  delete Swave;
  delete Pwave;
  delete Dwave;
}
//Make the data point and parameter set
void Bs2PhiKKTotal::MakePrototypes()
{
  //Make the DataPoint prototype
  allObservables.push_back( mKKName      );
  allObservables.push_back( ctheta_1Name );
  allObservables.push_back( ctheta_2Name );
  allObservables.push_back( phiName      );
  //Make the parameter set
  vector<string> parameterNames;
  parameterNames.push_back( ASzero2Name );
  parameterNames.push_back( APzero2Name );
  parameterNames.push_back( APpara2Name );
  parameterNames.push_back( ADzero2Name );
  parameterNames.push_back( ADpara2Name );
  parameterNames.push_back( nSwaveName  );
  parameterNames.push_back( nPwaveName  );
  parameterNames.push_back( nDwaveName  );
  allParameters = *( new ParameterSet(parameterNames) );
  cout << "Finished making prototypes" << endl;
}

//Not only set the physics parameters, but indicate that the cache is no longer valid
bool Bs2PhiKKTotal::SetPhysicsParameters( ParameterSet * NewParameterSet )
{
  bool isOK     = allParameters.SetPhysicsParameters(NewParameterSet);
  ASzero2       = allParameters.GetPhysicsParameter( ASzero2Name    )->GetValue();
  APzero2       = allParameters.GetPhysicsParameter( APzero2Name    )->GetValue();
  APpara2       = allParameters.GetPhysicsParameter( APpara2Name    )->GetValue();
  ADzero2       = allParameters.GetPhysicsParameter( ADzero2Name    )->GetValue();
  ADpara2       = allParameters.GetPhysicsParameter( ADpara2Name    )->GetValue();
  nSwave        = allParameters.GetPhysicsParameter( nSwaveName     )->GetValue();
  nPwave        = allParameters.GetPhysicsParameter( nPwaveName     )->GetValue();
  nDwave        = allParameters.GetPhysicsParameter( nDwaveName     )->GetValue();
  
  return isOK;
}
//Return a list of parameters not to be integrated
vector<string> Bs2PhiKKTotal::GetDoNotIntegrateList()
{
  vector<string> list;
  return list;
}
//Calculate the function value
double Bs2PhiKKTotal::Evaluate(DataPoint * measurement)
{
  mKK      = measurement->GetObservable(mKKname     )->GetValue();
  ctheta_1 = measurement->GetObservable(ctheta_1Name)->GetValue();
  ctheta_2 = measurement->GetObservable(ctheta_2Name)->GetValue();
  phi      = measurement->GetObservable(phiName     )->GetValue();
  // S-wave
  vector<TComplex> AS;
  AS.push_back(TComplex(sqrt(ASzero2), deltaS, true);
  Swave.SetParameters(AS);
  // P-wave
  TComplex APperp(sqrt(APperp2()), deltaPperp, true);
  TComplex APpara(sqrt(APpara2), deltaPpara, true);
  vector<TComplex> AP;
  AP.push_back((APpara-APperp)/sqrt(2));
  AP.push_back(TComplex(sqrt(APzero2), deltaPzero, true);
  AP.push_back((APpara+APperp)/sqrt(2));
  Swave.SetParameters(AP);
  // D-wave
  TComplex ADperp(sqrt(ADperp2()), deltaDperp, true);
  TComplex ADpara(sqrt(ADpara2), deltaDpara, true);
  vector<TComplex> AD;
  AD.push_back((ADpara-ADperp)/sqrt(2));
  AD.push_back(TComplex(sqrt(ADzero2), deltaDzero, true);
  AD.push_back((ADpara+ADperp)/sqrt(2));
  Swave.SetParameters(AD);
  
  double evalres
  TComplex amplitude = sqrt(nSwave) * Swave.Amplitude(mKK, phi, ctheta_1, ctheta_2)
                     + sqrt(nPwave) * Pwave.Amplitude(mKK, phi, ctheta_1, ctheta_2)
                     + sqrt(nDwave) * Dwave.Amplitude(mKK, phi, ctheta_1, ctheta_2);
  evalres = amplitude.Rho2();
  return evalres;
}
double Bs2PhiKKTotal::Normalisation(DataPoint * measurement, PhaseSpaceBoundary * boundary)
{
 	return nSwave + nPwave + nDwave;
}
