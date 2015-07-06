#include <string>

#include "HiggsAnalysis/SusyAnalysis.h"

void calcWScale(std::string sample, bool debug = false)
{
  SusyAnalysis* analysis = new SusyAnalysis();
  if (debug)
  {
    analysis->SetDebug();
  }

  std::string filename = "root-files/SUSY/" + sample + "_scale.root";
  TFile* file = new TFile(TString(filename.c_str()),"RECREATE");

  const int n_hists = 20;
  TH1D* histos[n_hists];
  TH2D* histos2[n_hists];

  //Declare histos here
  histos[0] = new TH1D("all_met","all_met",250,0,250);
  histos[1] = new TH1D("e_met","e_met",250,0,250);
  histos[2] = new TH1D("m_met","m_met",250,0,250);
  histos[3] = new TH1D("all_mT","all_mT",150,0,150);
  histos[4] = new TH1D("e_mT","e_mT",150,0,150);
  histos[5] = new TH1D("m_mT","m_mT",150,0,150);
  histos[6] = new TH1D("all_pT","all_pT",150,0,150);
  histos[7] = new TH1D("e_pT","e_pT",150,0,150);
  histos[8] = new TH1D("m_pT","m_pT",150,0,150);
  histos[9] = new TH1D("all_dphi","all_dphi",70,0,3.5);
  histos[10] = new TH1D("e_dphi","e_dphi",70,0,3.5);
  histos[11] = new TH1D("m_dphi","m_dphi",70,0,3.5);
  histos[12] = new TH1D("all_njets","all_njets",10,-0.5,9.5);
  histos[13] = new TH1D("e_njets","e_njets",10,-0.5,9.5);
  histos[14] = new TH1D("m_njets","m_njets",10,-0.5,9.5);

  histos[16] = new TH1D("all_met_s","all_met_s",250,0,250);
  histos[17] = new TH1D("e_met_s","e_met_s",250,0,250);
  histos[18] = new TH1D("m_met_s","m_met_s",250,0,250);

  analysis->wjetsFactor(sample, histos, histos2);

  file->Write();
  file->Close();
}
