#ifndef __AutoAngAcc_H__
#define __AutoAngAcc_H__
class AutoAngAcc
{
public:
  static int    gNVariables;        // Number of variables
  static int    gNCoefficients;     // Number of terms
  static double gDMean;             // Mean from training sample
  static double gXMean[];           // Mean from training sample
  static double gXMin[];            // Min from training sample
  static double gXMax[];            // Max from training sample 
  static double gCoefficient[];     // Coefficients
  static double gCoefficientRMS[];  // Coefficients
  static int    gPower[];           // Function powers
  static double MDF(double *);      // Do not use this one
  static double Eval(double * x)    // Because the auto-generated one segfaults
  {
    double returnValue = AutoAngAcc::gDMean;
    int i = 0, j = 0, k = 0;
    for (i = 0; i < AutoAngAcc::gNCoefficients ; i++)
    {
      // Evaluate the ith term in the expansion
      double term = AutoAngAcc::gCoefficient[i];
      for (j = 0; j < AutoAngAcc::gNVariables; j++)
      {
  // Evaluate the polynomial in the jth variable.
        int power = AutoAngAcc::gPower[AutoAngAcc::gNVariables * i + j]; 
        double p1 = 1, p2 = 0, p3 = 0, r = 0;
        double v =  1 + 2. / (AutoAngAcc::gXMax[j] - AutoAngAcc::gXMin[j]) * (x[j] - AutoAngAcc::gXMax[j]);
        // what is the power to use!
        switch(power) {
        case 1: r = 1; break; 
        case 2: r = v; break; 
        default: 
          p2 = v; 
          for (k = 3; k <= power; k++) { 
            p3 = p2 * v;
            p3 = ((2 * k - 3) * p2 * v - (k - 2) * p1) / (k - 1);
            p1 = p2; p2 = p3; 
          }
          r = p3;
        }
        // multiply this term by the poly in the jth var
        term *= r; 
      }
      // Add this term to the final result
      returnValue += term;
    }
    return returnValue;
  }
};
#endif


