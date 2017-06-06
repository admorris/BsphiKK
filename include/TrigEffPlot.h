#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TPad.h"
#include "TStyle.h"
#include "TAxis.h"
template<typename T>
std::string tostr(T value)
{
  std::stringstream s;
  s << std::setprecision(3) << std::fixed << value;
  return s.str();
}
TCanvas* DrawGraph(string name, TGraphErrors& graph)
{
  TCanvas* canvas = new TCanvas(name.c_str(),"",1200,900);
  canvas->SetFillColor(kWhite);
  canvas->cd();
  TPad pad;
  // left right bottom top
  pad.SetMargin(0.15,0.05,0.15,0.05);
  pad.SetTicks(1,1);
  pad.SetFillColor(kWhite);
  pad.Draw();
  pad.cd();
  graph = TGraphErrors(graph);
  float textsize = 0.055;
//  graph.SetMinimum(0);
//  graph.SetMaximum(100);
  graph.SetTitle("");
  graph.GetXaxis()->SetTitle("#it{m}(#it{K^{#plus}K^{#minus}}) [MeV/#it{c}^{2}]");
  graph.GetYaxis()->SetTitle("Efficiency [%]");
  graph.SetLineColor(kBlack);
  graph.SetLineWidth(1);
  graph.SetMarkerSize(1);
  graph.SetMarkerStyle(8);
  graph.SetMarkerColor(kBlack);
  graph.Draw("AP");
  graph.GetXaxis()->SetTitleSize(textsize*1.2);
  graph.GetYaxis()->SetTitleSize(textsize*1.2);
  graph.GetXaxis()->SetLabelSize(textsize);
  graph.GetYaxis()->SetLabelSize(textsize);
  graph.GetXaxis()->SetTitleFont(132);
  graph.GetYaxis()->SetTitleFont(132);
  graph.GetXaxis()->SetLabelFont(132);
  graph.GetYaxis()->SetLabelFont(132);
  graph.GetYaxis()->CenterTitle();
  canvas->SaveAs((name+".pdf").c_str());
  return canvas;
}
struct TrigEffPlot
{
  TrigEffPlot(std::string nam,int n,std::string b, std::string a) : name(nam), befcut(b), aftcut(a)
  {
    y.assign(n,0);
    ye.assign(n,0);
  }
  std::string name;
  std::string befcut;
  std::string aftcut;
  std::vector<double> y;
  std::vector<double> ye;
};
