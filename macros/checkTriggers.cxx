#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TClonesArray.h"

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

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>

void checkTriggers()
{
  //Get input trees for that run number.  It should be enough to put them all in one tree
  TChain* chain = new TChain("MTree");

  chain->AddFile("/net/s3_dataa/MTree/MTree-16-01-15/AOD/unskimmed/user.swalch.mc10_7TeV.116707.PowHegPythia_ggH150_WW2lep.merge.AOD.e773_s933_s946_r2302_r2300.MT16-01-15.unsk.v3.110817202251/user.swalch.005283.EXT0._00001.MTree.root");

  TClonesArray* trigObjects = new TClonesArray("MTree::TriggerObject");
  TClonesArray* electrons = new TClonesArray("MTree::Electron");
  TClonesArray* photons = new TClonesArray("MTree::Photon");
  TClonesArray* muons = new TClonesArray("MTree::Muon");
//   TClonesArray* caloMuons = new TClonesArray("MTree::Muon");
  TClonesArray* jets = new TClonesArray("MTree::Jet");
  TClonesArray* tracks = new TClonesArray("MTree::Track");
//   TClonesArray* standAloneMuons = new TClonesArray("MTree::Track");
//   TClonesArray* muTagMuons = new TClonesArray("MTree::Track");
//   TClonesArray* primaryVertices = new TClonesArray("MTree::PrimaryVertex");
//   TClonesArray* truthParticles = new TClonesArray("MTree::TruthParticle"); 
//   TClonesArray* L1EmTauTriggerROIs = new TClonesArray("MTree::L1TriggerROI"); 
//   TClonesArray* L1MuonTriggerROIs = new TClonesArray("MTree::L1TriggerROI"); 
//   TClonesArray* HLTTriggerROIs = new TClonesArray("MTree::HLTTriggerROI"); 
//   TClonesArray* missingETTerms = new TClonesArray("MTree::MissingET"); 
//   MTree::MissingET* missingET = new MTree::MissingET();
//   MTree::EventInfo* eventInfo = new MTree::EventInfo(); 
  MTree::Trigger* trigger = new MTree::Trigger(); 

  chain->SetBranchStatus("*", 1);
      
  chain->SetBranchAddress("TriggerObjects",&trigObjects);
  chain->SetBranchAddress("Electrons", &electrons);
  chain->SetBranchAddress("Photons", &photons);
  chain->SetBranchAddress("Muons", &muons);
//   chain->SetBranchAddress("CaloMuons", &caloMuons);
  chain->SetBranchAddress("Jets", &jets);
  chain->SetBranchAddress("Tracks", &tracks);
//   chain->SetBranchAddress("StandAloneMuons", &standAloneMuons);
//   chain->SetBranchAddress("MuTagMuons", &muTagMuons);
//   chain->SetBranchAddress("PrimaryVertices", &primaryVertices);
//   if (chain->GetBranch("TruthParticles")) {
//     chain->SetBranchAddress("TruthParticles", &truthParticles);
//   }
//   chain->SetBranchAddress("L1EmTauTriggerROIs", &L1EmTauTriggerROIs);
//   chain->SetBranchAddress("L1MuonTriggerROIs", &L1MuonTriggerROIs);
//   chain->SetBranchAddress("HLTTriggerROIs", &HLTTriggerROIs);
//   chain->SetBranchAddress("MissingET", &missingET);
//   chain->SetBranchAddress("MissingETTerms", &missingETTerms);
//   chain->SetBranchAddress("EventInfo", &eventInfo);
  chain->SetBranchAddress("Trigger", &trigger);

  //Loop over events.  If event number isn't already in a vector, store number and event.
//   set<int> eventNumbers;

//   int nrEvents = chain->GetEntries();
  int mEventNr = 0;
  while (chain->GetEntry(mEventNr++))
  {
//     if (mEventNr % 10000 == 0) std::cout << "checked " << mEventNr << " events" << std::endl;

    if (trigger->PassedTrigger("L1_MU11"))
    {
      std::cout << "Found L1_MU11!" << std::endl;
    }

    int nTrigObjs = trigObjects->GetEntriesFast();
    for (int i = 0; i != nTrigObjs; ++i)
    {
      MTree::TriggerObject* trigObj = (MTree::TriggerObject*)trigObjects->At(i);

      if (trigObj->PassedTrigger("L1_MU11"))
      {
	std::cout << "Trigger object for L1_MU11" << std::endl;
      }

//       std::set<std::string> names = trigObj->TriggerNames();
//       std::set<std::string>::iterator iter;
//       std::set<std::string>::iterator end = names.end();

//       for (iter = names.begin(); iter != end; ++iter)
//       {

// 	std::cout << (*iter) << std::endl;
//       }
    }

  }//Closes while loop over events


  delete chain;
}
