#ifndef __DATAANALYSIS_H
#define __DATAANALYSIS_H

#include "BaseAnalysis.h"

class DataAnalysis : public BaseAnalysis
{

 public:
  DataAnalysis();
  ~DataAnalysis();

  ClassDef(DataAnalysis,0);

  //Used to chop down datasets so they don't take forever to run.
  void skimEvents(std::string sample);
  void skimEvents2(std::string sample);

  //Main Analysis Functions
  void TriggerSelection(std::string sample, TH1D** histos, TH2D** histos2);
  void ElectronSelection(std::string sample, TH1D** histos, TH2D** histos2);
  void MuonSelection(std::string sample, TH1D** histos, TH2D** histos2);
  void JetSelection(std::string sample, TH1D** histos, TH2D** histos2);
  void BosonSelection(std::string sample, TH1D** histos, TH2D** histos2);
  void wwCutflow(std::string sample, TH1D** histos, TH2D** histos2);
  void wwFakeEst(std::string sample, TH1D** histos, TH2D** histos2);
  void wwFakeEstBack(std::string sample,TH1D** histos,TH2D** histos2);
  void wFakeEst(std::string sample, TH1D** histos, TH2D** histos2);

  //Helper analysis functions
  int maxNofPVTracks();
  double zPVmaxTracks();
  std::vector<MTree::Lepton*> looseLeptons(std::vector<MTree::Lepton*> leptons);
  std::vector<MTree::Lepton*> mediumLeptons(std::vector<MTree::Lepton*> leptons);
  std::vector<MTree::Lepton*> orderByPt(std::vector<MTree::Lepton*> leptons);
  std::vector<MTree::Lepton*> removeOverlap(std::vector<MTree::Lepton*> leptons);
  std::vector<MTree::Jet*> findNonOverlapJets(std::vector<MTree::Lepton*> leptons);
  std::vector<std::string> findChannels(std::vector<MTree::Lepton*> leptons);
  std::vector<std::string> findWWChannels(std::vector<MTree::Lepton*> leptons);
  std::vector<std::string> findChannels(std::vector<WCandidate> Ws);
  std::vector<std::string> findChannels(std::vector<ZCandidate> Zs);

  double getSigEff(int leadid, double pt1, double eta1, int subid, double pt2, double eta2);
  double getBackEff(int leadid, double pt1, double eta1, int subid, double pt2, double eta2);

  double getMET();
  double getDeltaR(MTree::Lepton* lepton1, MTree::Lepton* lepton2);

  bool IsJF17WZEvent();

  void printEverything();

 private:

  TH2D* true_elec_eff;
  TH2D* fake_elec_eff;
  TH2D* true_muon_eff;
  TH2D* fake_muon_eff;

};

#endif
