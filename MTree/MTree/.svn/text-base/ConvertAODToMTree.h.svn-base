#ifndef CONVERTAODTOMTREE_H
#define CONVERTAODTOMTREE_H

#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/ObjectVector.h"
#include "GaudiKernel/ToolHandle.h"
#include "CLHEP/Units/SystemOfUnits.h"
#include "StoreGate/StoreGateSvc.h"
#include "GaudiKernel/ITHistSvc.h"
#include "TrigDecisionTool/TrigDecisionTool.h"
#include "ITrackToVertex/ITrackToVertex.h"
#include "IsolationTool/IIsolationTool.h"
#include "MCTruthClassifier/IMCTruthClassifier.h"

/* #include "egammaEvent/egammaContainer.h" */
#include "egammaEvent/ElectronContainer.h"
#include "egammaEvent/PhotonContainer.h"
#include "egammaEvent/EMShower.h"
#include "egammaEvent/EMTrackMatch.h"
#include "JetEvent/JetCollection.h"
#include "JetMomentTools/JetVertexAssociationTool.h"
#include "JetMomentTools/JetVtxTrackHelper.h"

#include <string>

#include "TObject.h"
#include "TCut.h"
#include "TString.h"
#include "TGraphAsymmErrors.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TEventList.h"
#include "TChain.h"
#include "TSystem.h"
#include "TBranch.h"
#include "TLeaf.h"
#include "TClonesArray.h"
#include "TLorentzVector.h"

#include "HepMC/GenEvent.h"
#include "HepMC/GenParticle.h"
#include "HepMC/GenVertex.h"

#include "McParticleEvent/TruthParticle.h"
#include "McParticleEvent/TruthParticleContainer.h"

#include "MTree/Electron.h"
#include "MTree/Photon.h"
#include "MTree/EventInfo.h"
#include "MTree/Trigger.h"
#include "MTree/TriggerObject.h"
#include "MTree/Jet.h"
#include "MTree/MissingET.h"
#include "MTree/Muon.h"
#include "MTree/Track.h"
#include "MTree/PrimaryVertex.h"
#include "MTree/TruthParticle.h"
#include "MTree/L1TriggerROI.h"
#include "MTree/HLTTriggerROI.h"

#include "MTree/SmearingClass.h"
#include "MTree/EnergyRescaler.h"

class ConvertAODToMTree : public Algorithm  {

 public:

  ConvertAODToMTree(const std::string& name, ISvcLocator* pSvcLocator);
  ~ConvertAODToMTree();
  
  StatusCode initialize();
  StatusCode finalize();
  StatusCode execute();
  

  StoreGateSvc* m_storeGate;
  ToolHandle<Trig::TrigDecisionTool> m_trigDec;
  ToolHandle<Reco::ITrackToVertex> m_trackToVertex;
  ToolHandle<IIsolationTool> m_isolationTool;
  ToolHandle<IMCTruthClassifier> m_classifier;
  ToolHandle<JetVertexAssociationTool> m_jvfCalc;
/*   ToolHandle<IJetVtxTrackHelper> m_jetVtxHelp;  */

 private:
  
  MTree::TruthParticle* FindParticle(TClonesArray* particles, int pdg_id, int status, int parent_pdg_id, HepMC::FourVector& mom);
  void SetDaughterLinks(TClonesArray* particles);
  void FlagDuplicateTruthParticles(TClonesArray* particles);
  bool IsIdenticalTruthParticles(MTree::TruthParticle* particle1, MTree::TruthParticle* particle2, bool DoStatusMatch = true);
  void TransferTruthParticles(TClonesArray* particles);
  MTree::TruthParticle* GetMatchingTruthParticle(MTree::TruthParticle* inputParticle);

  int m_EventCounter;
  
  TClonesArray* m_triggerObjects;
  TClonesArray* m_electrons;
  TClonesArray* m_photons;
  TClonesArray* m_muons;
  TClonesArray* m_caloMuons;
  TClonesArray* m_jets;
  TClonesArray* m_tracks;
  TClonesArray* m_standAloneMuons;
  TClonesArray* m_muTagMuons;
  TClonesArray* m_primaryVertices;
  TClonesArray* m_truthParticles;
  TClonesArray* m_L1EmTauTriggerROIs;
  TClonesArray* m_L1MuonTriggerROIs;
  TClonesArray* m_HLTTriggerROIs;
  MTree::MissingET* m_missingET;
  MTree::EventInfo* m_eventInfo;
  MTree::Trigger* m_trigger;
  TFile* m_outFile;
  TTree* m_tree;

  TClonesArray* m_missingETTerms;

//skimming related member variables
  TFile* m_muFile;
  TH1D* m_muHist;
  TH1D* m_logHist;
  bool m_applySkim;
  int m_leptonPtCut;
  int m_nrLeptonsCut;
  bool m_smearLeptonPt;
  SmearingClass m_muSmear;
  EnergyRescaler m_elSmear;
  int m_nPlusEvents;
  int m_nMinusEvents;
  int m_nPlusEventsPassed;
  int m_nMinusEventsPassed;

  
  std::string m_RootFileName;
  std::string m_VtxContainerName;
  std::string m_JetContainerName;
  std::string m_TrackContainerName;
  std::string m_ElectronContainerName;
  std::string m_PhotonContainerName;
  std::vector<std::string> m_MuonContainerNames;  
  std::string m_CaloMuonContainerName;
  std::vector<std::string> m_StandaloneMuonContainerNames;
  std::string m_MuTagContainerName;
  std::string m_METContainerName;
  std::string m_MCTruthContainerName;


  std::vector<std::string> m_IgnoreL1TriggerNames;
  std::vector<std::string> m_IgnoreHLTTriggerNames;
  std::vector<std::string> m_HLTTriggerROINames;
  
  std::vector<std::string> m_METTermContainerNames;
  std::vector<std::string> m_METCaloContainerNames;
  
  int m_EventCounterInterval;
  double m_electronPtCut;
  double m_photonPtCut;
  double m_trackPtCut;

  std::vector<std::string> m_ConfiguredL1Names;
  std::vector<std::string> m_ConfiguredL2Names;
  std::vector<std::string> m_ConfiguredEFNames;
};

#endif // CONVERTAODTOMTREE_H

