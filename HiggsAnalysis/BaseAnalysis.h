#ifndef __BASEANALYSIS_H
#define __BASEANALYSIS_H

#include "TObject.h"
#include "TCut.h"
#include "TString.h"
#include "TGraphAsymmErrors.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TEventList.h"
#include "TChain.h"
#include "TSystem.h"
#include "TBranch.h"
#include "TLeaf.h"
#include "TStopwatch.h"

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
#include <algorithm>

#include "MTree/Electron.h"
#include "MTree/Photon.h"
#include "MTree/EventInfo.h"
#include "MTree/Trigger.h"
#include "MTree/Jet.h"
#include "MTree/MissingET.h"
#include "MTree/Muon.h"
#include "MTree/Track.h"
#include "MTree/PrimaryVertex.h"
#include "MTree/TruthParticle.h"
#include "MTree/L1TriggerROI.h"
#include "MTree/HLTTriggerROI.h"
#include "MTree/TriggerObject.h"

#include "GoodRunsLists/TGoodRunsList.h"
#include "GoodRunsLists/TGoodRunsListReader.h"
//#include "checkOQ.h"
#include "TPileupReweighting.h"
#include "JetUncertainties/JESUncertaintyProvider.h"

#include "egammaPIDdefs.h"

class BaseAnalysis : public TObject {
 public:
  BaseAnalysis();
  ~BaseAnalysis();

  TChain* GetChain(std::string chainName);
  TChain* GetChain(std::vector<std::string> chainNames);
  std::string convertNameToNum(std::string name);
  void setBranchAddresses(TChain* tree);

  double GetCrossSection(std::string chainName);
  std::vector<std::string> GetKnownChains();
  double CalculateSampleWeight(std::string sample);
  std::vector<std::string> GetAllSamples(std::string sample);
  void GetAllNames();

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
  ClassDef(BaseAnalysis,0);

 protected:
  bool _debug;
  TClonesArray* _electrons;
/*   TClonesArray* _photons; */
  TClonesArray* _muons;
  TClonesArray* _caloMuons;
  TClonesArray* _jets;
  TClonesArray* _tracks;
/*   TClonesArray* _standAloneMuons; */
/*   TClonesArray* _muTagMuons; */
  TClonesArray* _primaryVertices;
  TClonesArray* _truthParticles;
/*   TClonesArray* _L1EmTauTriggerROIs; */
  TClonesArray* _L1MuonTriggerROIs;
/*   TClonesArray* _HLTTriggerROIs; */
  MTree::MissingET* _missingET;
  TClonesArray* _missingETTerms;
  MTree::EventInfo* _eventInfo;
  MTree::Trigger* _trigger;
  TClonesArray* _triggerObjects;

  double GetSampleWeight(std::string chainName);
  std::vector<std::string> splitString(std::string phrase);

  double makeDouble(std::string input);

 private:
  void AddFile(std::string chainName, std::string dataset, std::string fileName);
  void AddAllFiles();
  void FillCrossSections();
  void FillSampleWeights();
  void FillSetNumbers();

  std::map<std::string, std::vector<std::string> > _fileNameMap;
  std::map<std::string, double> _crossSections;
  std::map<std::string, double> _sampleWeights;
  std::map<std::string, std::string> _setNumbers;

};

#endif
