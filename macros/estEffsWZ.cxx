#include <string>

#include "HiggsAnalysis/SusyAnalysis.h"

void estEffsWZ(std::string sample = "singleZmm1", bool debug = false)
{
  SusyAnalysis* analysis = new SusyAnalysis();
  if (debug)
  {
    analysis->SetDebug();
  }

  std::string filename = "root-files/SUSY/" + sample + "_WZeff.root";
  TFile* file = new TFile(TString(filename.c_str()),"RECREATE");

  const int n_hists = 10;
  TH1D* histos[n_hists];
  TH2D* histos2[n_hists];

  //Declare histos here

  histos[0] = new TH1D("m_ETCone20","m_ETCone20",100,0,100);
  histos[1] = new TH1D("m_ETCone40","m_ETCone40",100,0,100);
  histos[2] = new TH1D("m_ETCone20pT","m_ETCone20pT",50,0,1);
  histos[3] = new TH1D("m_ETCone40pT","m_ETCone40pT",50,0,1);

  histos[5] = new TH1D("e_ETCone20","e_ETCone20",100,0,100);
  histos[6] = new TH1D("e_ETCone40","e_ETCone40",100,0,100);
  histos[7] = new TH1D("e_ETCone20pT","e_ETCone20pT",50,0,1);
  histos[8] = new TH1D("e_ETCone40pT","e_ETCone40pT",50,0,1);

  histos2[0] = new TH2D("elec_eff_l","elec_eff_l",14,10,80,4,-3,3);
  histos2[0]->Sumw2();
  histos2[1] = new TH2D("elec_eff_t","elec_eff_t",14,10,80,4,-3,3);
  histos2[1]->Sumw2();
  histos2[2] = new TH2D("mu_eff_l","mu_eff_l",14,10,80,6,-3.3,3.3);
  histos2[2]->Sumw2();
  histos2[3] = new TH2D("mu_eff_t","mu_eff_t",14,10,80,6,-3.3,3.3);
  histos2[3]->Sumw2();

  analysis->EffEstWZ(sample, histos, histos2);

  file->Write();
  file->Close();
}
