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
#include "TLine.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

void plotHWWIsos(bool logScale = true)
{
  gStyle->SetOptStat(0);

  const double lumin = 0.0129;

//   const Color_t H130COLOR = kWhite;
//   const Color_t WWCOLOR = kAzure - 5;
//   const Color_t ZCOLOR = kRed + 1;
//   const Color_t WCOLOR = kSpring - 6;
//   const Color_t TOPCOLOR = kOrange - 4;

//   const Color_t H130COLOR = kRed;
//   const Color_t WWCOLOR = 4;
  const Color_t ZCOLOR = 210;
//   const Color_t WCOLOR = 227;
//   const Color_t TOPCOLOR = 219;
//   const Color_t WZCOLOR = 222;

  const Style_t DATAMARKER = 8;
  const Size_t DATASIZE = 1.5;
  const Width_t DATALINEWIDTH = 2.5;
  const Width_t MCLINEWIDTH = 2;

  const Float_t LEGTEXTSIZE = 0.04;

  TFile *DATA = new TFile("root-files/hww/data_hwwstudy.root");
//   TFile *WW = new TFile("root-files/hww/smWW_hwwstudy.root");
  TFile* Z = new TFile("root-files/hww/smZ_hwwstudy.root");
//   TFile* W = new TFile("root-files/hww/smW_hwwstudy.root");
//   TFile* TOP = new TFile("root-files/hww/top_hwwstudy.root");

  std::vector<std::string> particles;
  particles.push_back("e");
  particles.push_back("m");

  std::vector<std::string> variable;
  variable.push_back("et");
  variable.push_back("pt");

  std::vector<std::string> cone;
  cone.push_back("20");
  cone.push_back("30");
  cone.push_back("40");

  std::vector<std::string> phase;
  phase.push_back("0");
  phase.push_back("1");

  std::vector<int> rebins;
  rebins.push_back(4);
  rebins.push_back(4);
  rebins.push_back(4);
  rebins.push_back(4);
  rebins.push_back(4);
  rebins.push_back(4);

  for (int nPr = 0; nPr != (int)particles.size(); ++nPr)
  {
    for (int nVar = 0; nVar != (int)variable.size(); ++nVar)
    {
      for (int nC = 0; nC != (int)cone.size(); ++nC)
      {
// 	for (int nPh = 0; nPh != (int)phase.size(); ++nPh)
// 	{
	  if (nPr == 0 && nVar == 1 && (nC == 0 || nC == 2)) continue;
// 	  if (nPr > 0 || nVar > 0 || nC > 0) continue;

          std::string label = particles[nPr] + "_" + variable[nVar] + cone[nC];
          TCanvas* pad = new TCanvas(label.c_str(),label.c_str(),14,33,600,600);
          pad->SetFillColor(0);
          pad->SetTitle(label.c_str());
	  if (logScale)
	  {
	    pad->SetLogy();
	  }

          TLegend *leg;
          leg = new TLegend(0.7,0.75,0.9,0.88);
          leg->SetTextSize(LEGTEXTSIZE);
          leg->SetFillColor(0);
          leg->SetBorderSize(0);

          TH1D* data = (TH1D*)DATA->Get(label.c_str());
          data->SetMarkerStyle(DATAMARKER);
          data->SetMarkerSize(DATASIZE);
          data->SetLineWidth(DATALINEWIDTH);
          data->SetTitle("");
          data->Rebin(rebins[nC + 3*nVar]);

	  std::string vars;
	  if (nVar == 0)
	  {
	    vars = "ET";
	  }
	  else
	  {
	    vars = "PT";
	  }
	  std::string xtitle = vars + "Cone" + cone[nC] + "/Pt";
	  data->GetXaxis()->SetTitle(xtitle.c_str());
	  if (nPr == 0)
	  {
	    data->GetYaxis()->SetTitle("Electrons");
	  }
	  else
	  {
	    data->GetYaxis()->SetTitle("Muons");
	  }

//           TH1D* smww = (TH1D*)WW->Get(label.c_str());
//           smww->SetTitle("");
//           smww->SetFillColor(WWCOLOR);
//           smww->SetLineWidth(MCLINEWIDTH);
//           smww->Rebin(rebins[nC + 3*nVar]);

//           TH1D* smw = (TH1D*)W->Get(label.c_str());
//           smw->SetTitle("");
//           smw->SetFillColor(WCOLOR);
//           smw->SetLineWidth(MCLINEWIDTH);
//           smw->Rebin(rebins[nC + 3*nVar]);

          TH1D* smz = (TH1D*)Z->Get(label.c_str());
          smz->SetTitle("");
          smz->SetFillColor(ZCOLOR);
          smz->SetLineWidth(MCLINEWIDTH);
          smz->Rebin(rebins[nC + 3*nVar]);
  
//           TH1D* top = (TH1D*)TOP->Get(label.c_str());
//           top->SetTitle("");
//           top->SetFillColor(TOPCOLOR);
//           top->SetLineWidth(MCLINEWIDTH);
//           top->Rebin(rebins[nC + 3*nVar]);

	  double totData = data->Integral();
	  double totMC = smz->Integral();
// 	  smww->Scale(totData/totMC);
// 	  smw->Scale(totData/totMC);
	  smz->Scale(totData/totMC);
// 	  top->Scale(totData/totMC);

          THStack* stack = new THStack(label.c_str(),label.c_str());
          stack->SetTitle("");
//           stack->Add(top);
//           stack->Add(smw);
          stack->Add(smz);
//           stack->Add(smww);

          double max = data->GetBinContent(data->GetMaximumBin());
	  double height = 0;
          if (1.15*max > stack->GetMaximum())
          {
	    data->Draw("P E1 X0");
	    data->GetXaxis()->SetTitle(xtitle.c_str());
	    if (nPr == 0)
	    {
	      data->GetYaxis()->SetTitle("Electrons");
	    }
	    else
	    {
	      data->GetYaxis()->SetTitle("Muons");
	    }
	    if (!logScale)
	    {
	      data->SetMinimum(0.0001);
	    }
	    stack->Draw("same");
	    data->Draw("P E1 X0 same");
	    gPad->RedrawAxis();
	    height = data->GetMaximum();
          }
          else
          {
	    stack->Draw();
	    stack->GetXaxis()->SetTitle(xtitle.c_str());
	    if (nPr == 0)
	    {
	      stack->GetYaxis()->SetTitle("Electrons");
	    }
	    else
	    {
	      stack->GetYaxis()->SetTitle("Muons");
	    }
	    if (!logScale)
	    {
	      stack->SetMinimum(0.0001);
	    }
	    data->Draw("P E1 X0 same");
	    height = stack->GetMaximum();
          }

          leg->AddEntry(data,"2011 Data","p");
//       leg->AddEntry(hww,"130 H#rightarrowWW","f");
//           leg->AddEntry(smww,"WW#rightarrowl#nul#nu","f");
          leg->AddEntry(smz,"Z#rightarrowll","f");
//           leg->AddEntry(smw,"W#rightarrowl#nu","f");
//           leg->AddEntry(top,"Top","f");
          leg->Draw();


	  TLine* line;
	  if (nVar == 0)
	  {
	    //ETCone30/pT < 0.15
	    line = new TLine(0.15,0,0.15,0.5*height);
	  }
          else
	  {
	    //PTCone30/pT < 0.1
	    line = new TLine(0.1,0,0.1,0.5*height);
	  }
	  line->SetLineColor(kRed + 1);
	  line->SetLineWidth(3);
	  if (nC == 1)
	  {
	    line->Draw();
	  }

          std::string name;
          if (logScale)
          {
 	    name = "macros/HWWplots/" + label + "_log.png";
          }
          else
          {
	    name = "macros/HWWplots/" + label + ".png";
          }
          pad->Print(name.c_str());
// 	}
      }
    }
  }
}//closes function body
