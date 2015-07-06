#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2.h"
#include "THStack.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TMath.h"
#include "TGraphAsymmErrors.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

////////////////////////////////////////////////////////////////////////////
///////////////////////////Helper Functions/////////////////////////////////
////////////////////////////////////////////////////////////////////////////

double poisson_up(double y) {
  if (y < 0.0) return 0.0;
  double y1 = y + 1;
  double d = 1.0 - 1.0/(9.0*y1) + 1.0/(3*TMath::Sqrt(y1));
  return (y1*d*d*d - y);
}

double poisson_down(double y) {
  if (y <= 0.0) return 0.0;
  double d = 1.0 - 1.0/(9.0*y) - 1.0/(3.0*TMath::Sqrt(y));
  return (y - y*d*d*d);
}

TGraphAsymmErrors* GetEfficiencyGraph(TH1D* num) {
  int nrBins = num->GetNbinsX();
  //double xmin = num->GetBinCenter(1);
  double width = num->GetBinWidth(1);
  double* x = new double[nrBins];
  double* y = new double[nrBins];
  double* exl = new double[nrBins];
  double* exh = new double[nrBins];
  double* eyl = new double[nrBins];
  double* eyh = new double[nrBins];
  for (int i = 0; i != nrBins; ++i) {
    double m = num->GetBinContent(i+1);
    x[i] = num->GetBinCenter(i+1);
    y[i] = m;//n != 0.0 ? (m / n) : 0.0;
    exl[i] = width/2.0;
    exh[i] = width/2.0;
    eyl[i] = poisson_down(m);

    eyh[i] = poisson_up(m);
    if(y[i] == 0)
      {
        eyl[i] = 0;
        eyh[i] = 0;
      }
  }
  TGraphAsymmErrors* T_data = new TGraphAsymmErrors(nrBins, x, y, exl, exh, eyl, eyh);
//   T_data->SetLineWidth(2);
//   T_data->SetMarkerStyle(20);
//   T_data->SetMarkerColor(kBlack);
  return T_data;
} 

template<typename T>
std::string makeString(T const& value)
{
  std::stringstream sstr;
  sstr << value;
  return sstr.str();
}

/////////////////////////////////////////////////////////////////////////////////////

void plotHWWZero()
{
  gStyle->SetOptStat(0);

  const double lumin = 0.689279;

  const Color_t H130COLOR = kRed;
  const Color_t WWCOLOR = 4;
  const Color_t ZCOLOR = 210;
  const Color_t WCOLOR = 227;
  const Color_t TOPCOLOR = 219;
  const Color_t EWCOLOR = 222;

  const Style_t DATAMARKER = 8;
  const Size_t DATASIZE = 1.5;
  const Width_t DATALINEWIDTH = 2.5;
  const Width_t MCLINEWIDTH = 2;

  const Float_t LEGTEXTSIZE = 0.04;

//   TFile *HWW = new TFile("root-files/hww/h130_hwwstudy.root");
  TFile *DATA = new TFile("root-files/hww/data_hwwstudy.root");
  TFile *WW = new TFile("root-files/hww/smWW_hwwstudy.root");
  TFile* Z = new TFile("root-files/hww/smZ_hwwstudy.root");
  TFile* W = new TFile("root-files/hww/smW_hwwstudy.root");
  TFile* TOP = new TFile("root-files/hww/Top_hwwstudy.root");
  TFile* EW = new TFile("root-files/hww/smEW_hwwstudy.root");

  std::vector<std::string> property;
  property.push_back("leadpt");
  property.push_back("subpt");
  property.push_back("leadeta");
  property.push_back("subeta");
  property.push_back("leadphi");
  property.push_back("subphi");
  property.push_back("letiso");
  property.push_back("setiso");
  property.push_back("lptiso");
  property.push_back("sptiso");
  property.push_back("met");
  property.push_back("mT");
  property.push_back("dphi");
  property.push_back("mll");

  std::vector<std::string> xtitles;
  xtitles.push_back("p_{T}^{lead}");
  xtitles.push_back("p_{T}^{sub}");
  xtitles.push_back("#eta^{lead}");
  xtitles.push_back("#eta^{sub}");
  xtitles.push_back("#phi^{lead}");
  xtitles.push_back("#phi^{sub}");
  xtitles.push_back("ETCone30/p_{T} (lead)");
  xtitles.push_back("ETCone30/p_{T} (sub)");
  xtitles.push_back("PTCone30/p_{T} (lead)");
  xtitles.push_back("PTCone30/p_{T} (sub)");
  xtitles.push_back("E_{T,Rel}^{miss} [GeV]");
  xtitles.push_back("m_{T} [GeV]");
  xtitles.push_back("#Delta#phi_{ll}");
  xtitles.push_back("m_{ll} [GeV]");

  std::vector<std::string> emtitles;
  emtitles.push_back("p_{T}^{#mu}");
  emtitles.push_back("p_{T}^{e}");
  emtitles.push_back("#eta^{#mu}");
  emtitles.push_back("#eta^{e}");
  emtitles.push_back("#phi^{#mu}");
  emtitles.push_back("#phi^{e}");
  emtitles.push_back("ETCone30/p_{T} (#mu)");
  emtitles.push_back("ETCone30/p_{T} (e)");
  emtitles.push_back("PTCone30/p_{T} (#mu)");
  emtitles.push_back("PTCone30/p_{T} (e)");
  emtitles.push_back("E_{T}^{miss} [GeV]");
  emtitles.push_back("m_{T} [GeV]");
  emtitles.push_back("#Delta#phi_{ll}");
  emtitles.push_back("m_{ll} [GeV]");

  std::vector<std::string> ytitles;
  ytitles.push_back("Events/4 GeV");
  ytitles.push_back("Events/4 GeV");
  ytitles.push_back("Events");
  ytitles.push_back("Events");
  ytitles.push_back("Events");
  ytitles.push_back("Events");
  ytitles.push_back("Events");
  ytitles.push_back("Events");
  ytitles.push_back("Events");
  ytitles.push_back("Events");
  ytitles.push_back("Events/4 GeV");
  ytitles.push_back("Events/10 GeV");
  ytitles.push_back("Events");
  ytitles.push_back("Events/10 GeV");

  std::vector<int> rebins;
  rebins.push_back(4);
  rebins.push_back(4);
  rebins.push_back(2);
  rebins.push_back(2);
  rebins.push_back(2);
  rebins.push_back(2); 
  rebins.push_back(5);   
  rebins.push_back(5);  
  rebins.push_back(5);  
  rebins.push_back(5);  
  rebins.push_back(4);  
  rebins.push_back(10);  
  rebins.push_back(2);  
  rebins.push_back(10);

  std::vector<std::string> channels;
  channels.push_back("ee");
  channels.push_back("em");
  channels.push_back("mm");

  for (int nProp = 0; nProp != (int)property.size(); ++nProp)
  {
    for (int nChan = 0; nChan != (int)channels.size(); ++nChan)
    {
      for (int nPh = 0; nPh != ; ++nPh)
      {
	std::string label = property[nProp] + "_" + channels[nChan] + "0_" + makeString(nPh);
 
        TCanvas* pad = new TCanvas(label.c_str(),label.c_str(),14,33,600,600);
        pad->SetFillColor(0);
        pad->SetTitle(label.c_str());

        TLegend *leg;
        leg = new TLegend(0.65,0.7,0.85,0.9);
        leg->SetTextSize(LEGTEXTSIZE);
        leg->SetFillColor(0);
        leg->SetBorderSize(0);

        TH1D* data = (TH1D*)DATA->Get(label.c_str());
        data->SetMarkerStyle(DATAMARKER);
        data->SetMarkerSize(DATASIZE);
        data->SetLineWidth(DATALINEWIDTH);
        data->SetTitle("");
        data->Rebin(rebins[nProp]);
	data->GetXaxis()->SetTitle(xtitles[nProp].c_str());
	data->GetYaxis()->SetTitle(ytitles[nProp].c_str());

//         TH1D* hww = getAllHistos(HWW,property[nProp],channels[nChan],nPh);
//         hww->SetTitle("");
//         hww->SetFillColor(H130COLOR);
//         hww->SetLineWidth(MCLINEWIDTH);
//         hww->Rebin(rebins[nProp]);

        TH1D* smww = (TH1D*)WW->Get(label.c_str());
        smww->SetTitle("");
        smww->SetFillColor(WWCOLOR);
        smww->SetLineWidth(MCLINEWIDTH);
        smww->Rebin(rebins[nProp]);

        TH1D* smw = (TH1D*)W->Get(label.c_str());
        smw->SetTitle("");
        smw->SetFillColor(WCOLOR);
        smw->SetLineWidth(MCLINEWIDTH);
        smw->Rebin(rebins[nProp]);

        TH1D* smz = (TH1D*)Z->Get(label.c_str());
        smz->SetTitle("");
        smz->SetFillColor(ZCOLOR);
        smz->SetLineWidth(MCLINEWIDTH);
        smz->Rebin(rebins[nProp]);

        TH1D* top = (TH1D*)TOP->Get(label.c_str());
        top->SetTitle("");
        top->SetFillColor(TOPCOLOR);
        top->SetLineWidth(MCLINEWIDTH);
        top->Rebin(rebins[nProp]);

	TH1D* ew = (TH1D*)EW->Get(label.c_str());
	ew->SetTitle("");
	ew->SetFillColor(EWCOLOR);
	ew->SetLineWidth(MCLINEWIDTH);
	ew->Rebin(rebins[nProp]);

// 	lumin = data->Integral()/(smww->Integral() + smz->Integral();

	smww->Scale(lumin);
	smw->Scale(lumin);
	smz->Scale(lumin);
	top->Scale(lumin);
	ew->Scale(lumin);

        THStack* stack = new THStack(label.c_str(),label.c_str());
        stack->SetTitle("");
        stack->Add(top);
	stack->Add(ew);
        stack->Add(smw);
        stack->Add(smz);
        stack->Add(smww);
//         stack->Add(hww);

        double max = data->GetBinContent(data->GetMaximumBin());
        if (max + sqrt(max) > stack->GetMaximum())
        {
	  data->Draw("P E1 X0");
	  data->SetMinimum(0.0001);
// 	  if ((nProp > 1 && nProp < 6) || nProp == 13)
// 	  {
// 	    data->SetMaximum(1.4*max);
// 	  }
	  if (nChan == 1)
	  {
	    data->GetXaxis()->SetTitle(emtitles[nProp].c_str());
	  }
	  else
	  {
	    data->GetXaxis()->SetTitle(xtitles[nProp].c_str());
	  }
	  if (nProp > 5 && nProp < 10)
	  {
	    data->GetXaxis()->SetRangeUser(0,1);
	  }
	  if (nProp == 10)
	  {
	    data->GetXaxis()->SetRangeUser(0,140);
	  }
	  if (nProp == 10 && nPh == 1)
	  {
	    data->SetMinimum(0.1);
	  }
	  stack->Draw("same");
	  data->Draw("P E1 X0 sameaxis");
        }
        else
        {
	  stack->Draw();
	  if (nChan == 1)
	  {
	    stack->GetXaxis()->SetTitle(emtitles[nProp].c_str());
	  }
	  else
	  {
	    stack->GetXaxis()->SetTitle(xtitles[nProp].c_str());
	  }
	  stack->Draw();
	  if (nProp == 10 && nPh == 1)
	  {
	    stack->SetMinimum(0.1);
	  }
	  else
	  {
	    stack->SetMinimum(0.0001);
	  }
// 	  if ((nProp > 1 && nProp < 6) || nProp == 13)
// 	  {
// 	    stack->SetMaximum(1.4*max);
// 	  }
	  if (nProp > 5 && nProp < 10)
	  {
	    stack->GetXaxis()->SetRangeUser(0,1);
	  }
	  if (nProp == 10)
	  {
	    stack->GetXaxis()->SetRangeUser(0,140);
	  }
	  data->Draw("P E1 X0 same");
        }
	std::string dataTag;
	if (nChan == 0)
	{
	  dataTag = "2011 ee Data";
	}
        else if (nChan == 1)
	{
	  dataTag = "2011 e#mu Data";
	}
	else
	{
	  dataTag = "2011 #mu#mu Data";
	}
        leg->AddEntry(data,dataTag.c_str(),"p");
//         leg->AddEntry(hww,"130 H#rightarrowWW","f");
        leg->AddEntry(smww,"WW#rightarrowl#nul#nu","f");
        leg->AddEntry(smz,"Z#rightarrowll","f");
        leg->AddEntry(smw,"W#rightarrowl#nu","f");
	leg->AddEntry(ew,"Diboson","f");
        leg->AddEntry(top,"Top","f");
        leg->Draw();

        std::string name = "macros/HWWplots/channels/" + label + ".eps";
        pad->Print(name.c_str());
      }
    }
  }

}//closes main function
