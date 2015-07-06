#include "TCanvas.h"
#include "TFile.h"
#include "TH2.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TLatex.h"

#include <string>
#include <iostream>
#include <vector>

void makeWZEffs()
{
  std::vector<std::string> samples;
  samples.push_back("singleWenu");
  samples.push_back("singleZee");
  samples.push_back("singleWmnu");
  samples.push_back("singleZmm");
  samples.push_back("all");
  samples.push_back("166198JetTauEtmiss");

  for (int i = 0; i != (int)samples.size(); ++i)
  {
    std::string file;
    if (i == 5)
    {
      file = "root-files/SUSY/" + samples[i] + "_effs.root";
    }
    else
    {
      file = "root-files/SUSY/" + samples[i] + "_WZeffs.root";
    }
    TFile* f = new TFile(file.c_str());

    TH2D* fake_elec_b = (TH2D*)f->Get("elec_eff_l");
    TH2D* fake_elec_a = (TH2D*)f->Get("elec_eff_t");

    TH2D* fake_muon_b = (TH2D*)f->Get("mu_eff_l");
    TH2D* fake_muon_a = (TH2D*)f->Get("mu_eff_t");

    std::cout << "Sample " << samples[i] << " predicts . . . " << std::endl;
    std::cout << "fake electron efficiency: " << fake_elec_a->Integral()/fake_elec_b->Integral() << std::endl;
    std::cout << "fake muon efficiency: " << fake_muon_a->Integral()/fake_muon_b->Integral() << std::endl;
  }
}
