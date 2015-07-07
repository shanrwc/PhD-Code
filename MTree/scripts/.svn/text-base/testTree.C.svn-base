#include "TLorentzVector.h"
#include "TClonesArray.h"
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <vector>
#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TObjString.h"

void testTree() {

  //gSystem->Load("libMTree.so");

  //TFile* file = new TFile("MTree.root");
  //TTree* tree = (TTree*) file->Get("MTree");

  TChain* tree = new TChain("MTree");
  tree->AddFile("MTree_1.root", -1, "MTree");
  //tree->AddFile("MTree_2.root", -1, "MTree");
  //tree->AddFile("MTree_3.root", -1, "MTree");
  //tree->AddFile("MTree_4.root", -1, "MTree");

  TClonesArray* electrons = new TClonesArray("MTree::Electron");
  TClonesArray* photons = new TClonesArray("MTree::Photon");
  TClonesArray* muons = new TClonesArray("MTree::Muon");
  TClonesArray* caloMuons = new TClonesArray("MTree::Muon");
  TClonesArray* jets = new TClonesArray("MTree::Jet");
  TClonesArray* tracks = new TClonesArray("MTree::Track");
  TClonesArray* standAloneMuons = new TClonesArray("MTree::Track");
  TClonesArray* muTagMuons = new TClonesArray("MTree::Track");
  TClonesArray* primaryVertices = new TClonesArray("MTree::PrimaryVertex");
  TClonesArray* truthParticles = new TClonesArray("MTree::TruthParticle"); 
  TClonesArray* L1EmTauTriggerROIs = new TClonesArray("MTree::L1TriggerROI"); 
  TClonesArray* L1MuonTriggerROIs = new TClonesArray("MTree::L1TriggerROI"); 
  TClonesArray* HLTTriggerROIs = new TClonesArray("MTree::HLTTriggerROI"); 
  TClonesArray* missingETTerms = new TClonesArray("MTree::MissingET"); 
  MTree::MissingET* missingET = new MTree::MissingET();
  MTree::EventInfo* eventInfo = new MTree::EventInfo(); 
  MTree::Trigger* trigger = new MTree::Trigger(); 

  tree->SetBranchStatus("*", 1);

  tree->SetBranchAddress("Electrons", &electrons);
  tree->SetBranchAddress("Photons", &photons);
  tree->SetBranchAddress("Muons", &muons);
  tree->SetBranchAddress("CaloMuons", &caloMuons);
  tree->SetBranchAddress("Jets", &jets);
  tree->SetBranchAddress("Tracks", &tracks);
  tree->SetBranchAddress("PrimaryVertices", &primaryVertices);
  if (tree->GetBranch("TruthParticles")) {
    tree->SetBranchAddress("TruthParticles", &truthParticles);
  }
  tree->SetBranchAddress("L1EmTauTriggerROIs", &L1EmTauTriggerROIs);
  tree->SetBranchAddress("L1MuonTriggerROIs", &L1MuonTriggerROIs);
  tree->SetBranchAddress("HLTTriggerROIs", &HLTTriggerROIs);
  tree->SetBranchAddress("MissingET", &missingET);
  tree->SetBranchAddress("MissingETTerms", &missingETTerms);
  tree->SetBranchAddress("EventInfo", &eventInfo);
  tree->SetBranchAddress("Trigger", &trigger);
  int entry = 0;
  
  TH2F* hist = new TH2F("hist", "impact paramter", 50, -3.2, 3.2, 40, -5.0, 5.0);
 
  int total = tree->GetEntries();
 
  int countMe = 0;
  while (tree->GetEntry(entry++)) 
  {
    if (entry % 1000 == 0) std::cout  << "| Processing " << entry << " / " << total << std::endl;
  
    
    //if (entry != 400) continue;

    int nrTerms = missingETTerms->GetEntries();
    for (int i = 0; i != nrTerms; ++i) {
      MTree::MissingET* met = (MTree::MissingET*) missingETTerms->At(i);
      //std::cout << met->GetTerm() << std::endl;
    }
    //missingET->PrintData(std::cout);

    std::vector< std::string > vec = trigger->GetEFTriggersPassed();
    //std::cout << "--------------------------------------" << std::endl;
    for (int i = 0; i != (int)vec.size(); i++)
    {
      //std::cout << "| Passed " << vec[i] << std::endl;
    }
    //std::cout << "--------------------------------------" << std::endl;

    if (trigger->PassedEFTrigger("EF_e10_medium")) countMe += 1;

    for (int i = 0; i != tracks->GetEntries(); i++)
    {
      MTree::Track* t = (MTree::Track*) tracks->At(i);
      //t->PrintData(std::cout);
    }
  }
  std::cout << "CountMe: " << countMe << std::endl;
  std::cout << "I'm done!" << std::endl;

}

