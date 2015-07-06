#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2.h"
#include "THStack.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TF1.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

void temp()
{
  TFile* f = new TFile("root-files/WireSag/geowiresag_wiresag.root");

  std::vector<int> lowends;
  lowends.push_back(-1);
  lowends.push_back(50);
  lowends.push_back(1000);

  std::vector<int> highends;
  highends.push_back(50);
  highends.push_back(3000);
  highends.push_back(80000);

  for (int k = 0; k != (int)highends.size(); ++k)
  {

    std::string label;
    if (k == 0)
    {
      label = "lowrange_pt";
    }
    else if (k == 1)
    {
      label = "midrange_pt";
    }
    else
    {
      label = "highrange_pt";
    }

    TCanvas* pad = new TCanvas(label.c_str(),label.c_str(),14,33,600,600);
    pad->SetFillColor(0);
    pad->SetTitle(label.c_str());
    pad->SetLeftMargin(0.15);
    if (k == 2)
    {
      pad->SetLogy(1);
    }

    TLegend* leg = new TLegend(0.65,0.7,0.85,0.9);
    leg->SetTextSize(0.04);
    leg->SetFillColor(0);
    leg->SetBorderSize(0);

    TH1D* reco = (TH1D*)f->Get("pt_muon");
    reco->SetTitle("");
    reco->SetLineWidth(3);
    reco->SetLineColor(kAzure + 3);
//     std::cout << "Reco muons without pT: " << reco->GetBinContent(0) << std::endl;
//     std::cout << "Reco muons with pT < 50 GeV: " << reco->Integral(1,5) << std::endl;
//     std::cout << "Total reco muons: " << reco->GetBinContent(0) + reco->Integral(1,5) << std::endl << std::endl;

    TH1D* comb = (TH1D*)f->Get("pt_comb");
    comb->SetTitle("");
    comb->SetLineWidth(3);
    comb->SetLineColor(kAzure);
//     std::cout << "Comb. tracks without pT: " << comb->GetBinContent(0) << std::endl;
//     std::cout << "Comb. tracks with pT < 50 GeV: " << comb->Integral(1,5) << std::endl;
//     std::cout << "Total comb. tracks: " << comb->GetBinContent(0) + comb->Integral(1,5) << std::endl << std::endl;

    TH1D* extra = (TH1D*)f->Get("pt_extra");
    extra->SetTitle("");
    extra->SetLineWidth(3);
    extra->SetLineColor(kAzure + 1);
//     std::cout << "Extra. tracks without pT: " << extra->GetBinContent(0) << std::endl;
//     std::cout << "Extra. tracks with pT < 50 GeV: " << extra->Integral(1,5) << std::endl;
//     std::cout << "Total extra. tracks: " << extra->GetBinContent(0) + extra->Integral(1,5) << std::endl << std::endl;

    TH1D* sa = (TH1D*)f->Get("pt_sa");
    sa->SetTitle("");
    sa->SetLineWidth(3);
    sa->SetLineColor(kAzure + 4);
//     std::cout << "SA tracks without pT: " << sa->GetBinContent(0) << std::endl;
//     std::cout << "SA tracks with pT < 50 GeV: " << sa->Integral(1,5) << std::endl;
//     std::cout << "Total SA tracks: " << sa->GetBinContent(0) + sa->Integral(1,5) << std::endl << std::endl;

    reco->GetXaxis()->SetRangeUser(lowends[k],highends[k]);
    comb->GetXaxis()->SetRangeUser(lowends[k],highends[k]);
    extra->GetXaxis()->SetRangeUser(lowends[k],highends[k]);
    sa->GetXaxis()->SetRangeUser(lowends[k],highends[k]);
    if (k == 2)
    {
      reco->Rebin(50);
      comb->Rebin(50);
      extra->Rebin(50);
      sa->Rebin(50);
    }

    std::vector<TH1D*> pointers;
    pointers.push_back(reco);
    pointers.push_back(comb);
    pointers.push_back(extra);
    pointers.push_back(sa);

    for (int i = 0; i != (int)pointers.size(); ++i)
    {
      for (int j = i + 1; j != (int)pointers.size(); ++j)
      {
        if (pointers[j]->GetBinContent(pointers[j]->GetMaximumBin()) > pointers[i]->GetBinContent(pointers[i]->GetMaximumBin()))
        {
	  swap(pointers[i],pointers[j]);
        }
      }
    }

    for (int i = 0; i != (int)pointers.size(); ++i)
    {
      if (i == 0)
      {
        pointers[i]->GetXaxis()->SetTitle("pT [GeV]");
        pointers[i]->GetYaxis()->SetTitle("Muons");
        pointers[i]->Draw();
        pointers[i]->GetXaxis()->SetRangeUser(lowends[k],highends[k]);
        pointers[i]->GetXaxis()->SetNdivisions(505);
      }
      else
      {
        pointers[i]->Draw("same");
      }
    } 

    leg->AddEntry(reco,"Muon Track","f");
    leg->AddEntry(comb,"ID Track","f");
    leg->AddEntry(extra, "Extra. Track","f");
    leg->AddEntry(sa, "SA Track","f");
    leg->Draw();

    std::string name = "macros/WireSagplots/" + label + ".png";
    pad->Print(name.c_str());
  }
}
