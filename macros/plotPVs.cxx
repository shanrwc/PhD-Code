#include "TFile.h"
#include "TCanvas.h"
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


void plotPVs()
{
  TFile *DATA = new TFile("root-files/hww/data_hwwstudy.root");
  TFile* Z = new TFile("root-files/hww/smZ_hwwstudy.root");
//   TFile* shZ = new TFile("root-files/hww/shellZ_hwwstudy.root");
//   TFile* dyZ = new TFile("root-files/hww/dyZ_hwwstudy.root");

  //Get histograms and normalize them to one
  TH1D* data = (TH1D*)DATA->Get("mu_PV");

  data->SetTitle("");
//   data->Scale(1/data->Integral());
  data->SetMarkerStyle(8);
  data->SetMarkerSize(1.5);
  data->SetLineWidth(2.5);

  TH1D* mc = (TH1D*)Z->Get("mu_PV");
  mc->Scale(data->Integral()/mc->Integral());
  mc->SetTitle("");
  mc->SetFillColor(210);

//   TH1D* shell = (TH1D*)shZ->Get("pvplot");
// //   shell->Scale(1/shell->Integral());
//   shell->SetTitle("");
//   shell->SetLineColor(kRed + 1);

//   TH1D* dy = (TH1D*)dyZ->Get("pvplot");
// //   dy->Scale(1/dy->Integral());
//   dy->SetTitle("");
//   dy->SetLineColor(kSpring - 6);

  TCanvas* pad = new TCanvas("muplots1","muplots1",14,33,600,600);
  pad->SetFillColor(0);
  pad->SetTitle("PVplots");

  TLegend *leg = new TLegend(0.7,0.7,0.9,0.85);
  leg->SetTextSize(0.04);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);

//   dy->Draw();
//   shell->Draw("same");
  mc->Draw("");
  mc->GetXaxis()->SetTitle("#mu");
//   data->Draw("p e1 x0 same");

  leg->AddEntry(data,"2011 Data","p");
  leg->AddEntry(mc,"MC Z#rightarrowll","f");
  leg->Draw();

  std::string name = "macros/HWWplots/PVplots.png";
  pad->Print(name.c_str());
}
