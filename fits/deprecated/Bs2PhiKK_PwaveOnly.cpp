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
    Apara2(0.0)
  , Azero2(0.0)
  , GammaL(0.0)
  , GammaH(0.0)
  , deltapara(0.0)
  , phi(0.0)
  , ctheta_1(0.0)
  , ctheta_2(0.0)
  // Physics parameters
  , Apara2Name    ( config->getName("Apara2"   ) )
  , Azero2Name    ( config->getName("Azero2"   ) )
  , GammaLName    ( config->getName("GammaL"   ) )
  , GammaHName    ( config->getName("GammaH"   ) )
  , deltaparaName ( config->getName("deltapara") )
  // Observables
  , ctheta_1Name  ( config->getName("ctheta_1" ) )
  , ctheta_2Name  ( config->getName("ctheta_2" ) )
  , phiName       ( config->getName("phi"      ) )
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
  // parameterNames.push_back( AperpName );
  parameterNames.push_back( Apara2Name );
  parameterNames.push_back( Azero2Name );
  parameterNames.push_back( GammaLName );
  parameterNames.push_back( GammaHName );
  parameterNames.push_back( deltaparaName );
  allParameters = *( new ParameterSet(parameterNames) );
  cout << "Finished making prototypes" << endl;
}

//................................................
//  Copy Constructor
Bs2PhiKK::Bs2PhiKK( const Bs2PhiKK& input ) :
    BasePDF( (BasePDF) input )
  // , Aperp(input.Aperp)
  , Apara2(input.Apara2)
  , Azero2(input.Azero2)
  , GammaL(input.GammaL)
  , GammaH(input.GammaH)
  , deltapara(input.deltapara)
  , ctheta_1(input.ctheta_1)
  , ctheta_2(input.ctheta_2)
  , phi(input.phi)
  // Names
  // , AperpName(input.AperpName)
  , Apara2Name(input.Apara2Name)
  , Azero2Name(input.Azero2Name)
  , GammaLName(input.GammaLName)
  , GammaHName(input.GammaHName)
  , deltaparaName(input.deltaparaName)
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
  Apara2       = allParameters.GetPhysicsParameter( Apara2Name    )->GetValue();
  Azero2       = allParameters.GetPhysicsParameter( Azero2Name    )->GetValue();
  GammaL       = allParameters.GetPhysicsParameter( GammaLName    )->GetValue();
  GammaH       = allParameters.GetPhysicsParameter( GammaHName    )->GetValue();
  deltapara    = allParameters.GetPhysicsParameter( deltaparaName )->GetValue();
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
  ctheta_1 = measurement->GetObservable(ctheta_1Name)->GetValue();
  ctheta_2 = measurement->GetObservable(ctheta_2Name)->GetValue();
  phi      = measurement->GetObservable(phiName     )->GetValue();
  double evalres;
  evalres = HangleFactorA0A0()   * K1()
          + HangleFactorAPAP()   * K2()
          + HangleFactorATAT()   * K3()
          + HangleFactorImAPAT() * K4()
          + HangleFactorReA0AP() * K5()
          + HangleFactorImA0AT() * K6()
          ;
  if (std::isnan(evalres))
  {
    cout << "PDF\t" << evalres << endl;
    cout << "K1\t" << K1() << endl;
    cout << "K2\t" << K2() << endl;
    cout << "K3\t" << K3() << endl;
    cout << "K4\t" << K4() << endl;
    cout << "K5\t" << K5() << endl;
    cout << "K6\t" << K6() << endl;
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
 	return K1() + K2() + K3();
}
