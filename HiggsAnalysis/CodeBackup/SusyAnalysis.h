#ifndef __SUSYANALYSIS_H_
#define __SUSYANALYSIS_H_

#include "BaseAnalysis.h"
#include "robustIsEMDefs.h"

class SusyAnalysis : public BaseAnalysis
{
 public:
  SusyAnalysis();
  ~SusyAnalysis();

  ClassDef(SusyAnalysis,0);

  //Put useful functions (you know, like your real code, that stuff you work on so much) here.

  //This is the main one, with all the histograms and record-keeping and matrix method stuff
  void SusyCutflow(std::string sample, TH1D** histos, TH2D** histos2, std::vector<std::vector<std::vector<TH1D*> > > oshistos, std::vector<std::vector<std::vector<TH1D*> > > sshistos, std::vector<std::vector<std::vector<std::vector<std::vector<TH1D*> > > > > MMhistos);

  //This estimates isolation efficiencies without a trigger requirement, with a MET ceiling, and for single lepton events = for the fake rate from jets.
  void EstimateEfficiencies(std::string sample, TH1D** histos, TH2D** histos2);
  //This grabs dilepton events close to the Z peak and calculates efficiencies for their isolation = for the true lepton rates.
  void EstimateEfficienciesZ(std::string sample, TH1D** histos, TH2D** histos2);

  //This one produces histograms for the group's special qcd estimate
  void qcdEstimate(std::string sample, TH1D** histos, std::vector<std::vector<std::vector<TH1D*> > > oshistos, std::vector<std::vector<std::vector<TH1D*> > > sshistos);

  void wjetsFactor(std::string sample, TH1D** histos, TH2D** histos2);

  void EffEstWZ(std::string sample, TH1D** histos, TH2D** histos2);

 private:
  void passTriggers(int runNumber, bool& passE, bool& passM);

  double findMET(std::vector<MTree::Muon*> muons);
  double findMeff(std::vector<MTree::Jet*> jets, std::vector<MTree::Electron*> electrons, std::vector<MTree::Muon*> muons);

  template<class T, class S> 
  void removeOverlap(double cut, std::vector<T*> &goodones, std::vector<S*> &badones);

  template<class T>
  void orderByPt(std::vector<T*>& vec);

  bool removeDecayProducts(std::string sample, std::vector<MTree::Electron*> &elecs, std::vector<MTree::Muon*> &muons, std::vector<MTree::TruthParticle*> &truths);

  bool checkTrigger(std::string sample, int runNumber, std::vector<MTree::Lepton*> leptons);
  bool sillyHighPtCut(int runNumber, std::vector<MTree::Lepton*> leptons);
  bool sillierCheckTrigger(std::string sample, int runNumber, std::vector<MTree::Lepton*> leptons);

  double getSigEff(int leadid, double pt1, double eta1, int subid, double pt2, double eta2);
  double getBackEff(int leadid, double pt1, double eta1, int subid, double pt2, double eta2);

  std::vector<std::string> findChannels(std::vector<MTree::Lepton*> leptons);

  bool firstTriggerCheck(std::string sample, int runNumber);
  std::vector<MTree::PrimaryVertex*> getPVs();
  double getPileUpWeight(int nPVs);
  bool badJetsPresent();
  bool cosmicJetPresent(std::vector<MTree::Jet*> jets);
  bool OTXProblem(int runNumber,std::vector<MTree::Electron*> electrons);
  bool hasCosmicMuon(std::vector<MTree::Muon*> muons);

  std::vector<MTree::Electron*> getMediumElectrons(int runNumber);
  std::vector<MTree::Electron*> getCrackElectrons(int runNumber, bool wantLoose);
  std::vector<MTree::Electron*> getLooseElectrons(int runNumber);
  std::vector<MTree::Muon*> getGoodMuons();
  std::vector<MTree::Jet*> getJets();
  std::vector<MTree::Photon*> getPhotons();


  TH2D* true_elec_eff;
  TH2D* fake_elec_eff;
  TH2D* true_muon_eff;
  TH2D* fake_muon_eff;
};

#endif
