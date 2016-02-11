#include "LegendreMomentShape.h"
#include <gsl/gsl_sf_legendre.h>
#include <gsl/gsl_sf_gamma.h>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>
#include "TBranch.h"
#include "itoa.h"
using namespace std;
LegendreMomentShape::LegendreMomentShape(string _filename) : filename(_filename), init(true), copied(false)
{
  
  if(!filename.empty())
  {
    cout << "Opening " << filename << endl;
    file = TFile::Open(filename.c_str());
  }
  else return;
  if(file->IsZombie())
  {
    cout << "No file found. Defaulting to uniform shape." << endl;
    delete file;
    return;
  }
  tree = (TTree*)file->Get("LegendreMomentsTree");
  if(tree == (TTree*)0x0) throw runtime_error("LegendreMomentsTree not found");
  tree->SetBranchAddress("mKK_min",&mKK_min);
  tree->SetBranchAddress("mKK_max",&mKK_max);
  string branchtitle = tree->GetBranch("c")->GetTitle();
  cout << branchtitle << endl;
  vector<int*> maxima = {&l_max,&i_max,&k_max,&j_max};
  size_t found(0);
  for(auto maximum: maxima)
  {
    found = branchtitle.find('[',found+1);
    branchtitle.find(']',found);
    *maximum = atoi(branchtitle.substr(found+1,1).c_str());
  }
  createcoefficients();
  tree->GetEntry(0);
  storecoefficients();
  for ( int l = 0; l < l_max; l++ )
  {
    for ( int i = 0; i < i_max; i++ )
    {
      for ( int k = 0; k < k_max; k++ )
      {
        delete c[l][i][k];
      }
      delete c[l][i];
    }
    delete c[l];
  }
  delete c;
  delete tree;
  delete file;
  init = false;
}

LegendreMomentShape::LegendreMomentShape(const LegendreMomentShape& copy) : 
    mKK_min(copy.mKK_min)
  , mKK_max(copy.mKK_max)
  , coeffs(copy.coeffs)
  , init(copy.init)
  , copied(true)
{
}
LegendreMomentShape::~LegendreMomentShape()
{

}
double LegendreMomentShape::Evaluate(double mKK, double phi, double ctheta_1, double ctheta_2)
{
  if(init) return 1;
  double result = 0;
  double mKK_mapped = (mKK - mKK_min) / (mKK_max - mKK_min)*2 - 1;;
  if(abs(mKK_mapped) > 1)
  {
    return 0;
  }
  double Q_l = 0;
  double P_i = 0;
  double Y_jk = 0;
  for(auto coeff : coeffs)
  {
    Q_l  = gsl_sf_legendre_Pl   (coeff.l,  mKK_mapped);
    P_i  = gsl_sf_legendre_Pl   (coeff.i,  ctheta_2);
    // only consider case where k >= 0
    // these are the real valued spherical harmonics
    if ( coeff.k == 0 ) Y_jk =       gsl_sf_legendre_sphPlm (coeff.j, coeff.k, ctheta_1);
    else      Y_jk = sqrt(2) * gsl_sf_legendre_sphPlm (coeff.j, coeff.k, ctheta_1) * cos(coeff.k*phi);
    result += coeff.val*(Q_l * P_i * Y_jk);
  }
  return result;
}
double LegendreMomentShape::Integral(double mKKhi, double mKKlo, double phihi, double philo, double ctheta_1hi, double ctheta_1lo, double ctheta_2hi, double ctheta_2lo)
{
  if(init) return 1;
  double result = 0;
  double mKK_mappedhi = (mKKhi - mKK_min) / (mKK_max - mKK_min)*2 - 1;;
  double mKK_mappedlo = (mKKlo - mKK_min) / (mKK_max - mKK_min)*2 - 1;;
  if(abs(mKK_mappedhi)>1 || abs(mKK_mappedlo)>1) return 0;
  double Q_lhi = 0;
  double Q_llo = 0;
  double P_ihi = 0;
  double P_ilo = 0;
  double Y_jkhi = 0;
  double Y_jklo = 0;
  for(auto coeff : coeffs)
  {
    Q_lhi  = coeff.l>0 ? (gsl_sf_legendre_Pl(coeff.l+1, mKK_mappedhi) - gsl_sf_legendre_Pl(coeff.l-1, mKK_mappedhi)) / (2*coeff.l+1) : mKK_mappedhi;
    Q_llo  = coeff.l>0 ? (gsl_sf_legendre_Pl(coeff.l+1, mKK_mappedlo) - gsl_sf_legendre_Pl(coeff.l-1, mKK_mappedlo)) / (2*coeff.l+1) : mKK_mappedlo;
    P_ihi  = coeff.i>0 ? (gsl_sf_legendre_Pl(coeff.i+1,   ctheta_2hi) - gsl_sf_legendre_Pl(coeff.i-1,   ctheta_2hi)) / (2*coeff.i+1) : ctheta_2hi;
    P_ilo  = coeff.i>0 ? (gsl_sf_legendre_Pl(coeff.i+1,   ctheta_2lo) - gsl_sf_legendre_Pl(coeff.i-1,   ctheta_2lo)) / (2*coeff.i+1) : ctheta_2lo;
    // only consider case where k >= 0
    // these are the real valued spherical harmonics
    if ( coeff.k == 0 )
    {
      Y_jkhi = int_legendre_sphPlm(coeff.j, coeff.k, ctheta_1hi);
      Y_jklo = int_legendre_sphPlm(coeff.j, coeff.k, ctheta_1lo);
    }
    else 
    {
      Y_jkhi = sqrt(2) * int_legendre_sphPlm (coeff.j, coeff.k, ctheta_1hi) * sin(coeff.k*phihi)/(double)coeff.k;
      Y_jklo = sqrt(2) * int_legendre_sphPlm (coeff.j, coeff.k, ctheta_1lo) * sin(coeff.k*philo)/(double)coeff.k;
    }
    result += coeff.val*((Q_lhi - Q_llo) * (P_ihi - P_ilo) * (Y_jkhi - Y_jklo));
  }
  return result;
}
double LegendreMomentShape::int_legendre_sphPlm(int n, int mu, double z)
{
  double sum = 0;
  for(int k = 0; k <= n; k++)
  {
    sum+=((gsl_sf_poch(-n,k)*gsl_sf_poch(n+1,k))/(gsl_sf_gamma(1-mu+k)*factorial(k)))*gsl_sf_beta_inc((2+mu)/2,1+k-mu/2,(1+z)/2);
  }
  return 2*sum;
}
void LegendreMomentShape::createcoefficients()
{
  char branchtitle[10];
  c = new double***[l_max];
  for ( int l = 0; l < l_max; l++ )
  {
    c[l] = new double**[i_max];
    for ( int i = 0; i < i_max; i++ )
    {
      c[l][i] = new double*[k_max];
      for ( int k = 0; k < k_max; k++ )
      {
        c[l][i][k] = new double[j_max];
        for ( int j = 0; j < j_max; j++ )
        {
          if (j < k)
          {
            c[l][i][k][j] = 0;
            continue;
          }
          sprintf(branchtitle,"c%d%d%d%d",l,i,k,j);
          tree->SetBranchAddress(branchtitle,&c[l][i][k][j]);
        }
      }
    }
  }
}
void LegendreMomentShape::storecoefficients()
{
  for ( int l = 0; l < l_max; l++ )
  {
    for ( int i = 0; i < i_max; i++ )
    {
      for ( int k = 0; k < k_max; k++ )
      {
        for ( int j = 0; j < j_max; j++ )
        {
          if (abs(c[l][i][k][j]) < 1e-12) continue;
          coefficient coeff;
          coeff.l = l;
          coeff.i = i;
          coeff.k = k;
          coeff.j = j;
          coeff.val = c[l][i][k][j];
          coeffs.push_back(coeff);
        }
      }
    }
  }
  cout << coeffs.size() << " coefficients stored" << endl;
}
