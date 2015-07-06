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

TH1D* getAllHistos(TFile* file, std::string property, int phase)
{
  //First index corresponds to phase; second to channel (ee/em/mm)
  int book[7][3];
  book[0][0] = 0;
  book[1][0] = 1;
  book[2][0] = 2;
  book[3][0] = 3;
  book[4][0] = 6;
  book[5][0] = 7;
  book[6][0] = 8;

  book[0][1] = 0;
  book[1][1] = 1;
  book[2][1] = 2;
  book[3][1] = 3;
  book[4][1] = 8;
  book[5][1] = 9;
  book[6][1] = 10;

  book[0][2] = 0;
  book[1][2] = 1;
  book[2][2] = 2;
  book[3][2] = 3;
  book[4][2] = 12;
  book[5][2] = 13;
  book[6][2] = 14;

  TH1D* histo = (TH1D*)file->Get((property + "_ee0_" + makeString(book[phase][0])).c_str());
  histo->Add((TH1D*)file->Get((property + "_ee1_" + makeString(book[phase][1])).c_str()));
  histo->Add((TH1D*)file->Get((property + "_ee2_" + makeString(book[phase][2])).c_str()));
  histo->Add((TH1D*)file->Get((property + "_em0_" + makeString(book[phase][0])).c_str()));
  histo->Add((TH1D*)file->Get((property + "_em1_" + makeString(book[phase][1])).c_str()));
  histo->Add((TH1D*)file->Get((property + "_em2_" + makeString(book[phase][2])).c_str()));
  histo->Add((TH1D*)file->Get((property + "_mm0_" + makeString(book[phase][0])).c_str()));
  histo->Add((TH1D*)file->Get((property + "_mm1_" + makeString(book[phase][1])).c_str()));
  histo->Add((TH1D*)file->Get((property + "_mm2_" + makeString(book[phase][2])).c_str()));

  return(histo);
}

////////////////////////////////////////////////////////////////////////////////////

void plotHWWSum()
{
  gStyle->SetOptStat(0);

//   const double lumin = 0.0129;

//   const Color_t H130COLOR = kWhite;
//   const Color_t WWCOLOR = kAzure - 5;
//   const Color_t ZCOLOR = kRed + 1;
//   const Color_t WCOLOR = kSpring - 6;
//   const Color_t TOPCOLOR = kOrange - 4;
  const Color_t H130COLOR = kRed;
  const Color_t WWCOLOR = 4;
  const Color_t ZCOLOR = 210;
  const Color_t WCOLOR = 227;
  const Color_t TOPCOLOR = 219;
  const Color_t WZCOLOR = 222;

  const Style_t DATAMARKER = 8;
  const Size_t DATASIZE = 1.5;
  const Width_t DATALINEWIDTH = 2.5;
  const Width_t MCLINEWIDTH = 2;

  const Float_t LEGTEXTSIZE = 0.04;

//   TFile *PRL = new TFile("root-files/hww/prldata_hwwstudy.root");
//   TFile *HWW = new TFile("root-files/hww/h130_hwwstudy.root");
  TFile *DATA = new TFile("root-files/hww/data_hwwstudy.root");
  TFile *WW = new TFile("root-files/hww/smWW_hwwstudy.root");
  TFile* Z = new TFile("root-files/hww/smZ_hwwstudy.root");
  TFile* W = new TFile("root-files/hww/smW_hwwstudy.root");
  TFile* TOP = new TFile("root-files/hww/top_hwwstudy.root");

  std::vector<std::string> property;
  property.push_back("njets");
  property.push_back("met");
  property.push_back("mT");
  property.push_back("dphi");
  property.push_back("mll");

  std::vector<std::string> xtitles;
  xtitles.push_back("N_{jets}");
  xtitles.push_back("E_{T}^{miss} [GeV]");
  xtitles.push_back("m_{T} [GeV]");
  xtitles.push_back("#Delta#phi_{ll}");
  xtitles.push_back("m_{ll} [GeV]");

  std::vector<std::string> ytitles;
  ytitles.push_back("Events");
  ytitles.push_back("Events/10 GeV");
  ytitles.push_back("Events/10 GeV");
  ytitles.push_back("Events");
  ytitles.push_back("Events");

  std::vector<int> rebins;
  rebins.push_back(1);
  rebins.push_back(10);
  rebins.push_back(10);
  rebins.push_back(5);
  rebins.push_back(5);

  for (int nProp = 0; nProp != (int)property.size(); ++nProp)
  {
    for (int nPh = 0; nPh != 7; ++nPh)
    {
      if (nPh != 0 && nPh != 3 && nPh < 5) continue;
      std::string label = property[nProp] + "_" + makeString(nPh);
      TCanvas* pad = new TCanvas(label.c_str(),label.c_str(),14,33,600,600);
      pad->SetFillColor(0);
      pad->SetTitle(label.c_str());

      TLegend *leg;
      leg = new TLegend(0.7,0.7,0.9,0.9);
      leg->SetTextSize(LEGTEXTSIZE);
      leg->SetFillColor(0);
      leg->SetBorderSize(0);

      TH1D* data = getAllHistos(DATA,property[nProp],nPh);
      data->SetMarkerStyle(DATAMARKER);
      data->SetMarkerSize(DATASIZE);
      data->SetLineWidth(DATALINEWIDTH);
      data->SetTitle("");
      data->Rebin(rebins[nProp]);

//       TH1D* prl = getAllHistos(PRL,property[nProp],nPh);
//       prl->SetMarkerStyle(DATAMARKER);
//       prl->SetMarkerSize(DATASIZE);
//       prl->SetLineWidth(DATALINEWIDTH);
//       prl->SetTitle("");
//       prl->Rebin(rebins[nProp]);

//       TH1D* hww = getAllHistos(HWW,property[nProp],nPh);
//       hww->Scale(lumin);
//       hww->SetTitle("");
//       hww->SetFillColor(H130COLOR);
//       hww->SetLineWidth(MCLINEWIDTH);
//       hww->Rebin(rebins[nProp]);

      TH1D* smww = getAllHistos(WW,property[nProp],nPh);
      smww->SetTitle("");
      smww->SetFillColor(WWCOLOR);
      smww->SetLineWidth(MCLINEWIDTH);
      smww->Rebin(rebins[nProp]);

      TH1D* smw = getAllHistos(W,property[nProp],nPh);
      smw->SetTitle("");
      smw->SetFillColor(WCOLOR);
      smw->SetLineWidth(MCLINEWIDTH);
      smw->Rebin(rebins[nProp]);

      TH1D* smz = getAllHistos(Z,property[nProp],nPh);
      smz->SetTitle("");
      smz->SetFillColor(ZCOLOR);
      smz->SetLineWidth(MCLINEWIDTH);
      smz->Rebin(rebins[nProp]);

      TH1D* top = getAllHistos(TOP,property[nProp],nPh);
      top->SetTitle("");
      top->SetFillColor(TOPCOLOR);
      top->SetLineWidth(MCLINEWIDTH);
      top->Rebin(rebins[nProp]);

      double lumin = data->Integral()/(smww->Integral() + smw->Integral() + smz->Integral() + top->Integral());
      smww->Scale(lumin);
      smw->Scale(lumin);
      smz->Scale(lumin);
      top->Scale(lumin);

      THStack* stack = new THStack(label.c_str(),label.c_str());
      stack->SetTitle("");
      stack->Add(top);
      stack->Add(smw);
      stack->Add(smz);
      stack->Add(smww);
//       stack->Add(hww);

      data->GetXaxis()->SetTitle(xtitles[nProp].c_str());
      data->GetYaxis()->SetTitle(ytitles[nProp].c_str());

      double max = data->GetBinContent(data->GetMaximumBin());
      if (1.15*max > stack->GetMaximum())
      {
	data->Draw("P E1 X0");
	data->SetMinimum(0.0001);
	if (nProp == 2)
	{
	  data->GetXaxis()->SetRangeUser(70,160);
	}
	stack->Draw("same");
	data->Draw("P E1 X0 same");
      }
      else
      {
	stack->Draw();
	stack->SetMinimum(0.0001);
	if (nProp == 2)
	{
	  stack->GetXaxis()->SetRangeUser(60,160);
	}
	data->Draw("P E1 X0 same");
      }

      leg->AddEntry(data,"2011 Data","p");
//       leg->AddEntry(hww,"130 H#rightarrowWW","f");
      leg->AddEntry(smww,"WW#rightarrowl#nul#nu","f");
      leg->AddEntry(smz,"Z#rightarrowll","f");
      leg->AddEntry(smw,"W#rightarrowl#nu","f");
      leg->AddEntry(top,"Top","f");
      leg->Draw();

      std::string name = "macros/HWWplots/summed/" + label + ".png";
      pad->Print(name.c_str());
//       name = "macros/prlPlots/cutflow/" + label + ".eps";
//       pad->Print(name.c_str());
    }
  }//closes loop over properties;
}
