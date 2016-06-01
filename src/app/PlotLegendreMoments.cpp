#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>
#include <TNtuple.h>
#include <TMath.h>
#include <TProfile.h>
#include <TLine.h>
#include <TROOT.h>
#include <gsl/gsl_sf_legendre.h>
#include <iostream>

#include "GetTree.h"

int main(int argc, char* argv[])
{
  if(argc!=2)
  {
    std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
    return 1;
  }
//    gROOT->ProcessLine(".x ~/lhcb/lhcbStyle.C");
    //Set up efficiency
    static const int l_max = 2;
    static const int i_max = 2;
    static const int k_max = 2;
    static const int j_max = 2;
    double c[l_max+1][i_max+1][k_max+1][j_max+1];

    for ( int l = 0; l < l_max + 1; l++ )
    {
        for ( int i = 0; i < i_max + 1; i++ )
        {
            for ( int k = 0; k < k_max + 1; k++ )
            {
                for ( int j = 0; j < j_max + 1; j++ )
                {
                    c[l][i][k][j] = 0.;
                }
            }
        }
    }

	// Bs efficiency
	c[0][0][0][0] = 0.190436;// +- 0.000003;
	c[0][0][0][1] = -0.000805;// +- 0.000007;
	c[0][0][0][2] = 0.006609;// +- 0.000007;
	c[0][0][1][1] = 0.002799;// +- 0.000007;
	c[0][0][1][2] = -0.000126;// +- 0.000007;
	c[0][0][2][2] = 0.006394;// +- 0.000007;
	c[0][1][0][0] = -0.018081;// +- 0.000012;
	c[0][1][0][1] = -0.002133;// +- 0.000012;
	c[0][1][0][2] = -0.003026;// +- 0.000012;
	c[0][1][1][1] = -0.000991;// +- 0.000012;
	c[0][1][1][2] = 0.005612;// +- 0.000012;
	c[0][1][2][2] = 0.003032;// +- 0.000012;
	c[0][2][0][0] = -0.048005;// +- 0.000015;
	c[0][2][0][1] = 0.004255;// +- 0.000016;
	c[0][2][0][2] = 0.005039;// +- 0.000016;
	c[0][2][1][1] = -0.000790;// +- 0.000015;
	c[0][2][1][2] = 0.000236;// +- 0.000015;
	c[0][2][2][2] = -0.003310;// +- 0.000015;
	c[1][0][0][0] = 0.028022;// +- 0.000015;
	c[1][0][0][1] = 0.001951;// +- 0.000015;
	c[1][0][0][2] = -0.003592;// +- 0.000015;
	c[1][0][1][1] = -0.000999;// +- 0.000015;
	c[1][0][1][2] = 0.002400;// +- 0.000015;
	c[1][0][2][2] = 0.008856;// +- 0.000015;
	c[1][1][0][0] = 0.046359;// +- 0.000025;
	c[1][1][0][1] = 0.002633;// +- 0.000025;
	c[1][1][0][2] = 0.003984;// +- 0.000025;
	c[1][1][1][1] = -0.001046;// +- 0.000024;
	c[1][1][1][2] = 0.016059;// +- 0.000024;
	c[1][1][2][2] = 0.006096;// +- 0.000025;
	c[1][2][0][0] = 0.036591;// +- 0.000032;
	c[1][2][0][1] = 0.007403;// +- 0.000033;
	c[1][2][0][2] = 0.011290;// +- 0.000034;
	c[1][2][1][1] = 0.006795;// +- 0.000032;
	c[1][2][1][2] = -0.005211;// +- 0.000032;
	c[1][2][2][2] = -0.008533;// +- 0.000033;
	c[2][0][0][0] = 0.009822;// +- 0.000020;
	c[2][0][0][1] = 0.001300;// +- 0.000020;
	c[2][0][0][2] = -0.002539;// +- 0.000021;
	c[2][0][1][1] = 0.005795;// +- 0.000020;
	c[2][0][1][2] = 0.000852;// +- 0.000020;
	c[2][0][2][2] = -0.005574;// +- 0.000020;
	c[2][1][0][0] = 0.000689;// +- 0.000033;
	c[2][1][0][1] = 0.004604;// +- 0.000033;
	c[2][1][0][2] = -0.003658;// +- 0.000034;
	c[2][1][1][1] = -0.009699;// +- 0.000032;
	c[2][1][1][2] = 0.009891;// +- 0.000032;
	c[2][1][2][2] = -0.003647;// +- 0.000033;
	c[2][2][0][0] = 0.020384;// +- 0.000044;
	c[2][2][0][1] = 0.004512;// +- 0.000044;
	c[2][2][0][2] = 0.014787;// +- 0.000045;
	c[2][2][1][1] = 0.002289;// +- 0.000043;
	c[2][2][1][2] = -0.001272;// +- 0.000043;
	c[2][2][2][2] = 0.003469;// +- 0.000045;

	/*
        // Bd efficiency

    c[0][0][0][0] = 0.070537;// +- 0.000042
    c[0][0][0][2] = 0.003374;// +- 0.000108
    c[0][0][1][2] = -0.000793;// +- 0.000108
    c[0][0][2][2] = 0.004150;// +- 0.000104
    c[0][1][0][0] = -0.009559;// +- 0.000175
    c[0][1][0][2] = -0.001790;// +- 0.000180
    c[0][1][1][2] = 0.001620;// +- 0.000180
    c[0][1][2][2] = 0.000894;// +- 0.000172
    c[0][2][0][0] = -0.014521;// +- 0.000227
    c[0][2][1][2] = 0.001962;// +- 0.000233
    c[0][2][2][2] = -0.002894;// +- 0.000224
    c[0][4][0][0] = -0.002925;// +- 0.000307
    c[1][0][0][0] = 0.014735;// +- 0.000218
    c[1][0][0][2] = -0.004731;// +- 0.000224
    c[1][0][1][2] = 0.001244;// +- 0.000223
    c[1][0][2][2] = 0.004222;// +- 0.000215
    c[1][1][0][0] = 0.013788;// +- 0.000366
    c[1][1][1][2] = 0.008654;// +- 0.000375
    c[1][2][0][0] = 0.013075;// +- 0.000476
    c[1][2][0][2] = 0.008135;// +- 0.000492
    c[1][2][2][2] = -0.003336;// +- 0.000467
    c[1][3][1][2] = -0.004594;// +- 0.000575
    c[2][0][0][0] = 0.002701;// +- 0.000294
    c[2][1][1][2] = 0.004370;// +- 0.000507
    c[2][2][0][0] = 0.008449;// +- 0.000640
    c[2][2][0][2] = 0.006358;// +- 0.000663
    c[2][3][0][0] = -0.004795;// +- 0.000759
    c[2][3][1][2] = -0.004152;// +- 0.000771
    c[3][1][0][0] = -0.007938;// +- 0.000598
    c[3][2][0][0] = -0.006623;// +- 0.000772
    c[4][1][0][0] = 0.004372;// +- 0.000681
    c[4][3][0][0] = 0.006850;// +- 0.001044
    c[5][0][0][0] = -0.003132;// +- 0.000446
    c[6][0][0][0] = 0.004619;// +- 0.000485
    c[6][1][0][0] = -0.005224;// +- 0.000822
	*/
    double min(0.64);//0.633);
    double max(1.68);// Bs
    //double max(1.59);// Bd
    const int nbins(15);

    const int numMoments(9);
    TH1D histos_tot[numMoments];
    TH1D histos_bkg[numMoments];
    TH1D histos_sig[numMoments];

    char buffer[20];
    for (int i = 0; i < numMoments; i++)
    {
        sprintf(buffer, "tot_hprof%d", i);
        histos_tot[i] = TH1D(buffer,"hprof",nbins, min, max);
        histos_tot[i].Sumw2();
        sprintf(buffer, "bkg_hprof%d", i);
        histos_bkg[i] = TH1D(buffer,"hprof",nbins, min, max);
        histos_bkg[i].Sumw2();
        histos_bkg[i].SetMarkerColor(kRed);;
        histos_bkg[i].SetLineColor(kRed);;
        sprintf(buffer, "sig_hprof%d", i);
        histos_sig[i] = TH1D(buffer,"hprof",nbins, min, max);
        histos_sig[i].Sumw2();
    }

//    TFile * f_data = TFile::Open(argv[1]);
//    TTree * t_data = (TTree*)f_data->Get("tuple");
    TTree* t_data = GetTree(argv[1]);
    //TFile * f_mc   = TFile::Open("../fits/K892_K21430_NR_sFit_tight_data/test.root");
    //TFile * f_mc   = TFile::Open("../fits/K892_NR_sFit_tight_data/test.root");
    TFile * f_mc   = TFile::Open("toy_no_bkg_no_eff.root");
    //TFile * f_mc   = TFile::Open("toy_with_bkg_no_eff.root");
    TTree * t_mc   = (TTree*)f_mc  ->Get("dataNTuple");

    Float_t         m23;
    Float_t         phi;
    Float_t         cosTheta2;
    Float_t         cosTheta1;
    Float_t         Bs_sig_sw;
    Float_t         Bd_sig_sw;
    Float_t         pionID;	    
    Float_t         mlb;	    
    Float_t         Kplus_ProbNNp;	    

    TBranch        *b_m23;   //!
    TBranch        *b_phi;   //!
    TBranch        *b_cosTheta2;   //!
    TBranch        *b_cosTheta1;   //!
    TBranch        *b_Bs_sig_sw;   //!
    TBranch        *b_Bd_sig_sw;   //!
    TBranch	   *b_pionID;
    TBranch	   *b_mlb;
    TBranch	   *b_Kplus_ProbNNp;

    t_data->SetBranchAddress("m23", &m23, &b_m23);
    t_data->SetBranchAddress("phi", &phi, &b_phi);
    t_data->SetBranchAddress("cosTheta2", &cosTheta2, &b_cosTheta2);
    t_data->SetBranchAddress("cosTheta1", &cosTheta1, &b_cosTheta1);
    t_data->SetBranchAddress("Bs_sig_sw", &Bs_sig_sw, &b_Bs_sig_sw);
    t_data->SetBranchAddress("Bd_sig_sw", &Bd_sig_sw, &b_Bd_sig_sw);
    t_data->SetBranchAddress("pionID", &pionID, &b_pionID);
    t_data->SetBranchAddress("mlb", &mlb, &b_mlb);
    t_data->SetBranchAddress("Kplus_ProbNNp", &Kplus_ProbNNp, &b_Kplus_ProbNNp);
    double         m23_mc;
    double         phi_mc;
    double         cosTheta2_mc;
    double         cosTheta1_mc;
    t_mc->SetBranchAddress("m23", &m23_mc, &b_m23);
    t_mc->SetBranchAddress("phi", &phi_mc, &b_phi);
    t_mc->SetBranchAddress("cosTheta2", &cosTheta2_mc, &b_cosTheta2);
    t_mc->SetBranchAddress("cosTheta1", &cosTheta1_mc, &b_cosTheta1);

    int n_data = t_data->GetEntriesFast();
    int n_mc   = t_mc  ->GetEntriesFast();

    for (int jentry = 0; jentry < n_data; jentry++) {
        t_data->GetEntry(jentry);
        if (m23/1000. < min || m23/1000. > max) continue;

        double m23_mapped = (m23/1000. - min)/(max - min)*2. + (-1); // should really do this in a generic way

        double angularAcc(0.);
        double Q_l(0.);
        double P_i(0.);
        double Y_jk(0.);
        for ( int l = 0; l < l_max+1; l++ )
        {
            for ( int i = 0; i < i_max+1; i++ )
            {
                for ( int k = 0; k < k_max+1; k++ )
                {
                    for ( int j = 0; j < j_max+1; j++ ) // limiting the loop here to only look at terms we need
                    {
                        if (j < k) continue; // must have l >= k
                        Q_l  = gsl_sf_legendre_Pl     (l,    m23_mapped);
                        P_i  = gsl_sf_legendre_Pl     (i,    cosTheta2);
                        //std::cout << "Q_l " << Q_l << " " << m23_mapped << std::endl;
                        // only consider case where k >= 0
                        // these are the real valued spherical harmonics
                        if ( k == 0 ) Y_jk =           gsl_sf_legendre_sphPlm (j, k, cosTheta1);
                        else          Y_jk = sqrt(2) * gsl_sf_legendre_sphPlm (j, k, cosTheta1) * cos(k*phi);
                        angularAcc += c[l][i][k][j]*(Q_l * P_i * Y_jk);
                    }
                }
            }
        }

        for (int i = 0; i < numMoments; i++)
        {
            double p = gsl_sf_legendre_Pl(i, cosTheta2)/angularAcc;
            histos_tot[i].Fill(m23/1000., p*Bs_sig_sw);
	}
    }

    // Now get them for the MC
    for (int jentry = 0; jentry < n_mc; jentry++) {
        t_mc->GetEntry(jentry);
        for (int i = 0; i < numMoments; i++)
        {
            double p = gsl_sf_legendre_Pl(i, cosTheta2_mc);
            histos_bkg[i].Fill(m23_mc, p);
        }
    }

	
    for (int i = 0; i < numMoments; i++)
    	histos_bkg[i].Scale(n_data/float(n_mc));

    gStyle->SetOptStat(0);
    TCanvas * can = new TCanvas();
    TLine *l = new TLine(min,0.,max,0.);
    can->Divide(3,3);
    for ( int i = 0; i < numMoments; i++)
    {
        can->cd(i+1);
        sprintf(buffer, "<P_{%d}^{U}>", i);
        histos_tot[i].Draw();
        histos_bkg[i].Draw("same");
        histos_tot[i].GetYaxis()->SetTitle(buffer);
        histos_tot[i].GetXaxis()->SetTitle("m(K^{+}#pi^{#font[122]{-}}) [GeV/#it{c}^{2}]");
        can->Update();
        l->Draw();
    }
    can->SaveAs("moments_toy.pdf");

    TCanvas * can2 = new TCanvas();
    can2->Divide(3,3);
    for ( int i = 0; i < numMoments; i++)
    {
        can2->cd(i+1);
        sprintf(buffer, "<P_{%d}^{U}>", i);
        //histos_sig[i].Add(&(histos_tot[i]), &(histos_bkg[i]), 1., -1.);
        histos_tot[i].Draw();
        histos_tot[i].GetYaxis()->SetTitle(buffer);
        histos_tot[i].GetXaxis()->SetTitle("m(K^{+}#pi^{#font[122]{-}}) [GeV/#it{c}^{2}]");
        can2->Update();
        l->Draw();
    }
    can2->SaveAs("moments_weighted_mKpi_bkg_sub.pdf");

    return 1.;
}

