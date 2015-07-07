#include "SusyAnalysis.h"
#include "robustIsEMDefs.C"

SusyAnalysis::SusyAnalysis()
{
  //Grab reference histograms

  TFile *input = new TFile("MMInput.root");

  true_elec_eff = (TH2D*)input->Get("true_elec_eff");
  fake_elec_eff = (TH2D*)input->Get("fake_elec_eff");
  true_muon_eff = (TH2D*)input->Get("true_muon_eff");
  fake_muon_eff = (TH2D*)input->Get("fake_muon_eff");
}

//####################################################################################

SusyAnalysis::~SusyAnalysis()
{
  ;
}

//####################################################################################

void SusyAnalysis::SusyCutflow(std::string sample, TH1D** histos,TH2D** histos2, std::vector<std::vector<std::vector<TH1D*> > > oshistos, std::vector<std::vector<std::vector<TH1D*> > > sshistos, std::vector<std::vector<std::vector<std::vector<std::vector<TH1D*> > > > > MMhistos)
{
  if (_debug)
  {
    std::cout << "Calling SusyCutflow()" << std::endl;
  }

  bool isMC = false;
  if ((sample.find("EGamma") == string::npos) && (sample.find("Muons") == string::npos) && (sample.find("data") == string::npos))
  {
    isMC = true;
  }

  RecordClass* record = new RecordClass("events");
  RecordClass* bookE = new RecordClass("electrons");
  RecordClass* bookM = new RecordClass("muons");
  RecordClass* bookSS = new RecordClass("same-sign");
  RecordClass* bookOS = new RecordClass("oppo-sign");

  std::vector<std::string> channels;
  channels.push_back("ee");
  channels.push_back("em");
  channels.push_back("me");
  channels.push_back("mm");

  record->setChannels(channels);
  bookSS->setChannels(channels);
  bookOS->setChannels(channels);

  ofstream outfile1;
  std::string tag1 = "event-summaries/" + sample + "_afterJetCleaning.txt";
  outfile1.open(tag1.c_str());

  ofstream outfile2;
  std::string tag2 = "event-summaries/" + sample + "_afterCrackElecs.txt";
  outfile2.open(tag2.c_str());

  ofstream outfile3;
  std::string tag3 = "event-summaries/" + sample + "_afterOTX.txt";
  outfile3.open(tag3.c_str());

  ofstream outfile4;
  std::string tag4 = "event-summaries/" + sample + "_afterCosmics.txt";
  outfile4.open(tag4.c_str());

  ofstream outfile8;
  std::string tag8 = "event-summaries/" + sample + "_afterOneLep.txt";
  outfile8.open(tag8.c_str());

  ofstream outfile5;
  std::string tag5 = "event-summaries/" + sample + "_afterTwoLeps.txt";
  outfile5.open(tag5.c_str());

  ofstream outfile6;
  std::string tag6 = "event-summaries/" + sample + "_afterHighPt.txt";
  outfile6.open(tag6.c_str());

  ofstream outfile7;
  std::string tag7 = "event-summaries/" + sample + "_afterMll.txt";
  outfile7.open(tag7.c_str());

  ofstream outfile10;
  std::string tag10 = "event-summaries/" + sample + "_oscandidates.txt";
  outfile10.open(tag10.c_str());

  ofstream outfile9;
  std::string tag9 = "event-summaries/" + sample + "_sscandidates.txt";
  outfile9.open(tag9.c_str());

  ofstream outfile11;
  std::string tag11 = "event-summareis/" + sample + "mmcandidates.txt";
  outfile11.open(tag11.c_str());

  ofstream outfileS;
  std::string tagS = "event-summaries/" + sample + "_9102.txt";
  outfileS.open(tagS.c_str());

  if (_debug)
  {
    std::cout << "Setting up Good Run List" << std::endl;
  }
  TString list = "/home/swalch/macro-cosmos/HiggsAnalysis/GoodRunLists/susy_grl_init.xml";
  Root::TGoodRunsListReader foo(list);
  foo.Interpret();
  Root::TGoodRunsList grl1 =  foo.GetMergedGoodRunsList();

  if (_debug)
  {
    std::cout << "Getting all component samples to run over" << std::endl;
  }
  std::vector<std::string> samples = GetAllSamples(sample);
  std::vector<std::string>::iterator iter;
  std::vector<std::string>::iterator end = samples.end();

  for (iter = samples.begin(); iter != end; ++iter)
  {
    std::string sampleName = *iter;
    TChain* chain = GetChain(sampleName);
    setBranchAddresses(chain);
    std::cout << "Analyzing sample " << sampleName << std::endl;

    if (_debug)
    {
      std::cout << "Checking sample xsection/weight information" << std::endl;
    }

    double sampleWeight = 0.0;
    double sampleXSec = 1.0;
    if (isMC)
    {
      sampleWeight = GetSampleWeight(sampleName);
      sampleXSec = GetCrossSection(sampleName);
    }

    if (_debug)
    {
      std::cout << "Entering event loop" << std::endl;
    }
    int eventNr = 0;
    double weight = 1.0;
    while(chain->GetEntry(eventNr++))
    {
      if (_debug)
      {
	std::cout << "Analyzing event" << std::endl;
      }

      if (eventNr % 10000 == 0)
      {
	std::cout << "SusyCutflow has analyzed " << eventNr << " events." << std::endl;
      }
      int RUNNUMBER;
      if (isMC)
      {
	RUNNUMBER = 100000;
	weight = sampleXSec/sampleWeight*(_eventInfo->EventWeight());
	if ((sample.find("Z") != string::npos) || (sample.find("W") != string::npos))
	{
	  std::vector<MTree::PrimaryVertex*> pvs = getPVs();
          double temp = getPileUpWeight((int)pvs.size());
	  weight = weight * temp;
	}
      }
      else
      {
	RUNNUMBER = _eventInfo->RunNumber();
      }
      int cutNr = 0;
      cutNr = record->addEvent(cutNr,weight,"Initial number of events");

      bool isEvent = false;
//       if (_eventInfo->EventNumber() == 9102)
//       {
// 	isEvent = true;
//       }

      //Good Run List and Loading of Object Quality Maps go here.
      if (_debug)
      {
	std::cout << "Checking good run list" << std::endl;
      }
      bool passed = true;
      if (isMC)
      {
	egammaOQ::LoadOQMaps(166497);
      }
      else
      {
	int runNr = _eventInfo->RunNumber();
	int lumiBlock = _eventInfo->LumiBlock();
	passed = grl1.HasRunLumiBlock(runNr,lumiBlock);
	egammaOQ::LoadOQMaps(runNr);
      }

      if (!passed) continue;
      cutNr = record->addEvent(cutNr,weight,"Passed GRL");

      //Impose trigger requirements
      if (_debug)
      {
	std::cout << "imposing trigger requirements" << std::endl;
      }
      if (!firstTriggerCheck(sample, RUNNUMBER)) continue;
      cutNr = record->addEvent(cutNr,weight,"Passed appropriate triggers");

      bool passE;
      bool passM;
      passTriggers(RUNNUMBER, passE, passM);
      channels.clear();
      if (passE)
      {
	channels.push_back("ee");
	channels.push_back("em");
      }
      if (passM)
      {
	channels.push_back("me");
	channels.push_back("mm");
      }
      cutNr = record->addEvent(cutNr,weight, "Triggers by channel", channels);

      //Remove events with bad jets/calorimeter problems
      if (_debug)
      {
	std::cout << "Checking for bad jets" << std::endl;
      }
      if (badJetsPresent()) continue;
      cutNr = record->addEvent(cutNr, weight,"Passed cleaning cuts",channels);
//       outfile1 << _eventInfo->RunNumber() << ": " << _eventInfo->EventNumber() << std::endl;

      //Apply collision selection/primary vertex requirements
      if (_debug)
      {
	std::cout << "applying PV requirements" << std::endl;
      }

      std::vector<MTree::PrimaryVertex*> vertices = getPVs();
      if ((int)vertices.size() == 0) continue;
      cutNr = record->addEvent(cutNr,weight,"Has at least one good PV",channels);

      //Apply object selections
      if (_debug)
      {
	std::cout << "object selections" << std::endl;
      }

      //Electrons first.
      std::vector<MTree::Electron*> electrons = getMediumElectrons(RUNNUMBER);
      std::vector<MTree::Electron*> crackElectrons = getCrackElectrons(RUNNUMBER,false);

      std::vector<MTree::Muon*> muons = getGoodMuons();

      std::vector<MTree::Jet*> jets = getJets();

      //And behold, my first foray into photons.
      //Well, it moved over to the other file of source code.
      std::vector<MTree::Photon*> photons = getPhotons();


      //remove some overlaps
//       removeOverlap(0.2, photons, jets);
      removeOverlap(0.2, electrons, jets);

      if (_debug)
      {
	std::cout << "rejecting events with crack electrons" << std::endl;
      }
      if (crackElectrons.size() != 0) continue;
      cutNr = record->addEvent(cutNr,weight,"Events with no crack electrons",channels);
//       outfile2 << _eventInfo->RunNumber() << ": " << _eventInfo->EventNumber() << std::endl;
      if (isEvent)
      {
	outfileS << "had no crack electrons" << std::endl;
      }


      if (_debug)
      {
	std::cout << "Checking OTX veto" << std::endl;
      }
      if (OTXProblem(RUNNUMBER, electrons)) continue;
      cutNr = record->addEvent(cutNr,weight,"No OTX problems",channels);

      if (_debug)
      {
	std::cout << "Checking for cosmic events" << std::endl;
      }

      if (hasCosmicMuon(muons)) continue;
      cutNr = record->addEvent(cutNr,weight,"Rejected cosmic-like muon",channels);
//       outfile4 << _eventInfo->RunNumber() << ": " << _eventInfo->EventNumber() << std::endl;
      if (isEvent)
      {
	outfileS << "no cosmic-like muons" << std::endl;
      }

      //And yet more overlap (see why I templated that function?)
      int nEs = (int)electrons.size();
      int nMs = (int)muons.size();
      if (isEvent)
      {
	for (int k = 0; k != (int)jets.size(); ++k)
	{
	  jets[k]->PrintData(outfileS);
	  for (int l = 0; l != (int)muons.size(); ++l)
	  {
	    outfileS << "dR with muon " << l << ": " << TMath::Abs(muons[l]->DeltaR(*jets[k])) << std::endl;
	  }
	}
	for (int k = 0; k != (int)photons.size(); ++k)
	{
	  photons[k]->PrintData(outfileS);
	}
      }

      removeOverlap(0.4, jets, electrons);
      removeOverlap(0.4, jets, muons);

      if (isEvent)
      {
	outfileS << "Electrons before overlap: " << nEs << " and after: " << electrons.size() << std::endl;
	outfileS << "Muons before overlap: " << nMs << " and after: " << muons.size() << std::endl;
      }

      if (((int)electrons.size() + (int)muons.size()) < 1) continue;
      cutNr = record->addEvent(cutNr,weight,"At least one decent lepton",channels);
      outfile8 << _eventInfo->RunNumber() << ": " << _eventInfo->EventNumber() << std::endl;
      if (isEvent)
      {
	outfileS << "Made it past one decent lepton cut" << std::endl;
      }

      std::vector<MTree::Lepton*> leptons;
      for (int i = 0; i != (int)electrons.size(); ++i)
      {
	leptons.push_back(electrons[i]);
      }
      for (int i = 0; i != (int)muons.size(); ++i)
      {
	leptons.push_back(muons[i]);
      }
      orderByPt(leptons);

      if (_debug)
      {
	std::cout << "requiring at least one lepton" << std::endl;
      }

      if (!checkTrigger(sample,RUNNUMBER, leptons)) continue;
      cutNr = record->addEvent(cutNr,weight,"One lepton with trigger",channels);
      outfile8 << _eventInfo->RunNumber() << ": " << _eventInfo->EventNumber() << std::endl;

      if ((int)leptons.size() < 2) continue;
      cutNr = record->addEvent(cutNr,weight,"Nl >= 2",channels);
      outfile5 << _eventInfo->RunNumber() << ": " << _eventInfo->EventNumber() << std::endl;

      //Make sure there's a leading lepton with pT > 20 GeV
      if (_debug)
      {
	std::cout << "check leading lepton pT" << std::endl;
      }

      orderByPt(leptons);
      if (!sillyHighPtCut(RUNNUMBER, leptons)) continue;
      cutNr = record->addEvent(cutNr,weight,"Leading lepton has pT > 20",channels);
      outfile6 << _eventInfo->RunNumber() << ": " << _eventInfo->EventNumber() << std::endl;

      if (leptons.size() != 2) continue;
      channels.clear();
      channels = findChannels(leptons);
      cutNr = record->addEvent(cutNr,weight,"Exactly two leptons",channels);

      if (!sillierCheckTrigger(sample,RUNNUMBER, leptons)) continue;
      cutNr = record->addEvent(cutNr,weight,"Silly 2l trigger check",channels);
//       channels.clear();
//       channels = findChannels(leptons);
//       cutNr = record->addEvent(cutNr,weight,"2 lep trigger check",channels);
      //Second set of plots
      if (_debug)
      {
	std::cout << "Filling first set of plots" << std::endl;
      }
      int stage = 0;
      bool isSS = false;
      if (leptons[0]->Charge() == leptons[1]->Charge())
      {
	if (_debug)
	{
	  std::cout << "filling first ss plots" << std::endl;
	}
	isSS = true;
	if (channels[0] == "ee")
	{
	  sshistos[0][0][stage]->Fill(leptons[0]->Pt(),weight);
	  sshistos[1][0][stage]->Fill(leptons[1]->Pt(),weight);
	  sshistos[2][0][stage]->Fill(leptons[0]->Eta(),weight);
	  sshistos[3][0][stage]->Fill(leptons[1]->Eta(),weight);
	  sshistos[4][0][stage]->Fill(leptons[0]->Phi(),weight);
	  sshistos[5][0][stage]->Fill(leptons[1]->Phi(),weight);
	  sshistos[6][0][stage]->Fill(findMET(muons),weight);
	  sshistos[7][0][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  sshistos[8][0][stage]->Fill((int)jets.size(),weight);
	  sshistos[9][0][stage]->Fill((int)photons.size(),weight);
	  sshistos[10][0][stage]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  sshistos[11][0][stage]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	}
	else if (channels[0] == "em")
	{
	  sshistos[0][1][stage]->Fill(leptons[0]->Pt(),weight);
	  sshistos[1][1][stage]->Fill(leptons[1]->Pt(),weight);
	  sshistos[2][1][stage]->Fill(leptons[0]->Eta(),weight);
	  sshistos[3][1][stage]->Fill(leptons[1]->Eta(),weight);
	  sshistos[4][1][stage]->Fill(leptons[0]->Phi(),weight);
	  sshistos[5][1][stage]->Fill(leptons[1]->Phi(),weight);
	  sshistos[6][1][stage]->Fill(findMET(muons),weight);
	  sshistos[7][1][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  sshistos[8][1][stage]->Fill((int)jets.size(),weight);
	  sshistos[9][1][stage]->Fill((int)photons.size(),weight);
	  sshistos[10][1][stage]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  sshistos[11][1][stage]->Fill(((MTree::Muon*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	}
	else if(channels[0] == "me")
	{
	  sshistos[0][2][stage]->Fill(leptons[0]->Pt(),weight);
	  sshistos[1][2][stage]->Fill(leptons[1]->Pt(),weight);
	  sshistos[2][2][stage]->Fill(leptons[0]->Eta(),weight);
	  sshistos[3][2][stage]->Fill(leptons[1]->Eta(),weight);
	  sshistos[4][2][stage]->Fill(leptons[0]->Phi(),weight);
	  sshistos[5][2][stage]->Fill(leptons[1]->Phi(),weight);
	  sshistos[6][2][stage]->Fill(findMET(muons),weight);
	  sshistos[7][2][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  sshistos[8][2][stage]->Fill((int)jets.size(),weight);
	  sshistos[9][2][stage]->Fill((int)photons.size(),weight);
	  sshistos[10][2][stage]->Fill(((MTree::Muon*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  sshistos[11][2][stage]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	}
	else if (channels[0] == "mm")
	{
	  sshistos[0][3][stage]->Fill(leptons[0]->Pt(),weight);
	  sshistos[1][3][stage]->Fill(leptons[1]->Pt(),weight);
	  sshistos[2][3][stage]->Fill(leptons[0]->Eta(),weight);
	  sshistos[3][3][stage]->Fill(leptons[1]->Eta(),weight);
	  sshistos[4][3][stage]->Fill(leptons[0]->Phi(),weight);
	  sshistos[5][3][stage]->Fill(leptons[1]->Phi(),weight);
	  sshistos[6][3][stage]->Fill(findMET(muons),weight);
	  sshistos[7][3][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  sshistos[8][3][stage]->Fill((int)jets.size(),weight);
	  sshistos[9][3][stage]->Fill((int)photons.size(),weight);
	  sshistos[10][3][stage]->Fill(((MTree::Muon*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  sshistos[11][3][stage]->Fill(((MTree::Muon*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	}
	++stage;
      }
      else
      {
	if (_debug)
	{
	  std::cout << "filling first os plots" << std::endl;
	}
	if (channels[0] == "ee")
	{
	  oshistos[0][0][stage]->Fill(leptons[0]->Pt(),weight);
	  oshistos[1][0][stage]->Fill(leptons[1]->Pt(),weight);
	  oshistos[2][0][stage]->Fill(leptons[0]->Eta(),weight);
	  oshistos[3][0][stage]->Fill(leptons[1]->Eta(),weight);
	  oshistos[4][0][stage]->Fill(leptons[0]->Phi(),weight);
	  oshistos[5][0][stage]->Fill(leptons[1]->Phi(),weight);
	  oshistos[6][0][stage]->Fill(findMET(muons),weight);
	  oshistos[7][0][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  oshistos[8][0][stage]->Fill((int)jets.size(),weight);
	  oshistos[9][0][stage]->Fill((int)photons.size(),weight);
	  oshistos[10][0][stage]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  oshistos[11][0][stage]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	}
	else if (channels[0] == "em")
	{
	  oshistos[0][1][stage]->Fill(leptons[0]->Pt(),weight);
	  oshistos[1][1][stage]->Fill(leptons[1]->Pt(),weight);
	  oshistos[2][1][stage]->Fill(leptons[0]->Eta(),weight);
	  oshistos[3][1][stage]->Fill(leptons[1]->Eta(),weight);
	  oshistos[4][1][stage]->Fill(leptons[0]->Phi(),weight);
	  oshistos[5][1][stage]->Fill(leptons[1]->Phi(),weight);
	  oshistos[6][1][stage]->Fill(findMET(muons),weight);
	  oshistos[7][1][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  oshistos[8][1][stage]->Fill((int)jets.size(),weight);
	  oshistos[9][1][stage]->Fill((int)photons.size(),weight);
	  oshistos[10][1][stage]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  oshistos[11][1][stage]->Fill(((MTree::Muon*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	}
	else if(channels[0] == "me")
	{
	  oshistos[0][2][stage]->Fill(leptons[0]->Pt(),weight);
	  oshistos[1][2][stage]->Fill(leptons[1]->Pt(),weight);
	  oshistos[2][2][stage]->Fill(leptons[0]->Eta(),weight);
	  oshistos[3][2][stage]->Fill(leptons[1]->Eta(),weight);
	  oshistos[4][2][stage]->Fill(leptons[0]->Phi(),weight);
	  oshistos[5][2][stage]->Fill(leptons[1]->Phi(),weight);
	  oshistos[6][2][stage]->Fill(findMET(muons),weight);
	  oshistos[7][2][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  oshistos[8][2][stage]->Fill((int)jets.size(),weight);
	  oshistos[9][2][stage]->Fill((int)photons.size(),weight);
	  oshistos[10][2][stage]->Fill(((MTree::Muon*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  oshistos[11][2][stage]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	}
	else if (channels[0] == "mm")
	{
	  oshistos[0][3][stage]->Fill(leptons[0]->Pt(),weight);
	  oshistos[1][3][stage]->Fill(leptons[1]->Pt(),weight);
	  oshistos[2][3][stage]->Fill(leptons[0]->Eta(),weight);
	  oshistos[3][3][stage]->Fill(leptons[1]->Eta(),weight);
	  oshistos[4][3][stage]->Fill(leptons[0]->Phi(),weight);
	  oshistos[5][3][stage]->Fill(leptons[1]->Phi(),weight);
	  oshistos[6][3][stage]->Fill(findMET(muons),weight);
	  oshistos[7][3][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  oshistos[8][3][stage]->Fill((int)jets.size(),weight);
	  oshistos[9][3][stage]->Fill((int)photons.size(),weight);
	  oshistos[10][3][stage]->Fill(((MTree::Muon*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  oshistos[11][3][stage]->Fill(((MTree::Muon*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	}
	++stage;
      }

      if (_debug)
      {
	std::cout << "checking dilepton mass" << std::endl;
      }
      TLorentzVector vec = *leptons[0] + *leptons[1];
      if (vec.M() < 5) continue;
      cutNr = record->addEvent(cutNr,weight,"Mll > 5",channels);
      outfile7 << _eventInfo->RunNumber() << ": " << _eventInfo->EventNumber() << std::endl;

      //Now, treat OS and SS differently.
      if (isSS)
      {
	if (_debug)
	{
	  std::cout << "entering ss cuts" << std::endl;
	}
	int cutSS = 0;
	cutSS = bookSS->addEvent(cutSS,weight,"Number of SS events");
	cutSS = bookSS->addEvent(cutSS,weight,"SS by channel",channels);
	if ((vec.M() < 60) && (findMET(muons) < 15))
	{
	  histos[0]->Fill(0.0,weight);
	}

	if (channels[0] == "ee")
	{
	  sshistos[0][0][stage]->Fill(leptons[0]->Pt(),weight);
	  sshistos[1][0][stage]->Fill(leptons[1]->Pt(),weight);
	  sshistos[2][0][stage]->Fill(leptons[0]->Eta(),weight);
	  sshistos[3][0][stage]->Fill(leptons[1]->Eta(),weight);
	  sshistos[4][0][stage]->Fill(leptons[0]->Phi(),weight);
	  sshistos[5][0][stage]->Fill(leptons[1]->Phi(),weight);
	  sshistos[6][0][stage]->Fill(findMET(muons),weight);
	  sshistos[7][0][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  sshistos[8][0][stage]->Fill((int)jets.size(),weight);
	  sshistos[9][0][stage]->Fill((int)photons.size(),weight);
	  sshistos[10][0][stage]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  sshistos[11][0][stage]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);

	}
	else if (channels[0] == "em")
	{
	  sshistos[0][1][stage]->Fill(leptons[0]->Pt(),weight);
	  sshistos[1][1][stage]->Fill(leptons[1]->Pt(),weight);
	  sshistos[2][1][stage]->Fill(leptons[0]->Eta(),weight);
	  sshistos[3][1][stage]->Fill(leptons[1]->Eta(),weight);
	  sshistos[4][1][stage]->Fill(leptons[0]->Phi(),weight);
	  sshistos[5][1][stage]->Fill(leptons[1]->Phi(),weight);
	  sshistos[6][1][stage]->Fill(findMET(muons),weight);
	  sshistos[7][1][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  sshistos[8][1][stage]->Fill((int)jets.size(),weight);
	  sshistos[9][1][stage]->Fill((int)photons.size(),weight);
	  sshistos[10][1][stage]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  sshistos[11][1][stage]->Fill(((MTree::Muon*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	}
	else if(channels[0] == "me")
	{
	  sshistos[0][2][stage]->Fill(leptons[0]->Pt(),weight);
	  sshistos[1][2][stage]->Fill(leptons[1]->Pt(),weight);
	  sshistos[2][2][stage]->Fill(leptons[0]->Eta(),weight);
	  sshistos[3][2][stage]->Fill(leptons[1]->Eta(),weight);
	  sshistos[4][2][stage]->Fill(leptons[0]->Phi(),weight);
	  sshistos[5][2][stage]->Fill(leptons[1]->Phi(),weight);
	  sshistos[6][2][stage]->Fill(findMET(muons),weight);
	  sshistos[7][2][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  sshistos[8][2][stage]->Fill((int)jets.size(),weight);
	  sshistos[9][2][stage]->Fill((int)photons.size(),weight);
	  sshistos[10][2][stage]->Fill(((MTree::Muon*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  sshistos[11][2][stage]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	}
	else if (channels[0] == "mm")
	{
	  sshistos[0][3][stage]->Fill(leptons[0]->Pt(),weight);
	  sshistos[1][3][stage]->Fill(leptons[1]->Pt(),weight);
	  sshistos[2][3][stage]->Fill(leptons[0]->Eta(),weight);
	  sshistos[3][3][stage]->Fill(leptons[1]->Eta(),weight);
	  sshistos[4][3][stage]->Fill(leptons[0]->Phi(),weight);
	  sshistos[5][3][stage]->Fill(leptons[1]->Phi(),weight);
	  sshistos[6][3][stage]->Fill(findMET(muons),weight);
	  sshistos[7][3][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  sshistos[8][3][stage]->Fill((int)jets.size(),weight);
	  sshistos[9][3][stage]->Fill((int)photons.size(),weight);
	  sshistos[10][3][stage]->Fill(((MTree::Muon*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  sshistos[11][3][stage]->Fill(((MTree::Muon*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	}
	++stage;

	orderByPt(jets);
	if (jets.size() < 2) continue;
	if (jets[1]->Pt() < 50) continue;
	cutSS = bookSS->addEvent(cutSS,weight,"2 jets with pT > 50",channels);


	if (channels[0] == "ee")
	{
	  sshistos[0][0][stage]->Fill(leptons[0]->Pt(),weight);
	  sshistos[1][0][stage]->Fill(leptons[1]->Pt(),weight);
	  sshistos[2][0][stage]->Fill(leptons[0]->Eta(),weight);
	  sshistos[3][0][stage]->Fill(leptons[1]->Eta(),weight);
	  sshistos[4][0][stage]->Fill(leptons[0]->Phi(),weight);
	  sshistos[5][0][stage]->Fill(leptons[1]->Phi(),weight);
	  sshistos[6][0][stage]->Fill(findMET(muons),weight);
	  sshistos[7][0][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  sshistos[8][0][stage]->Fill((int)jets.size(),weight);
	  sshistos[9][0][stage]->Fill((int)photons.size(),weight);
	  sshistos[10][0][stage]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  sshistos[11][0][stage]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	}
	else if (channels[0] == "em")
	{
	  sshistos[0][1][stage]->Fill(leptons[0]->Pt(),weight);
	  sshistos[1][1][stage]->Fill(leptons[1]->Pt(),weight);
	  sshistos[2][1][stage]->Fill(leptons[0]->Eta(),weight);
	  sshistos[3][1][stage]->Fill(leptons[1]->Eta(),weight);
	  sshistos[4][1][stage]->Fill(leptons[0]->Phi(),weight);
	  sshistos[5][1][stage]->Fill(leptons[1]->Phi(),weight);
	  sshistos[6][1][stage]->Fill(findMET(muons),weight);
	  sshistos[7][1][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  sshistos[8][1][stage]->Fill((int)jets.size(),weight);
	  sshistos[9][1][stage]->Fill((int)photons.size(),weight);
	  sshistos[10][1][stage]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  sshistos[11][1][stage]->Fill(((MTree::Muon*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	}
	else if(channels[0] == "me")
	{
	  sshistos[0][2][stage]->Fill(leptons[0]->Pt(),weight);
	  sshistos[1][2][stage]->Fill(leptons[1]->Pt(),weight);
	  sshistos[2][2][stage]->Fill(leptons[0]->Eta(),weight);
	  sshistos[3][2][stage]->Fill(leptons[1]->Eta(),weight);
	  sshistos[4][2][stage]->Fill(leptons[0]->Phi(),weight);
	  sshistos[5][2][stage]->Fill(leptons[1]->Phi(),weight);
	  sshistos[6][2][stage]->Fill(findMET(muons),weight);
	  sshistos[7][2][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  sshistos[8][2][stage]->Fill((int)jets.size(),weight);
	  sshistos[9][2][stage]->Fill((int)photons.size(),weight);
	  sshistos[10][2][stage]->Fill(((MTree::Muon*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  sshistos[11][2][stage]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	}
	else if (channels[0] == "mm")
	{
	  sshistos[0][3][stage]->Fill(leptons[0]->Pt(),weight);
	  sshistos[1][3][stage]->Fill(leptons[1]->Pt(),weight);
	  sshistos[2][3][stage]->Fill(leptons[0]->Eta(),weight);
	  sshistos[3][3][stage]->Fill(leptons[1]->Eta(),weight);
	  sshistos[4][3][stage]->Fill(leptons[0]->Phi(),weight);
	  sshistos[5][3][stage]->Fill(leptons[1]->Phi(),weight);
	  sshistos[6][3][stage]->Fill(findMET(muons),weight);
	  sshistos[7][3][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  sshistos[8][3][stage]->Fill((int)jets.size(),weight);
	  sshistos[9][3][stage]->Fill((int)photons.size(),weight);
	  sshistos[10][3][stage]->Fill(((MTree::Muon*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  sshistos[11][3][stage]->Fill(((MTree::Muon*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	}
	++stage;

	if (findMET(muons) < 80) continue;
	cutSS = bookSS->addEvent(cutSS,weight, "MET > 80 GeV",channels);
	if (!isMC)
	{
	  _eventInfo->PrintData(outfile9);
	  for (int i = 0; i != (int)leptons.size(); ++i)
	  {
	    if (leptons[i]->LeptonPdgID() == 11)
	    {
	      ((MTree::Electron*)leptons[i])->PrintData(outfile9);
	    }
	    else
	    {
	      ((MTree::Muon*)leptons[i])->PrintData(outfile9);
	    }
	  }
	  for (int i = 0; i !=  (int)jets.size(); ++i)
	  {
	    jets[i]->PrintData(outfile9);
	  }
	  outfile9 << "MET = " << findMET(muons) << std::endl;
	  outfile9 << "mll = " << (*leptons[0]+*leptons[1]).M() << std::endl;
	}

	//Add Matrix Method histograms
	orderByPt(leptons);
	double effS = getSigEff(leptons[0]->LeptonPdgID(),leptons[0]->Pt(),leptons[0]->Eta(),leptons[1]->LeptonPdgID(),leptons[1]->Pt(),leptons[1]->Eta());
	double effB = getBackEff(leptons[0]->LeptonPdgID(),leptons[0]->Pt(),leptons[0]->Eta(),leptons[1]->LeptonPdgID(),leptons[1]->Pt(),leptons[1]->Eta());
	if (!isMC)
	{
	  outfile9 << "Signal efficiency = " << effS << std::endl;
	  outfile9 << "Background efficiency = " << effB << std::endl << std::endl;
	}

	double sigL = effB/(effB-effS);
	double backL = effS/(effS-effB);

	if (channels[0] == "ee")
	{
	  sshistos[0][0][stage]->Fill(leptons[0]->Pt(),weight);
	  sshistos[1][0][stage]->Fill(leptons[1]->Pt(),weight);
	  sshistos[2][0][stage]->Fill(leptons[0]->Eta(),weight);
	  sshistos[3][0][stage]->Fill(leptons[1]->Eta(),weight);
	  sshistos[4][0][stage]->Fill(leptons[0]->Phi(),weight);
	  sshistos[5][0][stage]->Fill(leptons[1]->Phi(),weight);
	  sshistos[6][0][stage]->Fill(findMET(muons),weight);
	  sshistos[7][0][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  sshistos[8][0][stage]->Fill((int)jets.size(),weight);
	  sshistos[9][0][stage]->Fill((int)photons.size(),weight);
	  sshistos[10][0][stage]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  sshistos[11][0][stage]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);

	  MMhistos[0][0][1][0][0]->Fill(findMET(muons),weight*sigL);
	  MMhistos[0][0][1][1][0]->Fill(findMET(muons),weight*backL);
	  MMhistos[1][0][1][0][0]->Fill((*leptons[0]+*leptons[1]).M(),weight*sigL);
	  MMhistos[1][0][1][1][0]->Fill((*leptons[0]+*leptons[1]).M(),weight*backL);
	  MMhistos[2][0][1][0][0]->Fill((int)jets.size(),weight*sigL);
	  MMhistos[2][0][1][1][0]->Fill((int)jets.size(),weight*backL);
	}
	else if (channels[0] == "em")
	{
	  sshistos[0][1][stage]->Fill(leptons[0]->Pt(),weight);
	  sshistos[1][1][stage]->Fill(leptons[1]->Pt(),weight);
	  sshistos[2][1][stage]->Fill(leptons[0]->Eta(),weight);
	  sshistos[3][1][stage]->Fill(leptons[1]->Eta(),weight);
	  sshistos[4][1][stage]->Fill(leptons[0]->Phi(),weight);
	  sshistos[5][1][stage]->Fill(leptons[1]->Phi(),weight);
	  sshistos[6][1][stage]->Fill(findMET(muons),weight);
	  sshistos[7][1][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  sshistos[8][1][stage]->Fill((int)jets.size(),weight);
	  sshistos[9][1][stage]->Fill((int)photons.size(),weight);
	  sshistos[10][1][stage]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  sshistos[11][1][stage]->Fill(((MTree::Muon*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);

	  MMhistos[0][1][1][0][0]->Fill(findMET(muons),weight*sigL);
	  MMhistos[0][1][1][1][0]->Fill(findMET(muons),weight*backL);
	  MMhistos[1][1][1][0][0]->Fill((*leptons[0]+*leptons[1]).M(),weight*sigL);
	  MMhistos[1][1][1][1][0]->Fill((*leptons[0]+*leptons[1]).M(),weight*backL);
	  MMhistos[2][1][1][0][0]->Fill((int)jets.size(),weight*sigL);
	  MMhistos[2][1][1][1][0]->Fill((int)jets.size(),weight*backL);
	}
	else if(channels[0] == "me")
	{
	  sshistos[0][2][stage]->Fill(leptons[0]->Pt(),weight);
	  sshistos[1][2][stage]->Fill(leptons[1]->Pt(),weight);
	  sshistos[2][2][stage]->Fill(leptons[0]->Eta(),weight);
	  sshistos[3][2][stage]->Fill(leptons[1]->Eta(),weight);
	  sshistos[4][2][stage]->Fill(leptons[0]->Phi(),weight);
	  sshistos[5][2][stage]->Fill(leptons[1]->Phi(),weight);
	  sshistos[6][2][stage]->Fill(findMET(muons),weight);
	  sshistos[7][2][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  sshistos[8][2][stage]->Fill((int)jets.size(),weight);
	  sshistos[9][2][stage]->Fill((int)photons.size(),weight);
	  sshistos[10][2][stage]->Fill(((MTree::Muon*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  sshistos[11][2][stage]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);

	  MMhistos[0][2][1][0][0]->Fill(findMET(muons),weight*sigL);
	  MMhistos[0][2][1][1][0]->Fill(findMET(muons),weight*backL);
	  MMhistos[1][2][1][0][0]->Fill((*leptons[0]+*leptons[1]).M(),weight*sigL);
	  MMhistos[1][2][1][1][0]->Fill((*leptons[0]+*leptons[1]).M(),weight*backL);
	  MMhistos[2][2][1][0][0]->Fill((int)jets.size(),weight*sigL);
	  MMhistos[2][2][1][1][0]->Fill((int)jets.size(),weight*backL);
	}
	else if (channels[0] == "mm")
	{
	  sshistos[0][3][stage]->Fill(leptons[0]->Pt(),weight);
	  sshistos[1][3][stage]->Fill(leptons[1]->Pt(),weight);
	  sshistos[2][3][stage]->Fill(leptons[0]->Eta(),weight);
	  sshistos[3][3][stage]->Fill(leptons[1]->Eta(),weight);
	  sshistos[4][3][stage]->Fill(leptons[0]->Phi(),weight);
	  sshistos[5][3][stage]->Fill(leptons[1]->Phi(),weight);
	  sshistos[6][3][stage]->Fill(findMET(muons),weight);
	  sshistos[7][3][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  sshistos[8][3][stage]->Fill((int)jets.size(),weight);
	  sshistos[9][3][stage]->Fill((int)photons.size(),weight);
	  sshistos[10][3][stage]->Fill(((MTree::Muon*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  sshistos[11][3][stage]->Fill(((MTree::Muon*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);

	  MMhistos[0][3][1][0][0]->Fill(findMET(muons),weight*sigL);
	  MMhistos[0][3][1][1][0]->Fill(findMET(muons),weight*backL);
	  MMhistos[1][3][1][0][0]->Fill((*leptons[0]+*leptons[1]).M(),weight*sigL);
	  MMhistos[1][3][1][1][0]->Fill((*leptons[0]+*leptons[1]).M(),weight*backL);
	  MMhistos[2][3][1][0][0]->Fill((int)jets.size(),weight*sigL);
	  MMhistos[2][3][1][1][0]->Fill((int)jets.size(),weight*backL);
	}
	++stage;

//Put cut for matrix method here.
	bool twoGoodOnes = true;
	for (int i = 0; i != (int)leptons.size(); ++i)
	{
	  double iso;
	  if (leptons[i]->LeptonPdgID() == 11)
	  {
	    MTree::Electron* elec = (MTree::Electron*)leptons[i];
	    iso = elec->ETCone40()/elec->Pt();
	    histos2[0]->Fill(elec->Pt(),elec->Eta(),weight);
	  }
	  else
	  {
	    MTree::Muon* muon = (MTree::Muon*)leptons[i];
	    iso = muon->ETCone40()/muon->Pt();
	    histos2[2]->Fill(muon->Pt(),muon->Eta(),weight);
	  }
	  if (iso > 0.1)
	  {
	    twoGoodOnes = false;
	  }
	  else
	  {
	    if (leptons[i]->LeptonPdgID() == 11)
	    {
	      histos2[1]->Fill(leptons[i]->Pt(),leptons[i]->Eta(),weight);
	    }
	    else
	    {
	      histos2[3]->Fill(leptons[i]->Pt(),leptons[i]->Eta(),weight);
	    }
	  }
	}
	cutSS = bookSS->addEvent(cutSS,weight,"Two Isolated Leptons",channels);
	if (!isMC)
	{
	  _eventInfo->PrintData(outfile11);
	}
	double sigT = 1/(effS-effB);
	double backT = 1/(effB-effS);

	if (channels[0] == "ee")
	{
	  sshistos[0][0][stage]->Fill(leptons[0]->Pt(),weight);
	  sshistos[1][0][stage]->Fill(leptons[1]->Pt(),weight);
	  sshistos[2][0][stage]->Fill(leptons[0]->Eta(),weight);
	  sshistos[3][0][stage]->Fill(leptons[1]->Eta(),weight);
	  sshistos[4][0][stage]->Fill(leptons[0]->Phi(),weight);
	  sshistos[5][0][stage]->Fill(leptons[1]->Phi(),weight);
	  sshistos[6][0][stage]->Fill(findMET(muons),weight);
	  sshistos[7][0][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  sshistos[8][0][stage]->Fill((int)jets.size(),weight);
	  sshistos[9][0][stage]->Fill((int)photons.size(),weight);
	  sshistos[10][0][stage]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  sshistos[11][0][stage]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);

	  MMhistos[0][0][1][0][0]->Fill(findMET(muons),weight*sigT);
	  MMhistos[0][0][1][1][0]->Fill(findMET(muons),weight*backT);
	  MMhistos[1][0][1][0][0]->Fill((*leptons[0]+*leptons[1]).M(),weight*sigT);
	  MMhistos[1][0][1][1][0]->Fill((*leptons[0]+*leptons[1]).M(),weight*backT);
	  MMhistos[2][0][1][0][0]->Fill((int)jets.size(),weight*sigT);
	  MMhistos[2][0][1][1][0]->Fill((int)jets.size(),weight*backT);
	}
	else if (channels[0] == "em")
	{
	  sshistos[0][1][stage]->Fill(leptons[0]->Pt(),weight);
	  sshistos[1][1][stage]->Fill(leptons[1]->Pt(),weight);
	  sshistos[2][1][stage]->Fill(leptons[0]->Eta(),weight);
	  sshistos[3][1][stage]->Fill(leptons[1]->Eta(),weight);
	  sshistos[4][1][stage]->Fill(leptons[0]->Phi(),weight);
	  sshistos[5][1][stage]->Fill(leptons[1]->Phi(),weight);
	  sshistos[6][1][stage]->Fill(findMET(muons),weight);
	  sshistos[7][1][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  sshistos[8][1][stage]->Fill((int)jets.size(),weight);
	  sshistos[9][1][stage]->Fill((int)photons.size(),weight);
	  sshistos[10][1][stage]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  sshistos[11][1][stage]->Fill(((MTree::Muon*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);

	  MMhistos[0][1][1][0][0]->Fill(findMET(muons),weight*sigT);
	  MMhistos[0][1][1][1][0]->Fill(findMET(muons),weight*backT);
	  MMhistos[1][1][1][0][0]->Fill((*leptons[0]+*leptons[1]).M(),weight*sigT);
	  MMhistos[1][1][1][1][0]->Fill((*leptons[0]+*leptons[1]).M(),weight*backT);
	  MMhistos[2][1][1][0][0]->Fill((int)jets.size(),weight*sigT);
	  MMhistos[2][1][1][1][0]->Fill((int)jets.size(),weight*backT);
	}
	else if(channels[0] == "me")
	{
	  sshistos[0][2][stage]->Fill(leptons[0]->Pt(),weight);
	  sshistos[1][2][stage]->Fill(leptons[1]->Pt(),weight);
	  sshistos[2][2][stage]->Fill(leptons[0]->Eta(),weight);
	  sshistos[3][2][stage]->Fill(leptons[1]->Eta(),weight);
	  sshistos[4][2][stage]->Fill(leptons[0]->Phi(),weight);
	  sshistos[5][2][stage]->Fill(leptons[1]->Phi(),weight);
	  sshistos[6][2][stage]->Fill(findMET(muons),weight);
	  sshistos[7][2][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  sshistos[8][2][stage]->Fill((int)jets.size(),weight);
	  sshistos[9][2][stage]->Fill((int)photons.size(),weight);
	  sshistos[10][2][stage]->Fill(((MTree::Muon*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  sshistos[11][2][stage]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);

	  MMhistos[0][2][1][0][0]->Fill(findMET(muons),weight*sigT);
	  MMhistos[0][2][1][1][0]->Fill(findMET(muons),weight*backT);
	  MMhistos[1][2][1][0][0]->Fill((*leptons[0]+*leptons[1]).M(),weight*sigT);
	  MMhistos[1][2][1][1][0]->Fill((*leptons[0]+*leptons[1]).M(),weight*backT);
	  MMhistos[2][2][1][0][0]->Fill((int)jets.size(),weight*sigT);
	  MMhistos[2][2][1][1][0]->Fill((int)jets.size(),weight*backT);
	}
	else if (channels[0] == "mm")
	{
	  sshistos[0][3][stage]->Fill(leptons[0]->Pt(),weight);
	  sshistos[1][3][stage]->Fill(leptons[1]->Pt(),weight);
	  sshistos[2][3][stage]->Fill(leptons[0]->Eta(),weight);
	  sshistos[3][3][stage]->Fill(leptons[1]->Eta(),weight);
	  sshistos[4][3][stage]->Fill(leptons[0]->Phi(),weight);
	  sshistos[5][3][stage]->Fill(leptons[1]->Phi(),weight);
	  sshistos[6][3][stage]->Fill(findMET(muons),weight);
	  sshistos[7][3][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  sshistos[8][3][stage]->Fill((int)jets.size(),weight);
	  sshistos[9][3][stage]->Fill((int)photons.size(),weight);
	  sshistos[10][3][stage]->Fill(((MTree::Muon*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  sshistos[11][3][stage]->Fill(((MTree::Muon*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);

	  MMhistos[0][3][1][0][0]->Fill(findMET(muons),weight*sigT);
	  MMhistos[0][3][1][1][0]->Fill(findMET(muons),weight*backT);
	  MMhistos[1][3][1][0][0]->Fill((*leptons[0]+*leptons[1]).M(),weight*sigT);
	  MMhistos[1][3][1][1][0]->Fill((*leptons[0]+*leptons[1]).M(),weight*backT);
	  MMhistos[2][3][1][0][0]->Fill((int)jets.size(),weight*sigT);
	  MMhistos[2][3][1][1][0]->Fill((int)jets.size(),weight*backT);
	}
	++stage;
      }
      else
      {
	if (_debug)
	{
	  std::cout << "entering os part of cutflow" << std::endl;
	}
	int cutOS = 0;
	cutOS = bookOS->addEvent(cutOS,weight,"Number of OS events");
	cutOS = bookOS->addEvent(cutOS,weight,"OS dudes by channel",channels);

	if (channels[0] == "ee")
	{
	  oshistos[0][0][stage]->Fill(leptons[0]->Pt(),weight);
	  oshistos[1][0][stage]->Fill(leptons[1]->Pt(),weight);
	  oshistos[2][0][stage]->Fill(leptons[0]->Eta(),weight);
	  oshistos[3][0][stage]->Fill(leptons[1]->Eta(),weight);
	  oshistos[4][0][stage]->Fill(leptons[0]->Phi(),weight);
	  oshistos[5][0][stage]->Fill(leptons[1]->Phi(),weight);
	  oshistos[6][0][stage]->Fill(findMET(muons),weight);
	  oshistos[7][0][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  oshistos[8][0][stage]->Fill((int)jets.size(),weight);
	  oshistos[9][0][stage]->Fill((int)photons.size(),weight);
	  oshistos[10][0][stage]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  oshistos[11][0][stage]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	}
	else if (channels[0] == "em")
	{
	  oshistos[0][1][stage]->Fill(leptons[0]->Pt(),weight);
	  oshistos[1][1][stage]->Fill(leptons[1]->Pt(),weight);
	  oshistos[2][1][stage]->Fill(leptons[0]->Eta(),weight);
	  oshistos[3][1][stage]->Fill(leptons[1]->Eta(),weight);
	  oshistos[4][1][stage]->Fill(leptons[0]->Phi(),weight);
	  oshistos[5][1][stage]->Fill(leptons[1]->Phi(),weight);
	  oshistos[6][1][stage]->Fill(findMET(muons),weight);
	  oshistos[7][1][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  oshistos[8][1][stage]->Fill((int)jets.size(),weight);
	  oshistos[9][1][stage]->Fill((int)photons.size(),weight);
	  oshistos[10][1][stage]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  oshistos[11][1][stage]->Fill(((MTree::Muon*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	}
	else if(channels[0] == "me")
	{
	  oshistos[0][2][stage]->Fill(leptons[0]->Pt(),weight);
	  oshistos[1][2][stage]->Fill(leptons[1]->Pt(),weight);
	  oshistos[2][2][stage]->Fill(leptons[0]->Eta(),weight);
	  oshistos[3][2][stage]->Fill(leptons[1]->Eta(),weight);
	  oshistos[4][2][stage]->Fill(leptons[0]->Phi(),weight);
	  oshistos[5][2][stage]->Fill(leptons[1]->Phi(),weight);
	  oshistos[6][2][stage]->Fill(findMET(muons),weight);
	  oshistos[7][2][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  oshistos[8][2][stage]->Fill((int)jets.size(),weight);
	  oshistos[9][2][stage]->Fill((int)photons.size(),weight);
	  oshistos[10][2][stage]->Fill(((MTree::Muon*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  oshistos[11][2][stage]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	}
	else if (channels[0] == "mm")
	{
	  oshistos[0][3][stage]->Fill(leptons[0]->Pt(),weight);
	  oshistos[1][3][stage]->Fill(leptons[1]->Pt(),weight);
	  oshistos[2][3][stage]->Fill(leptons[0]->Eta(),weight);
	  oshistos[3][3][stage]->Fill(leptons[1]->Eta(),weight);
	  oshistos[4][3][stage]->Fill(leptons[0]->Phi(),weight);
	  oshistos[5][3][stage]->Fill(leptons[1]->Phi(),weight);
	  oshistos[6][3][stage]->Fill(findMET(muons),weight);
	  oshistos[7][3][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  oshistos[8][3][stage]->Fill((int)jets.size(),weight);
	  oshistos[9][3][stage]->Fill((int)photons.size(),weight);
	  oshistos[10][3][stage]->Fill(((MTree::Muon*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  oshistos[11][3][stage]->Fill(((MTree::Muon*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	}
	++stage;

	if (_debug)
	{
	  std::cout << "entering loop over met" << std::endl;
	}
// 	int m = 0;

	if (findMET(muons) > 30) 
        {
	  orderByPt(leptons);
	  double effS = getSigEff(leptons[0]->LeptonPdgID(),leptons[0]->Pt(),leptons[0]->Eta(),leptons[1]->LeptonPdgID(),leptons[1]->Pt(),leptons[1]->Eta());
	  double effB = getBackEff(leptons[0]->LeptonPdgID(),leptons[0]->Pt(),leptons[0]->Eta(),leptons[1]->LeptonPdgID(),leptons[1]->Pt(),leptons[1]->Eta());

	  double sigL = effB/(effB-effS);
	  double backL = effS/(effS-effB);

	  //Fill first set of plots
	  if (channels[0] == "ee")
	  {
	    oshistos[0][0][2]->Fill(leptons[0]->Pt(),weight);
	    oshistos[1][0][2]->Fill(leptons[1]->Pt(),weight);
	    oshistos[2][0][2]->Fill(leptons[0]->Eta(),weight);
	    oshistos[3][0][2]->Fill(leptons[1]->Eta(),weight);
	    oshistos[4][0][2]->Fill(leptons[0]->Phi(),weight);
	    oshistos[5][0][2]->Fill(leptons[1]->Phi(),weight);
	    oshistos[6][0][2]->Fill(findMET(muons),weight);
	    oshistos[7][0][2]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	    oshistos[8][0][2]->Fill((int)jets.size(),weight);
	    oshistos[9][0][2]->Fill((int)photons.size(),weight);
	    oshistos[10][0][2]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	    oshistos[11][0][2]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);

	    MMhistos[0][0][0][0][0]->Fill(findMET(muons),weight*sigL);
	    MMhistos[0][0][0][1][0]->Fill(findMET(muons),weight*backL);
	    MMhistos[1][0][0][0][0]->Fill((*leptons[0]+*leptons[1]).M(),weight*sigL);
	    MMhistos[1][0][0][1][0]->Fill((*leptons[0]+*leptons[1]).M(),weight*backL);
	    MMhistos[2][0][0][0][0]->Fill((int)jets.size(),weight*sigL);
	    MMhistos[2][0][0][1][0]->Fill((int)jets.size(),weight*backL);
	  }
	  else if (channels[0] == "em")
	  {
	    oshistos[0][1][2]->Fill(leptons[0]->Pt(),weight);
	    oshistos[1][1][2]->Fill(leptons[1]->Pt(),weight);
	    oshistos[2][1][2]->Fill(leptons[0]->Eta(),weight);
	    oshistos[3][1][2]->Fill(leptons[1]->Eta(),weight);
	    oshistos[4][1][2]->Fill(leptons[0]->Phi(),weight);
	    oshistos[5][1][2]->Fill(leptons[1]->Phi(),weight);
	    oshistos[6][1][2]->Fill(findMET(muons),weight);
	    oshistos[7][1][2]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	    oshistos[8][1][2]->Fill((int)jets.size(),weight);
	    oshistos[9][1][2]->Fill((int)photons.size(),weight);
	    oshistos[10][1][2]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	    oshistos[11][1][2]->Fill(((MTree::Muon*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);

	    MMhistos[0][1][0][0][0]->Fill(findMET(muons),weight*sigL);
	    MMhistos[0][1][0][1][0]->Fill(findMET(muons),weight*backL);
	    MMhistos[1][1][0][0][0]->Fill((*leptons[0]+*leptons[1]).M(),weight*sigL);
	    MMhistos[1][1][0][1][0]->Fill((*leptons[0]+*leptons[1]).M(),weight*backL);
	    MMhistos[2][1][0][0][0]->Fill((int)jets.size(),weight*sigL);
	    MMhistos[2][1][0][1][0]->Fill((int)jets.size(),weight*backL);
	  }
	  else if(channels[0] == "me")
	  {
	    oshistos[0][2][2]->Fill(leptons[0]->Pt(),weight);
	    oshistos[1][2][2]->Fill(leptons[1]->Pt(),weight);
	    oshistos[2][2][2]->Fill(leptons[0]->Eta(),weight);
	    oshistos[3][2][2]->Fill(leptons[1]->Eta(),weight);
	    oshistos[4][2][2]->Fill(leptons[0]->Phi(),weight);
	    oshistos[5][2][2]->Fill(leptons[1]->Phi(),weight);
	    oshistos[6][2][2]->Fill(findMET(muons),weight);
	    oshistos[7][2][2]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	    oshistos[8][2][2]->Fill((int)jets.size(),weight);
	    oshistos[9][2][2]->Fill((int)photons.size(),weight);
	    oshistos[10][2][2]->Fill(((MTree::Muon*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	    oshistos[11][2][2]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);

	    MMhistos[0][2][0][0][0]->Fill(findMET(muons),weight*sigL);
	    MMhistos[0][2][0][1][0]->Fill(findMET(muons),weight*backL);
	    MMhistos[1][2][0][0][0]->Fill((*leptons[0]+*leptons[1]).M(),weight*sigL);
	    MMhistos[1][2][0][1][0]->Fill((*leptons[0]+*leptons[1]).M(),weight*backL);
	    MMhistos[2][2][0][0][0]->Fill((int)jets.size(),weight*sigL);
	    MMhistos[2][2][0][1][0]->Fill((int)jets.size(),weight*backL);
	  }
	  else if (channels[0] == "mm")
	  {
	    oshistos[0][3][2]->Fill(leptons[0]->Pt(),weight);
	    oshistos[1][3][2]->Fill(leptons[1]->Pt(),weight);
	    oshistos[2][3][2]->Fill(leptons[0]->Eta(),weight);
	    oshistos[3][3][2]->Fill(leptons[1]->Eta(),weight);
	    oshistos[4][3][2]->Fill(leptons[0]->Phi(),weight);
	    oshistos[5][3][2]->Fill(leptons[1]->Phi(),weight);
	    oshistos[6][3][2]->Fill(findMET(muons),weight);
	    oshistos[7][3][2]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	    oshistos[8][3][2]->Fill((int)jets.size(),weight);
	    oshistos[9][3][2]->Fill((int)photons.size(),weight);
	    oshistos[10][3][2]->Fill(((MTree::Muon*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	    oshistos[11][3][2]->Fill(((MTree::Muon*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);

	    MMhistos[0][3][0][0][0]->Fill(findMET(muons),weight*sigL);
	    MMhistos[0][3][0][1][0]->Fill(findMET(muons),weight*backL);
	    MMhistos[1][3][0][0][0]->Fill((*leptons[0]+*leptons[1]).M(),weight*sigL);
	    MMhistos[1][3][0][1][0]->Fill((*leptons[0]+*leptons[1]).M(),weight*backL);
	    MMhistos[2][3][0][0][0]->Fill((int)jets.size(),weight*sigL);
	    MMhistos[2][3][0][1][0]->Fill((int)jets.size(),weight*backL);
	  }
	  //Calculate isolations of the leptons
	  double iso1 = 100;
	  double iso2 = 100;

	  if (leptons[0]->LeptonPdgID() == 11)
	  {
	    iso1 = ((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt();
	  }
	  else
	  {
	    iso1 = ((MTree::Muon*)leptons[0])->ETCone40()/leptons[0]->Pt();
	  }

	  if (leptons[1]->LeptonPdgID() == 11)
	  {
	    iso2 = ((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt();
	  }
	  else
	  {
	    iso2 = ((MTree::Muon*)leptons[1])->ETCone40()/leptons[1]->Pt();
	  }

	  //Check isolation of first lepton and fill second set
	  if (iso1 < 0.1)
	  {
	    if (channels[0] == "ee")
	    {
	      oshistos[0][0][3]->Fill(leptons[0]->Pt(),weight);
	      oshistos[1][0][3]->Fill(leptons[1]->Pt(),weight);
	      oshistos[2][0][3]->Fill(leptons[0]->Eta(),weight);
	      oshistos[3][0][3]->Fill(leptons[1]->Eta(),weight);
	      oshistos[4][0][3]->Fill(leptons[0]->Phi(),weight);
	      oshistos[5][0][3]->Fill(leptons[1]->Phi(),weight);
	      oshistos[6][0][3]->Fill(findMET(muons),weight);
	      oshistos[7][0][3]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	      oshistos[8][0][3]->Fill((int)jets.size(),weight);
	      oshistos[9][0][3]->Fill((int)photons.size(),weight);
	      oshistos[10][0][3]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	      oshistos[11][0][3]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	    }
	    else if (channels[0] == "em")
	    {
	      oshistos[0][1][3]->Fill(leptons[0]->Pt(),weight);
	      oshistos[1][1][3]->Fill(leptons[1]->Pt(),weight);
	      oshistos[2][1][3]->Fill(leptons[0]->Eta(),weight);
	      oshistos[3][1][3]->Fill(leptons[1]->Eta(),weight);
	      oshistos[4][1][3]->Fill(leptons[0]->Phi(),weight);
	      oshistos[5][1][3]->Fill(leptons[1]->Phi(),weight);
	      oshistos[6][1][3]->Fill(findMET(muons),weight);
	      oshistos[7][1][3]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	      oshistos[8][1][3]->Fill((int)jets.size(),weight);
	      oshistos[9][1][3]->Fill((int)photons.size(),weight);
	      oshistos[10][1][3]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	      oshistos[11][1][3]->Fill(((MTree::Muon*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	    }
	    else if(channels[0] == "me")
	    {
	      oshistos[0][2][3]->Fill(leptons[0]->Pt(),weight);
	      oshistos[1][2][3]->Fill(leptons[1]->Pt(),weight);
	      oshistos[2][2][3]->Fill(leptons[0]->Eta(),weight);
	      oshistos[3][2][3]->Fill(leptons[1]->Eta(),weight);
	      oshistos[4][2][3]->Fill(leptons[0]->Phi(),weight);
	      oshistos[5][2][3]->Fill(leptons[1]->Phi(),weight);
	      oshistos[6][2][3]->Fill(findMET(muons),weight);
	      oshistos[7][2][3]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	      oshistos[8][2][3]->Fill((int)jets.size(),weight);
	      oshistos[9][2][3]->Fill((int)photons.size(),weight);
	      oshistos[10][2][3]->Fill(((MTree::Muon*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	      oshistos[11][2][3]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	    }
	    else if (channels[0] == "mm")
	    {
	      oshistos[0][3][3]->Fill(leptons[0]->Pt(),weight);
	      oshistos[1][3][3]->Fill(leptons[1]->Pt(),weight);
	      oshistos[2][3][3]->Fill(leptons[0]->Eta(),weight);
	      oshistos[3][3][3]->Fill(leptons[1]->Eta(),weight);
	      oshistos[4][3][3]->Fill(leptons[0]->Phi(),weight);
	      oshistos[5][3][3]->Fill(leptons[1]->Phi(),weight);
	      oshistos[6][3][3]->Fill(findMET(muons),weight);
	      oshistos[7][3][3]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	      oshistos[8][3][3]->Fill((int)jets.size(),weight);
	      oshistos[9][3][3]->Fill((int)photons.size(),weight);
	      oshistos[10][3][3]->Fill(((MTree::Muon*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	      oshistos[11][3][3]->Fill(((MTree::Muon*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	    }
	  }

	  //Check isolation of second lepton and fill third set
	  double sigT = 1/(effS-effB);
	  double backT = 1/(effB-effS);
	  if ((iso1 < 0.1) && (iso2 < 0.1))
	  {
	    if (channels[0] == "ee")
	    {
	      oshistos[0][0][4]->Fill(leptons[0]->Pt(),weight);
	      oshistos[1][0][4]->Fill(leptons[1]->Pt(),weight);
	      oshistos[2][0][4]->Fill(leptons[0]->Eta(),weight);
	      oshistos[3][0][4]->Fill(leptons[1]->Eta(),weight);
	      oshistos[4][0][4]->Fill(leptons[0]->Phi(),weight);
	      oshistos[5][0][4]->Fill(leptons[1]->Phi(),weight);
	      oshistos[6][0][4]->Fill(findMET(muons),weight);
	      oshistos[7][0][4]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	      oshistos[8][0][4]->Fill((int)jets.size(),weight);
	      oshistos[9][0][4]->Fill((int)photons.size(),weight);
	      oshistos[10][0][4]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	      oshistos[11][0][4]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);

	      MMhistos[0][0][0][0][0]->Fill(findMET(muons),weight*sigT);
	      MMhistos[0][0][0][1][0]->Fill(findMET(muons),weight*backT);
	      MMhistos[1][0][0][0][0]->Fill((*leptons[0]+*leptons[1]).M(),weight*sigT);
	      MMhistos[1][0][0][1][0]->Fill((*leptons[0]+*leptons[1]).M(),weight*backT);
	      MMhistos[2][0][0][0][0]->Fill((int)jets.size(),weight*sigT);
	      MMhistos[2][0][0][1][0]->Fill((int)jets.size(),weight*backT);
	    }
	    else if (channels[0] == "em")
	    {
	      oshistos[0][1][4]->Fill(leptons[0]->Pt(),weight);
	      oshistos[1][1][4]->Fill(leptons[1]->Pt(),weight);
	      oshistos[2][1][4]->Fill(leptons[0]->Eta(),weight);
	      oshistos[3][1][4]->Fill(leptons[1]->Eta(),weight);
	      oshistos[4][1][4]->Fill(leptons[0]->Phi(),weight);
	      oshistos[5][1][4]->Fill(leptons[1]->Phi(),weight);
	      oshistos[6][1][4]->Fill(findMET(muons),weight);
	      oshistos[7][1][4]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	      oshistos[8][1][4]->Fill((int)jets.size(),weight);
	      oshistos[9][1][4]->Fill((int)photons.size(),weight);
	      oshistos[10][1][4]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	      oshistos[11][1][4]->Fill(((MTree::Muon*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);

	      MMhistos[0][1][0][0][0]->Fill(findMET(muons),weight*sigT);
	      MMhistos[0][1][0][1][0]->Fill(findMET(muons),weight*backT);
	      MMhistos[1][1][0][0][0]->Fill((*leptons[0]+*leptons[1]).M(),weight*sigT);
	      MMhistos[1][1][0][1][0]->Fill((*leptons[0]+*leptons[1]).M(),weight*backT);
	      MMhistos[2][1][0][0][0]->Fill((int)jets.size(),weight*sigT);
	      MMhistos[2][1][0][1][0]->Fill((int)jets.size(),weight*backT);
	    }
	    else if(channels[0] == "me")
	    {
	      oshistos[0][2][4]->Fill(leptons[0]->Pt(),weight);
	      oshistos[1][2][4]->Fill(leptons[1]->Pt(),weight);
	      oshistos[2][2][4]->Fill(leptons[0]->Eta(),weight);
	      oshistos[3][2][4]->Fill(leptons[1]->Eta(),weight);
	      oshistos[4][2][4]->Fill(leptons[0]->Phi(),weight);
	      oshistos[5][2][4]->Fill(leptons[1]->Phi(),weight);
	      oshistos[6][2][4]->Fill(findMET(muons),weight);
	      oshistos[7][2][4]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	      oshistos[8][2][4]->Fill((int)jets.size(),weight);
	      oshistos[9][2][4]->Fill((int)photons.size(),weight);
	      oshistos[10][2][4]->Fill(((MTree::Muon*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	      oshistos[11][2][4]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);

	      MMhistos[0][2][0][0][0]->Fill(findMET(muons),weight*sigT);
	      MMhistos[0][2][0][1][0]->Fill(findMET(muons),weight*backT);
	      MMhistos[1][2][0][0][0]->Fill((*leptons[0]+*leptons[1]).M(),weight*sigT);
	      MMhistos[1][2][0][1][0]->Fill((*leptons[0]+*leptons[1]).M(),weight*backT);
	      MMhistos[2][2][0][0][0]->Fill((int)jets.size(),weight*sigT);
	      MMhistos[2][2][0][1][0]->Fill((int)jets.size(),weight*backT);
	    }
	    else if (channels[0] == "mm")
	    {
	      oshistos[0][3][4]->Fill(leptons[0]->Pt(),weight);
	      oshistos[1][3][4]->Fill(leptons[1]->Pt(),weight);
	      oshistos[2][3][4]->Fill(leptons[0]->Eta(),weight);
	      oshistos[3][3][4]->Fill(leptons[1]->Eta(),weight);
	      oshistos[4][3][4]->Fill(leptons[0]->Phi(),weight);
	      oshistos[5][3][4]->Fill(leptons[1]->Phi(),weight);
	      oshistos[6][3][4]->Fill(findMET(muons),weight);
	      oshistos[7][3][4]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	      oshistos[8][3][4]->Fill((int)jets.size(),weight);
	      oshistos[9][3][4]->Fill((int)photons.size(),weight);
	      oshistos[10][3][4]->Fill(((MTree::Muon*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	      oshistos[11][3][4]->Fill(((MTree::Muon*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);

	      MMhistos[0][3][0][0][0]->Fill(findMET(muons),weight*sigT);
	      MMhistos[0][3][0][1][0]->Fill(findMET(muons),weight*backT);
	      MMhistos[1][3][0][0][0]->Fill((*leptons[0]+*leptons[1]).M(),weight*sigT);
	      MMhistos[1][3][0][1][0]->Fill((*leptons[0]+*leptons[1]).M(),weight*backT);
	      MMhistos[2][3][0][0][0]->Fill((int)jets.size(),weight*sigT);
	      MMhistos[2][3][0][1][0]->Fill((int)jets.size(),weight*backT);
	    }
	  }

	}//Closes if-statement over MET > 30
	if (findMET(muons) > 100)
        {
	  orderByPt(leptons);
	  double effS = getSigEff(leptons[0]->LeptonPdgID(),leptons[0]->Pt(),leptons[0]->Eta(),leptons[1]->LeptonPdgID(),leptons[1]->Pt(),leptons[1]->Eta());
	  double effB = getBackEff(leptons[0]->LeptonPdgID(),leptons[0]->Pt(),leptons[0]->Eta(),leptons[1]->LeptonPdgID(),leptons[1]->Pt(),leptons[1]->Eta());

	  double sigL = effB/(effB-effS);
	  double backL = effS/(effS-effB);

	  //Fill first set of plots
	  if (channels[0] == "ee")
	  {
	    oshistos[0][0][5]->Fill(leptons[0]->Pt(),weight);
	    oshistos[1][0][5]->Fill(leptons[1]->Pt(),weight);
	    oshistos[2][0][5]->Fill(leptons[0]->Eta(),weight);
	    oshistos[3][0][5]->Fill(leptons[1]->Eta(),weight);
	    oshistos[4][0][5]->Fill(leptons[0]->Phi(),weight);
	    oshistos[5][0][5]->Fill(leptons[1]->Phi(),weight);
	    oshistos[6][0][5]->Fill(findMET(muons),weight);
	    oshistos[7][0][5]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	    oshistos[8][0][5]->Fill((int)jets.size(),weight);
	    oshistos[9][0][5]->Fill((int)photons.size(),weight);
	    oshistos[10][0][5]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	    oshistos[11][0][5]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);

	    MMhistos[0][0][0][0][1]->Fill(findMET(muons),weight*sigL);
	    MMhistos[0][0][0][1][1]->Fill(findMET(muons),weight*backL);
	    MMhistos[1][0][0][0][1]->Fill((*leptons[0]+*leptons[1]).M(),weight*sigL);
	    MMhistos[1][0][0][1][1]->Fill((*leptons[0]+*leptons[1]).M(),weight*backL);
	    MMhistos[2][0][0][0][1]->Fill((int)jets.size(),weight*sigL);
	    MMhistos[2][0][0][1][1]->Fill((int)jets.size(),weight*backL);
	  }
	  else if (channels[0] == "em")
	  {
	    oshistos[0][1][5]->Fill(leptons[0]->Pt(),weight);
	    oshistos[1][1][5]->Fill(leptons[1]->Pt(),weight);
	    oshistos[2][1][5]->Fill(leptons[0]->Eta(),weight);
	    oshistos[3][1][5]->Fill(leptons[1]->Eta(),weight);
	    oshistos[4][1][5]->Fill(leptons[0]->Phi(),weight);
	    oshistos[5][1][5]->Fill(leptons[1]->Phi(),weight);
	    oshistos[6][1][5]->Fill(findMET(muons),weight);
	    oshistos[7][1][5]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	    oshistos[8][1][5]->Fill((int)jets.size(),weight);
	    oshistos[9][1][5]->Fill((int)photons.size(),weight);
	    oshistos[10][1][5]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	    oshistos[11][1][5]->Fill(((MTree::Muon*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);

	    MMhistos[0][1][0][0][1]->Fill(findMET(muons),weight*sigL);
	    MMhistos[0][1][0][1][1]->Fill(findMET(muons),weight*backL);
	    MMhistos[1][1][0][0][1]->Fill((*leptons[0]+*leptons[1]).M(),weight*sigL);
	    MMhistos[1][1][0][1][1]->Fill((*leptons[0]+*leptons[1]).M(),weight*backL);
	    MMhistos[2][1][0][0][1]->Fill((int)jets.size(),weight*sigL);
	    MMhistos[2][1][0][1][1]->Fill((int)jets.size(),weight*backL);
	  }
	  else if(channels[0] == "me")
	  {
	    oshistos[0][2][5]->Fill(leptons[0]->Pt(),weight);
	    oshistos[1][2][5]->Fill(leptons[1]->Pt(),weight);
	    oshistos[2][2][5]->Fill(leptons[0]->Eta(),weight);
	    oshistos[3][2][5]->Fill(leptons[1]->Eta(),weight);
	    oshistos[4][2][5]->Fill(leptons[0]->Phi(),weight);
	    oshistos[5][2][5]->Fill(leptons[1]->Phi(),weight);
	    oshistos[6][2][5]->Fill(findMET(muons),weight);
	    oshistos[7][2][5]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	    oshistos[8][2][5]->Fill((int)jets.size(),weight);
	    oshistos[9][2][5]->Fill((int)photons.size(),weight);
	    oshistos[10][2][5]->Fill(((MTree::Muon*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	    oshistos[11][2][5]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);

	    MMhistos[0][2][0][0][1]->Fill(findMET(muons),weight*sigL);
	    MMhistos[0][2][0][1][1]->Fill(findMET(muons),weight*backL);
	    MMhistos[1][2][0][0][1]->Fill((*leptons[0]+*leptons[1]).M(),weight*sigL);
	    MMhistos[1][2][0][1][1]->Fill((*leptons[0]+*leptons[1]).M(),weight*backL);
	    MMhistos[2][2][0][0][1]->Fill((int)jets.size(),weight*sigL);
	    MMhistos[2][2][0][1][1]->Fill((int)jets.size(),weight*backL);
	  }
	  else if (channels[0] == "mm")
	  {
	    oshistos[0][3][5]->Fill(leptons[0]->Pt(),weight);
	    oshistos[1][3][5]->Fill(leptons[1]->Pt(),weight);
	    oshistos[2][3][5]->Fill(leptons[0]->Eta(),weight);
	    oshistos[3][3][5]->Fill(leptons[1]->Eta(),weight);
	    oshistos[4][3][5]->Fill(leptons[0]->Phi(),weight);
	    oshistos[5][3][5]->Fill(leptons[1]->Phi(),weight);
	    oshistos[6][3][5]->Fill(findMET(muons),weight);
	    oshistos[7][3][5]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	    oshistos[8][3][5]->Fill((int)jets.size(),weight);
	    oshistos[9][3][5]->Fill((int)photons.size(),weight);
	    oshistos[10][3][5]->Fill(((MTree::Muon*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	    oshistos[11][3][5]->Fill(((MTree::Muon*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);

	    MMhistos[0][3][0][0][1]->Fill(findMET(muons),weight*sigL);
	    MMhistos[0][3][0][1][1]->Fill(findMET(muons),weight*backL);
	    MMhistos[1][3][0][0][1]->Fill((*leptons[0]+*leptons[1]).M(),weight*sigL);
	    MMhistos[1][3][0][1][1]->Fill((*leptons[0]+*leptons[1]).M(),weight*backL);
	    MMhistos[2][3][0][0][1]->Fill((int)jets.size(),weight*sigL);
	    MMhistos[2][3][0][1][1]->Fill((int)jets.size(),weight*backL);
	  }
	  //Calculate isolations of the leptons
	  double iso1 = 100;
	  double iso2 = 100;

	  if (leptons[0]->LeptonPdgID() == 11)
	  {
	    iso1 = ((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt();
	  }
	  else
	  {
	    iso1 = ((MTree::Muon*)leptons[0])->ETCone40()/leptons[0]->Pt();
	  }

	  if (leptons[1]->LeptonPdgID() == 11)
	  {
	    iso2 = ((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt();
	  }
	  else
	  {
	    iso2 = ((MTree::Muon*)leptons[1])->ETCone40()/leptons[1]->Pt();
	  }

	  //Check isolation of first lepton and fill second set
	  if (iso1 < 0.1)
	  {
	    if (channels[0] == "ee")
	    {
	      oshistos[0][0][6]->Fill(leptons[0]->Pt(),weight);
	      oshistos[1][0][6]->Fill(leptons[1]->Pt(),weight);
	      oshistos[2][0][6]->Fill(leptons[0]->Eta(),weight);
	      oshistos[3][0][6]->Fill(leptons[1]->Eta(),weight);
	      oshistos[4][0][6]->Fill(leptons[0]->Phi(),weight);
	      oshistos[5][0][6]->Fill(leptons[1]->Phi(),weight);
	      oshistos[6][0][6]->Fill(findMET(muons),weight);
	      oshistos[7][0][6]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	      oshistos[8][0][6]->Fill((int)jets.size(),weight);
	      oshistos[9][0][6]->Fill((int)photons.size(),weight);
	      oshistos[10][0][6]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	      oshistos[11][0][6]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	    }
	    else if (channels[0] == "em")
	    {
	      oshistos[0][1][6]->Fill(leptons[0]->Pt(),weight);
	      oshistos[1][1][6]->Fill(leptons[1]->Pt(),weight);
	      oshistos[2][1][6]->Fill(leptons[0]->Eta(),weight);
	      oshistos[3][1][6]->Fill(leptons[1]->Eta(),weight);
	      oshistos[4][1][6]->Fill(leptons[0]->Phi(),weight);
	      oshistos[5][1][6]->Fill(leptons[1]->Phi(),weight);
	      oshistos[6][1][6]->Fill(findMET(muons),weight);
	      oshistos[7][1][6]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	      oshistos[8][1][6]->Fill((int)jets.size(),weight);
	      oshistos[9][1][6]->Fill((int)photons.size(),weight);
	      oshistos[10][1][6]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	      oshistos[11][1][6]->Fill(((MTree::Muon*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	    }
	    else if(channels[0] == "me")
	    {
	      oshistos[0][2][6]->Fill(leptons[0]->Pt(),weight);
	      oshistos[1][2][6]->Fill(leptons[1]->Pt(),weight);
	      oshistos[2][2][6]->Fill(leptons[0]->Eta(),weight);
	      oshistos[3][2][6]->Fill(leptons[1]->Eta(),weight);
	      oshistos[4][2][6]->Fill(leptons[0]->Phi(),weight);
	      oshistos[5][2][6]->Fill(leptons[1]->Phi(),weight);
	      oshistos[6][2][6]->Fill(findMET(muons),weight);
	      oshistos[7][2][6]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	      oshistos[8][2][6]->Fill((int)jets.size(),weight);
	      oshistos[9][2][6]->Fill((int)photons.size(),weight);
	      oshistos[10][2][6]->Fill(((MTree::Muon*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	      oshistos[11][2][6]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	    }
	    else if (channels[0] == "mm")
	    {
	      oshistos[0][3][6]->Fill(leptons[0]->Pt(),weight);
	      oshistos[1][3][6]->Fill(leptons[1]->Pt(),weight);
	      oshistos[2][3][6]->Fill(leptons[0]->Eta(),weight);
	      oshistos[3][3][6]->Fill(leptons[1]->Eta(),weight);
	      oshistos[4][3][6]->Fill(leptons[0]->Phi(),weight);
	      oshistos[5][3][6]->Fill(leptons[1]->Phi(),weight);
	      oshistos[6][3][6]->Fill(findMET(muons),weight);
	      oshistos[7][3][6]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	      oshistos[8][3][6]->Fill((int)jets.size(),weight);
	      oshistos[9][3][6]->Fill((int)photons.size(),weight);
	      oshistos[10][3][6]->Fill(((MTree::Muon*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	      oshistos[11][3][6]->Fill(((MTree::Muon*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	    }
	  }

	  //Check isolation of second lepton and fill third set
	  double sigT = 1/(effS-effB);
	  double backT = 1/(effB-effS);
	  if ((iso1 < 0.1) && (iso2 < 0.1))
	  {
	    if (channels[0] == "ee")
	    {
	      oshistos[0][0][7]->Fill(leptons[0]->Pt(),weight);
	      oshistos[1][0][7]->Fill(leptons[1]->Pt(),weight);
	      oshistos[2][0][7]->Fill(leptons[0]->Eta(),weight);
	      oshistos[3][0][7]->Fill(leptons[1]->Eta(),weight);
	      oshistos[4][0][7]->Fill(leptons[0]->Phi(),weight);
	      oshistos[5][0][7]->Fill(leptons[1]->Phi(),weight);
	      oshistos[6][0][7]->Fill(findMET(muons),weight);
	      oshistos[7][0][7]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	      oshistos[8][0][7]->Fill((int)jets.size(),weight);
	      oshistos[9][0][7]->Fill((int)photons.size(),weight);
	      oshistos[10][0][7]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	      oshistos[11][0][7]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);

	      MMhistos[0][0][0][0][1]->Fill(findMET(muons),weight*sigT);
	      MMhistos[0][0][0][1][1]->Fill(findMET(muons),weight*backT);
	      MMhistos[1][0][0][0][1]->Fill((*leptons[0]+*leptons[1]).M(),weight*sigT);
	      MMhistos[1][0][0][1][1]->Fill((*leptons[0]+*leptons[1]).M(),weight*backT);
	      MMhistos[2][0][0][0][1]->Fill((int)jets.size(),weight*sigT);
	      MMhistos[2][0][0][1][1]->Fill((int)jets.size(),weight*backT);
	    }
	    else if (channels[0] == "em")
	    {
	      oshistos[0][1][7]->Fill(leptons[0]->Pt(),weight);
	      oshistos[1][1][7]->Fill(leptons[1]->Pt(),weight);
	      oshistos[2][1][7]->Fill(leptons[0]->Eta(),weight);
	      oshistos[3][1][7]->Fill(leptons[1]->Eta(),weight);
	      oshistos[4][1][7]->Fill(leptons[0]->Phi(),weight);
	      oshistos[5][1][7]->Fill(leptons[1]->Phi(),weight);
	      oshistos[6][1][7]->Fill(findMET(muons),weight);
	      oshistos[7][1][7]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	      oshistos[8][1][7]->Fill((int)jets.size(),weight);
	      oshistos[9][1][7]->Fill((int)photons.size(),weight);
	      oshistos[10][1][7]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	      oshistos[11][1][7]->Fill(((MTree::Muon*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);

	      MMhistos[0][1][0][0][1]->Fill(findMET(muons),weight*sigT);
	      MMhistos[0][1][0][1][1]->Fill(findMET(muons),weight*backT);
	      MMhistos[1][1][0][0][1]->Fill((*leptons[0]+*leptons[1]).M(),weight*sigT);
	      MMhistos[1][1][0][1][1]->Fill((*leptons[0]+*leptons[1]).M(),weight*backT);
	      MMhistos[2][1][0][0][1]->Fill((int)jets.size(),weight*sigT);
	      MMhistos[2][1][0][1][1]->Fill((int)jets.size(),weight*backT);
	    }
	    else if(channels[0] == "me")
	    {
	      oshistos[0][2][7]->Fill(leptons[0]->Pt(),weight);
	      oshistos[1][2][7]->Fill(leptons[1]->Pt(),weight);
	      oshistos[2][2][7]->Fill(leptons[0]->Eta(),weight);
	      oshistos[3][2][7]->Fill(leptons[1]->Eta(),weight);
	      oshistos[4][2][7]->Fill(leptons[0]->Phi(),weight);
	      oshistos[5][2][7]->Fill(leptons[1]->Phi(),weight);
	      oshistos[6][2][7]->Fill(findMET(muons),weight);
	      oshistos[7][2][7]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	      oshistos[8][2][7]->Fill((int)jets.size(),weight);
	      oshistos[9][2][7]->Fill((int)photons.size(),weight);
	      oshistos[10][2][7]->Fill(((MTree::Muon*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	      oshistos[11][2][7]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);

	      MMhistos[0][2][0][0][1]->Fill(findMET(muons),weight*sigT);
	      MMhistos[0][2][0][1][1]->Fill(findMET(muons),weight*backT);
	      MMhistos[1][2][0][0][1]->Fill((*leptons[0]+*leptons[1]).M(),weight*sigT);
	      MMhistos[1][2][0][1][1]->Fill((*leptons[0]+*leptons[1]).M(),weight*backT);
	      MMhistos[2][2][0][0][1]->Fill((int)jets.size(),weight*sigT);
	      MMhistos[2][2][0][1][1]->Fill((int)jets.size(),weight*backT);
	    }
	    else if (channels[0] == "mm")
	    {
	      oshistos[0][3][7]->Fill(leptons[0]->Pt(),weight);
	      oshistos[1][3][7]->Fill(leptons[1]->Pt(),weight);
	      oshistos[2][3][7]->Fill(leptons[0]->Eta(),weight);
	      oshistos[3][3][7]->Fill(leptons[1]->Eta(),weight);
	      oshistos[4][3][7]->Fill(leptons[0]->Phi(),weight);
	      oshistos[5][3][7]->Fill(leptons[1]->Phi(),weight);
	      oshistos[6][3][7]->Fill(findMET(muons),weight);
	      oshistos[7][3][7]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	      oshistos[8][3][7]->Fill((int)jets.size(),weight);
	      oshistos[9][3][7]->Fill((int)photons.size(),weight);
	      oshistos[10][3][7]->Fill(((MTree::Muon*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	      oshistos[11][3][7]->Fill(((MTree::Muon*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);

	      MMhistos[0][3][0][0][1]->Fill(findMET(muons),weight*sigT);
	      MMhistos[0][3][0][1][1]->Fill(findMET(muons),weight*backT);
	      MMhistos[1][3][0][0][1]->Fill((*leptons[0]+*leptons[1]).M(),weight*sigT);
	      MMhistos[1][3][0][1][1]->Fill((*leptons[0]+*leptons[1]).M(),weight*backT);
	      MMhistos[2][3][0][0][1]->Fill((int)jets.size(),weight*sigT);
	      MMhistos[2][3][0][1][1]->Fill((int)jets.size(),weight*backT);
	    }
	  }
	}//Closes if-statement over MET > 100


// 	while ((findMET(muons) > (30 + m*35)) &&  (m < 3))
// 	{
// 	  if (_debug)
// 	  {
// 	    std::cout << "met iteration " << m << std::endl;
// 	  }
// 	  ++m;
// 	  //Add Matrix Method histograms
// 	  orderByPt(leptons);
// 	  double effS = getSigEff(leptons[0]->LeptonPdgID(),leptons[0]->Pt(),leptons[0]->Eta(),leptons[1]->LeptonPdgID(),leptons[1]->Pt(),leptons[1]->Eta());
// 	  double effB = getBackEff(leptons[0]->LeptonPdgID(),leptons[0]->Pt(),leptons[0]->Eta(),leptons[1]->LeptonPdgID(),leptons[1]->Pt(),leptons[1]->Eta());

// 	  double sigL = effB/(effB-effS);
// 	  double backL = effS/(effS-effB);

// 	  if (channels[0] == "ee")
// 	  {
// 	    if (_debug)
// 	    {
// 	      std::cout << "filling os met loop ee plots" << std::endl;
// 	    }
// 	    oshistos[0][0][stage]->Fill(leptons[0]->Pt(),weight);
// 	    oshistos[1][0][stage]->Fill(leptons[1]->Pt(),weight);
// 	    oshistos[2][0][stage]->Fill(leptons[0]->Eta(),weight);
// 	    oshistos[3][0][stage]->Fill(leptons[1]->Eta(),weight);
// 	    oshistos[4][0][stage]->Fill(leptons[0]->Phi(),weight);
// 	    oshistos[5][0][stage]->Fill(leptons[1]->Phi(),weight);
// 	    oshistos[6][0][stage]->Fill(findMET(muons),weight);
// 	    oshistos[7][0][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
// 	    oshistos[8][0][stage]->Fill((int)jets.size(),weight);
// 	    oshistos[9][0][stage]->Fill((int)photons.size(),weight);
// 	    oshistos[10][0][stage]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
// 	    oshistos[11][0][stage]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);

// 	    MMhistos[0][0][0][0][m-1]->Fill(findMET(muons),weight*sigL);
// 	    MMhistos[0][0][0][1][m-1]->Fill(findMET(muons),weight*backL);
// 	    MMhistos[1][0][0][0][m-1]->Fill((*leptons[0]+*leptons[1]).M(),weight*sigL);
// 	    MMhistos[1][0][0][1][m-1]->Fill((*leptons[0]+*leptons[1]).M(),weight*backL);
// 	    MMhistos[2][0][0][0][m-1]->Fill((int)jets.size(),weight*sigL);
// 	    MMhistos[2][0][0][1][m-1]->Fill((int)jets.size(),weight*backL);
// 	  }
// 	  else if (channels[0] == "em")
// 	  {
// 	    if (_debug)
// 	    {
// 	      std::cout << "filling os met loop em plots" << std::endl;
// 	    }
// 	    oshistos[0][1][stage]->Fill(leptons[0]->Pt(),weight);
// 	    oshistos[1][1][stage]->Fill(leptons[1]->Pt(),weight);
// 	    oshistos[2][1][stage]->Fill(leptons[0]->Eta(),weight);
// 	    oshistos[3][1][stage]->Fill(leptons[1]->Eta(),weight);
// 	    oshistos[4][1][stage]->Fill(leptons[0]->Phi(),weight);
// 	    oshistos[5][1][stage]->Fill(leptons[1]->Phi(),weight);
// 	    oshistos[6][1][stage]->Fill(findMET(muons),weight);
// 	    oshistos[7][1][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
// 	    oshistos[8][1][stage]->Fill((int)jets.size(),weight);
// 	    oshistos[9][1][stage]->Fill((int)photons.size(),weight);
// 	    oshistos[10][1][stage]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
// 	    oshistos[11][1][stage]->Fill(((MTree::Muon*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);

// 	    MMhistos[0][1][0][0][m-1]->Fill(findMET(muons),weight*sigL);
// 	    MMhistos[0][1][0][1][m-1]->Fill(findMET(muons),weight*backL);
// 	    MMhistos[1][1][0][0][m-1]->Fill((*leptons[0]+*leptons[1]).M(),weight*sigL);
// 	    MMhistos[1][1][0][1][m-1]->Fill((*leptons[0]+*leptons[1]).M(),weight*backL);
// 	    MMhistos[2][1][0][0][m-1]->Fill((int)jets.size(),weight*sigL);
// 	    MMhistos[2][1][0][1][m-1]->Fill((int)jets.size(),weight*backL);
// 	  }
// 	  else if(channels[0] == "me")
// 	  {
// 	    if (_debug)
// 	    {
// 	      std::cout << "filling os met loop me plots" << std::endl;
// 	    }
// 	    oshistos[0][2][stage]->Fill(leptons[0]->Pt(),weight);
// 	    oshistos[1][2][stage]->Fill(leptons[1]->Pt(),weight);
// 	    oshistos[2][2][stage]->Fill(leptons[0]->Eta(),weight);
// 	    oshistos[3][2][stage]->Fill(leptons[1]->Eta(),weight);
// 	    oshistos[4][2][stage]->Fill(leptons[0]->Phi(),weight);
// 	    oshistos[5][2][stage]->Fill(leptons[1]->Phi(),weight);
// 	    oshistos[6][2][stage]->Fill(findMET(muons),weight);
// 	    oshistos[7][2][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
// 	    oshistos[8][2][stage]->Fill((int)jets.size(),weight);
// 	    oshistos[9][2][stage]->Fill((int)photons.size(),weight);
// 	    oshistos[10][2][stage]->Fill(((MTree::Muon*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
// 	    oshistos[11][2][stage]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);

// 	    MMhistos[0][2][0][0][m-1]->Fill(findMET(muons),weight*sigL);
// 	    MMhistos[0][2][0][1][m-1]->Fill(findMET(muons),weight*backL);
// 	    MMhistos[1][2][0][0][m-1]->Fill((*leptons[0]+*leptons[1]).M(),weight*sigL);
// 	    MMhistos[1][2][0][1][m-1]->Fill((*leptons[0]+*leptons[1]).M(),weight*backL);
// 	    MMhistos[2][2][0][0][m-1]->Fill((int)jets.size(),weight*sigL);
// 	    MMhistos[2][2][0][1][m-1]->Fill((int)jets.size(),weight*backL);
// 	  }
// 	  else if (channels[0] == "mm")
// 	  {
// 	    if (_debug)
// 	    {
// 	      std::cout << "filling os met loop mm plots" << std::endl;
// 	      std::cout << "stage " << stage << std::endl;
// 	    }
// 	    oshistos[0][3][stage]->Fill(leptons[0]->Pt(),weight);
// 	    oshistos[1][3][stage]->Fill(leptons[1]->Pt(),weight);
// 	    oshistos[2][3][stage]->Fill(leptons[0]->Eta(),weight);
// 	    oshistos[3][3][stage]->Fill(leptons[1]->Eta(),weight);
// 	    oshistos[4][3][stage]->Fill(leptons[0]->Phi(),weight);
// 	    oshistos[5][3][stage]->Fill(leptons[1]->Phi(),weight);
// 	    oshistos[6][3][stage]->Fill(findMET(muons),weight);
// 	    oshistos[7][3][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
// 	    oshistos[8][3][stage]->Fill((int)jets.size(),weight);
// 	    oshistos[9][3][stage]->Fill((int)photons.size(),weight);
// 	    oshistos[10][3][stage]->Fill(((MTree::Muon*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
// 	    oshistos[11][3][stage]->Fill(((MTree::Muon*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
// 	    if (_debug)
// 	    {
// 	      std::cout << "now filling matrix method histos" << std::endl;
// 	    }
// 	    MMhistos[0][3][0][0][m-1]->Fill(findMET(muons),weight*sigL);
// 	    MMhistos[0][3][0][1][m-1]->Fill(findMET(muons),weight*backL);
// 	    if (_debug)
// 	    {
// 	      std::cout << "filled met plots" << std::endl;
// 	    }
// 	    MMhistos[1][3][0][0][m-1]->Fill((*leptons[0]+*leptons[1]).M(),weight*sigL);
// 	    MMhistos[1][3][0][1][m-1]->Fill((*leptons[0]+*leptons[1]).M(),weight*backL);
// 	    if (_debug)
// 	    {
// 	      std::cout << "filled mll plots" << std::endl;
// 	    }
// 	    MMhistos[2][3][0][0][m-1]->Fill((int)jets.size(),weight*sigL);
// 	    MMhistos[2][3][0][1][m-1]->Fill((int)jets.size(),weight*backL);
// 	    if (_debug)
// 	    {
// 	      std::cout << "finished filling mm mm plots" << std::endl;
// 	    }
// 	  }
// 	  ++stage;

//           //Put cut on isolation of leptons here.
// 	  if (_debug)
// 	  {
// 	    std::cout << "checking lepton isolation" << std::endl;
// 	  }
// 	  bool twoGoodOnes = true;
// 	  for (int i = 0; i != (int)leptons.size(); ++i)
//  	  {
// 	    double iso;
// 	    if (leptons[i]->LeptonPdgID() == 11)
// 	    {
// 	      MTree::Electron* elec = (MTree::Electron*)leptons[i];
// 	      iso = elec->ETCone40()/elec->Pt();
// 	      histos2[0]->Fill(elec->Pt(),elec->Eta(),weight);
// 	    }
// 	    else
// 	    {
// 	      MTree::Muon* muon = (MTree::Muon*)leptons[i];
// 	      iso = muon->ETCone40()/muon->Pt();
// 	      histos2[2]->Fill(muon->Pt(),muon->Eta(),weight);
// 	    }
// 	    if (iso > 0.1)
// 	    {
// 	      twoGoodOnes = false;
// 	    }
// 	    else
// 	    {
// 	      if (leptons[i]->LeptonPdgID() == 11)
// 	      {
// 	        histos2[1]->Fill(leptons[i]->Pt(),leptons[i]->Eta(),weight);
// 	      }
// 	      else
// 	      {
// 	        histos2[3]->Fill(leptons[i]->Pt(),leptons[i]->Eta(),weight);
// 	      }
// 	    }
// 	  }

// 	  if (!twoGoodOnes) 
//           {
// 	    ++stage;
//             continue;
// 	  }

// 	  double sigT = 1/(effS-effB);
// 	  double backT = 1/(effB-effS);

// 	  if (channels[0] == "ee")
// 	  {
// 	    oshistos[0][0][stage]->Fill(leptons[0]->Pt(),weight);
// 	    oshistos[1][0][stage]->Fill(leptons[1]->Pt(),weight);
// 	    oshistos[2][0][stage]->Fill(leptons[0]->Eta(),weight);
// 	    oshistos[3][0][stage]->Fill(leptons[1]->Eta(),weight);
// 	    oshistos[4][0][stage]->Fill(leptons[0]->Phi(),weight);
// 	    oshistos[5][0][stage]->Fill(leptons[1]->Phi(),weight);
// 	    oshistos[6][0][stage]->Fill(findMET(muons),weight);
// 	    oshistos[7][0][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
// 	    oshistos[8][0][stage]->Fill((int)jets.size(),weight);
// 	    oshistos[9][0][stage]->Fill((int)photons.size(),weight);
// 	    oshistos[10][0][stage]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
// 	    oshistos[11][0][stage]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);

// 	    MMhistos[0][0][0][0][m-1]->Fill(findMET(muons),weight*sigT);
// 	    MMhistos[0][0][0][1][m-1]->Fill(findMET(muons),weight*backT);
// 	    MMhistos[1][0][0][0][m-1]->Fill((*leptons[0]+*leptons[1]).M(),weight*sigT);
// 	    MMhistos[1][0][0][1][m-1]->Fill((*leptons[0]+*leptons[1]).M(),weight*backT);
// 	    MMhistos[2][0][0][0][m-1]->Fill((int)jets.size(),weight*sigT);
// 	    MMhistos[2][0][0][1][m-1]->Fill((int)jets.size(),weight*backT);
// 	  }
// 	  else if (channels[0] == "em")
// 	  {
// 	    oshistos[0][1][stage]->Fill(leptons[0]->Pt(),weight);
// 	    oshistos[1][1][stage]->Fill(leptons[1]->Pt(),weight);
// 	    oshistos[2][1][stage]->Fill(leptons[0]->Eta(),weight);
// 	    oshistos[3][1][stage]->Fill(leptons[1]->Eta(),weight);
// 	    oshistos[4][1][stage]->Fill(leptons[0]->Phi(),weight);
// 	    oshistos[5][1][stage]->Fill(leptons[1]->Phi(),weight);
// 	    oshistos[6][1][stage]->Fill(findMET(muons),weight);
// 	    oshistos[7][1][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
// 	    oshistos[8][1][stage]->Fill((int)jets.size(),weight);
// 	    oshistos[9][1][stage]->Fill((int)photons.size(),weight);
// 	    oshistos[10][1][stage]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
// 	    oshistos[11][1][stage]->Fill(((MTree::Muon*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);

// 	    MMhistos[0][1][0][0][m-1]->Fill(findMET(muons),weight*sigT);
// 	    MMhistos[0][1][0][1][m-1]->Fill(findMET(muons),weight*backT);
// 	    MMhistos[1][1][0][0][m-1]->Fill((*leptons[0]+*leptons[1]).M(),weight*sigT);
// 	    MMhistos[1][1][0][1][m-1]->Fill((*leptons[0]+*leptons[1]).M(),weight*backT);
// 	    MMhistos[2][1][0][0][m-1]->Fill((int)jets.size(),weight*sigT);
// 	    MMhistos[2][1][0][1][m-1]->Fill((int)jets.size(),weight*backT);
// 	  }
// 	  else if(channels[0] == "me")
// 	  {
// 	    oshistos[0][2][stage]->Fill(leptons[0]->Pt(),weight);
// 	    oshistos[1][2][stage]->Fill(leptons[1]->Pt(),weight);
// 	    oshistos[2][2][stage]->Fill(leptons[0]->Eta(),weight);
// 	    oshistos[3][2][stage]->Fill(leptons[1]->Eta(),weight);
// 	    oshistos[4][2][stage]->Fill(leptons[0]->Phi(),weight);
// 	    oshistos[5][2][stage]->Fill(leptons[1]->Phi(),weight);
// 	    oshistos[6][2][stage]->Fill(findMET(muons),weight);
// 	    oshistos[7][2][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
// 	    oshistos[8][2][stage]->Fill((int)jets.size(),weight);
// 	    oshistos[9][2][stage]->Fill((int)photons.size(),weight);
// 	    oshistos[10][2][stage]->Fill(((MTree::Muon*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
// 	    oshistos[11][2][stage]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);

// 	    MMhistos[0][2][0][0][m-1]->Fill(findMET(muons),weight*sigT);
// 	    MMhistos[0][2][0][1][m-1]->Fill(findMET(muons),weight*backT);
// 	    MMhistos[1][2][0][0][m-1]->Fill((*leptons[0]+*leptons[1]).M(),weight*sigT);
// 	    MMhistos[1][2][0][1][m-1]->Fill((*leptons[0]+*leptons[1]).M(),weight*backT);
// 	    MMhistos[2][2][0][0][m-1]->Fill((int)jets.size(),weight*sigT);
// 	    MMhistos[2][2][0][1][m-1]->Fill((int)jets.size(),weight*backT);
// 	  }
// 	  else if (channels[0] == "mm")
// 	  {
// 	    oshistos[0][3][stage]->Fill(leptons[0]->Pt(),weight);
// 	    oshistos[1][3][stage]->Fill(leptons[1]->Pt(),weight);
// 	    oshistos[2][3][stage]->Fill(leptons[0]->Eta(),weight);
// 	    oshistos[3][3][stage]->Fill(leptons[1]->Eta(),weight);
// 	    oshistos[4][3][stage]->Fill(leptons[0]->Phi(),weight);
// 	    oshistos[5][3][stage]->Fill(leptons[1]->Phi(),weight);
// 	    oshistos[6][3][stage]->Fill(findMET(muons),weight);
// 	    oshistos[7][3][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
// 	    oshistos[8][3][stage]->Fill((int)jets.size(),weight);
// 	    oshistos[9][3][stage]->Fill((int)photons.size(),weight);
// 	    oshistos[10][3][stage]->Fill(((MTree::Muon*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
// 	    oshistos[11][3][stage]->Fill(((MTree::Muon*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);

// 	    MMhistos[0][3][0][0][m-1]->Fill(findMET(muons),weight*sigT);
// 	    MMhistos[0][3][0][1][m-1]->Fill(findMET(muons),weight*backT);
// 	    MMhistos[1][3][0][0][m-1]->Fill((*leptons[0]+*leptons[1]).M(),weight*sigT);
// 	    MMhistos[1][3][0][1][m-1]->Fill((*leptons[0]+*leptons[1]).M(),weight*backT);
// 	    MMhistos[2][3][0][0][m-1]->Fill((int)jets.size(),weight*sigT);
// 	    MMhistos[2][3][0][1][m-1]->Fill((int)jets.size(),weight*backT);
// 	  }
// 	  ++stage;
// 	}//closes while loop over met's


	if (!isMC && findMET(muons) > 100)
	{
	  _eventInfo->PrintData(outfile10);
	  for (int i = 0; i != (int)leptons.size(); ++i)
	  {
	    if (leptons[i]->LeptonPdgID() == 11)
	    {
	      ((MTree::Electron*)leptons[i])->PrintData(outfile10);
	    }
	    else
	    {
	      ((MTree::Muon*)leptons[i])->PrintData(outfile10);
	    }
	  }
	  for (int i = 0; i !=  (int)jets.size(); ++i)
	  {
	    jets[i]->PrintData(outfile10);
	  }
	  outfile10 << "MET = " << findMET(muons) << std::endl;
	  outfile10 << "mll = " << (*leptons[0]+*leptons[1]).M() << std::endl;
	}
      }//Closes os portion

      if (_debug)
      {
	std::cout << "reached end of event loop" << std::endl;
      }

    }//Closes event loop
  }//Closes loop over samples

  outfile1.close();
  outfile2.close();
  outfile3.close();
  outfile4.close();
  outfile5.close();
  outfile6.close();
  outfile7.close();
  outfile8.close();
  outfile9.close();
  outfile10.close();
  outfile11.close();
  outfileS.close();

  record->print(sample + "_susy");
  bookE->print(sample + "_electrons");
  bookM->print(sample + "_muons");
  bookSS->print(sample + "_ss");
  bookOS->print(sample + "_os");
}//Closes SusyCutflow()

//######################################################################################

void SusyAnalysis::passTriggers(int runNumber, bool& passE, bool& passM)
{
  //Monte Carlo
  if (runNumber == 100000)
  {
    passE = _trigger->PassedTrigger("EF_e10_medium");
    passM = _trigger->PassedTrigger("EF_mu10_MSonly");
  }
  //Periods A-D
  else if (runNumber < 160387)
  {
    passE = _trigger->PassedTrigger("EF_e10_loose");
//     passE = _trigger->PassedTrigger("EF_g10_loose");
    passM = _trigger->PassedTrigger("L1_MU6");
  }
  //Period E
  else if ((runNumber >= 160387) && (runNumber < 162347))
  {
    passE = _trigger->PassedTrigger("EF_g17_etcut");
    passM = _trigger->PassedTrigger("EF_mu10_MSonly");
  } 
  //Period F
  else if ((runNumber >= 162347) && (runNumber < 165591))
  {
    passE = _trigger->PassedTrigger("EF_e15_medium");
    passM = _trigger->PassedTrigger("EF_mu10_MSonly");
  } 
  //Period G-H
  else if ((runNumber >= 165591) && (runNumber < 167575))
  {
    passE = _trigger->PassedTrigger("EF_e15_medium");
    passM = _trigger->PassedTrigger("EF_mu13");
  }
  else if (runNumber >= 167575)
  {
    passE = _trigger->PassedTrigger("EF_e15_medium");
    passM = _trigger->PassedTrigger("EF_mu13_tight");
  }
}

//######################################################################################

double SusyAnalysis::findMET(std::vector<MTree::Muon*> muons)
{
  //NEW SUSY MET DEFINITION, called SimplifiedRefFinal for reasons that defy my tired attempts at logic
  //right, now . . .
  //RefJet(EMJES) + RefEle + MuonBoy_Track + MuonBoy_Spectro + CellOut(EM)

  //I assume I should add them in quadrature as usual.

  //1. Add up momentum contributions from muons
  double pxtot = 0;
  double pytot = 0;

  for (int i = 0; i != (int)muons.size(); ++i)
  {
    pxtot += muons[i]->Px();
    pytot += muons[i]->Py();
  }

  //2. Get MET_LocHadTopo term
  double lhtx = 0;
  double lhty = 0;

  int nMETs = _missingETTerms->GetEntries();
  for (int i = 0; i != nMETs; ++i)
  {
    MTree::MissingET* term = (MTree::MissingET*)_missingETTerms->At(i);
    std::string name = term->GetTerm();

    if (name.compare("MET_LocHadTopo") == 0)
    {
      lhtx = term->Ex();
      lhty = term->Ey();
    }
  }

  //3. Calculate MET and return it
  double xterms = lhtx - pxtot;
  double yterms = lhty - pytot;

  double met = sqrt(xterms*xterms + yterms*yterms);

  return(met);
}

//######################################################################################

template<class T, class S> 
void SusyAnalysis::removeOverlap(double cut, std::vector<T*> &goodones, std::vector<S*> &badones)
{
  for (int i = 0; i < (int)goodones.size(); ++i)
  {
    for (int j = 0; j < (int)badones.size(); ++j)
    {
      if (goodones[i]->DeltaR(*badones[j]) < cut)
      {
	badones.erase(badones.begin() + j);
	--j;
      }
    }
  }
}

//#######################################################################################

template<class T>
void SusyAnalysis::orderByPt(std::vector<T*> &vec)
{
  for (int i = 0; i != (int)vec.size(); ++i)
  {
    for (int j = i + 1; j != (int)vec.size(); ++j)
    {
      if (vec[i]->Pt() < vec[j]->Pt())
      {
	swap(vec[i],vec[j]);
      }
    }
  }
}


//########################################################################################

bool SusyAnalysis::checkTrigger(std::string sample, int runNumber, std::vector<MTree::Lepton*> leptons)
{

  bool correctTrigger = false;

  bool ECaloStream = false;
  bool MuonStream = false;
  bool MCStream = false;

  if (sample.find("EGamma") != string::npos)
  {
    ECaloStream = true;
  }
  else if (sample.find("Muons") != string::npos)
  {
    MuonStream = true;
  }
  else
  {
    MCStream = true;
  }

  //1.  First, figure out if this event needs to have passed an electron trigger or a muon one.
  int nEs = 0;
  for (int i = 0; i != (int)leptons.size(); ++i)
  {
    if (leptons[i]->LeptonPdgID() == 11)
    {
      ++nEs;
    }
  }

  int nMs = (int)leptons.size() - nEs;

  bool passE;
  bool passM;
  passTriggers(runNumber,passE,passM);

  if (((nEs > 0) && (passE) && (ECaloStream)) || ((nMs > 0) && (passM) && (MuonStream)) || (MCStream && (((nEs > 0) && (passE)) || ((nMs > 0) && passM))))
  {
    correctTrigger = true;
  }  

  return(correctTrigger);
}

//###################################################################################################

bool SusyAnalysis::sillyHighPtCut(int runNumber, std::vector<MTree::Lepton*> leptons)
{
  orderByPt(leptons);

  bool goodE = false;
  bool goodM = false;

  bool eTrigger;
  bool mTrigger;
  passTriggers(runNumber,eTrigger,mTrigger);

  for (int i = 0; i != (int)leptons.size(); ++i)
  {
    if (leptons[i]->LeptonPdgID() == 11)
    {
      if ((leptons[i]->Pt() > 20) && (eTrigger))
      {
	goodE = true;
      }
    }
    else
    {
      if ((leptons[i]->Pt() > 20) && (mTrigger))
      {
	goodM = true;
      }
    }
  }

  bool finalDecision = false;

  if (goodE || goodM)
  {
    finalDecision = true;
  }

  return(finalDecision);
}

//#####################################################################################################

bool SusyAnalysis::sillierCheckTrigger(std::string sample, int runNumber, std::vector<MTree::Lepton*> leptons)
{
  //1. First, figure out what stream this event comes from, if applicable.
  bool ECaloStream = false;
  bool MuonStream = false;
  bool MCStream = false;

  if (sample.find("EGamma") != string::npos)
  {
    ECaloStream = true;
  }
  else if (sample.find("Muons") != string::npos)
  {
    MuonStream = true;
  }
  else
  {
    MCStream = true;
  }

  //1.5. Figure out what channel this event goes in.
  int nEs = 0;
  int nMs = 0;
  for (int i = 0; i != (int)leptons.size(); ++i)
  {
    if (leptons[i]->LeptonPdgID() == 11)
    {
      ++nEs;
    }
    else
    {
      ++nMs;
    }
  }

  bool isEE = false;
  bool isMM = false;
  bool isEM = false;
  if (nEs == 2)
  {
    isEE = true;
  }
  else if (nMs == 2)
  {
    isMM = true;
  }
  else if ((nEs == 1) && (nMs == 1))
  {
    isEM = true;
  }

  //2. Check for leptons over 20 GeV of pT
  bool highptE = false;
  bool highptM = false;

  for (int i = 0; i != (int)leptons.size(); ++i)
  {
    if (leptons[i]->Pt() > 20)
    {
      if (leptons[i]->LeptonPdgID() == 11)
      {
	highptE = true;
      }
      else
      {
	highptM = true;
      }
    }
  }

  //3. Check triggers
  bool eTrigger;
  bool mTrigger;
  passTriggers(runNumber,eTrigger,mTrigger);

  //4. Implement crazy boolean logic
  bool pass = false;

  if (isEE && highptE && eTrigger && (ECaloStream || MCStream))
  {
    pass = true;
  }
  if (isMM && highptM && mTrigger && (MuonStream || MCStream))
  {
    pass = true;
  }
  if (isEM && ((MCStream && ((highptE && eTrigger) || (highptM && mTrigger))) || (ECaloStream && highptE && eTrigger) || (MuonStream && highptM && mTrigger && !(highptE && eTrigger))))
  {
    pass = true;
  } 

  return(pass);
}

//#######################################################################################################

double SusyAnalysis::getSigEff(int leadid, double pt1, double eta1, int subid, double pt2, double eta2)
{
  //One day, this will have a lovely complicated function to find a pt- and eta-dependent 
  //efficiency for true electrons to go from loose to tight.  
  //This is not that day.

  //Now is that day.

  //Now I have to keep all the gosh-darn efficiencies straight.
  //This function gets signal lepton efficiencies from the references and converts them 
  //to event efficiencies, which it returns.  I suppose I could have one function do it,
  //'cause the math is the same, but then I need to have another slightly non-intuitive
  //argument to the function.  Right now, I'm thinking no.

  //Get efficiencies from histograms.
  double leff;
  double seff;
  if (leadid == 11)
  {
    leff = 0.680;
//     leff = true_elec_eff->GetBinContent(true_elec_eff->FindBin(pt1, eta1));
  }
  else
  {
    leff = 0.933;
//     leff = true_muon_eff->GetBinContent(true_muon_eff->FindBin(pt1, eta1));
  }

  if (subid == 11)
  {
    seff = 0.680;
//     seff = true_elec_eff->GetBinContent(true_elec_eff->FindBin(pt2, eta2));
  }
  else
  {
    seff = 0.933;
//     seff = true_muon_eff->GetBinContent(true_muon_eff->FindBin(pt2, eta2));
  }

  //Do calculation
  double event_efficiency = leff*seff;
//   if (leadid == subid)
//   {
//     event_efficiency = (leff*seff)/(leff+seff-(leff*seff));
//   }
//   else
//   {
//     event_efficiency = seff;
//   }

  return(event_efficiency);
}

//#######################################################################################################

double SusyAnalysis::getBackEff(int leadid, double pt1, double eta1, int subid, double pt2, double eta2)
{
  //See note above.

  //'Cept, I'm going to cheat for now and skip the converting from lepton to signal.
  //'Cause it don't change much.

  double back_efficiency = 0;
  double leff;
  double seff;
  if (leadid == 11)
  {
    leff = 0.680;
  }
  else
  {
    leff = 0.933;
  }

  if (subid == 11)
  {
    seff = 0.063;
  }
  else
  {
    seff = 0.070;
  }

  back_efficiency = leff*seff; 
//   if (subid == 11)
//   {
//     back_efficiency = fake_elec_eff->GetBinContent(fake_elec_eff->FindBin(pt2, eta2));
//   }
//   else
//   {
//     back_efficiency = fake_muon_eff->GetBinContent(fake_muon_eff->FindBin(pt2, eta2));
//   }

  return(back_efficiency);
}

//#######################################################################################################

std::vector<std::string> SusyAnalysis::findChannels(std::vector<MTree::Lepton*> leptons)
{
  orderByPt(leptons);

  std::string result = "";

  for (int i = 0; i != (int)leptons.size(); ++i)
  {
    if (leptons[i]->LeptonPdgID() == 11)
    {
      result += "e";
    }
    else
    {
      result += "m";
    }
  }

  std::vector<std::string> channels;
  channels.push_back(result);

  return(channels);
}

//#######################################################################################################
void SusyAnalysis::EstimateEfficiencies(std::string sample, TH1D** histos, TH2D** histos2)
{
  if (_debug)
  {
    std::cout << "Calling EstimateEfficiencies()" << std::endl;
  }

  bool isMC = false;
  if ((sample.find("EGamma") == string::npos) && (sample.find("Muon") == string::npos) && (sample.find("data") == string::npos) && (sample.find("Elec") == string::npos) && (sample.find("JetTauEtmiss") == string::npos))
  {
    isMC = true;
  }

  if (_debug)
  {
    std::cout << "Setting up Good Run List" << std::endl;
  }
  TString list = "/home/swalch/macro-cosmos/HiggsAnalysis/GoodRunLists/susy_grl_init.xml";
  Root::TGoodRunsListReader foo(list);
  foo.Interpret();
  Root::TGoodRunsList grl1 =  foo.GetMergedGoodRunsList();

  if (_debug)
  {
    std::cout << "Getting all component samples to run over" << std::endl;
  }
  std::vector<std::string> samples = GetAllSamples(sample);
  std::vector<std::string>::iterator iter;
  std::vector<std::string>::iterator end = samples.end();

  for (iter = samples.begin(); iter != end; ++iter)
  {
    std::string sampleName = *iter;
    TChain* chain = GetChain(sampleName);
    setBranchAddresses(chain);
    std::cout << "Analyzing sample " << sampleName << std::endl;

    if (_debug)
    {
      std::cout << "Checking sample xsection/weight information" << std::endl;
    }

    double sampleWeight = 0.0;
    double sampleXSec = 1.0;
    if (isMC)
    {
      sampleWeight = GetSampleWeight(sampleName);
      sampleXSec = GetCrossSection(sampleName);
    }

    if (_debug)
    {
      std::cout << "Entering event loop" << std::endl;
    }
    int eventNr = 0;
    double weight = 1.0;
    while(chain->GetEntry(eventNr++))
    {
      if (_debug)
      {
	std::cout << "Analyzing event" << std::endl;
      }

      if (eventNr % 10000 == 0)
      {
	std::cout << "SusyCutflow has analyzed " << eventNr << " events." << std::endl;
      }
      int RUNNUMBER;
      if (isMC)
      {
	weight = sampleXSec/sampleWeight*(_eventInfo->EventWeight());
	if ((sample.find("Z") != string::npos) || (sample.find("W") != string::npos))
	{
	  std::vector<MTree::PrimaryVertex*> pvs = getPVs();
          double temp = getPileUpWeight((int)pvs.size());
	  weight = weight * temp;
	}
	RUNNUMBER = 100000;
      }
      else
      {
	RUNNUMBER = _eventInfo->RunNumber();
      }

      //Good Run List and Loading of Object Quality Maps go here.
      if (_debug)
      {
	std::cout << "Checking good run list" << std::endl;
      }
      bool passed = true;
      if (isMC)
      {
	egammaOQ::LoadOQMaps(159040);
      }
      else
      {
	int runNr = _eventInfo->RunNumber();
	int lumiBlock = _eventInfo->LumiBlock();
	passed = grl1.HasRunLumiBlock(runNr,lumiBlock);
	egammaOQ::LoadOQMaps(runNr);
      }

      if (!passed) continue;

      //Remove events with bad jets/calorimeter problems
      if (_debug)
      {
	std::cout << "Checking for bad jets" << std::endl;
      }
      if (badJetsPresent()) continue;

      //Apply collision selection/primary vertex requirements
      if (_debug)
      {
	std::cout << "applying PV requirements" << std::endl;
      }
      std::vector<MTree::PrimaryVertex*> vertices = getPVs();
      if ((int)vertices.size() == 0) continue;

      //Apply object selections
      if (_debug)
      {
	std::cout << "object selections" << std::endl;
      }
      //Electrons first.
      std::vector<MTree::Electron*> electrons = getMediumElectrons(RUNNUMBER);
      std::vector<MTree::Electron*> crackElectrons = getCrackElectrons(RUNNUMBER,false);

      std::vector<MTree::Muon*> muons = getGoodMuons();

      std::vector<MTree::Jet*> jets = getJets();

      std::vector<MTree::Photon*> photons = getPhotons();


      //Now removing jets that are really photons, and veto events with trackless, 
      //high pT jets.
      if (_debug)
      {
	std::cout << "removing events with trackless jets" << std::endl;
      }
      removeOverlap(0.2, photons, jets);

      if (cosmicJetPresent(jets)) continue;

      if (crackElectrons.size() != 0) continue;

      //More removing of overlaps
      removeOverlap(0.2, electrons, jets);

      if (_debug)
      {
	std::cout << "Checking OTX veto" << std::endl;
      }
      if (OTXProblem(RUNNUMBER, electrons)) continue;

      if (_debug)
      {
	std::cout << "Checking for cosmic events" << std::endl;
      }
      if (hasCosmicMuon(muons)) continue;

      //And yet more overlap (see why I templated that function?)
      removeOverlap(0.4, jets, electrons);
      removeOverlap(0.4, jets, muons);

      std::vector<MTree::Lepton*> leptons;
      for (int i = 0; i != (int)electrons.size(); ++i)
      {
	leptons.push_back(electrons[i]);
      }
      for (int i = 0; i != (int)muons.size(); ++i)
      {
	leptons.push_back(muons[i]);
      }
      if (leptons.size() < 1) continue;


      //Add ceiling on MET and veto events with too many leptons. For now, I will just try this method 
      //with electrons, and leave the muon part blank.
      if (findMET(muons) > 10) continue;
      if (muons.size() == 0)
      {
	if (electrons.size() > 1) continue;

	MTree::Electron* elec = electrons[0];
	histos[0]->Fill(elec->ETCone20(),weight);
	histos[1]->Fill(elec->ETCone30(),weight);
	histos[2]->Fill(elec->ETCone40(),weight);
	histos[3]->Fill(elec->ETCone20()/elec->Pt(),weight);
	histos[4]->Fill(elec->ETCone30()/elec->Pt(),weight);
	histos[5]->Fill(elec->ETCone40()/elec->Pt(),weight);
	histos[12]->Fill(elec->Pt(),weight);
	histos[14]->Fill(elec->Eta(),weight);
	histos[16]->Fill(elec->Phi(),weight);
	histos[18]->Fill((int)jets.size(),weight);
	histos2[0]->Fill(elec->Pt(),elec->Eta(),weight);

	histos2[5]->Fill(elec->Pt(),elec->ETCone20()/elec->Pt(),weight);
	histos2[6]->Fill(elec->Pt(),elec->ETCone30()/elec->Pt(),weight);
	histos2[7]->Fill(elec->Pt(),elec->ETCone40()/elec->Pt(),weight);

	if (elec->ETCone40()/elec->Pt() > 0.1) continue;

	histos[20]->Fill(elec->ETCone20(),weight);
	histos[21]->Fill(elec->ETCone30(),weight);
	histos[22]->Fill(elec->ETCone40(),weight);
	histos[23]->Fill(elec->ETCone20()/elec->Pt(),weight);
	histos[24]->Fill(elec->ETCone30()/elec->Pt(),weight);
	histos[25]->Fill(elec->ETCone40()/elec->Pt(),weight);
	histos[32]->Fill(elec->Pt(),weight);
	histos[34]->Fill(elec->Eta(),weight);
	histos[36]->Fill(elec->Phi(),weight);
	histos[38]->Fill((int)jets.size(),weight);
	histos2[1]->Fill(elec->Pt(),elec->Eta(),weight);
      }

      if (electrons.size() == 0)
      {
	if (muons.size() > 1) continue;

	MTree::Muon* muon = muons[0];
	histos[6]->Fill(muon->ETCone20(),weight);
	histos[7]->Fill(muon->ETCone30(),weight);
	histos[8]->Fill(muon->ETCone40(),weight);
	histos[9]->Fill(muon->ETCone20()/muon->Pt(),weight);
	histos[10]->Fill(muon->ETCone30()/muon->Pt(),weight);
	histos[11]->Fill(muon->ETCone40()/muon->Pt(),weight);
	histos[13]->Fill(muon->Pt(),weight);
	histos[15]->Fill(muon->Eta(),weight);
	histos[17]->Fill(muon->Phi(),weight);
	histos[19]->Fill((int)jets.size(),weight);
	histos2[2]->Fill(muon->Pt(),muon->Eta(),weight);

	histos2[8]->Fill(muon->Pt(),muon->ETCone20()/muon->Pt(),weight);
	histos2[9]->Fill(muon->Pt(),muon->ETCone30()/muon->Pt(),weight);
	histos2[10]->Fill(muon->Pt(),muon->ETCone40()/muon->Pt(),weight);

	if ((muon->ETCone40()/muon->Pt()) > 0.1) continue;

	histos[26]->Fill(muon->ETCone20(),weight);
	histos[27]->Fill(muon->ETCone30(),weight);
	histos[28]->Fill(muon->ETCone40(),weight);
	histos[29]->Fill(muon->ETCone20()/muon->Pt(),weight);
	histos[30]->Fill(muon->ETCone30()/muon->Pt(),weight);
	histos[31]->Fill(muon->ETCone40()/muon->Pt(),weight);
	histos[33]->Fill(muon->Pt(),weight);
	histos[35]->Fill(muon->Eta(),weight);
	histos[37]->Fill(muon->Phi(),weight);
	histos[39]->Fill((int)jets.size(),weight);
	histos2[3]->Fill(muon->Pt(),muon->Eta(),weight);
      }

//       if (eventNr > 1000)
//       {
// 	eventNr = 1000000000;
//       }

      if (_debug)
      {
	std::cout << "reached end of event loop" << std::endl;
      }

    }//Closes event loop
  }//Closes loop over samples

}//Closes EstimateEfficiencies function

//######################################################################################################

bool SusyAnalysis::removeDecayProducts(std::string sample, std::vector<MTree::Electron*> &elecs, std::vector<MTree::Muon*> &muons, std::vector<MTree::TruthParticle*> &truthies)
{
  bool worked = true;

  //Figure out from sample what kind of boson to look for
  int parentID = -1;
  if (sample.find("W") != string::npos)
  {
    parentID = 24;
  }
  else if (sample.find("Z") != string::npos)
  {
    parentID = 23;
  }

  //Dig through truth information to find the true leptons to remove
  std::vector<MTree::TruthParticle*> daughters;
  truthies.clear();
  int nTruthParticles = _truthParticles->GetEntriesFast();
  for (int i = 0; i != nTruthParticles; ++i)
  {
    MTree::TruthParticle* truth = (MTree::TruthParticle*)_truthParticles->At(i);
    if ((TMath::Abs(truth->ParentPdgID()) == parentID) && ((TMath::Abs(truth->PdgID()) == 11) || (TMath::Abs(truth->PdgID()) == 13)))
    {
      daughters.push_back(truth);
    }
    else
    {
      truthies.push_back(truth);
    }
  }
  double highPt = 0;
  for (int i = 0; i != (int)daughters.size(); ++i)
  {
    if (daughters[i]->Pt() > highPt)
    {
      highPt = daughters[i]->Pt();
    }
  }
  if (highPt < 20)
  {
    worked = false;
  }

//   if ((parentID == 24) && ((int)daughters.size() != 1))
//   {
//     std::cout << "A W somehow doesn't have exactly one daughter!" << std::endl;
//     worked = false;
//   }
//   if ((parentID == 23) && ((int)daughters.size() != 2))
//   {
//     std::cout << "A Z somehow doesn't have exactly two daughters!" << std::endl;
//     for (int j = 0; j != (int)daughters.size(); ++j)
//     {
//       std::cout << "Daughter " << j << " has id " << daughters[j]->PdgID() << std::endl;
//     }
//     std::cout << std::endl;
//     worked = false;
//   }

  //Use deltaR matching to remove those leptons from the vectors of reconstructed stuff
  if (TMath::Abs(daughters[0]->PdgID()) == 11)
  {
    int initial = (int)elecs.size();
    for (int i = 0; i != (int)daughters.size(); ++i)
    {
      for (int j = 0; j != (int)elecs.size(); ++j)
      {
	if (TMath::Abs(elecs[j]->DeltaR(*daughters[i])) < 0.4)
	{
	  elecs.erase(elecs.begin() + j);
          --j;
	}
      }
    }

    if ((((int)elecs.size() + (int)daughters.size()) >= initial) && (initial != 0))
    {
      worked = false;
    }
  }
  else if (TMath::Abs(daughters[0]->PdgID()) == 13)
  {
    int initial = (int)muons.size();
    for (int i = 0; i != (int)daughters.size(); ++i)
    {
      for (int j = 0; j != (int)muons.size(); ++j)
      {
	if (TMath::Abs(muons[j]->DeltaR(*daughters[i])) < 0.4)
	{
	  muons.erase(muons.begin() + j);
	  --j;
	}
      }
    }
    if ((((int)muons.size() + (int)daughters.size()) >= initial) && (initial != 0))
    {
      worked = false;
    }

    //If the reco'd particle is on top of a truth photon, evict the reco'd particle, too.
    for (int i = 0; i != (int)truthies.size(); ++i)
    {
      MTree::TruthParticle* truth = truthies[i];
      if (truth->PdgID() == 22)
      {
        for (int j = 0; j != (int)elecs.size(); ++j)
        {
	  if (TMath::Abs(elecs[j]->DeltaR(*truth)) < 0.2)
	  {
	    elecs.erase(elecs.begin() + j);
	    --j;

	  }
        }
	for (int j = 0; j != (int)muons.size(); ++j)
	{
	  if (TMath::Abs(muons[j]->DeltaR(*truth)) < 0.2)
	  {
	    muons.erase(muons.begin() + j);
	    --j;

	  }
	}
      }
    }
  }
  return(worked);
}

//######################################################################################################

void SusyAnalysis::qcdEstimate(std::string sample, TH1D** histos, std::vector<std::vector<std::vector<TH1D*> > > oshistos, std::vector<std::vector<std::vector<TH1D*> > > sshistos)
{
  if (_debug)
  {
    std::cout << "Calling qcdEstimate()" << std::endl;
  }

  bool isMC = false;
  if ((sample.find("EGamma") == string::npos) && (sample.find("Muons") == string::npos) && (sample.find("data") == string::npos))
  {
    isMC = true;
  }

  if (_debug)
  {
    std::cout << "Setting up Good Run List" << std::endl;
  }
  TString list = "/home/swalch/macro-cosmos/HiggsAnalysis/GoodRunLists/susy_grl_init.xml";
  Root::TGoodRunsListReader foo(list);
  foo.Interpret();
  Root::TGoodRunsList grl1 =  foo.GetMergedGoodRunsList();

  if (_debug)
  {
    std::cout << "Getting all component samples to run over" << std::endl;
  }
  std::vector<std::string> samples = GetAllSamples(sample);
  std::vector<std::string>::iterator iter;
  std::vector<std::string>::iterator end = samples.end();

  for (iter = samples.begin(); iter != end; ++iter)
  {
    std::string sampleName = *iter;
    TChain* chain = GetChain(sampleName);
    setBranchAddresses(chain);
    std::cout << "Analyzing sample " << sampleName << std::endl;

    if (_debug)
    {
      std::cout << "Checking sample xsection/weight information" << std::endl;
    }

    double sampleWeight = 0.0;
    double sampleXSec = 1.0;
    if (isMC)
    {
      sampleWeight = GetSampleWeight(sampleName);
      sampleXSec = GetCrossSection(sampleName);
    }

    if (_debug)
    {
      std::cout << "Entering event loop" << std::endl;
    }
    int eventNr = 0;
    double weight = 1.0;
    while(chain->GetEntry(eventNr++))
    {
      if (_debug)
      {
	std::cout << "Analyzing event" << std::endl;
      }

      if (eventNr % 10000 == 0)
      {
	std::cout << "qcdEstimate has analyzed " << eventNr << " events." << std::endl;
      }
      int RUNNUMBER;
      if (isMC)
      {
	weight = sampleXSec/sampleWeight*(_eventInfo->EventWeight())*getPileUpWeight((int)(getPVs()).size());
	if ((sample.find("Z") != string::npos) || (sample.find("W") != string::npos))
	{
	  weight = weight*getPileUpWeight((int)(getPVs()).size());
	}
	RUNNUMBER = 100000;
      }
      else
      {
	RUNNUMBER = _eventInfo->RunNumber();
      }

      //Good Run List and Loading of Object Quality Maps go here.
      if (_debug)
      {
	std::cout << "Checking good run list" << std::endl;
      }
      bool passed = true;
      if (isMC)
      {
	egammaOQ::LoadOQMaps(166497);
      }
      else
      {
	int runNr = _eventInfo->RunNumber();
	int lumiBlock = _eventInfo->LumiBlock();
	passed = grl1.HasRunLumiBlock(runNr,lumiBlock);
	egammaOQ::LoadOQMaps(runNr);
      }

      if (!passed) continue;

      //Impose trigger requirements
      if (_debug)
      {
	std::cout << "imposing trigger requirements" << std::endl;
      }
      if (!firstTriggerCheck(sample, RUNNUMBER)) continue;

      //Remove events with bad jets/calorimeter problems
      if (_debug)
      {
	std::cout << "Checking for bad jets" << std::endl;
      }
      if (badJetsPresent()) continue;

      //Apply collision selection/primary vertex requirements
      if (_debug)
      {
	std::cout << "applying PV requirements" << std::endl;
      }
      std::vector<MTree::PrimaryVertex*> vertices = getPVs();
      if ((int)vertices.size() == 0) continue;

      //Apply object selections
      if (_debug)
      {
	std::cout << "object selections" << std::endl;
      }

      //Electrons first.
      std::vector<MTree::Electron*> electrons = getMediumElectrons(RUNNUMBER);
      std::vector<MTree::Electron*> looseElectrons = getLooseElectrons(RUNNUMBER);
      std::vector<MTree::Electron*> crackElectrons = getCrackElectrons(RUNNUMBER,true);
//       int nElectrons = _electrons->GetEntries();
//       for (int i = 0; i != nElectrons; ++i)
//       {
// 	MTree::Electron* elec = (MTree::Electron*)_electrons->At(i);

// 	if (elec->Pt() < 10) continue;
// 	if (TMath::Abs(elec->ClusterEta()) > 2.47) continue;
// 	if (elec->Author() != 1 && elec->Author() != 3) continue;

// 	double et = 1000*elec->ClusterE()/TMath::CosH(TMath::Abs(elec->ClusterEta()));

// 	bool medium = isRobustMedium(elec->IsEM(),elec->ClusterEta(),et,elec->E237()/elec->E277(),elec->WEta2());
// 	bool loose = isRobustLoose(elec->IsEM(),elec->ClusterEta(),et,elec->E237()/elec->E277(),elec->WEta2());

// 	int trackBLayer = 16;
// 	const unsigned BLAYER = 0x1 << trackBLayer;
// 	bool hasBLayerHit = (elec->IsEM() & BLAYER) == 0;
// 	if (elec->ExpectHitInBLayer() && !hasBLayerHit) continue;

// // 	if ((medium) && (TMath::Abs(elec->ClusterEta()) > 1.37) && (TMath::Abs(elec->ClusterEta()) < 1.52))
// // 	{
// // 	  crackElectrons.push_back(elec);
// // 	  continue;
// // 	}
// 	if (elec->ETCone20() > 10) continue;

// 	if (medium)
// 	{
// // 	  electrons.push_back(elec);
// 	}
// 	else if (loose)
// 	{
// 	  looseElectrons.push_back(elec);
// 	}
//       }

      std::vector<MTree::Muon*> muons = getGoodMuons();

      //Make sure I have exactly a medium+loose electron pair, and veto everything else.
      if (!(((int)muons.size() == 0) && ((int)electrons.size() == 1) && ((int)looseElectrons.size() == 1))) continue;
      electrons.push_back(looseElectrons[0]);

      std::vector<MTree::Jet*> jets = getJets();

      //And behold, my first foray into photons.
      std::vector<MTree::Photon*> photons = getPhotons();

      //Now removing jets that are really photons, and veto events with trackless, 
      //high pT jets.
      if (_debug)
      {
	std::cout << "removing events with trackless jets" << std::endl;
      }
//       removeOverlap(0.2, photons, jets);

//       bool failJetPhot = false;
//       int nTracks = _tracks->GetEntriesFast();
//       for (int i = 0; i != (int)jets.size(); i++)
//       {
//         int nTracksForJet = 0;
//         for (int j = 0; j != nTracks; j++)
//         {
//           MTree::Track* tr = (MTree::Track*)_tracks->At(j);
//           if (tr->Pt() < 0.5) continue;
//           int nrHitsPix = tr->NrOfPixelHits();
//           int nrHitsSCT = tr->NrOfSCTHits();
//           double d0vtx = tr->Vtx_D0();
//           double z0vtx = tr->Vtx_Z0();
//           if (nrHitsPix < 1) continue;
//           if (nrHitsSCT < 1) continue;
//           if (TMath::Abs(d0vtx) > 1.5) continue;
//           if (TMath::Abs(z0vtx) > 1.5) continue;
//           double dR = tr->DeltaR(*jets[i]);
//           if (dR < 0.4) 
// 	  {
//             nTracksForJet++;
// 	  }
//         }
//         double pt = jets[i]->GetRawPt();
//         pt *= jets[i]->GetEmJESFactor();
//         if (pt > 50 && nTracksForJet == 0) 
// 	{
// 	  failJetPhot = true;
// 	}
//       }

//       if (cosmicJetPresent(jets)) continue;

      if (crackElectrons.size() != 0) continue;

      //More removing of overlaps
      removeOverlap(0.2, electrons, jets);

      if (_debug)
      {
	std::cout << "Checking OTX veto" << std::endl;
      }
//       bool OTXVeto = false;
//       for (int i = 0; i != (int)electrons.size(); ++i)
//       {
// 	int run = 0;
// 	if (isMC)
// 	{
// 	  run = 159040;
// 	}
// 	else
// 	{
// 	  run = _eventInfo->RunNumber();
// 	}
// 	if (egammaOQ::checkOQClusterElectron(run,electrons[i]->ClusterEta(),electrons[i]->ClusterPhi()) == 3)
// 	{
// 	  OTXVeto = true;
// 	}
//       }

      if (OTXProblem(RUNNUMBER, electrons)) continue;

      if (_debug)
      {
	std::cout << "Checking for cosmic events" << std::endl;
      }
//       bool CosmicVeto = false;
//       for (int i = 0; i != (int)muons.size(); ++i)
//       {
// 	MTree::Track* track = muons[i]->GetTrack();
// 	if (TMath::Abs(track->Vtx_Z0()) >= 10)
// 	{
// 	  CosmicVeto = true;
// 	}
//       }

	  if (hasCosmicMuon(muons)) continue;

      //And yet more overlap (see why I templated that function?)
      removeOverlap(0.4, jets, electrons);
      removeOverlap(0.4, jets, muons);

      if (((int)electrons.size() + (int)muons.size()) < 1) continue;

      std::vector<MTree::Lepton*> leptons;
      for (int i = 0; i != (int)electrons.size(); ++i)
      {
	leptons.push_back(electrons[i]);
      }
      for (int i = 0; i != (int)muons.size(); ++i)
      {
	leptons.push_back(muons[i]);
      }
      orderByPt(leptons);

      if (_debug)
      {
	std::cout << "requiring at least one lepton" << std::endl;
      }

      if (!checkTrigger(sample,RUNNUMBER, leptons)) continue;

      if ((int)leptons.size() < 2) continue;

      //Make sure there's a leading lepton with pT > 20 GeV
      if (_debug)
      {
	std::cout << "check leading lepton pT" << std::endl;
      }

      orderByPt(leptons);
      if (!sillyHighPtCut(RUNNUMBER, leptons)) continue;

      if (leptons.size() != 2) continue;

      if (!sillierCheckTrigger(sample,RUNNUMBER, leptons)) continue;
      std::vector<std::string> channels;
      channels = findChannels(leptons);

      //Second set of plots
      if (_debug)
      {
	std::cout << "Filling first set of plots" << std::endl;
      }
      int stage = 0;
      bool isSS = false;
      if (leptons[0]->Charge() == leptons[1]->Charge())
      {
	isSS = true;
	if (channels[0] == "ee")
	{
	  sshistos[0][0][stage]->Fill(leptons[0]->Pt(),weight);
	  sshistos[1][0][stage]->Fill(leptons[1]->Pt(),weight);
	  sshistos[2][0][stage]->Fill(leptons[0]->Eta(),weight);
	  sshistos[3][0][stage]->Fill(leptons[1]->Eta(),weight);
	  sshistos[4][0][stage]->Fill(leptons[0]->Phi(),weight);
	  sshistos[5][0][stage]->Fill(leptons[1]->Phi(),weight);
	  sshistos[6][0][stage]->Fill(findMET(muons),weight);
	  sshistos[7][0][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  sshistos[8][0][stage]->Fill((int)jets.size(),weight);
	  sshistos[9][0][stage]->Fill((int)photons.size(),weight);
	  sshistos[10][0][stage]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  sshistos[11][0][stage]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	}
	++stage;
      }
      else
      {
	if (channels[0] == "ee")
	{
	  oshistos[0][0][stage]->Fill(leptons[0]->Pt(),weight);
	  oshistos[1][0][stage]->Fill(leptons[1]->Pt(),weight);
	  oshistos[2][0][stage]->Fill(leptons[0]->Eta(),weight);
	  oshistos[3][0][stage]->Fill(leptons[1]->Eta(),weight);
	  oshistos[4][0][stage]->Fill(leptons[0]->Phi(),weight);
	  oshistos[5][0][stage]->Fill(leptons[1]->Phi(),weight);
	  oshistos[6][0][stage]->Fill(findMET(muons),weight);
	  oshistos[7][0][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  oshistos[8][0][stage]->Fill((int)jets.size(),weight);
	  oshistos[9][0][stage]->Fill((int)photons.size(),weight);
	  oshistos[10][0][stage]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  oshistos[11][0][stage]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	}
	++stage;
      }

      TLorentzVector vec = *leptons[0] + *leptons[1];
      if (vec.M() < 5) continue;

      //Now, treat OS and SS differently.
      if (isSS)
      {
	//Take a moment and fill QCD control region counter.
	if ((vec.M() < 60) && (findMET(muons) < 15) && !(isMC))
	{
	  histos[0]->Fill(0.0,weight);
	}
	if (channels[0] == "ee")
	{
	  sshistos[0][0][stage]->Fill(leptons[0]->Pt(),weight);
	  sshistos[1][0][stage]->Fill(leptons[1]->Pt(),weight);
	  sshistos[2][0][stage]->Fill(leptons[0]->Eta(),weight);
	  sshistos[3][0][stage]->Fill(leptons[1]->Eta(),weight);
	  sshistos[4][0][stage]->Fill(leptons[0]->Phi(),weight);
	  sshistos[5][0][stage]->Fill(leptons[1]->Phi(),weight);
	  sshistos[6][0][stage]->Fill(findMET(muons),weight);
	  sshistos[7][0][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  sshistos[8][0][stage]->Fill((int)jets.size(),weight);
	  sshistos[9][0][stage]->Fill((int)photons.size(),weight);
	  sshistos[10][0][stage]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  sshistos[11][0][stage]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	}
	++stage;

	orderByPt(jets);
	if (jets.size() < 2) continue;
	if (jets[1]->Pt() < 50) continue;

	if (channels[0] == "ee")
	{
	  sshistos[0][0][stage]->Fill(leptons[0]->Pt(),weight);
	  sshistos[1][0][stage]->Fill(leptons[1]->Pt(),weight);
	  sshistos[2][0][stage]->Fill(leptons[0]->Eta(),weight);
	  sshistos[3][0][stage]->Fill(leptons[1]->Eta(),weight);
	  sshistos[4][0][stage]->Fill(leptons[0]->Phi(),weight);
	  sshistos[5][0][stage]->Fill(leptons[1]->Phi(),weight);
	  sshistos[6][0][stage]->Fill(findMET(muons),weight);
	  sshistos[7][0][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  sshistos[8][0][stage]->Fill((int)jets.size(),weight);
	  sshistos[9][0][stage]->Fill((int)photons.size(),weight);
	  sshistos[10][0][stage]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  sshistos[11][0][stage]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	}

	if (findMET(muons) < 80) continue;

	if (channels[0] == "ee")
	{
	  sshistos[0][0][stage]->Fill(leptons[0]->Pt(),weight);
	  sshistos[1][0][stage]->Fill(leptons[1]->Pt(),weight);
	  sshistos[2][0][stage]->Fill(leptons[0]->Eta(),weight);
	  sshistos[3][0][stage]->Fill(leptons[1]->Eta(),weight);
	  sshistos[4][0][stage]->Fill(leptons[0]->Phi(),weight);
	  sshistos[5][0][stage]->Fill(leptons[1]->Phi(),weight);
	  sshistos[6][0][stage]->Fill(findMET(muons),weight);
	  sshistos[7][0][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  sshistos[8][0][stage]->Fill((int)jets.size(),weight);
	  sshistos[9][0][stage]->Fill((int)photons.size(),weight);
	  sshistos[10][0][stage]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  sshistos[11][0][stage]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	}
	++stage;

//Put cut for matrix method here.
	bool twoGoodOnes = true;
	for (int i = 0; i != (int)leptons.size(); ++i)
	{
	  double iso;
	  if (leptons[i]->LeptonPdgID() == 11)
	  {
	    MTree::Electron* elec = (MTree::Electron*)leptons[i];
	    iso = elec->ETCone40()/elec->Pt();
	  }
	  else
	  {
	    MTree::Muon* muon = (MTree::Muon*)leptons[i];
	    iso = muon->ETCone40()/muon->Pt();
	  }
	  if (iso > 0.1)
	  {
	    twoGoodOnes = false;
	  }
	}

	if (channels[0] == "ee")
	{
	  sshistos[0][0][stage]->Fill(leptons[0]->Pt(),weight);
	  sshistos[1][0][stage]->Fill(leptons[1]->Pt(),weight);
	  sshistos[2][0][stage]->Fill(leptons[0]->Eta(),weight);
	  sshistos[3][0][stage]->Fill(leptons[1]->Eta(),weight);
	  sshistos[4][0][stage]->Fill(leptons[0]->Phi(),weight);
	  sshistos[5][0][stage]->Fill(leptons[1]->Phi(),weight);
	  sshistos[6][0][stage]->Fill(findMET(muons),weight);
	  sshistos[7][0][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  sshistos[8][0][stage]->Fill((int)jets.size(),weight);
	  sshistos[9][0][stage]->Fill((int)photons.size(),weight);
	  sshistos[10][0][stage]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  sshistos[11][0][stage]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	}
	++stage;
      }
      else
      {
	if (channels[0] == "ee")
	{
	  oshistos[0][0][stage]->Fill(leptons[0]->Pt(),weight);
	  oshistos[1][0][stage]->Fill(leptons[1]->Pt(),weight);
	  oshistos[2][0][stage]->Fill(leptons[0]->Eta(),weight);
	  oshistos[3][0][stage]->Fill(leptons[1]->Eta(),weight);
	  oshistos[4][0][stage]->Fill(leptons[0]->Phi(),weight);
	  oshistos[5][0][stage]->Fill(leptons[1]->Phi(),weight);
	  oshistos[6][0][stage]->Fill(findMET(muons),weight);
	  oshistos[7][0][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	  oshistos[8][0][stage]->Fill((int)jets.size(),weight);
	  oshistos[9][0][stage]->Fill((int)photons.size(),weight);
	  oshistos[10][0][stage]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	  oshistos[11][0][stage]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	}
	++stage;

	if (_debug)
	{
	  std::cout << "entering loop over met" << std::endl;
	}
	int m = 0;
	while ((findMET(muons) > (30 + m*35)) &&  (m < 3))
	{
	  if (_debug)
	  {
	    std::cout << "met iteration " << m << std::endl;
	  }
	  ++m;

	  orderByPt(leptons);

	  if (channels[0] == "ee")
	  {
	    if (_debug)
	    {
	      std::cout << "filling os met loop ee plots" << std::endl;
	    }
	    oshistos[0][0][stage]->Fill(leptons[0]->Pt(),weight);
	    oshistos[1][0][stage]->Fill(leptons[1]->Pt(),weight);
	    oshistos[2][0][stage]->Fill(leptons[0]->Eta(),weight);
	    oshistos[3][0][stage]->Fill(leptons[1]->Eta(),weight);
	    oshistos[4][0][stage]->Fill(leptons[0]->Phi(),weight);
	    oshistos[5][0][stage]->Fill(leptons[1]->Phi(),weight);
	    oshistos[6][0][stage]->Fill(findMET(muons),weight);
	    oshistos[7][0][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	    oshistos[8][0][stage]->Fill((int)jets.size(),weight);
	    oshistos[9][0][stage]->Fill((int)photons.size(),weight);
	    oshistos[10][0][stage]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	    oshistos[11][0][stage]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	  }
	  ++stage;

          //Put cut on isolation of leptons here.
	  if (_debug)
	  {
	    std::cout << "checking lepton isolation" << std::endl;
	  }
	  bool twoGoodOnes = true;
	  for (int i = 0; i != (int)leptons.size(); ++i)
 	  {
	    double iso;
	    if (leptons[i]->LeptonPdgID() == 11)
	    {
	      MTree::Electron* elec = (MTree::Electron*)leptons[i];
	      iso = elec->ETCone40()/elec->Pt();
	    }
	    else
	    {
	      MTree::Muon* muon = (MTree::Muon*)leptons[i];
	      iso = muon->ETCone40()/muon->Pt();
	    }
	    if (iso > 0.1)
	    {
	      twoGoodOnes = false;
	    }
	  }

	  if (!twoGoodOnes) 
          {
	    ++stage;
            continue;
	  }

	  if (channels[0] == "ee")
	  {
	    oshistos[0][0][stage]->Fill(leptons[0]->Pt(),weight);
	    oshistos[1][0][stage]->Fill(leptons[1]->Pt(),weight);
	    oshistos[2][0][stage]->Fill(leptons[0]->Eta(),weight);
	    oshistos[3][0][stage]->Fill(leptons[1]->Eta(),weight);
	    oshistos[4][0][stage]->Fill(leptons[0]->Phi(),weight);
	    oshistos[5][0][stage]->Fill(leptons[1]->Phi(),weight);
	    oshistos[6][0][stage]->Fill(findMET(muons),weight);
	    oshistos[7][0][stage]->Fill((*leptons[0]+*leptons[1]).M(),weight);
	    oshistos[8][0][stage]->Fill((int)jets.size(),weight);
	    oshistos[9][0][stage]->Fill((int)photons.size(),weight);
	    oshistos[10][0][stage]->Fill(((MTree::Electron*)leptons[0])->ETCone40()/leptons[0]->Pt(),weight);
	    oshistos[11][0][stage]->Fill(((MTree::Electron*)leptons[1])->ETCone40()/leptons[1]->Pt(),weight);
	  }
	  ++stage;
	}//closes while loop over met's

      }//Closes os half of if statement

      if (_debug)
      {
	std::cout << "reached end of event loop" << std::endl;
      }

    }//Closes event loop
  }//Closes loop over samples
}//Closes qcdEstimate

//######################################################################################################

void SusyAnalysis::EstimateEfficienciesZ(std::string sample, TH1D** histos, TH2D** histos2)
{
  if (_debug)
  {
    std::cout << "Calling EstimateEfficienciesZ()" << std::endl;
  }

  bool isMC = false;
  if ((sample.find("EGamma") == string::npos) && (sample.find("Muon") == string::npos) && (sample.find("data") == string::npos) && (sample.find("Elec") == string::npos))
  {
    isMC = true;
  }

  if (_debug)
  {
    std::cout << "Setting up Good Run List" << std::endl;
  }
  TString list = "/home/swalch/macro-cosmos/HiggsAnalysis/GoodRunLists/susy_grl_init.xml";
  Root::TGoodRunsListReader foo(list);
  foo.Interpret();
  Root::TGoodRunsList grl1 =  foo.GetMergedGoodRunsList();

  if (_debug)
  {
    std::cout << "Getting all component samples to run over" << std::endl;
  }
  std::vector<std::string> samples = GetAllSamples(sample);
  std::vector<std::string>::iterator iter;
  std::vector<std::string>::iterator end = samples.end();

  for (iter = samples.begin(); iter != end; ++iter)
  {
    std::string sampleName = *iter;
    TChain* chain = GetChain(sampleName);
    setBranchAddresses(chain);
    std::cout << "Analyzing sample " << sampleName << std::endl;

    if (_debug)
    {
      std::cout << "Checking sample xsection/weight information" << std::endl;
    }

    double sampleWeight = 0.0;
    double sampleXSec = 1.0;
    if (isMC)
    {
      sampleWeight = GetSampleWeight(sampleName);
      sampleXSec = GetCrossSection(sampleName);
    }

    if (_debug)
    {
      std::cout << "Entering event loop" << std::endl;
    }
    int eventNr = 0;
    double weight = 1.0;
    while(chain->GetEntry(eventNr++))
    {
      if (_debug)
      {
	std::cout << "Analyzing event" << std::endl;
      }

      if (eventNr % 10000 == 0)
      {
	std::cout << "Est. Eff. Z  has analyzed " << eventNr << " events." << std::endl;
      }
      int RUNNUMBER;
      if (isMC)
      {
	weight = sampleXSec/sampleWeight*(_eventInfo->EventWeight());
	if ((sample.find("Z") != string::npos) || (sample.find("W") != string::npos))
	{
	  std::vector<MTree::PrimaryVertex*> pvs = getPVs();
          double temp = getPileUpWeight((int)pvs.size());
	  weight = weight * temp;
	}
	RUNNUMBER = 100000;
      }
      else
      {
	RUNNUMBER = _eventInfo->RunNumber();
      }

      //Good Run List and Loading of Object Quality Maps go here.
      if (_debug)
      {
	std::cout << "Checking good run list" << std::endl;
      }
      bool passed = true;
      if (isMC)
      {
	egammaOQ::LoadOQMaps(159040);
      }
      else
      {
	int runNr = _eventInfo->RunNumber();
	int lumiBlock = _eventInfo->LumiBlock();
	passed = grl1.HasRunLumiBlock(runNr,lumiBlock);
	egammaOQ::LoadOQMaps(runNr);
      }

      if (!passed) continue;

      //Impose trigger requirements
      if (_debug)
      {
	std::cout << "imposing trigger requirements" << std::endl;
      }

      if (!firstTriggerCheck(sample, RUNNUMBER)) continue;

      //Remove events with bad jets/calorimeter problems
      if (_debug)
      {
	std::cout << "Checking for bad jets" << std::endl;
      }

      if (badJetsPresent()) continue;

      //Apply collision selection/primary vertex requirements
      if (_debug)
      {
	std::cout << "applying PV requirements" << std::endl;
      }
      std::vector<MTree::PrimaryVertex*> primaryVs = getPVs();

      if ((int)primaryVs.size() == 0) continue;

      //Apply object selections
      if (_debug)
      {
	std::cout << "object selections" << std::endl;
      }
      //Electrons first.
      std::vector<MTree::Electron*> electrons = getMediumElectrons(RUNNUMBER);
      std::vector<MTree::Electron*> crackElectrons = getCrackElectrons(RUNNUMBER,false);

      std::vector<MTree::Muon*> muons = getGoodMuons();

      std::vector<MTree::Jet*> jets = getJets();

      std::vector<MTree::Photon*> photons = getPhotons();


      //Now removing jets that are really photons, and veto events with trackless, 
      //high pT jets.
      if (_debug)
      {
	std::cout << "removing events with trackless jets" << std::endl;
      }
      removeOverlap(0.2, photons, jets);

      if (cosmicJetPresent(jets)) continue;

      if (crackElectrons.size() != 0) continue;

      //More removing of overlaps
      removeOverlap(0.2, electrons, jets);

      if (_debug)
      {
	std::cout << "Checking OTX veto" << std::endl;
      }

      if (OTXProblem(RUNNUMBER, electrons)) continue;

      if (_debug)
      {
	std::cout << "Checking for cosmic events" << std::endl;
      }

      if (hasCosmicMuon(muons)) continue;

      //And yet more overlap (see why I templated that function?)
      removeOverlap(0.4, jets, electrons);
      removeOverlap(0.4, jets, muons);

      std::vector<MTree::Lepton*> leptons;
      for (int i = 0; i != (int)electrons.size(); ++i)
      {
	leptons.push_back(electrons[i]);
      }
      for (int i = 0; i != (int)muons.size(); ++i)
      {
	leptons.push_back(muons[i]);
      }
      if (leptons.size() != 2) continue;

      if (!sillierCheckTrigger(sample, RUNNUMBER,leptons)) continue;

      //Fill primary vertex histogram
      histos[15]->Fill((int)primaryVs.size(),weight);

      //Add a cut on dilepton mass to zero in on Zs
      TLorentzVector ll = *leptons[0] + *leptons[1];
      if (ll.M() < 76.0) continue;
      if (ll.M() > 106.0) continue;

      //And since Z's have to have opposite sign and same flavor . . .
      if (leptons[0]->Charge() == leptons[1]->Charge()) continue;
      if (TMath::Abs(leptons[0]->LeptonPdgID()) != TMath::Abs(leptons[1]->LeptonPdgID())) continue;

      if ((int)jets.size() < 2) continue;

      //Fill isolation plots, to see what would be a good variable to use, and 'loose' maps.
      //Oh, and why not make the cut and fill the tight ones at the same time.
      //Just for kicks.
      if (_debug)
      {
	std::cout << "filling necessary plots" << std::endl;
      }
      for (int i = 0; i != (int)electrons.size(); ++i)
      {
	MTree::Electron* elec = electrons[i];
	histos[0]->Fill(elec->ETCone20(),weight);
	histos[1]->Fill(elec->ETCone30(),weight);
	histos[2]->Fill(elec->ETCone40(),weight);
	histos[3]->Fill(elec->ETCone20()/elec->Pt(),weight);
	histos[4]->Fill(elec->ETCone30()/elec->Pt(),weight);
	histos[5]->Fill(elec->ETCone40()/elec->Pt(),weight);
	histos2[0]->Fill(elec->Pt(),elec->Eta(),weight);

	histos2[5]->Fill(elec->Pt(),elec->ETCone20()/elec->Pt(),weight);
	histos2[6]->Fill(elec->Pt(),elec->ETCone30()/elec->Pt(),weight);
	histos2[7]->Fill(elec->Pt(),elec->ETCone40()/elec->Pt(),weight);

	if ((elec->ETCone40()/elec->Pt()) > 0.1) continue;
// 	double et = 1000*elec->ClusterE()/TMath::CosH(TMath::Abs(elec->ClusterEta()));
// 	if (!(isRobusterTight(elec->IsEM(),elec->ExpectHitInBLayer(),elec->ClusterEta(),et,elec->E237()/elec->E277(),elec->WEta2()))) continue;

	histos2[1]->Fill(elec->Pt(),elec->Eta(),weight);
      }
      for (int i = 0; i != (int)muons.size(); ++i)
      {
	MTree::Muon* muon = muons[i];
	histos[6]->Fill(muon->ETCone20(),weight);
	histos[7]->Fill(muon->ETCone30(),weight);
	histos[8]->Fill(muon->ETCone40(),weight);
	histos[9]->Fill(muon->ETCone20()/muon->Pt(),weight);
	histos[10]->Fill(muon->ETCone30()/muon->Pt(),weight);
	histos[11]->Fill(muon->ETCone40()/muon->Pt(),weight);
        histos2[2]->Fill(muon->Pt(),muon->Eta(),weight);

	histos2[8]->Fill(muon->Pt(),muon->ETCone20(),weight);
	histos2[9]->Fill(muon->Pt(),muon->ETCone30(),weight);
	histos2[10]->Fill(muon->Pt(),muon->ETCone40(),weight);

	if ((muon->ETCone40()/muon->Pt()) > 0.1) continue;

	histos2[3]->Fill(muon->Pt(),muon->Eta(),weight);
      }


//       if (eventNr > 100)
//       {
// 	eventNr = 1000000000;
//       }

      if (_debug)
      {
	std::cout << "reached end of event loop" << std::endl;
      }

    }//Closes event loop
  }//Closes loop over samples
}

//########################################################################################################

void SusyAnalysis::EffEstWZ(std::string sample, TH1D** histos, TH2D** histos2)
{
  if (_debug)
  {
    std::cout << "Calling EffEstWZ()" << std::endl;
  }

  bool isMC = false;
  if ((sample.find("EGamma") == string::npos) && (sample.find("Muon") == string::npos) && (sample.find("data") == string::npos) && (sample.find("Elec") == string::npos))
  {
    isMC = true;
  }

  if (_debug)
  {
    std::cout << "Setting up Good Run List" << std::endl;
  }
  TString list = "/home/swalch/macro-cosmos/HiggsAnalysis/GoodRunLists/susy_grl_init.xml";
  Root::TGoodRunsListReader foo(list);
  foo.Interpret();
  Root::TGoodRunsList grl1 =  foo.GetMergedGoodRunsList();

  if (_debug)
  {
    std::cout << "Getting all component samples to run over" << std::endl;
  }
  std::vector<std::string> samples = GetAllSamples(sample);
  std::vector<std::string>::iterator iter;
  std::vector<std::string>::iterator end = samples.end();

  for (iter = samples.begin(); iter != end; ++iter)
  {
    std::string sampleName = *iter;
    TChain* chain = GetChain(sampleName);
    setBranchAddresses(chain);
    std::cout << "Analyzing sample " << sampleName << std::endl;

    if (_debug)
    {
      std::cout << "Checking sample xsection/weight information" << std::endl;
    }

    double sampleWeight = 0.0;
    double sampleXSec = 1.0;
    if (isMC)
    {
      sampleWeight = GetSampleWeight(sampleName);
      sampleXSec = GetCrossSection(sampleName);
    }

    if (_debug)
    {
      std::cout << "Entering event loop" << std::endl;
    }
    int eventNr = 0;
    double weight = 1.0;
    while(chain->GetEntry(eventNr++))
    {
      if (_debug)
      {
	std::cout << "Analyzing event" << std::endl;
      }

      if (eventNr % 10000 == 0)
      {
	std::cout << "estEffsWZ has analyzed " << eventNr << " events." << std::endl;
      }
      if (isMC)
      {
	weight = sampleXSec/sampleWeight*(_eventInfo->EventWeight());
      }

      //Good Run List and Loading of Object Quality Maps go here.
      if (_debug)
      {
	std::cout << "Checking good run list" << std::endl;
      }
      bool passed = true;
      if (isMC)
      {
	egammaOQ::LoadOQMaps(159040);
      }
      else
      {
	int runNr = _eventInfo->RunNumber();
	int lumiBlock = _eventInfo->LumiBlock();
	passed = grl1.HasRunLumiBlock(runNr,lumiBlock);
	egammaOQ::LoadOQMaps(runNr);
      }

      if (!passed) continue;

      //Impose trigger requirements
      if (_debug)
      {
	std::cout << "imposing trigger requirements" << std::endl;
      }
      bool passTrigger = false;
      bool passE;
      bool passM;

      int RUNNUMBER;
      if (isMC)
      {
	RUNNUMBER = 100000;
      }
      else
      {
	RUNNUMBER = _eventInfo->RunNumber();
      }

      passTriggers(RUNNUMBER,passE,passM);

      if (passE || passM)
      {
	passTrigger = true;
      }

      if (!passTrigger) continue;

      //Remove events with bad jets/calorimeter problems
      if (_debug)
      {
	std::cout << "Checking for bad jets" << std::endl;
      }
      bool failCleaning = false;
      int nJets = _jets->GetEntries();
      for (int i = 0; i != nJets; ++i)
      {
	MTree::Jet* jet = (MTree::Jet*)_jets->At(i);

	if (jet->GetRawPt() < 10) continue;

	if ((jet->N90() <= 5) && (jet->HECFrac() > 0.8))
	{
	  failCleaning = true;
	}
	if ((TMath::Abs(jet->JetQuality()) > 0.8) && (jet->JetEMFraction() > 0.95))
	{
	  failCleaning = true;
	}
	if (TMath::Abs(jet->JetTime()) >= 50)
	{
	  failCleaning = true;
	}
	if (jet->HECFrac() >= 1 - TMath::Abs(jet->JetQuality()))
	{
	  failCleaning = true;
	}
      }

      if (failCleaning) continue;

      //Apply collision selection/primary vertex requirements
      if (_debug)
      {
	std::cout << "applying PV requirements" << std::endl;
      }
      bool passVertex = false;
      int nPVs = _primaryVertices->GetEntries();
      for (int i = 0; i != nPVs; ++i)
      {
	MTree::PrimaryVertex* vertex = (MTree::PrimaryVertex*)_primaryVertices->At(i);
	if (vertex->NTracks() < 5) continue;
	passVertex = true;
      }

      if (!passVertex) continue;

      //HERE'S WHERE I REMOVE THE TRUTH STUFF TO MAKE SURE I LOOK AT ONLY FAKES
      if (_debug)
      {
	std::cout << "Removing W and Z decay products" << std::endl;
      }
      std::vector<MTree::Electron*> elecObjects;
      int nElectrons = _electrons->GetEntries();
      for (int i = 0; i != nElectrons; ++i)
      {
	elecObjects.push_back((MTree::Electron*)_electrons->At(i));
      }
      std::vector<MTree::Muon*> muonObjects;
      int nMuons = _muons->GetEntries();
      for (int i = 0; i != nMuons; ++i)
      {
	muonObjects.push_back((MTree::Muon*)_muons->At(i));
      }
      std::vector<MTree::TruthParticle*> truthies;
      if (!(removeDecayProducts(sample, elecObjects, muonObjects, truthies))) continue;


      //Apply object selections
      if (_debug)
      {
	std::cout << "object selections" << std::endl;
      }
      //Electrons first.
      std::vector<MTree::Electron*> electrons;
      std::vector<MTree::Electron*> crackElectrons;
      for (int i = 0; i != (int)elecObjects.size(); ++i)
      {
	MTree::Electron* elec = elecObjects[i];

	if (elec->Pt() < 10) continue;
	if (TMath::Abs(elec->ClusterEta()) > 2.47) continue;
	if (elec->Author() != 1 && elec->Author() != 3) continue;

	double et = 1000*elec->ClusterE()/TMath::CosH(TMath::Abs(elec->ClusterEta()));

	if (!(isRobustMedium(elec->IsEM(),elec->ClusterEta(),et,elec->E237()/elec->E277(),elec->WEta2()))) continue;

	int trackBLayer = 16;
	const unsigned BLAYER = 0x1 << trackBLayer;
	bool hasBLayerHit = (elec->IsEM() & BLAYER) == 0;
	if (elec->ExpectHitInBLayer() && !hasBLayerHit) continue;

	if ((TMath::Abs(elec->ClusterEta()) > 1.37) && (TMath::Abs(elec->ClusterEta()) < 1.52))
	{
	  crackElectrons.push_back(elec);
	  continue;
	}
	if (elec->ETCone20() > 10) continue;
	electrons.push_back(elec);
      }

      std::vector<MTree::Muon*> muons;
      for (int i = 0; i != (int)muonObjects.size(); ++i)
      {
	MTree::Muon* muon = muonObjects[i];

	if (muon->Pt() < 10) continue;
	if (TMath::Abs(muon->Eta()) > 2.4) continue;
	if (muon->Author() != 6) continue;
	if (!(muon->IsCombinedMuon())) continue;
	if (muon->MatchChi2() < 0 || muon->MatchChi2() > 100) continue;

	MTree::Track* track = muon->GetTrack();
	if (track == 0) continue;
	if (muon->ETCone20() > 10) continue;

	muons.push_back(muon);
      }

      std::vector<MTree::Jet*> jets;
      for (int i = 0; i != nJets; ++i)
      {
	MTree::Jet* jet = (MTree::Jet*)_jets->At(i);

	double jetpT = (jet->GetRawPt())*(jet->GetEmJESFactor());
	if (jetpT < 30) continue;
	if (TMath::Abs(jet->Eta()) > 2.5) continue;

	jets.push_back(jet);
      }

      std::vector<MTree::Photon*> photons;
      int nPhotons = _photons->GetEntries();
      for (int i = 0; i != nPhotons; ++i)
      {
	MTree::Photon* phot = (MTree::Photon*)_photons->At(i);

	if (phot->Pt() < 20) continue;
	if (TMath::Abs(phot->ClusterEta()) > 2.47) continue;
	if (!(phot->IsTight())) continue;
	double photET =  phot->ClusterE()/TMath::CosH(TMath::Abs(phot->ClusterEta()));
	if (phot->ETCone20()/photET > 0.1) continue;

	photons.push_back(phot);
      }

      //Now removing jets that are really photons, and veto events with trackless, 
      //high pT jets.
      if (_debug)
      {
	std::cout << "removing events with trackless jets" << std::endl;
      }
      removeOverlap(0.2, photons, jets);

      bool failJetPhot = false;
      int nTracks = _tracks->GetEntriesFast();
      for (int i = 0; i != (int)jets.size(); i++)
      {
        int nTracksForJet = 0;
        for (int j = 0; j != nTracks; j++)
        {
          MTree::Track* tr = (MTree::Track*)_tracks->At(j);
          if (tr->Pt() < 0.5) continue;
          int nrHitsPix = tr->NrOfPixelHits();
          int nrHitsSCT = tr->NrOfSCTHits();
          double d0vtx = tr->Vtx_D0();
          double z0vtx = tr->Vtx_Z0();
          if (nrHitsPix < 1) continue;
          if (nrHitsSCT < 1) continue;
          if (TMath::Abs(d0vtx) > 1.5) continue;
          if (TMath::Abs(z0vtx) > 1.5) continue;
          double dR = tr->DeltaR(*jets[i]);
          if (dR < 0.4) 
	  {
            nTracksForJet++;
	  }
        }
        double pt = jets[i]->GetRawPt();
        pt *= jets[i]->GetEmJESFactor();
        if (pt > 50 && nTracksForJet == 0) 
	{
	  failJetPhot = true;
	}
      }

      if (failJetPhot) continue;

      if (crackElectrons.size() != 0) continue;

      //More removing of overlaps
      removeOverlap(0.2, electrons, jets);

      if (_debug)
      {
	std::cout << "Checking OTX veto" << std::endl;
      }
      bool OTXVeto = false;
      for (int i = 0; i != (int)electrons.size(); ++i)
      {
	int run = 0;
	if (isMC)
	{
	  run = 159040;
	}
	else
	{
	  run = _eventInfo->RunNumber();
	}
	if (egammaOQ::checkOQClusterElectron(run,electrons[i]->ClusterEta(),electrons[i]->ClusterPhi()) == 3)
	{
	  OTXVeto = true;
	}
      }

      if (OTXVeto) continue;

      if (_debug)
      {
	std::cout << "Checking for cosmic events" << std::endl;
      }
      bool CosmicVeto = false;
      for (int i = 0; i != (int)muons.size(); ++i)
      {
	MTree::Track* track = muons[i]->GetTrack();
	if (TMath::Abs(track->Vtx_Z0()) >= 10)
	{
	  CosmicVeto = true;
	}
      }

      if (CosmicVeto) continue;

      //And yet more overlap (see why I templated that function?)
      removeOverlap(0.4, jets, electrons);
      removeOverlap(0.4, jets, muons);

      std::vector<MTree::Lepton*> leptons;
      for (int i = 0; i != (int)electrons.size(); ++i)
      {
	leptons.push_back(electrons[i]);
      }
      for (int i = 0; i != (int)muons.size(); ++i)
      {
	leptons.push_back(muons[i]);
      }
      if (leptons.size() < 1) continue;

      //Fill isolation plots, to see what would be a good variable to use, and 'loose' maps.
      //Oh, and why not make the cut and fill the tight ones at the same time.
      //Just for kicks.
      if (_debug)
      {
	std::cout << "filling necessary plots" << std::endl;
      }
      for (int i = 0; i != (int)electrons.size(); ++i)
      {
	MTree::Electron* elec = electrons[i];
	histos[5]->Fill(elec->ETCone20(),weight);
	histos[6]->Fill(elec->ETCone40(),weight);
	histos[7]->Fill((elec->ETCone20()/elec->Pt()),weight);
	histos[8]->Fill((elec->ETCone40()/elec->Pt()),weight);

	histos2[0]->Fill(elec->Pt(),elec->Eta(),weight);

	if ((elec->ETCone40()/elec->Pt()) > 0.1) continue;
// 	double et = 1000*elec->ClusterE()/TMath::CosH(TMath::Abs(elec->ClusterEta()));
// 	if (!(isRobusterTight(elec->IsEM(),elec->ExpectHitInBLayer(),elec->ClusterEta(),et,elec->E237()/elec->E277(),elec->WEta2()))) continue;

	histos2[1]->Fill(elec->Pt(),elec->Eta(),weight);
      }
      for (int i = 0; i != (int)muons.size(); ++i)
      {
	MTree::Muon* muon = muons[i];
	histos[0]->Fill(muon->ETCone20(),weight);
	histos[1]->Fill(muon->ETCone40(),weight);
	histos[2]->Fill((muon->ETCone20()/muon->Pt()),weight);
	histos[3]->Fill((muon->ETCone40()/muon->Pt()),weight);

        histos2[2]->Fill(muon->Pt(),muon->Eta(),weight);

	if ((muon->ETCone40()/muon->Pt()) > 0.1) continue;
	histos2[3]->Fill(muon->Pt(),muon->Eta(),weight);
      }


//       if (eventNr > 100)
//       {
// 	eventNr = 1000000000;
//       }

      if (_debug)
      {
	std::cout << "reached end of event loop" << std::endl;
      }

    }//Closes event loop
  }//Closes loop over samples

}

//##################################################################################

void SusyAnalysis::wjetsFactor(std::string sample, TH1D** histos, TH2D** histos2)
{
  if (_debug)
  {
    std::cout << "Calling wjetsFactor()" << std::endl;
  }

  RecordClass* record = new RecordClass("events");
  std::vector<std::string> channels;
  channels.push_back("e");
  channels.push_back("m");
  record->setChannels(channels);

  bool isMC = false;
  if ((sample.find("EGamma") == string::npos) && (sample.find("Muon") == string::npos) && (sample.find("data") == string::npos) && (sample.find("Elec") == string::npos) && (sample.find("JetTauEtmiss") == string::npos))
  {
    isMC = true;
  }

  if (_debug)
  {
    std::cout << "Setting up Good Run List" << std::endl;
  }
  TString list = "/home/swalch/macro-cosmos/HiggsAnalysis/GoodRunLists/susy_grl_init.xml";
  Root::TGoodRunsListReader foo(list);
  foo.Interpret();
  Root::TGoodRunsList grl1 =  foo.GetMergedGoodRunsList();

  if (_debug)
  {
    std::cout << "Getting all component samples to run over" << std::endl;
  }
  std::vector<std::string> samples = GetAllSamples(sample);
  std::vector<std::string>::iterator iter;
  std::vector<std::string>::iterator end = samples.end();

  for (iter = samples.begin(); iter != end; ++iter)
  {
    std::string sampleName = *iter;
    TChain* chain = GetChain(sampleName);
    setBranchAddresses(chain);
    std::cout << "Analyzing sample " << sampleName << std::endl;

    if (_debug)
    {
      std::cout << "Checking sample xsection/weight information" << std::endl;
    }

    double sampleWeight = 0.0;
    double sampleXSec = 1.0;
    if (isMC)
    {
      sampleWeight = GetSampleWeight(sampleName);
      sampleXSec = GetCrossSection(sampleName);
    }

    if (_debug)
    {
      std::cout << "Entering event loop" << std::endl;
    }
    int eventNr = 0;
    double weight = 1.0;
    while(chain->GetEntry(eventNr++))
    {
      if (_debug)
      {
	std::cout << "Analyzing event" << std::endl;
      }

      if (eventNr % 10000 == 0)
      {
	std::cout << "SusyCutflow has analyzed " << eventNr << " events." << std::endl;
      }
      int cutNr = 0;
      cutNr = record->addEvent(cutNr, weight, "Initial number of events");

      int RUNNUMBER;
      if (isMC)
      {
	weight = sampleXSec/sampleWeight*(_eventInfo->EventWeight());
	if ((sample.find("Z") != string::npos) || (sample.find("W") != string::npos))
	{
	  std::vector<MTree::PrimaryVertex*> pvs = getPVs();
          double temp = getPileUpWeight((int)pvs.size());
	  weight = weight * temp;
	}
	RUNNUMBER = 100000;
      }
      else
      {
	RUNNUMBER = _eventInfo->RunNumber();
      }

      //Good Run List and Loading of Object Quality Maps go here.
      if (_debug)
      {
	std::cout << "Checking good run list" << std::endl;
      }
      bool passed = true;
      if (isMC)
      {
	egammaOQ::LoadOQMaps(166497);
      }
      else
      {
	int runNr = _eventInfo->RunNumber();
	int lumiBlock = _eventInfo->LumiBlock();
	passed = grl1.HasRunLumiBlock(runNr,lumiBlock);
	egammaOQ::LoadOQMaps(runNr);
      }

      if (!passed) continue;
      cutNr = record->addEvent(cutNr,weight,"GRL");

      if (_debug)
      {
	std::cout << "imposing trigger requirements" << std::endl;
      }
      if (!firstTriggerCheck(sample, RUNNUMBER)) continue;
      cutNr = record->addEvent(cutNr,weight,"Passed appropriate triggers");

      //Remove events with bad jets/calorimeter problems
      if (_debug)
      {
	std::cout << "Checking for bad jets" << std::endl;
      }
      if (badJetsPresent()) continue;
      cutNr = record->addEvent(cutNr,weight,"No bad jets");

      //Apply collision selection/primary vertex requirements
      if (_debug)
      {
	std::cout << "applying PV requirements" << std::endl;
      }
      std::vector<MTree::PrimaryVertex*> vertices = getPVs();
      if ((int)vertices.size() == 0) continue;
      cutNr = record->addEvent(cutNr,weight,"At least one PV");

      //Apply object selections
      if (_debug)
      {
	std::cout << "object selections" << std::endl;
      }
      //Electrons first.
      std::vector<MTree::Electron*> electrons = getMediumElectrons(RUNNUMBER);
      std::vector<MTree::Electron*> crackElectrons = getCrackElectrons(RUNNUMBER,false);

      std::vector<MTree::Muon*> muons = getGoodMuons();

      std::vector<MTree::Jet*> jets = getJets();

      std::vector<MTree::Photon*> photons = getPhotons();

      channels.clear();
      if (electrons.size() > 0)
      {
	channels.push_back("e");
      }
      if (muons.size() > 0)
      {
	channels.push_back("m");
      }
      if (channels.size() == 0) continue;
      cutNr = record->addEvent(cutNr,weight,"Events with leptons");
      cutNr = record->addEvent(cutNr,weight,"Events with leptons",channels);

      //Now removing jets that are really photons, and veto events with trackless, 
      //high pT jets.
      if (_debug)
      {
	std::cout << "removing events with trackless jets" << std::endl;
      }
//       removeOverlap(0.2, photons, jets);

//       if (cosmicJetPresent(jets)) continue;

      if (crackElectrons.size() != 0) continue;
      cutNr = record->addEvent(cutNr,weight,"No crack elecs",channels);

      //More removing of overlaps
      removeOverlap(0.2, electrons, jets);

      if (_debug)
      {
	std::cout << "Checking OTX veto" << std::endl;
      }
      if (OTXProblem(RUNNUMBER, electrons)) continue;
      cutNr = record->addEvent(cutNr,weight,"No OTX problems",channels);

      if (_debug)
      {
	std::cout << "Checking for cosmic events" << std::endl;
      }
      if (hasCosmicMuon(muons)) continue;
      cutNr = record->addEvent(cutNr,weight,"No cosmic muons",channels);

      //And yet more overlap (see why I templated that function?)
      removeOverlap(0.4, jets, electrons);
      removeOverlap(0.4, jets, muons);

      channels.clear();
      if (electrons.size() > 0)
      {
	channels.push_back("e");
      }
      if (muons.size() > 0)
      {
	channels.push_back("m");
      }
      if (channels.size() == 0) continue; 
      cutNr = record->addEvent(cutNr,weight,"Overlap removal",channels);     


//Okay, so now I need to select some good W events.  What defines a good W event?  I can't use MET, since I need to count that.  I'll probably just use a histogram for the missingET distribution; then I can easily add the histos between different samples, and it should be easy to write a little macro that can integrate the whole thing and compare it to the top end of the tail. 

//So an MET > 30 cut, and I can probably put a cut on transverse mass, say greater than 40 GeV.  I can also tighten the selections on the electrons, going to robusterTight.  Are there some comparable cuts I can use for muons?


//So, first, tighten cuts on leptons, and select single lepton events.
      if (_debug)
      {
	std::cout << "imposing extra electron requirements" << std::endl;
      }
      for (int i = 0; i != (int)electrons.size(); ++i)
      {
	//Select some really nice electrons here
	if (electrons[i]->Pt() < 20)
	{
	  electrons.erase(electrons.begin() + i);
	  --i;
	  continue;
	}
        double et = 1000*electrons[i]->ClusterE()/TMath::CosH(TMath::Abs(electrons[i]->ClusterEta()));
        if (!(isRobusterTight(electrons[i]->IsEM(),electrons[i]->ExpectHitInBLayer(),electrons[i]->ClusterEta(),et,electrons[i]->E237()/electrons[i]->E277(),electrons[i]->WEta2())))
	{
	  electrons.erase(electrons.begin() + i);
	  --i;
	  continue;
	}
      }
      channels.clear();
      if (electrons.size() > 0)
      {
	channels.push_back("e");
      }
      if (muons.size() > 0)
      {
	channels.push_back("m");
      }
      if (channels.size() == 0) continue;
      cutNr = record->addEvent(cutNr,weight,"Extra e reqs",channels);

      if (_debug)
      {
	std::cout << "imposing extra muon requirements" << std::endl;
      }
      for (int i = 0; i != (int)muons.size(); ++i)
      {
	//Select some equally pretty muons here
	MTree::Muon* muon = muons[i];
	MTree::Track* track = muon->GetTrack();
	MTree::Track* trackMS = muon->GetStandAloneTrack();
	if ((track == 0) || (trackMS == 0))
	{
	  muons.erase(muons.begin() + i);
	  --i;
	  continue;
	}
	if (muon->Pt() < 20)
	{
	  muons.erase(muons.begin() + i);
	  --i;
	  continue;
	}
	if ((track->NrOfPixelHits() < 2) || (track->NrOfSCTHits() < 6))
	{
	  muons.erase(muons.begin() + i);
	  --i;
	  continue;
	}
// 	if ((TMath::Abs(track->Eta()) < 2) && (track->NrOfTRTHits() == 0))
// 	{
// 	  muons.erase(muons.begin() + i);
// 	  --i;
// 	  continue;
// 	}
// 	if ((trackMS->Pt() < 10) || (TMath::Abs(track->Pt() - trackMS->Pt())/(track->Pt()) > 0.5))
// 	{
// 	  muons.erase(muons.begin() + i);
// 	  --i;
// 	  continue;
// 	}
// 	if (TMath::Abs(track->Vtx_D0()) > 0.1)
// 	{
// 	  muons.erase(muons.begin() + i);
// 	  --i;
// 	  continue;
// 	}
	if (muon->PTCone20() > 1.8)
	{
	  muons.erase(muons.begin() + i);
	  --i;
	  continue;
	}
      }
      channels.clear();
      if (electrons.size() > 0)
      {
	channels.push_back("e");
      }
      if (muons.size() > 0)
      {
	channels.push_back("m");
      }
      if (channels.size() == 0) continue;
      cutNr = record->addEvent(cutNr,weight,"extra mu reqs",channels);

      //Veto potential Z, WW, ttbar, other dilepton sources here.
      if (_debug)
      {
	std::cout << "vetoing dilepton events" << std::endl;
      }
      if ((int)muons.size() > 1) continue;
      if ((int)electrons.size() > 1) continue;
      if (((int)muons.size() + (int)electrons.size()) > 1) continue;
      if (((int)electrons.size() == 0) && ((int)muons.size() == 0)) continue;
      channels.clear();
      if (electrons.size() > 0)
      {
	channels.push_back("e");
      }
      if (muons.size() > 0)
      {
	channels.push_back("m");
      }
      cutNr = record->addEvent(cutNr,weight,"single leptons only",channels);

      //Pick out the pretty W lepton here, and store it all special-like.
      if (_debug)
      {
	std::cout << "picking out W lepton" << std::endl;
      }
      MTree::Lepton* lepton;
      if ((int)electrons.size() > 0)
      {
	if (_debug)
	{
	  std::cout << "getting electron candidate" << std::endl;
	}
	lepton = electrons[0];
      }
      else
      {
	if (_debug)
	{
	  std::cout << "getting muon candidate" << std::endl;
	}
	lepton = muons[0];
      }

      //Aaaaaaaaaaand make sure it's coming from the right stream, to remove double-counting
      bool eStream = false;
      bool mStream = false;
      bool mcStream = false;
      if (sample.find("EGamma") != string::npos)
      {
	eStream = true;
      }
      else if (sample.find("Muons") != string::npos)
      {
	mStream = true;
      }
      else
      {
	mcStream = true;
      }

      if ((eStream && (TMath::Abs(lepton->LeptonPdgID() == 13))) || (mStream && (TMath::Abs(lepton->LeptonPdgID() == 11))))
      {
	continue;
// 	if (eventNr < 50)
// 	{
// 	  std::cout << "Event from wrong stream" << std::endl;
// 	  if (eStream)
// 	  {
// 	    std::cout << "From EGamma stream" << std::endl;
// 	  }
// 	  if (mStream)
// 	  {
// 	    std::cout << "From Muons stream" << std::endl;
// 	  }
// 	  std::cout << "Lepton ID = " << lepton->LeptonPdgID() << std::endl;
// 	}
      }
      cutNr = record->addEvent(cutNr,weight,"Stream double-check",channels);
      //DO I NEED TO REQUIRE JETS FOR THIS W+JETS SCALE FACTOR?
      //Well, I'm going to, because it makes sense to me to do so.
      if (_debug)
      {
	std::cout << "vetoing on number of jets" << std::endl;
      }
      if ((int)jets.size() == 0) continue;
      cutNr = record->addEvent(cutNr,weight,"Njets > 0",channels);

      //Get MET object.
      if (_debug)
      {
	std::cout << "calculating MET" << std::endl;
      }
      int nMETs = _missingETTerms->GetEntries();
      double xcom = 0;
      double ycom = 0;
      TLorentzVector* vector;
      for (int i = 0; i != nMETs; ++i)
      {
        MTree::MissingET* term = (MTree::MissingET*)_missingETTerms->At(i);
        std::string name = term->GetTerm();

        if (name.compare("MET_LocHadTopo") == 0)
        {
	  vector = term;
	  xcom = term->Ex();
	  ycom = term->Ey();
        }      
      }
      if (xcom == 0)
      {
	std::cout << "Didn't find MET_LocHadTopo!" << std::endl;
	std::cout << "The following terms are available: " << std::endl;
	for (int i = 0; i != nMETs; ++i)
	{
	  std::cout << ((MTree::MissingET*)_missingETTerms->At(i))->GetTerm() << std::endl;
	}
      }
      double metPx = vector->Px();
      double metPy = vector->Py();
      if (lepton->LeptonPdgID() == 13)
      {
	xcom = xcom - lepton->Px();
	ycom = ycom - lepton->Py();
	metPx = metPx - lepton->Px();
	metPy = metPy - lepton->Py();
      }
      double MET = TMath::Sqrt(xcom*xcom+ycom*ycom);
      histos[16]->Fill(MET,weight);
      if (lepton->LeptonPdgID() == 11)
      {
	histos[17]->Fill(MET,weight);
      }
      else
      {
	histos[18]->Fill(MET,weight);
      }
      if (MET < 30) continue;
      cutNr = record->addEvent(cutNr,weight,"MET > 30",channels);

      if (_debug)
      {
	std::cout << "calculating mT " << std::endl;
      }
      double lpt = lepton->Pt();
      double mpt = TMath::Sqrt(metPx*metPx+metPy*metPy);
      double deltaPhi = lepton->DeltaPhi(*vector);

      double mT = sqrt(2*lpt*mpt*(1-TMath::Cos(deltaPhi)));

      if (mT < 40) continue;
      cutNr = record->addEvent(cutNr,weight,"mT > 40",channels);

      //Fill Histograms here.
      histos[0]->Fill(MET,weight);
      histos[3]->Fill(mT,weight);
      histos[6]->Fill(lepton->Pt(),weight);
      histos[9]->Fill(deltaPhi,weight);
      histos[12]->Fill((int)jets.size(),weight);
      if (TMath::Abs(lepton->LeptonPdgID()) == 11)
      {
        histos[1]->Fill(MET,weight);
        histos[4]->Fill(mT,weight);
        histos[7]->Fill(lepton->Pt(),weight);
        histos[10]->Fill(deltaPhi,weight);
	histos[13]->Fill((int)jets.size(),weight);
      }
      else
      {
        histos[2]->Fill(MET,weight);
        histos[5]->Fill(mT,weight);
        histos[8]->Fill(lepton->Pt(),weight);
        histos[11]->Fill(deltaPhi,weight);
	histos[14]->Fill((int)jets.size(),weight);
      }

      if (_debug)
      {
	std::cout << "reached end of event loop" << std::endl;
      }

    }//Closes event loop
  }//Closes loop over samples
  record->print((sample + "_wjets").c_str());
}//Closes wjetsFactor function
