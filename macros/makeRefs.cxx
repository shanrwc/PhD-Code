#include "TFile.h"
#include "TH2.h"

#include <string>
#include <iostream>

void makeRefs()
{
  //Get histograms!
  TFile* a = new TFile("root-files/SUSY/susy/Zee_susy.root");
  TFile* b = new TFile("root-files/SUSY/susy/Zmm_susy.root");
  TFile* d = new TFile("root-files/SUSY/Effs/166198JetTauEtmiss_effs.root");
  TFile* g = new TFile("root-files/SUSY/susy/data_susy.root");
  TFile* h = new TFile("root-files/SUSY/susy/ZeeNp2_susy.root");
  TFile* l = new TFile("root-files/SUSY/susy/ZmmNp2_susy.root");
  TFile* t = new TFile("root-files/SUSY/susy/tt_susy.root");

  TH2D* true_elec_b = (TH2D*)t->Get("elec_eff_l");
  TH2D* true_elec_a = (TH2D*)t->Get("elec_eff_t");

  TH2D* true_muon_b = (TH2D*)t->Get("mu_eff_l");
  TH2D* true_muon_a = (TH2D*)t->Get("mu_eff_t");

  TH2D* fake_elec_b = (TH2D*)d->Get("elec_eff_l");
  TH2D* fake_elec_a = (TH2D*)d->Get("elec_eff_t");

  TH2D* fake_muon_b = (TH2D*)d->Get("mu_eff_l");
  TH2D* fake_muon_a = (TH2D*)d->Get("mu_eff_t");

  //Open TFile to put the references in!
  TFile* outfile = new TFile("MMInput.root","recreate");

  //Do the division!
  TH2D* true_elec_eff = (TH2D*)true_elec_a->Clone("true_elec_eff");
  true_elec_eff->Divide(true_elec_b);
  TH2D* fake_elec_eff = (TH2D*)fake_elec_a->Clone("fake_elec_eff");
  fake_elec_eff->Divide(fake_elec_b);

  //Do fancy division and binning for muons!
  TH2D* true_muon_eff = (TH2D*)true_muon_a->Clone("true_muon_eff");
  true_muon_eff->Divide(true_muon_b);
  int xbins = true_muon_eff->GetNbinsX();
  for (int i = 1; i <= xbins; ++i)
  {
    double loweff = (true_muon_a->GetBinContent(true_muon_a->GetBin(i,1)) + true_muon_a->GetBinContent(true_muon_a->GetBin(i,2)))/(true_muon_b->GetBinContent(true_muon_b->GetBin(i,1)) + true_muon_b->GetBinContent(true_muon_b->GetBin(i,2)));
    true_muon_eff->SetBinContent(i,1,loweff);
    true_muon_eff->SetBinContent(i,2,loweff);
    double higheff = (true_muon_a->GetBinContent(true_muon_a->GetBin(i,5)) + true_muon_a->GetBinContent(true_muon_a->GetBin(i,6)))/(true_muon_b->GetBinContent(true_muon_b->GetBin(i,5)) + true_muon_b->GetBinContent(true_muon_b->GetBin(i,6)));
    true_muon_eff->SetBinContent(i,5,higheff);
    true_muon_eff->SetBinContent(i,6,higheff);
  }

  TH2D* fake_muon_eff = (TH2D*)fake_muon_a->Clone("fake_muon_eff");
  fake_muon_eff->Divide(fake_muon_b);
  xbins = fake_muon_eff->GetNbinsX();
  for (int i = 1; i <= xbins; ++i)
  {
    double loweff = (fake_muon_a->GetBinContent(fake_muon_a->GetBin(i,1)) + fake_muon_a->GetBinContent(fake_muon_a->GetBin(i,2)))/(fake_muon_b->GetBinContent(fake_muon_b->GetBin(i,1)) + fake_muon_b->GetBinContent(fake_muon_b->GetBin(i,2)));
    fake_muon_eff->SetBinContent(i,1,loweff);
    fake_muon_eff->SetBinContent(i,2,loweff);
    double higheff = (fake_muon_a->GetBinContent(fake_muon_a->GetBin(i,5)) + fake_muon_a->GetBinContent(fake_muon_a->GetBin(i,6)))/(fake_muon_b->GetBinContent(fake_muon_b->GetBin(i,5)) + fake_muon_b->GetBinContent(fake_muon_b->GetBin(i,6)));
    fake_muon_eff->SetBinContent(i,5,higheff);
    fake_muon_eff->SetBinContent(i,6,higheff);
  }

  //Calculate some average efficiencies!
  std::cout << "Average true electron efficiency: " << (true_elec_a->Integral()/true_elec_b->Integral()) << std::endl;
  std::cout << "Average fake electron efficiency: " << (fake_elec_a->Integral()/fake_elec_b->Integral()) << std::endl;
  std::cout << "Average true muon efficiency: " << (true_muon_a->Integral()/true_muon_b->Integral()) << std::endl;
  std::cout << "Average fake muon efficiency: " << (fake_muon_a->Integral()/fake_muon_b->Integral()) << std::endl;

  //End the madness with the exclamation points!  Now!!!!!!
  outfile->Write();
  outfile->Close();
}
