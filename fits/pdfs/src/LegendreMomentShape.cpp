#include "LegendreMomentShape.h"
#include <gsl/gsl_sf_legendre.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
using std::cout;
using std::endl;
using std::string;
using std::size_t;
using std::vector;
using std::atoi;
LegendreMomentShape::LegendreMomentShape(string filename)
{
  TFile file(filename.c_str());
  TTree* tree = (TTree*)file.Get("LegendreMomentsTree");
  string branchtitle = tree->GetBranch("c")->GetTitle();
  tree->SetBranchAddress("c",c);
  vector<int*> maxima = {&l_max,&i_max,&k_max,&j_max};
  size_t lfound(0), rfound(0);
  for(auto maximum: maxima)
  {
    lfound = branchtitle.find("[",lfound+1);
    rfound = branchtitle.find("]",rfound+1);
    *maximum = atoi(branchtitle.substr(lfound+1,rfound).c_str());
  }
  createcoefficients();
  tree->GetEntry(0);
  file.Close();
}
LegendreMomentShape::LegendreMomentShape(const LegendreMomentShape& copy) : 
    mKK_min(copy.mKK_min)
  , mKK_max(copy.mKK_max)
  , l_max(copy.l_max)
  , i_max(copy.i_max)
  , k_max(copy.k_max)
  , j_max(copy.j_max)
{
  copycoefficients(copy.c);
}
LegendreMomentShape::~LegendreMomentShape()
{
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
}
double LegendreMomentShape::Evaluate(double mKK, double phi, double ctheta_1, double ctheta_2)
{
  double acceptance = 0;
  double mKK_mapped = (mKK - mKK_min) / (mKK_max - mKK_min)*2 - 1;
  double Q_l = 0;
  double P_i = 0;
  double Y_jk = 0;
  for ( int l = 0; l < l_max; l++ )
  {
    for ( int i = 0; i < i_max; i++ )
    {
      for ( int k = 0; k < k_max; k++ )
      {
        for ( int j = 0; j < j_max; j++ )
        {
          if (j < k) continue; // must have l >= k
          Q_l  = gsl_sf_legendre_Pl   (l,  mKK_mapped);
          P_i  = gsl_sf_legendre_Pl   (i,  ctheta_2);
          // only consider case where k >= 0
          // these are the real valued spherical harmonics
          if ( k == 0 ) Y_jk =       gsl_sf_legendre_sphPlm (j, k, ctheta_1);
          else      Y_jk = sqrt(2) * gsl_sf_legendre_sphPlm (j, k, ctheta_1) * cos(k*phi);
          acceptance += c[l][i][k][j]*(Q_l * P_i * Y_jk);
        }
      }
    }
  }
  return acceptance;
}
double LegendreMomentShape::mKK_min = 988;
double LegendreMomentShape::mKK_max = 1800;
int LegendreMomentShape::l_max = 7;
int LegendreMomentShape::i_max = 7;
int LegendreMomentShape::k_max = 3;
int LegendreMomentShape::j_max = 3;
void LegendreMomentShape::createcoefficients()
{
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
          c[l][i][k][j] = 0;
        }
      }
    }
  }
}
void LegendreMomentShape::copycoefficients(double**** otherc)
{
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
          c[l][i][k][j] = otherc[l][i][k][j];
        }
      }
    }
  }
}
