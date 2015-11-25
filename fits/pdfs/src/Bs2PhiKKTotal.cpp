/** @class Bs2PhiKKTotal Bs2PhiKKTotal.cpp
 *
 *  RapidFit PDF for Bs2PhiKKTotal
 *
 *  @author Adam Morris
 *  @date Nov-Dec 2015
 */
// Self
#include "Bs2PhiKKTotal.h"
// Std Libraries
#include <iostream>
// ROOT Libraries
#include "TComplex.h"
// RapidFit
#include "PDFConfigurator.h"
#define DEBUGFLAG true

PDF_CREATOR( Bs2PhiKKTotal )

//Constructor
Bs2PhiKKTotal::Bs2PhiKKTotal(PDFConfigurator* config) :
  // Dependent variables
    mKK(0.0)
  , phi(0.0)
  , ctheta_1(0.0)
  , ctheta_2(0.0)
  // Dependent variable names
  , mKKName       ( config->getName("mKK"     ) )
  , ctheta_1Name  ( config->getName("ctheta_1") )
  , ctheta_2Name  ( config->getName("ctheta_2") )
  , phiName       ( config->getName("phi"     ) )
  // Options
  , init(true)
{
  // Set physics parameters to zero for now
  ASsq      = 0;
  deltaS    = 0;
  for(unsigned short i = 0; i < 3; i++)
  {
    APsq[i]   = 0;
    ADsq[i]   = 0;
    deltaP[i] = 0;
    deltaD[i] = 0;
  }
  // Magnitude-squared of helicity amplitudes
  ASsqName    = config->getName("ASzero2" );
  APsqName[0] = config->getName("APminus2");
  APsqName[1] = config->getName("APzero2" );
  APsqName[2] = config->getName("APplus2" );
  ADsqName[0] = config->getName("ADminus2");
  ADsqName[1] = config->getName("ADzero2" );
  ADsqName[2] = config->getName("ADplus2" );
  // Phases of helicity amplitudes
  deltaSName    = config->getName("deltaSzero" );
  deltaPName[0] = config->getName("deltaPminus");
  deltaPName[1] = config->getName("deltaPzero" );
  deltaPName[2] = config->getName("deltaPplus" );
  deltaDName[0] = config->getName("deltaDminus");
  deltaDName[1] = config->getName("deltaDzero" );
  deltaDName[2] = config->getName("deltaDplus" );  
  MakePrototypes();
  cout << "Making PhiKK" << endl;
  // Initialise the signal components
  Swave = new Bs2PhiKKComponent(0, 980    ,100    ,"FT");
  Pwave = new Bs2PhiKKComponent(1,1019.461,  4.266,"BW");
  Dwave = new Bs2PhiKKComponent(2,1525    , 73    ,"BW");
}
//Destructor
Bs2PhiKKTotal::~Bs2PhiKKTotal()
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
  parameterNames.push_back( ASsqName );
  parameterNames.push_back( deltaSName );
  for(unsigned short i = 0; i < 3; i++)
  {
    parameterNames.push_back( APsqName[i] );
    parameterNames.push_back( ADsqName[i] );
    parameterNames.push_back( deltaPName[i] );
    parameterNames.push_back( deltaDName[i] );
  }
  allParameters = *( new ParameterSet(parameterNames) );
  cout << "Finished making prototypes" << endl;
}

//Not only set the physics parameters, but indicate that the cache is no longer valid
bool Bs2PhiKKTotal::SetPhysicsParameters( ParameterSet * NewParameterSet )
{
  bool isOK = allParameters.SetPhysicsParameters(NewParameterSet);
  ASsq      = allParameters.GetPhysicsParameter(ASsqName     )->GetValue();
  deltaS    = allParameters.GetPhysicsParameter(deltaSName   )->GetValue();
  for(unsigned short i = 0; i < 3; i++)
  {
    APsq[i]   = allParameters.GetPhysicsParameter(APsqName[i]  )->GetValue();
    ADsq[i]   = allParameters.GetPhysicsParameter(ADsqName[i]  )->GetValue();
    deltaP[i] = allParameters.GetPhysicsParameter(deltaPName[i])->GetValue();
    deltaD[i] = allParameters.GetPhysicsParameter(deltaDName[i])->GetValue();
  }
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
  mKK      = measurement->GetObservable(mKKName     )->GetValue();
  ctheta_1 = measurement->GetObservable(ctheta_1Name)->GetValue();
  ctheta_2 = measurement->GetObservable(ctheta_2Name)->GetValue();
  phi      = measurement->GetObservable(phiName     )->GetValue();
  // Construct and set helicity amplitudes
  // S-wave
  vector<TComplex> AS;
  AS.push_back(TComplex(sqrt(ASsq), deltaS, true));
  Swave->SetHelicityAmplitudes(AS);
  // P- and D-wave
  vector<TComplex> AP, AD;
  for(unsigned short i = 0; i < 3; i++)
  {
    AP.push_back(TComplex(sqrt(APsq[i]), deltaP[i], true));
    AD.push_back(TComplex(sqrt(ADsq[i]), deltaD[i], true));
  }
  Pwave->SetHelicityAmplitudes(AP);
  Dwave->SetHelicityAmplitudes(AD);
  // Sum-square of component amplitudes 
  double evalres;
  TComplex amplitude = Swave->Amplitude(mKK, phi, ctheta_1, ctheta_2)
                     + Pwave->Amplitude(mKK, phi, ctheta_1, ctheta_2)
                     + Dwave->Amplitude(mKK, phi, ctheta_1, ctheta_2);
  evalres = amplitude.Rho2();
  return evalres;
}
double Bs2PhiKKTotal::Normalisation(DataPoint * measurement, PhaseSpaceBoundary * boundary)
{
  double norm = 0;
  norm += ASsq;
  for(unsigned short i = 0; i < 3; i++)
  {
    norm+=APsq[i];
    norm+=ADsq[i];
  }
 	return norm;
}
