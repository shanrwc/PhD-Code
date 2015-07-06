#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <iostream>

#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "THStack.h"
#include "TLegend.h"

void mmbyhand()
{
  //Step Zero: Define necessary constants, set up canvas and legend.
  double sigE = 0.608;
  double backE = 0.049;

  const Style_t DATAMARKER = 8;
  const Size_t DATASIZE = 1.5;
  const Width_t DATALINEWIDTH = 2.5;

  std::string label = "mm_met_byhand_low";
  TCanvas* pad = new TCanvas(label.c_str(),label.c_str(),14,33,600,600);
  pad->SetFillColor(0);
  pad->SetTitle(label.c_str());

  TLegend* leg = new TLegend(0.64,0.68,0.88,0.88);
  leg->SetTextSize(0.04);
  leg->SetFillColor(0);


  //Step One: Read in and add up loose and tight sample histos.  Do any rebinning.
  TFile* f = new TFile("root-files/SUSY/susy/data_susy.root");

  TH1D* loose = (TH1D*)f->Get("met_ee_os_2");
  loose->Add((TH1D*)f->Get("met_em_os_2"));
  loose->Add((TH1D*)f->Get("met_me_os_2"));
  loose->Add((TH1D*)f->Get("met_mm_os_2"));
  loose->Rebin(10);

  TH1D* tight = (TH1D*)f->Get("met_ee_os_3");
  tight->Add((TH1D*)f->Get("met_em_os_3"));
  tight->Add((TH1D*)f->Get("met_me_os_3"));
  tight->Add((TH1D*)f->Get("met_mm_os_3"));
  tight->Rebin(10);


  //Step Two: Clone histos, loop over them and fill the signal and background histos.

  TH1D* signal = (TH1D*)loose->Clone();
  TH1D* back = (TH1D*)loose->Clone();

  int nbins = loose->GetNbinsX();
  for (int i = 0; i != nbins; ++i)
  {
    int nL = loose->GetBinContent(i + 1);
    int nT = tight->GetBinContent(i + 1);

    double estS = (nT - nL*backE)/(sigE - backE);
    double estB = (nL*sigE - nT)/(sigE - backE);

    if (estS < 0)
    {
      estB = estB + estS;
      estS = 0;
    }
    if (estB < 0)
    {
      estS = estS + estB;
      estB = 0;
    }

    signal->SetBinContent(i + 1, estS);
    back->SetBinContent(i + 1, estB);
  }

  //Step Three:  Prettify, stack, draw, and print.
  loose->SetMarkerStyle(DATAMARKER);
  loose->SetMarkerSize(DATASIZE);
  loose->SetLineWidth(DATALINEWIDTH);
  loose->SetTitle("");
  loose->GetXaxis()->SetTitle("E_{T}^{miss} [GeV]");
  loose->GetYaxis()->SetTitle("Events/20 GeV");
  loose->GetXaxis()->SetRangeUser(20,200);

  signal->SetTitle("");
  signal->SetFillColor(kAzure - 8);
  signal->SetLineWidth(2);

  back->SetTitle("");
  back->SetFillColor(kRed - 7);
  back->SetLineWidth(2);

  THStack* stack = new THStack(label.c_str(),label.c_str());
  stack->Add(back);
  stack->Add(signal);

  loose->Draw("P E1 X0");
  stack->Draw("same");
  loose->Draw("P E1 X0 same");

  leg->AddEntry(loose, "Data","p");
  leg->AddEntry(signal, "Est. Signal", "f");
  leg->AddEntry(back, "Est. Fakes", "f");
  leg->Draw();

  pad->Print(("macros/SUSYplots/MMResults/" + label + ".png").c_str());
  pad->Print(("macros/SUSYplots/MMResults/" + label + ".eps").c_str());
}
