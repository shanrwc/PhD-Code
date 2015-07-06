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

void mergeStreams(std::string runNumber)
{

  std::vector<std::string> run;

  std::string file = "fullDataList.txt"; 
  ifstream infile (file.c_str());
  std::string line;
  if (infile.is_open())
  {
    while (infile.good())
    {
      getline(infile,line);
      if (line.find(runNumber) == std::string::npos) continue;
      run.push_back(line);
    }
  }

  std::sort(run.begin(),run.end());

  //Make appropriate output file
  std::stringstream fileName; 
  fileName << "/net/s3_datad/MTree/MTree2l/16-01-15/data/Merged/Run-" << runNumber;
  system(("mkdir -vp " + fileName.str()).c_str());

  fileName << "/" << runNumber;
  TFile* outFile = new TFile((fileName.str() + ".root").c_str(),"recreate");

  //Get input trees for that run number.  It should be enough to put them all in one tree
  TChain* chain = new TChain("MTree");

  std::vector<std::string>::reverse_iterator rit;
  for (rit = run.rbegin(); rit != run.rend(); ++rit)
  {
    chain->AddFile((*rit).c_str());
  }

//   for (int i = 0; i != (int)run.size(); ++i)
//   {
//     chain->AddFile(run[i].c_str());
//   }

  //Do turn on all the branches, and make an empty tree to store the chosen events.
  //With of course an appropriatly named output file to store everything.
  TTree* newtree = chain->CloneTree(0);

  TClonesArray* trigObjects = new TClonesArray("MTree::TriggerObject");
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

  chain->SetBranchStatus("*", 1);
      
  chain->SetBranchAddress("TriggerObjects",&trigObjects);
  chain->SetBranchAddress("Electrons", &electrons);
  chain->SetBranchAddress("Photons", &photons);
  chain->SetBranchAddress("Muons", &muons);
  chain->SetBranchAddress("CaloMuons", &caloMuons);
  chain->SetBranchAddress("Jets", &jets);
  chain->SetBranchAddress("Tracks", &tracks);
  chain->SetBranchAddress("StandAloneMuons", &standAloneMuons);
  chain->SetBranchAddress("MuTagMuons", &muTagMuons);
  chain->SetBranchAddress("PrimaryVertices", &primaryVertices);
  if (chain->GetBranch("TruthParticles")) {
    chain->SetBranchAddress("TruthParticles", &truthParticles);
  }
  chain->SetBranchAddress("L1EmTauTriggerROIs", &L1EmTauTriggerROIs);
  chain->SetBranchAddress("L1MuonTriggerROIs", &L1MuonTriggerROIs);
  chain->SetBranchAddress("HLTTriggerROIs", &HLTTriggerROIs);
  chain->SetBranchAddress("MissingET", &missingET);
  chain->SetBranchAddress("MissingETTerms", &missingETTerms);
  chain->SetBranchAddress("EventInfo", &eventInfo);
  chain->SetBranchAddress("Trigger", &trigger);

  //Loop over events.  If event number isn't already in a vector, store number and event.
  set<int> eventNumbers;

  int nrEvents = chain->GetEntries();
  int mEventNr = 0;
  while (chain->GetEntry(mEventNr++))
  {

    int eventNumber = eventInfo->EventNumber();
    if (eventNumbers.find(eventNumber) != eventNumbers.end())
    {
      continue;
    }
    eventNumbers.insert(eventNumber);
    newtree->Fill();
    outFile = newtree->GetCurrentFile();
  }//Closes while loop over events

  outFile->Write();
  outFile->Close();

  std::cout << "Initial: " << nrEvents << std::endl;
  std::cout << "Merged: " << eventNumbers.size() << std::endl;

  delete chain;
}
