/** @class Bs2PhiKK Bs2PhiKK.cpp
 *
 *  RapidFit PDF for Bs2PhiKK
 *
 *  @author Adam Morris
 *  @date Sept 2015
 */

#include "Bs2PhiKK.h"
#include <iostream>
#include "math.h"
#include "TMath.h"
#include "PDFConfigurator.h"
#define DEBUGFLAG true

PDF_CREATOR( Bs2PhiKK )

//Constructor
Bs2PhiKK::Bs2PhiKK(PDFConfigurator* config) : 
    K1(0.0), K2(0.0), K3(0.0), K4(0.0), K5(0.0), K6(0.0)
  , phi(0.0), ctheta_1(0.0), ctheta_2(0.0)
  // Physics parameters
  , K1Name        ( config->getName("K1") )
  , K2Name        ( config->getName("K2") )
  , K3Name        ( config->getName("K3") )
  , K4Name        ( config->getName("K4") )
  , K5Name        ( config->getName("K5") )
  , K6Name        ( config->getName("K6") )
  // Observables
  , ctheta_1Name  ( config->getName("ctheta_1") )
  , ctheta_2Name  ( config->getName("ctheta_2") )
  , phiName       ( config->getName("phi") )
  // Options
  , init(true)
{
    MakePrototypes();
    cout << "Making PhiKK" << endl;
}

//Make the data point and parameter set
void Bs2PhiKK::MakePrototypes()
{
  //Make the DataPoint prototype
  allObservables.push_back( ctheta_1Name );
  allObservables.push_back( ctheta_2Name );
  allObservables.push_back( phiName );
  //Make the parameter set
  vector<string> parameterNames;
  parameterNames.push_back( K1Name );
  parameterNames.push_back( K2Name );
  parameterNames.push_back( K3Name );
  parameterNames.push_back( K4Name );
  parameterNames.push_back( K5Name );
  parameterNames.push_back( K6Name );
  allParameters = *( new ParameterSet(parameterNames) );
  cout << "Finished making prototypes" << endl;
}

//................................................
//  Copy Constructor
Bs2PhiKK::Bs2PhiKK( const Bs2PhiKK& input ) :
    BasePDF( (BasePDF) input )
  , K1(input.K1)
  , K2(input.K2)
  , K3(input.K3)
  , K4(input.K4)
  , K5(input.K5)
  , K6(input.K6)
  , ctheta_1(input.ctheta_1)
  , ctheta_2(input.ctheta_2)
  , phi(input.phi)
  // Names
  , K1Name(input.K1Name)
  , K2Name(input.K2Name)
  , K3Name(input.K3Name)
  , K4Name(input.K4Name)
  , K5Name(input.K5Name)
  , K6Name(input.K6Name)
  , ctheta_1Name(input.ctheta_1Name)
  , ctheta_2Name(input.ctheta_2Name)
  , phiName(input.phiName)
{
}

//Destructor
Bs2PhiKK::~Bs2PhiKK()
{
}

//Not only set the physics parameters, but indicate that the cache is no longer valid
bool Bs2PhiKK::SetPhysicsParameters( ParameterSet * NewParameterSet )
{
  bool isOK = allParameters.SetPhysicsParameters(NewParameterSet);
  K1        = allParameters.GetPhysicsParameter( K1Name )->GetValue();
  K2        = allParameters.GetPhysicsParameter( K2Name )->GetValue();
  K3        = allParameters.GetPhysicsParameter( K3Name )->GetValue();
  K4        = allParameters.GetPhysicsParameter( K4Name )->GetValue();
  K5        = allParameters.GetPhysicsParameter( K5Name )->GetValue();
  K6        = allParameters.GetPhysicsParameter( K6Name )->GetValue();
  return isOK;
}
//Return a list of parameters not to be integrated
vector<string> Bs2PhiKK::GetDoNotIntegrateList()
{
  vector<string> list;
  return list;
}
//Calculate the function value
double Bs2PhiKK::Evaluate(DataPoint * measurement)
{
  double evalres;
  evalres = HangleFactorA0A0()   * K1
          + HangleFactorAPAP()   * K2
          + HangleFactorATAT()   * K3
          + HangleFactorImAPAT() * K4
          + HangleFactorReA0AP() * K5
          + HangleFactorImA0AT() * K6
          ;
  if (std::isnan(evalres))
  {
    cout << "PDF\t" << evalres << endl;
    cout << "K1\t" << K1 << endl;
    cout << "K2\t" << K2 << endl;
    cout << "K3\t" << K3 << endl;
    cout << "K4\t" << K4 << endl;
    cout << "K5\t" << K5 << endl;
    cout << "K6\t" << K6 << endl;
    cout << "f1\t" << HangleFactorA0A0() << endl   ;
    cout << "f2\t" << HangleFactorAPAP() << endl   ;
    cout << "f3\t" << HangleFactorATAT() << endl   ;
    cout << "f4\t" << HangleFactorImAPAT() << endl ;
    cout << "f5\t" << HangleFactorReA0AP() << endl ;
    cout << "f6\t" << HangleFactorImA0AT() << endl ;
  }
  return evalres;
}
double Bs2PhiKK::Normalisation(DataPoint * measurement, PhaseSpaceBoundary * boundary)
{
 	return K1 + K2 + K3;
}

