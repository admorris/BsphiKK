/** @class LegendreMomentShapePDF LegendreMomentShapePDF.h
 *
 *  RapidFit PDF for LegendreMomentShapePDF
 *
 *  @author Adam Morris
 *  @date Feb 2016
 */
#ifndef LegendreMomentShapePDF_H
#define LegendreMomentShapePDF_H

#ifndef __CINT__
#include "BasePDF.h"
#endif
#ifdef __CINT__
#include "framework/include/BasePDF.h"
#endif
// C++
#include <string>
// Self
#include "LegendreMomentShape.h"

class LegendreMomentShapePDF : public BasePDF
{
    public:
      // *structors
      LegendreMomentShapePDF(PDFConfigurator*);
      LegendreMomentShapePDF(const LegendreMomentShapePDF&);
      ~LegendreMomentShapePDF();
      // Required methods
      virtual double Evaluate(DataPoint*);
      virtual double Normalisation(PhaseSpaceBoundary*);
      virtual bool SetPhysicsParameters(ParameterSet*);
      virtual vector<string> GetDoNotIntegrateList();
    protected:
      // K+K− mass and helicity angles
      double        mKK,     ctheta_1,     ctheta_2,     phi;
      ObservableRef mKKName, ctheta_1Name, ctheta_2Name, phiName;
      // Acceptance object
      LegendreMomentShape* shape;
    private:
      string filename;
      // Stuff to do on creation
      void Initialise();
      void MakePrototypes();
};
#endif

