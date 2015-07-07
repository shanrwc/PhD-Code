#ifndef __WIRESAGBASE_H
#define __WIRESAGBASE_H

#include "TObject.h"
#include "TCut.h"
#include "TString.h"
#include "TGraphAsymmErrors.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TEventList.h"
#include "TChain.h"
#include "TSystem.h"
#include "TBranch.h"
#include "TLeaf.h"
#include "TMath.h"
#include "TLorentzVector.h"

#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <iosfwd>
#include <streambuf>
#include <istream>
#include <string>
#include <set>
#include <cstdlib>
#include <cmath>

#include "RecordClass.h"
#include "EndCapMap.h"

class WireSagBase : public TObject {
 public:
  WireSagBase();
  ~WireSagBase();

  TChain* GetChain(std::string chainName);
  TChain* GetChain(std::vector<std::string> chainNames);
  void setBranchAddresses(TChain* tree);

  std::vector<std::string> GetKnownChains();
  std::vector<std::string> GetAllSamples(std::string sample);

  //
  // Helper functions for histograms
  //
  void includeOverflow(TH1D* hist);
  void includeUnderflow(TH1D* hist);
  void removeNegativeBins(TH1D* hist);
  double efficiency_symm(double m, double n);
  double eff_pos_err(double m, double n);
  double eff_neg_err(double m, double n);   
  double poisson_up(double y);
  double poisson_down(double y);
  TGraphAsymmErrors* GetEfficiencyGraph(TH1D* num, TH1D* den);

  void SetDebug() {_debug = true;}
  ClassDef(WireSagBase,0);

 protected:
  bool _debug;

  //Put vectors to receive variables from tree here!
  int run_number;
  int event_number;

  int reco_muon_n;
  std::vector<double>* reco_muon_pt;
  std::vector<double>* reco_muon_eta;
  std::vector<double>* reco_muon_phi;
  std::vector<double>* reco_muon_mass;
  std::vector<int>* reco_muon_author;
  std::vector<bool>* reco_muon_iscomb;
  std::vector<int>* reco_muon_charge;
  std::vector<double>* reco_muon_fitchi2;
  std::vector<double>* reco_muon_fitchi2dof;
  std::vector<double>* reco_muon_matchchi2;
  std::vector<double>* reco_muon_matchchi2dof;

  int truth_muon_n;
  std::vector<double>* truth_muon_pt;
  std::vector<double>* truth_muon_eta;
  std::vector<double>* truth_muon_phi;
  std::vector<double>* truth_muon_mass;
  std::vector<int>* truth_muon_charge;

  int track_stand_n;
  std::vector<double>* track_stand_pt;
  std::vector<double>* track_stand_eta;
  std::vector<double>* track_stand_phi;
  std::vector<int>* track_stand_charge;
  std::vector<double>* track_stand_ndof;
  std::vector<double>* track_stand_chi2;
  std::vector<double>* track_stand_d0;
  std::vector<double>* track_stand_d0err;
  std::vector<double>* track_stand_z0;
  std::vector<double>* track_stand_z0err;
  std::vector<double>* track_stand_vd0;
  std::vector<double>* track_stand_vd0err;
  std::vector<double>* track_stand_vz0;
  std::vector<double>* track_stand_vz0err;
  std::vector<double>* track_stand_qoverp;
  std::vector<double>* track_stand_qperr;

  int track_extra_n;
  std::vector<double>* track_extra_pt;
  std::vector<double>* track_extra_eta;
  std::vector<double>* track_extra_phi;
  std::vector<int>* track_extra_charge;
  std::vector<double>* track_extra_ndof;
  std::vector<double>* track_extra_chi2;
  std::vector<double>* track_extra_d0;
  std::vector<double>* track_extra_d0err;
  std::vector<double>* track_extra_z0;
  std::vector<double>* track_extra_z0err;
  std::vector<double>* track_extra_vd0;
  std::vector<double>* track_extra_vd0err;
  std::vector<double>* track_extra_vz0;
  std::vector<double>* track_extra_vz0err;
  std::vector<double>* track_extra_qoverp;
  std::vector<double>* track_extra_qperr;

  int track_comb_n;
  std::vector<double>* track_comb_pt;
  std::vector<double>* track_comb_eta;
  std::vector<double>* track_comb_phi;
  std::vector<int>* track_comb_charge;
  std::vector<double>* track_comb_ndof;
  std::vector<double>* track_comb_chi2;
  std::vector<double>* track_comb_d0;
  std::vector<double>* track_comb_d0err;
  std::vector<double>* track_comb_z0;
  std::vector<double>* track_comb_z0err;
  std::vector<double>* track_comb_vd0;
  std::vector<double>* track_comb_vd0err;
  std::vector<double>* track_comb_vz0;
  std::vector<double>* track_comb_vz0err;
  std::vector<double>* track_comb_qoverp;
  std::vector<double>* track_comb_qperr;
  std::vector<int>* track_comb_nbhits;
  std::vector<int>* track_comb_npixhits;
  std::vector<int>* track_comb_nscthits;
  std::vector<int>* track_comb_ntrthits;

 private:

  void AddFile(std::string chainName, std::string dataset, std::string fileName);
  void AddAllFiles();

  std::map<std::string, std::vector<std::string> > _fileNameMap;

};

#endif
