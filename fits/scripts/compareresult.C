struct phiphiresult
{
  phiphiresult(std::string newtitle, std::pair<double,double> newAp, std::pair<double,double> newAz, std::pair<double,double> newdp)
    : title(newtitle), Ap(newAp), Az(newAz), dp(newdp) {}
  std::string title;
  std::pair<double,double> Ap;
  std::pair<double,double> Az;
  std::pair<double,double> dp;
};
void compareresult(std::pair<double,double> magAplus, std::pair<double,double> deltaplus, std::pair<double,double> magAzero, std::pair<double,double> deltaminus)
{
  std::vector<phiphiresult> results;
  std::complex<double> Aplus, Aminus, Aperp, Apara;
  Aplus = std::polar<double>(magAplus.first,deltaplus.first);
  Aminus = std::polar<double>(std::sqrt(1. - std::pow(magAplus.first,2) - std::pow(magAzero.first,2)),deltaminus.first);
  Apara = (Aplus + Aminus) / std::sqrt(2.);
  Aperp = (Aplus - Aminus) / std::sqrt(2.);
  results.push_back(phiphiresult("This fit",{std::pow(std::abs(Aperp),2),0.0},{std::pow(magAzero.first,2),0.0},{std::arg(Apara),0.0}));
  results.push_back(phiphiresult("LHCb Run 1",{0.305,0.013},{0.364,0.013},{2.54,0.07}));
  results.push_back(phiphiresult("LHCb 2011", {0.291,0.024},{0.365,0.024},{2.58,0.12}));
  results.push_back(phiphiresult("CDF and MC",{0.365,0.041},{0.348,0.043},{2.71,0.26}));
  TGraphErrors Ap_graph;
  TGraphErrors Az_graph;
  TGraphErrors dp_graph;
  std::vector<std::string> graphnames = {"Aperpsq","Azerosq","deltapara"};
  std::vector<std::string> graphtitles = {"|A_{#perp}  |^{2}","|A_{0}|^{2}","#delta_{#parallel}"};
  for(auto& result: results)
  {
    int i = &result - &results[0];
    i++;
    Ap_graph.SetPoint(i,std::get<0>(result.Ap),i); Ap_graph.SetPointError(i,std::get<1>(result.Ap),0);
    Az_graph.SetPoint(i,std::get<0>(result.Az),i); Az_graph.SetPointError(i,std::get<1>(result.Az),0);
    dp_graph.SetPoint(i,std::get<0>(result.dp),i); dp_graph.SetPointError(i,std::get<1>(result.dp),0);
  }
  std::vector<TGraphErrors> graphs = {Ap_graph, Az_graph, dp_graph};
  TText text;
  text.SetTextFont(132);
  TLine line;
  line.SetLineStyle(3);
  line.SetLineWidth(2);
  TLine redline;
  redline.SetLineStyle(kDotted);
  redline.SetLineColor(kRed);
  for(auto& graph: graphs)
  {
    TCanvas can;
    can.SetTicks(1,0);
    can.SetLeftMargin(0.3);
    can.SetBottomMargin(0.2);
    int i = &graph - &graphs[0];
    graph.SetLineWidth(2);
    graph.SetMarkerStyle(8);
    graph.SetMarkerSize(0.8);
    graph.GetXaxis()->SetTitle(graphtitles[i].c_str());
    graph.GetXaxis()->SetLabelSize(0.04);
    graph.GetXaxis()->SetTitleSize(0.06);
    graph.GetXaxis()->SetLabelFont(132);
    graph.GetXaxis()->SetTitleFont(132);
    graph.GetYaxis()->SetTickLength(0);
    graph.GetYaxis()->SetLabelSize(0);
    graph.SetMinimum(0.5);
    graph.SetMaximum(results.size()+0.5);
    graph.Draw("APE"); // Draw an ape :)
    double j(0);
    for(auto result: results)
    {
      double scale = (1-can.GetTopMargin()-can.GetBottomMargin())/results.size();
      text.DrawTextNDC(0.05,can.GetBottomMargin()+(0.5+j++)*scale,result.title.c_str());
      if(j<results.size())
        line.DrawLineNDC(can.GetLeftMargin(),can.GetBottomMargin()+j*scale,1-can.GetRightMargin(),can.GetBottomMargin()+j*scale);
    }
    double x(0), y(0);
    if(graph.GetPoint(1,x,y) != -1)
    {
      std::cout << "Drawing vertical line at " << x << std::endl;
      redline.DrawLine(x,graph.GetMinimum(),x,graph.GetMaximum());
    }
    else
      std::cout << "Problem getting point 0 on graph" << std::endl;
    can.SaveAs((graphnames[i]+".pdf").c_str());
  }
}
