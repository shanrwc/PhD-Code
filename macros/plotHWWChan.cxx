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

#include "HiggsAnalysis/HWWConstants.h"

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

TH1D* getAllHistos(TFile* file, std::string property, std::string channel, int phase)
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

  TH1D* histo = (TH1D*)file->Get((property + "_" + channel + "0_" + makeString(book[phase][0])).c_str());
  histo->Add((TH1D*)file->Get((property + "_" + channel + "1_" + makeString(book[phase][1])).c_str()));
  histo->Add((TH1D*)file->Get((property + "_" + channel + "2_" + makeString(book[phase][2])).c_str()));

  return(histo);
}

/////////////////////////////////////////////////////////////////////////////////////////

void plotHWWChan()
{
  gStyle->SetOptStat(0);

  const double lumin = 1.035;

//   const Color_t H130COLOR = kWhite;
//   const Color_t WWCOLOR = kAzure - 5;
//   const Color_t ZCOLOR = kRed + 1;
//   const Color_t WCOLOR = kSpring - 6;
//   const Color_t TOPCOLOR = kOrange - 4;
  const Color_t H130COLOR = kRed;
  const Color_t WWCOLOR = 4;
  const Color_t ZCOLOR = 210;
  const Color_t WCOLOR = 227;
  const Color_t TTCOLOR = 219;
  const Color_t STCOLOR = kOrange - 3;
  const Color_t EWCOLOR = 222;

  const Style_t DATAMARKER = 8;
  const Size_t DATASIZE = 1.5;
  const Width_t DATALINEWIDTH = 2.5;
  const Width_t MCLINEWIDTH = 2;

  const Float_t LEGTEXTSIZE = 0.04;

  const Float_t AXISTITLESIZE = 0.06;

//   TFile *HWW = new TFile("root-files/hww/h150_hwwstudy.root");
  TFile *DATA = new TFile("root-files/hww/data_hwwstudy.root");
  TFile *WW = new TFile("root-files/hww/smWW_hwwstudy.root");
  TFile* Z = new TFile("root-files/hww/smZ_hwwstudy.root");
  TFile* W = new TFile("root-files/hww/smW_hwwstudy.root");
  TFile* TTB = new TFile("root-files/hww/ttbar_hwwstudy.root");
  TFile* STOP = new TFile("root-files/hww/sTop_hwwstudy.root");
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
  property.push_back("njets");
//   property.push_back("met");
  property.push_back("mT");
  property.push_back("dphi");
  property.push_back("mll");
//   property.push_back("meteta");
//   property.push_back("metphi");
//   property.push_back("dpmetl");
//   property.push_back("dpmetj");

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
  xtitles.push_back("N_{jets}");
//   xtitles.push_back("E_{T,Rel}^{miss} [GeV]");
  xtitles.push_back("m_{T} [GeV]");
  xtitles.push_back("#Delta#phi_{ll}");
  xtitles.push_back("m_{ll} [GeV]");
  xtitles.push_back("#eta^{E_{T}^{miss}}");
  xtitles.push_back("#phi^{E_{T}^{miss}}");
  xtitles.push_back("#Delta#phi_{E_{T}^{miss},l}");
  xtitles.push_back("#Delta#phi_{E_{T}^{miss},j}");

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
  emtitles.push_back("N_{jets}");
//   emtitles.push_back("E_{T}^{miss} [GeV]");
  emtitles.push_back("m_{T} [GeV]");
  emtitles.push_back("#Delta#phi_{ll}");
  emtitles.push_back("m_{ll} [GeV]");
  emtitles.push_back("#eta^{E_{T}^{miss}}");
  emtitles.push_back("#phi^{E_{T}^{miss}}");
  emtitles.push_back("#Delta#phi_{E_{T}^{miss},l}");
  emtitles.push_back("#Delta#phi_{E_{T}^{miss},j}");

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
  ytitles.push_back("Events");
//   ytitles.push_back("Events/4 GeV");
  ytitles.push_back("Events/10 GeV");
  ytitles.push_back("Events");
  ytitles.push_back("Events/10 GeV");
  ytitles.push_back("Events");
  ytitles.push_back("Events");
  ytitles.push_back("Events");
  ytitles.push_back("Events");

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
  rebins.push_back(1);  
//   rebins.push_back(4);  
  rebins.push_back(10);  
  rebins.push_back(2);  
  rebins.push_back(10);
  rebins.push_back(1);
  rebins.push_back(1);
  rebins.push_back(1);
  rebins.push_back(1);

  std::vector<std::string> channels;
  channels.push_back("ee");
  channels.push_back("em");
  channels.push_back("mm");

  for (int nProp = 0; nProp != (int)property.size(); ++nProp)
  {
    for (int nChan = 0; nChan != (int)channels.size(); ++nChan)
    {
      for (int nPh = 0; nPh != 7; ++nPh)
      {
	if (nPh > 3) continue;
// 	if (nProp != 11) continue;
	if (nProp > 5 && nProp < 10) continue;

        std::string label = property[nProp] + "_" + channels[nChan] + "_" + makeString(nPh);
        TCanvas* pad = new TCanvas(label.c_str(),label.c_str(),14,33,600,600);
        pad->SetFillColor(0);
        pad->SetTitle(label.c_str());
// 	if (nProp == 11 && nPh == 1)
// 	{
// 	  pad->SetLogy(1);
// 	}

        TLegend *leg;
        leg = new TLegend(0.65,0.66,0.85,0.92);
        leg->SetTextSize(LEGTEXTSIZE);
        leg->SetFillColor(0);
        leg->SetBorderSize(0);

        TH1D* data = getAllHistos(DATA,property[nProp],channels[nChan],nPh);
        data->SetMarkerStyle(DATAMARKER);
        data->SetMarkerSize(DATASIZE);
        data->SetLineWidth(DATALINEWIDTH);
        data->SetTitle("");
        data->Rebin(rebins[nProp]);

//         TH1D* hww = getAllHistos(HWW,property[nProp],channels[nChan],nPh);
//         hww->SetTitle("");
//         hww->SetLineColor(H130COLOR);
//         hww->SetLineWidth(MCLINEWIDTH);
//         hww->Rebin(rebins[nProp]);

        TH1D* smww = getAllHistos(WW,property[nProp],channels[nChan],nPh);
        smww->SetTitle("");
        smww->SetFillColor(WWCOLOR);
        smww->SetLineWidth(MCLINEWIDTH);
        smww->Rebin(rebins[nProp]);

        TH1D* smw = getAllHistos(W,property[nProp],channels[nChan],nPh);
        smw->SetTitle("");
        smw->SetFillColor(WCOLOR);
        smw->SetLineWidth(MCLINEWIDTH);
        smw->Rebin(rebins[nProp]);

        TH1D* smz = getAllHistos(Z,property[nProp],channels[nChan],nPh);
        smz->SetTitle("");
        smz->SetFillColor(ZCOLOR);
        smz->SetLineWidth(MCLINEWIDTH);
        smz->Rebin(rebins[nProp]);

        TH1D* ttb = getAllHistos(TTB,property[nProp],channels[nChan],nPh);
        ttb->SetTitle("");
        ttb->SetFillColor(TTCOLOR);
        ttb->SetLineWidth(MCLINEWIDTH);
        ttb->Rebin(rebins[nProp]);

	TH1D* stop = getAllHistos(STOP,property[nProp],channels[nChan],nPh);
        stop->SetTitle("");  
	stop->SetFillColor(STCOLOR);
        stop->SetLineWidth(MCLINEWIDTH);
        stop->Rebin(rebins[nProp]);

	TH1D* ew = getAllHistos(EW,property[nProp],channels[nChan],nPh);
	ew->SetTitle("");
	ew->SetFillColor(EWCOLOR);
	ew->SetLineWidth(MCLINEWIDTH);
	ew->Rebin(rebins[nProp]);

// 	LUMINOSITY = data->Integral()/(smww->Integral() + smz->Integral();

	smww->Scale(LUMINOSITY);
	smw->Scale(LUMINOSITY);
	smz->Scale(LUMINOSITY);
	stop->Scale(LUMINOSITY);
	ttb->Scale(LUMINOSITY);
	ew->Scale(LUMINOSITY);

        THStack* stack = new THStack(label.c_str(),label.c_str());
        stack->SetTitle("");
        stack->Add(stop);
	stack->Add(ttb);
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
	  data->GetXaxis()->SetTitleSize(AXISTITLESIZE);
	  data->GetXaxis()->SetTitleOffset(0.92);
	  data->GetYaxis()->SetTitle(ytitles[nProp].c_str());
	  data->GetYaxis()->SetTitleSize(AXISTITLESIZE);
	  data->GetYaxis()->SetTitleOffset(0.99);
	  if (nProp > 5 && nProp < 10)
	  {
	    data->GetXaxis()->SetRangeUser(0,1);
	  }
// 	  if (nProp == 11)
// 	  {
// 	    data->GetXaxis()->SetRangeUser(0,140);
// 	  }
// 	  if (nProp == 11 && nPh == 1)
// 	  {
// 	    data->SetMinimum(0.1);
// 	  }
	  if (nProp > 1 && nProp < 6)
	  {
	    data->SetMaximum(1.4*max);
	  }
	  if (data->GetMaximum() > 1000)
	  {
	    pad->SetLeftMargin(0.2);
	    data->GetYaxis()->SetTitleOffset(1.8);
	  }
	  data->GetYaxis()->SetRangeUser(0.0001,1.15*(data->GetMaximum()));
	  stack->Draw("same");
// 	  hww->Draw("same");
	  data->Draw("P E1 X0 same");
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
	  stack->GetXaxis()->SetTitleSize(AXISTITLESIZE);
	  stack->GetXaxis()->SetTitleOffset(0.92);
	  stack->GetYaxis()->SetTitle(ytitles[nProp].c_str());
	  stack->GetYaxis()->SetTitleSize(AXISTITLESIZE);
	  stack->GetYaxis()->SetTitleOffset(0.99);
	  stack->Draw();
// 	  if (nProp == 11 && nPh == 1)
// 	  {
// 	    stack->SetMinimum(0.1);
// 	  }
// 	  else
// 	  {
	    stack->SetMinimum(0.0001);
// 	  }
	  if (nProp > 5 && nProp < 10)
	  {
	    stack->GetXaxis()->SetRangeUser(0,1);
	  }
// 	  if (nProp == 11)
// 	  {
// 	    stack->GetXaxis()->SetRangeUser(0,140);
// 	  }
	  if (nProp > 1 && nProp < 6)
	  {
	    stack->SetMaximum(1.4*max);
	  }
	  if (stack->GetMaximum() > 1000)
	  {
	    pad->SetLeftMargin(0.2);
	    stack->GetYaxis()->SetTitleOffset(1.8);
	  }
	  stack->GetYaxis()->SetRangeUser(stack->GetYaxis()->GetXmin(),1.2*(stack->GetYaxis()->GetXmax()));
	  data->Draw("P E1 X0 same");
// 	  hww->Draw("same");
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
//         leg->AddEntry(hww,"150 H#rightarrowWW","f");
        leg->AddEntry(smww,"WW#rightarrowl#nul#nu","f");
        leg->AddEntry(smz,"Z#rightarrowll","f");
        leg->AddEntry(smw,"W#rightarrowl#nu","f");
	leg->AddEntry(ew,"Diboson","f");
	leg->AddEntry(ttb,"TTbar","f");
        leg->AddEntry(stop,"Single Top","f");
        leg->Draw();

	TLatex* lumtex = new TLatex();
	lumtex->SetNDC();
	lumtex->SetTextSize(0.03);
	lumtex->SetTextColor(1);
	lumtex->DrawLatex(0.35,0.88,("#int Ldt = " + makeString(LUMINOSITY) + " fb^{-1}").c_str());

        std::string name = "macros/HWWplots/channels/" + label + ".eps";
        pad->Print(name.c_str());

      }//closes loop over phases
    }//closes loop over channels
  }//closes loop over properties

}//closes function
