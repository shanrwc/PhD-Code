#include "WWPreselection.h"

WWPreselection::WWPreselection() {
  _Leptons[0] = _Leptons[1] = 0;
  _Jets[0] = _Jets[1] = 0;
}

WWPreselection::~WWPreselection() {
}

void WWPreselection::wwTMVASelection(std::string sample, TH1D** histos, TH2D** histos2D)
{
  if (_debug)
  {
    std::cout << "Calling wwSelection" << std::endl;
  }

  //Step Zero: Create and initialize necessary ints, double, RecordClass object, 
  //trees to save results for TMVA, etc.
  int i;

  RecordClass* record = new RecordClass("TMVA");
  record->setChannels("ee","emu","mumu");

  string filename = "tmvaInput/" + sample + ".root";
  TFile *file = new TFile (filename.c_str(),"recreate");
  TTree* emuTree;
  TTree* eeTree;
  TTree *mumuTree;

  //Step One: Produce vector of sample names to be run over.
  std::vector<std::string> samples = GetAllSamples(sample);

  //Step Two: Loop over samples--get chain, set branch addresses, finding cross-section and
  //sum of event weights, etc.
  std::vector<std::string>::iterator iter;
  std::vector<std::string>::iterator end = samples.end();

  emuTree = (GetChain(*samples.begin()))->CloneTree(0);
  TopologicalVarsClass* topoVars = new TopologicalVarsClass();
  emuTree->Branch("topoVars",&topoVars);
  eeTree = emuTree->CloneTree(0);
  mumuTree = emuTree->CloneTree(0);

  for (iter = samples.begin(); iter != end; ++iter)
  {
    if (_debug)
    {
      std::cout << "Analyzing sample " << *iter << std::endl;
    }
    double sampleWeight = 0.0;
    double sampleXSec;

    std::string sampleName = *iter;
    TChain *chain = GetChain(sampleName);
    std::cout<<"For sample "<<sampleName<<std::endl;
    sampleWeight = GetSampleWeight(sampleName);
    sampleXSec = GetCrossSection(sampleName);
    record->setWeightSum(sampleWeight);

    setBranchAddresses(chain);

    if (_debug)
    {
      std::cout << "Entering event loop" << endl;
    }

    int eventNr = 0; 
    while(chain->GetEntry(eventNr++))
    {
      //0. Calculate event's x-section and initialize RecordClass.
      int cutNr = 0;
      double weight = _eventInfo->EventWeight();
      if (weight == 0)
      {
	weight = 1;
      }
      double xsec = sampleXSec/sampleWeight*weight;
      cutNr = record->addEvent(cutNr,xsec,"Before cuts");

      //1. Trigger: at least one lepton must complete either mu10 or el10_loose trigger
      //chain.
      if (_debug)
      {
	std::cout << "Checking trigger" << std::endl;
      }
      
      MTree::Trigger* trigger = _trigger;
      bool pass1 = wwL1Trigger(trigger);
      if (pass1)
      {
	cutNr = record->addEvent(cutNr, xsec, "L1 Trigger selection");
      }
      else continue;

      bool pass2 = wwL2Trigger(trigger);
      if (pass2)
      {
	cutNr = record->addEvent(cutNr, xsec, "L2 Trigger selection");
      }
      else continue;

      bool pass3 = wwEFTrigger(trigger);
      if (pass3)
      {
	cutNr = record->addEvent(cutNr, xsec, "EF Trigger selection");
      }

      //2. Create a vector of leptons to play with. Note: once, I only have two
      //leptons to work with, I can save them in the class as a data-member.
      std::vector<std::string> channels;
      std::vector<MTree::Lepton*> leptons;

      int n_electrons = _electrons->GetEntriesFast();
      for (i = 0; i < n_electrons; i++)
      {
	leptons.push_back((MTree::Electron*)_electrons->At(i));
      }
      int n_muons = _muons->GetEntriesFast();
      for (i = 0; i < n_muons; i++)
      {
	leptons.push_back((MTree::Muon*)_muons->At(i));
      }

      //3. Lepton Preselection: loose electrons, staco muons, 2 leptons with 
      //pt > 10 GeV.
      if (_debug)
      {
	std::cout << "Evaluating lepton preselection" << std::endl;
      }
      leptons = wwPreselection(leptons);
      if (leptons.size() > 1)
      {
	channels = wwLeptonChannel(leptons);
	cutNr = record->addEvent(cutNr, xsec, "Lepton Preselection",channels);
      }
      else continue;
      channels.clear();

      //4. Lepton Selection, including pt, eta, isolation, and D0 requirements.
      if (_debug)
      {
	std::cout << "Evaluating candidate selection" << std::endl;
      }
      leptons = wwLeptonSelection(leptons);
      if (leptons.size() > 1)
      {
	channels = wwLeptonChannel(leptons);
	cutNr = record->addEvent(cutNr, xsec, "Higgs candidate selection",channels);
      }
      else continue;
      channels.clear();

      //5-1a. Require at least one pair of opposite sign leptons.
      if (_debug)
      {
	std::cout << "Evaluating lepton charge" << std::endl;
      }
      leptons = wwCharge(leptons);
      if (leptons.size() > 1)
      {
	channels = wwLeptonChannel(leptons);
	cutNr = record->addEvent(cutNr, xsec, "1a. At least one oppositely charged pair",channels);
      }
      else continue;
      channels.clear();

      //5-1b. Keep only event with exactly two leptons. Store said leptons as data 
      //members.
      if (_debug)
      {
	std::cout << "Veto events with more than two leptons" << std::endl;
      }
      if (leptons.size() == 2)
      {
	channels = wwLeptonChannel(leptons);
	cutNr = record->addEvent(cutNr, xsec, "1b. Exactly two leptons", channels);
      }
      else continue;

      _Leptons[0] = leptons[0];
      _Leptons[1] = leptons[1];

      //6. Count number of jets in an event.  Veto events with jets.
      std::vector<MTree::Jet*> jets = wwJetOverlapRemoval(0.4);
      int nJets = wwJetCounter(jets);
      if (nJets == 0)
      {
	channels = wwLeptonChannel(leptons);
	cutNr = record->addEvent(cutNr, xsec, "Exactly zero jets", channels);
      }
      else continue;

      //Step Three: give information to trees.
      topoVars->calculateVars(_missingET,_Leptons[0],_Leptons[1],xsec,sampleName,nJets);

      if (channels[0] == "emu")
      {
	emuTree->Fill();
      }
      if (channels[0] == "ee")
      {
	eeTree->Fill();
      }
      if (channels[0] == "mumu")
      {
	mumuTree->Fill();
      }

      topoVars->clear();

      if (eventNr % 10000 == 0)
      {
	std::cout<< ". . . . "<<eventNr<<" events analyzed"<<std::endl;
      }

    }//Close loop over events in a sample.    

  }//Close loop over samples.
    record->print(sample);
    emuTree->SetName("emuTree");
    eeTree->SetName("eeTree");
    mumuTree->SetName("mumuTree");
    file->Write();
    file->Close();
}
double WWPreselection::GetWWTransverseMass() {
  TLorentzVector dilepton = (*_Leptons[0]) + (*_Leptons[1]);
  double dPhi = TMath::Abs(dilepton.DeltaPhi(*_missingET));
  return TMath::Sqrt(2 * dilepton.Pt() * _missingET->MET() * (1.0 - TMath::Cos(dPhi)));
}

double WWPreselection::GetWTransverseMass(MTree::Lepton* lepton) {
  TVector3 lep(lepton->Px(), lepton->Py(), 0.0);
  TVector3 met(_missingET->Ex(), _missingET->Ey(), 0.0);
  return TMath::Sqrt(2.0) * TMath::Sqrt(lep.Mag() * met.Mag() - lep.X() * met.X() - lep.Y() * met.Y());
}

void WWPreselection::getX1X2(double& x1, double& x2) {

  double dphi1 = _missingET->DeltaPhi(*_Leptons[0]);
  double dphi2 = _missingET->DeltaPhi(*_Leptons[1]);
  double met = _missingET->MET();
  
  double ratio1 = TMath::Sin(dphi2) != 0.0 ? TMath::Sin(dphi1) / TMath::Sin(dphi2) : 0.0;
  double angleTerm = TMath::Cos(dphi1) - ratio1 * TMath::Cos(dphi2);
  double v1 = angleTerm != 0.0 ? met / angleTerm : 0.0;
  double v2 = -v1 * ratio1;

  double denom1 = _Leptons[0]->Pt() + v1;
  x1 = denom1 != 0.0 ? _Leptons[0]->Pt() / denom1 : 0.0;
  double denom2 = _Leptons[1]->Pt() + v2;
  x2 = denom2 != 0.0 ? _Leptons[1]->Pt() / denom2 : 0.0;
}

ClassImp(WWPreselection)
