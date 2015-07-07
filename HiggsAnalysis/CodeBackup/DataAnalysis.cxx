 #include "DataAnalysis.h"


DataAnalysis::DataAnalysis()
{
  //Grab reference histograms

  TFile *input = new TFile("MMInput.root");

  true_elec_eff = (TH2D*)input->Get("true_elec_eff");
  fake_elec_eff = (TH2D*)input->Get("fake_elec_eff");
  true_muon_eff = (TH2D*)input->Get("true_muon_eff");
  fake_muon_eff = (TH2D*)input->Get("fake_muon_eff");
}

DataAnalysis::~DataAnalysis() {
}

//#############################################################################

void DataAnalysis::TriggerSelection(std::string sample, TH1D** histos, TH2D** histos2)
{
  if (_debug)
  {
    std::cout << "Calling TriggerSelection()" << std::endl;
  }

  TChain* chain = GetChain(sample);
  setBranchAddresses(chain);

  if (_debug)
  {
    std::cout << "Setting up good run list" << std::endl;
  }

  TString list = "/home/swalch/laboratory/macro-cosmos/HiggsAnalysis/GoodRunLists/GRL_7TeV_init.xml";
  Root::TGoodRunsListReader foo(list);
  foo.Interpret();
  Root::TGoodRunsList grl1 =  foo.GetMergedGoodRunsList();

  int eventsT = 0;
  int eventsGRL = 0;
  int eventsMBTS = 0;

  std::map<std::string, int> triglist;
  triglist["L1_EM3"] = 0;
  triglist["L1_EM7"] = 0;
  triglist["L1_EM13"] = 0;
  triglist["L1_EM13i"] = 0;
  triglist["L1_2EM3"] = 0;
  triglist["L1_MU4"] = 0;
  triglist["L1_MU10"] = 0;
  triglist["L1_2MU4"] = 0;
  triglist["L1_MU4_EM3"] = 0;
  triglist["L1_EM7_MU6"] = 0;

  std::map<std::string, int>::iterator iter;
  std::map<std::string, int>::iterator end = triglist.end();

  if (_debug)
  {
    std::cout << "Calculating event weight" << std::endl;
  }

  bool isMC = false;
  double sampleWeight = 1.0;
  double sampleXSec = 0.0;

  if (sample.find("_7TeV") != string::npos)
  {
    sampleWeight = GetSampleWeight(sample);
    sampleXSec = GetCrossSection(sample);
    isMC = true;
  }

  if (_debug)
  {
    std::cout << "Entering event loop . . ." << std::endl;
  }
  int eventNr = 0;
  double weight = 1.0;
  while (chain->GetEntry(++eventNr))
  {
    if (eventNr % 10000 == 0)
    {
      std::cout<<"Analyzed "<<eventNr<<" events."<<std::endl;
    }
    if (isMC)
    {
      weight = sampleXSec/sampleWeight*(_eventInfo->EventWeight());
    }

    eventsT+=weight;

    bool passed = true;
    if (sample.find("data") != string::npos)
    {
      int runNr = _eventInfo->RunNumber();
      int lumiBlock = _eventInfo->LumiBlock();
      passed = grl1.HasRunLumiBlock(runNr,lumiBlock);
    }
    if (!passed) continue;
    eventsGRL+=weight;

    if (!(_trigger->PassedL1Trigger("L1_MBTS_1_1"))) continue;
    eventsMBTS+=weight;

    for (iter = triglist.begin(); iter != end; ++iter)
    {
      if (_trigger->PassedL1Trigger(iter->first))
      {
	(iter->second)+=weight;
      }
    }

  }//Closes Event loop

  std::cout << "Trigger Statistics" << std::endl;
  std::cout << "Total Number of events: " << eventsT << std::endl;
  std::cout << "Events on GRL" << eventsGRL << std::endl;
  std::cout << "Events passing L1_MBT: " << eventsMBTS << std::endl;
  for (iter = triglist.begin(); iter != end; ++iter)
  {
    std::cout << "Events passing " << iter->first << ": " << iter->second << std::endl;
  }

}//Closes TriggerSelection function

//#############################################################################

void DataAnalysis::ElectronSelection(std::string sample, TH1D** histos, TH2D** histos2)
{
  if (_debug)
  {
    std::cout<<"Calling ElectronSelection()"<<std::endl;
  }

  TChain *chain = GetChain(sample);
  setBranchAddresses(chain);

  if (_debug)
  {
    std::cout<<"Setting up RecordClass"<<std::endl;
  }
  RecordClass* record = new RecordClass("Electrons");
  record->setChannels("electrons");
  record->unlinkChannels();

  if (_debug)
  {
    std::cout<<"Setting up good run list"<<std::endl;
  }
  TString list;
  if (sample == "data09")
  {
    list = "/home/swalch/laboratory/macro-cosmos/HiggsAnalysis/GoodRunLists/ListSolenoidOnToroidOn.xml";
  }
  else
  {
    list = "/home/swalch/laboratory/macro-cosmos/HiggsAnalysis/GoodRunLists/GRL_7TeV_init.xml";
  }

  Root::TGoodRunsListReader foo(list);
  foo.Interpret();
  Root::TGoodRunsList grl1 =  foo.GetMergedGoodRunsList();

  if (_debug)
  {
    std::cout << "Looking of sample weight/cross-section" << std::endl;
  }

  bool isMC = false;
  double sampleWeight = 0.0;
  double sampleXSec = 1.0;

  if (sample.find("_7TeV") != string::npos)
  {
    sampleWeight = GetSampleWeight(sample);
    sampleXSec = GetCrossSection(sample);
    isMC = true;
  }

  if (_debug)
  {
    std::cout<<"Entering event loop . . ." <<std::endl;
  }

  int eventNr = 0;
  double weight = 1.0;
  while(chain->GetEntry(eventNr++))
  {
    if (eventNr % 10000 == 0)
    {
      std::cout<<"Analyzed "<<eventNr<<" events."<<std::endl;
    }
    int cutNr = 0;
    if (isMC)
    {
      weight = sampleXSec/sampleWeight*(_eventInfo->EventWeight());
    }
    cutNr = record->addEvent(cutNr,weight,"Before event selection");

    //Apply Good Run List
    bool passed = true;
    if (sample.find("data") != string::npos)
    {
      int runNr = _eventInfo->RunNumber();
      int lumiBlock = _eventInfo->LumiBlock();
      passed = grl1.HasRunLumiBlock(runNr,lumiBlock);
    }
    if (!passed) continue;
    cutNr = record->addEvent(cutNr, weight, "Good Run List");

    //Trigger Selection
    std::string trigger;
    if (sample == "data09" || sample == "minBiasMC")
    {
      trigger = "L1_MBTS_1_1_Col";
    }
    else
    {
      trigger = "L1_MBTS_1_1";
    }

    if (!(_trigger->PassedL1Trigger(trigger))) continue;
    cutNr = record->addEvent(cutNr, weight, "Trigger Selection");

    //Primary Vertex Selection
    int maxNTracks = maxNofPVTracks();
    histos[6]->Fill(maxNTracks);
    if (maxNTracks < 3) continue;
    cutNr = record->addEvent(cutNr, weight,"Primary Vertex Selection");

    //Electron selections
    int nEl = _electrons->GetEntries();
    int goodEl = 0;
    std::vector<std::string> channels;
    channels.push_back("electrons");
    const int channelStart = cutNr;

    for (int i = 0; i != nEl; ++i)
    {
      cutNr = channelStart;
      cutNr = record->addEvent(cutNr,weight,"Initial number of electrons",channels);

      MTree::Electron* electron = (MTree::Electron*)_electrons->At(i);

      histos[1]->Fill(electron->Pt(),weight);
      histos[2]->Fill(electron->Eta(),weight);
      histos[3]->Fill(electron->Phi(),weight);
      histos[8]->Fill(electron->ETCone40(),weight);
      histos[10]->Fill(electron->ETCone40()/electron->Pt(),weight);
      histos[12]->Fill(electron->TrackIso40(),weight);
      histos[14]->Fill(electron->TrackIso40()/electron->Pt(),weight);

      if (electron->Pt() < 2.5) continue;
      cutNr = record->addEvent(cutNr, weight,"Pt > 2.5 GeV",channels);

      if (fabs(electron->Eta()) > 2.5) continue;
      if (fabs(electron->Eta()) > 1.37 && fabs(electron->Eta()) < 1.52) continue;
      cutNr = record->addEvent(cutNr, weight,"|Eta| < 2.5 and not in crack region",channels);

      if (electron->Author() != 1 && electron->Author() != 3) continue;
      cutNr = record->addEvent(cutNr, weight,"Author = 1 or 3",channels);

      if (electron->GetTrack() == 0) continue;
      cutNr = record->addEvent(cutNr, weight,"Electron has a track",channels);

      if (!(electron->IsLoose())) continue;
      cutNr = record->addEvent(cutNr, weight,"Loose Electrons",channels);

      if (!(electron->IsMedium())) continue;
      cutNr = record->addEvent(cutNr, weight,"Medium Electrons",channels);

      ++goodEl;

      histos[4]->Fill(electron->Pt(),weight);
      histos[5]->Fill(electron->Eta(),weight);
      histos[7]->Fill(electron->Phi(),weight);
      histos[9]->Fill(electron->ETCone40(),weight);
      histos[11]->Fill(electron->ETCone40()/electron->Pt(),weight);
      histos[13]->Fill(electron->TrackIso40(),weight);
      histos[15]->Fill(electron->TrackIso40()/electron->Pt(),weight);
    }
    histos[0]->Fill(goodEl);

  }//Close event loop

  record->print(sample + "_electron");

}//Close function brace

//#############################################################################

void DataAnalysis::MuonSelection(std::string sample, TH1D** histos, TH2D** histos2)
{
  if (_debug)
  {
    std::cout<<"Calling MuonSelection()"<<std::endl;
  }

  TChain *chain = GetChain(sample);
  setBranchAddresses(chain);

  if (_debug)
  {
    std::cout<<"Setting up RecordClass"<<std::endl;
  }
  RecordClass* record =  new RecordClass("Muons");
  record->setChannels("muons");
  record->unlinkChannels();

  if (_debug)
  {
    std::cout<<"Setting up good run list"<<std::endl;
  }
  TString list;
  if (sample == "data09")
  {
    list = "/home/swalch/laboratory/macro-cosmos/HiggsAnalysis/GoodRunLists/ListSolenoidOnToroidOn.xml";
  }
  else
  {
    list = "/home/swalch/laboratory/macro-cosmos/HiggsAnalysis/GoodRunLists/GRL_7TeV_init.xml";
  }

  Root::TGoodRunsListReader foo(list);
  foo.Interpret();
  Root::TGoodRunsList grl1 =  foo.GetMergedGoodRunsList();
  //  grl1.Summary(kTRUE);

  if (_debug)
  {
    std::cout << "Looking of sample weight/cross-section" << std::endl;
  }

  bool isMC = false;
  double sampleWeight = 1.0;
  double sampleXSec = 0.0;

  if (sample.find("_7TeV") != string::npos)
  {
    sampleWeight = GetSampleWeight(sample);
    sampleXSec = GetCrossSection(sample);
    isMC = true;
  }


  if (_debug)
  {
    std::cout<<"Entering event loop"<<std::endl;
  }

  std::vector<MTree::Jet*> looseJets;
  std::vector<MTree::Jet*> cleanJets;

  int eventNr = 0;
  double weight = 1.0;
  while(chain->GetEntry(eventNr++))
  {
    int cutNr = 0;
    if (isMC)
    {
      weight = sampleXSec/sampleWeight*(_eventInfo->EventWeight());
    }
    cutNr = record->addEvent(cutNr, weight, "Before Event Selection");
    if (eventNr % 10000 == 0)
    {
      std::cout<<"Analyzed "<<eventNr<<" events."<<std::endl;
    }

    bool passed = true;
    if (sample.find("data") != string::npos)
    {
      int runNr = _eventInfo->RunNumber();
      int lumiBlock = _eventInfo->LumiBlock();
      passed = grl1.HasRunLumiBlock(runNr,lumiBlock);
    }
    if (!passed) continue;
    cutNr = record->addEvent(cutNr, weight, "Good Run List");

    //Trigger Selection
    std::string trigger;
    if (sample == "data09" || sample == "minBiasMC")
    {
      trigger = "L1_MBTS_1_1_Col";
    }
    else
    {
      trigger = "L1_MBTS_1_1";
    }

    if (!(_trigger->PassedL1Trigger(trigger))) continue;
    cutNr = record->addEvent(cutNr, weight, "Trigger Selection");

    //Primary Vertex Selection
    int maxNTracks = maxNofPVTracks();
    histos[6]->Fill(maxNTracks);
    if (maxNTracks < 3) continue;
    cutNr = record->addEvent(cutNr, weight, "Primary Vertex Selection");

    //Muon Requirements
    int nMu = _muons->GetEntries();
    int goodMu = 0;
    std::vector<MTree::Muon> muons;
    std::vector<std::string> channels;
    channels.push_back("muons");
    const int channelStart = cutNr;
    for (int i = 0; i != nMu; i++)
    {
      cutNr = channelStart;
      MTree::Muon* muon = (MTree::Muon*)_muons->At(i);
      MTree::Track* track = muon->GetTrack();
      int author = muon->Author();
      if (author == 11 || author ==  12 || author == 13 || author == 18)
      {
        cutNr = record->addEvent(cutNr, weight, "Initial author requirement",channels);
      }
      else continue;

      histos[1]->Fill(muon->P(),weight);
      histos[8]->Fill(muon->Pt(),weight);
      histos[2]->Fill(muon->Eta(),weight);
      histos[3]->Fill(muon->Phi(),weight);
      histos[10]->Fill(muon->PTCone40(),weight);
      histos[12]->Fill(muon->PTCone40()/muon->Pt(),weight);
      histos[15]->Fill(muon->MatchChi2(),weight);
      histos[17]->Fill(muon->MatchChi2OverDoF(),weight);
      histos[19]->Fill(muon->FitChi2(),weight);
      histos[21]->Fill(muon->FitChi2OverDoF(),weight);
      histos[23]->Fill(muon->TrackIso40(),weight);
      histos[25]->Fill(muon->TrackIso40()/muon->Pt(),weight);

      histos2[1]->Fill(muon->Eta(),muon->MatchChi2(),weight);
      histos2[3]->Fill(muon->Eta(),muon->MatchChi2OverDoF(),weight);

      if (muon->P() < 4) continue;
      cutNr = record->addEvent(cutNr,weight, "P > 4 GeV",channels);

      if (muon->Pt() < 2.5) continue;
      cutNr = record->addEvent(cutNr,weight, "Pt > 2.5 GeV",channels);

      if (fabs(muon->Eta()) > 2.5) continue;
      cutNr = record->addEvent(cutNr, weight, "Eta < 2.5",channels);

      if (track == 0) continue;
      cutNr = record->addEvent(cutNr, weight, "Has a Track", channels);

      if (!(muon->IsCombinedMuon())) continue;
      if (muon->Author() != 12) continue;
      cutNr = record->addEvent(cutNr, weight, "MuID combined muon",channels);

      if (track->NrOfPixelHits() < 1) continue;
      if (track->NrOfSCTHits() < 6) continue;
      cutNr = record->addEvent(cutNr,weight, "Pixel and SCT hits",channels);

      histos[4]->Fill(muon->P(),weight);
      histos[5]->Fill(muon->Eta(),weight);
      histos[7]->Fill(muon->Phi(),weight);
      histos[9]->Fill(muon->Pt(),weight);
      histos[11]->Fill(muon->PTCone40(),weight);
      histos[13]->Fill(muon->PTCone40()/muon->Pt(),weight);
      histos[16]->Fill(muon->MatchChi2(),weight);
      histos[18]->Fill(muon->MatchChi2OverDoF(),weight);
      histos[20]->Fill(muon->FitChi2(),weight);
      histos[22]->Fill(muon->FitChi2OverDoF(),weight);
      histos[24]->Fill(muon->TrackIso40(),weight);
      histos[26]->Fill(muon->TrackIso40()/muon->Pt(),weight);


      histos2[0]->Fill(muon->Eta(),muon->Phi(),weight);
      histos2[2]->Fill(muon->Eta(),muon->MatchChi2(),weight);
      histos2[4]->Fill(muon->Eta(),muon->MatchChi2OverDoF(),weight);

      ++goodMu;

      muons.push_back(*muon);

   //
    // Jet selection
    //
      looseJets.clear();
      cleanJets.clear();
      int nrJets = _jets->GetEntriesFast();
      for (int i = 0; i != nrJets; i++)
      {
        MTree::Jet* jet = (MTree::Jet*)_jets->At(i);
        double jetEta = fabs(jet->Eta());
        if (jet->Pt() < 15.0) continue;
        if (jetEta > 3.0) continue;
        looseJets.push_back(jet);
        if (jet->N90()<3) continue;
        if (jet->SamplingMax() == 17) continue;
//         if (jet->Bad_Cells_Corr_E()/jet->GetRawE() > 0.2) continue;
        cleanJets.push_back(jet);
      }

      double dr = 100.0;

      for (int j = 0; j < (int)looseJets.size(); ++j)
      {
        double deltaR = muon->DeltaR(*looseJets[j]);
        if (deltaR < dr)
        {
	  dr = deltaR;
        }
      }
      if (dr != 100)
      {
        histos[14]->Fill(dr);
      }

    }//Closes muon loop
    histos[0]->Fill(goodMu);

  }//Closes event loop
  record->print(sample + "_muon");
}

  //############################################################################

void DataAnalysis::JetSelection(std::string sample, TH1D** histos, TH2D** histos2)
{
  if (_debug)
  {
    std::cout<<"Calling JetSelection()"<<std::endl;
  }

  TChain *chain = GetChain(sample);
  setBranchAddresses(chain);

  if (_debug)
  {
    std::cout<<"Setting up good run list"<<std::endl;
  }
  Root::TGoodRunsListReader foo(TString("/home/swalch/laboratory/macro-cosmos/HiggsAnalysis/collisions_stablebeams_magsol_pixsctgreen_minbias_900GeV.xml"));
  foo.Interpret();
  Root::TGoodRunsList grl1 = foo.GetMergedGoodRunsList();
  //grl1.Summary(kTRUE);

  int eventNr = 0;
  int goodEvents = 0;
  int totalJets = 0;
  if (_debug)
  {
    std::cout<<"Entering event loop . . ."<<std::endl;
  }
  while(chain->GetEntry(eventNr++))
  {
    if (eventNr % 10000 == 0)
    {
      std::cout<<"Analyzed "<<eventNr<<" events"<<std::endl;
    }

    //For data, check to make sure we have a good lumi-block. Set event weight.
    bool goodBlock = true;
    double weight = 1.0;
    if (sample == "data09")
    {
      int runNr = _eventInfo->RunNumber();
      int lumiBlock = _eventInfo->LumiBlock();
      goodBlock = grl1.HasRunLumiBlock(runNr, lumiBlock);
    }
    else
    {
      weight = 353363.0/2726469;
    }
    if (!goodBlock) continue;

    //Jet selections

    MTree::PrimaryVertex* vert1 = (MTree::PrimaryVertex*)_primaryVertices->At(0);
    if (vert1->NTracks() < 3) continue;

    int n_jets = _jets->GetEntries();
    int goodJets = 0;
    for (int i = 0; i != n_jets; ++i)
    {
      MTree::Jet* jet = (MTree::Jet*)_jets->At(i);

      if (jet->Pt() < 7) continue;
      if (fabs(jet->Eta()) > 2.5) continue;

      ++goodJets;
      ++totalJets;
      histos[0]->Fill(jet->Pt(),weight);
      histos[2]->Fill(jet->Eta(),weight);
      histos[3]->Fill(jet->Phi(),weight);
      histos2[0]->Fill(jet->Eta(),jet->Phi());
    }

    histos[1]->Fill(goodJets,weight);
    ++goodEvents;
  }

  std::cout<<"Total Number of Good Events: "<<goodEvents<<std::endl;
  std::cout<<"Total Number of Good Jets: "<<totalJets<<std::endl;
}

//#################################################################################

void DataAnalysis::BosonSelection(std::string sample, TH1D** histos, TH2D** histos2)
{
  if (_debug)
  {
    std::cout<<"Calling BosonSelection()"<<std::endl;
  }

  bool isMC = false;
  if (sample.compare("singleW") == 0 || sample.compare("singleZ") == 0 || sample.compare("JF17") == 0)
  {
    isMC = true;
  }

  //Set up one RecordClass object to keep track of e, mu, ee, mumu, and emu events. 
  //Set up a couple more to keep track of W and Z cutflows.  Oh, and one for jets.
  RecordClass* record = new RecordClass("lepton");
  std::vector<std::string> channels;
  channels.push_back("e");
  channels.push_back("mu");
  channels.push_back("ee");
  channels.push_back("mumu");
  channels.push_back("emu");
  record->setChannels(channels);

  RecordClass* Wrecord = new RecordClass("Ws");
  std::vector<std::string> Wchannels;
  Wchannels.push_back("E");
  Wchannels.push_back("Mu");
  Wrecord->setChannels(Wchannels);

  RecordClass* Zrecord = new RecordClass("Zs");
  std::vector<std::string> Zchannels;
  Zchannels.push_back("EE");
  Zchannels.push_back("MuMu");
  Zchannels.push_back("EMu");
  Zrecord->setChannels(Zchannels);

  RecordClass* Jetrecord = new RecordClass("Jets");
  std::vector<std::string> Jetchannels;
  Jetchannels.push_back("electron");
  Jetchannels.push_back("muon");
  Jetrecord->setChannels(Jetchannels);

  //Produce vector of sample names (for samples made of multiple parts) to run over.
  std::vector<std::string> samples = GetAllSamples(sample);

  if (_debug)
  {
    std::cout<<"Setting up good run list"<<std::endl;
  }
  TString list;
  list = "/home/swalch/laboratory/macro-cosmos/HiggsAnalysis/GoodRunLists/GRL_7TeV_init.xml";

  Root::TGoodRunsListReader foo(list);
  foo.Interpret();
  Root::TGoodRunsList grl1 =  foo.GetMergedGoodRunsList();
  //  grl1.Summary(kTRUE);

  std::vector<std::string>::iterator iter;
  std::vector<std::string>::iterator end = samples.end();

  std::vector<WCandidate> wCandsFinal;
  std::vector<ZCandidate> zCandsFinal;
  //  std::vector<WCandidate> wwCandsFinal;
  int multiLeptons = 0;
  int wwCandidates = 0;
  for (iter = samples.begin(); iter != end; ++iter)
  {

    std::string sampleName = *iter;
    TChain *chain = GetChain(sampleName);
    setBranchAddresses(chain);
    std::cout << "Analyzing sample " << sampleName << std::endl;

    if (_debug)
    {
      std::cout << "Looking up sample weight/cross-section" << std::endl;
    }

    double sampleWeight = 0.0;
    double sampleXSec = 0.0;

    if (isMC)
    {
      sampleWeight = GetSampleWeight(sampleName);
      sampleXSec = GetCrossSection(sampleName);
    }

    if (_debug)
    {
      std::cout<<"Entering event loop"<<std::endl;
    }

    int eventNr = 0;
    double weight = 1.0;
    while(chain->GetEntry(eventNr++))
    {
      int cutNr = 0;
      if (eventNr % 10000 == 0)
      {
        std::cout<<"Analyzed "<<eventNr<<" events."<<std::endl;
      }

      //Check for W/Z events in JF17 sample
      if ((sample.find("JF17") != string::npos) && (IsJF17WZEvent()))
      { 
	std::cout << "Rejecting JF17 W/Z event!" << std::endl;
        continue;
      }

      if (isMC)
      {
        weight = sampleXSec/sampleWeight*(_eventInfo->EventWeight());
      }

      cutNr = record->addEvent(cutNr, weight, "Before Event Selection");

      //Trigger Selection
      if (_debug)
      {
        std::cout << "Trigger Selection" << std::endl;
      }
      std::string etrigger = "L1_EM5";
      std::string mtrigger = "L1_MU10";

      if (!(_trigger->PassedL1Trigger(etrigger)) && !(_trigger->PassedL1Trigger(mtrigger))) continue;
      cutNr = record->addEvent(cutNr, weight, "L1 Triggers");

      //Primary Vertex Selection
      if (_debug)
      {
        std::cout << "Primary Vertex Selection" << std::endl;
      }
      int maxNTracks = maxNofPVTracks();
      if (maxNTracks < 3) continue;
      cutNr = record->addEvent(cutNr, weight, "Primary Vertex has 3 or more tracks");

      if (_debug)
      {
	std::cout << "Checking z of primary vertex" << std::endl;
      }
      double pvZ = zPVmaxTracks();
      if (fabs(pvZ) > 150) continue;
      cutNr = record->addEvent(cutNr, weight, "|z_(pv with most tracks)| < 150 mm");

      if (_debug)
      {
        std::cout << "Checking Good Runlist" << std::endl;
      }

      bool passed = true;
      if (sample == "data-skimmed")
      {
        int runNr = _eventInfo->RunNumber();
        int lumiBlock = _eventInfo->LumiBlock();
        passed = grl1.HasRunLumiBlock(runNr,lumiBlock);
      }
      if (!passed) continue;
      cutNr = record->addEvent(cutNr, weight, "Good Run List");

      //Lepton Requirements
      int nMu = _muons->GetEntries();
      int nEl = _electrons->GetEntries();
      std::vector<MTree::Lepton*> leptons;
      std::vector<MTree::Lepton*> medLeptons;

      if (_debug)
      {
        std::cout << "Creating vector of leptons" << std::endl;
      }
      for (int i = 0; i != nEl; ++i)
      {
        MTree::Electron* electron = (MTree::Electron*)_electrons->At(i);
        leptons.push_back(electron);

        histos[0]->Fill(electron->Pt(),weight);
        if (electron->Pt() != 0)
        {
          histos[1]->Fill(electron->Eta(),weight);
        } 
        histos[2]->Fill(electron->Phi(),weight);
        histos[3]->Fill(electron->ETCone40()/electron->Pt(),weight);
        histos[4]->Fill(electron->TrackIso40()/electron->Pt(),weight);
      }

    for (int i = 0; i != nMu; ++i)
    {
      MTree::Muon* muon = (MTree::Muon*)_muons->At(i);
      int author = muon->Author();
      if (author == 11 || author == 12 || author == 13 || author == 18)
      {
        leptons.push_back(muon);
        histos[5]->Fill(muon->Pt(),weight);
	if (muon->Pt() != 0)
	{
          histos[6]->Fill(muon->Eta(),weight);
	}
        histos[7]->Fill(muon->Phi(),weight);
        histos[8]->Fill(muon->ETCone40()/muon->Pt(),weight);
        histos[9]->Fill(muon->TrackIso40()/muon->Pt(),weight);
	histos[30]->Fill(muon->MatchChi2OverDoF());
      }
    }

    if (leptons.size() < 1) continue;
    channels.clear();
    channels = findChannels(leptons);
    cutNr = record->addEvent(cutNr,weight,"Event has >= 1 lepton",channels);

    //Pt and P requirement
    if (_debug)
    {
      std::cout << "Applying pT and p cuts" << std::endl;
    }
    for (int i = 0; i != (int)leptons.size(); ++i)
    {
      if (leptons[i]->Pt() < 2.5)
      {
	leptons.erase(leptons.begin() + i);
	--i;
      }
    }
    for (int i = 0; i != (int)leptons.size(); ++i)
    {
      if (leptons[i]->LeptonPdgID() == 13 && leptons[i]->P() < 4)
      {
	leptons.erase(leptons.begin() + i);
	--i;
      }
    }
    if (leptons.size() < 1) continue;
    channels.clear();
    channels = findChannels(leptons);
    cutNr = record->addEvent(cutNr, weight,"Pt > 2.5, P > 4 (muons)",channels);

    //Eta requirements
    if (_debug)
    {
      std::cout << "Applying eta cuts" << std::endl;
    }
    for (int i = 0; i != (int)leptons.size(); ++i)
    {
      if (fabs(leptons[i]->Eta()) > 2.5)
      {
	leptons.erase(leptons.begin() + i);
	--i;
      }
    }
    for (int i = 0; i != (int)leptons.size(); ++i)
    {
      if (leptons[i]->LeptonPdgID() == 11)
      {
	if (fabs(leptons[i]->Eta()) > 1.37 && fabs(leptons[i]->Eta()) < 1.52)
	{
	  leptons.erase(leptons.begin() + i);
	  --i;
	}
      }
    }
    if (leptons.size() < 1) continue;
    channels.clear();
    channels = findChannels(leptons);
    cutNr = record->addEvent(cutNr, weight, "|Eta| < 2.5, not in crack region (electrons)",channels);

    //All leptons have tracks
    if (_debug)
    {
      std::cout << "Checking for tracks" << std::endl;
    }
    for (int i = 0; i != (int)leptons.size(); ++i)
    {
      if (leptons[i]->GetTrack() == 0)
      {
	leptons.erase(leptons.begin() + i);
	--i;
      }
    }
    if (leptons.size() < 1) continue;
    channels.clear();
    channels = findChannels(leptons);
    cutNr = record->addEvent(cutNr,weight,"Leptons have tracks",channels);

    //Author requirements.
    if (_debug)
    {
      std::cout << "Checking lepton authors" << std::endl;
    }
    for (int i = 0; i != (int)leptons.size(); ++i)
    {
      if (leptons[i]->LeptonPdgID() == 11)
      {
	if ((leptons[i]->Author() != 1) && (leptons[i]->Author() != 3))
	{
	  leptons.erase(leptons.begin() + i);
	  --i;
	}
      }
      else if (leptons[i]->LeptonPdgID() == 13)
      {
	if (leptons[i]->Author() != 12)
	{
	  leptons.erase(leptons.begin() + i);
	  --i;
	}
      }
    }
    if (leptons.size() < 1) continue;
    channels.clear();
    channels = findChannels(leptons);
    cutNr = record->addEvent(cutNr,weight,"Author= 1 || 3 (electrons), 12 (muons)",channels);

    //(Muons) Vertex requirements to veto cosmics.
    if (_debug)
    {
      std::cout << "Checking vertex on muons" << std::endl;
    }
    for (int i = 0; i != (int)leptons.size(); ++i)
    {
      if (leptons[i]->LeptonPdgID() == 13)
      {
	MTree::Track* mstrack = ((MTree::Muon*)leptons[i])->GetStandAloneTrack();
	if (mstrack == 0)
	{
	  leptons.erase(leptons.begin() + i);
	  --i;
	}
	else if (fabs(mstrack->Vtx_Z0()) > 10)
	{
	  leptons.erase(leptons.begin() + i);
	  --i;
	}
      }
    }
    if (leptons.size() < 1) continue;
    channels.clear();
    channels = findChannels(leptons);
    cutNr = record->addEvent(cutNr,weight,"(Muons) |z0-z_vtx| < 10 mm",channels);

    //Medium requirements
    if (_debug)
    {
      std::cout << "Applying medium requirements" << std::endl;
    }
    for (int i = 0; i != (int)leptons.size(); ++i)
    {
      if (leptons[i]->LeptonPdgID() == 11)
      {
	if (!(((MTree::Electron*)leptons[i])->IsMedium()))
	{
	  leptons.erase(leptons.begin() + i);
	  --i;
	}
      }
      else if (leptons[i]->LeptonPdgID() == 13)
      {
	MTree::Muon* muon = (MTree::Muon*)leptons[i];
	MTree::Track* track = muon->GetTrack();
	if (!(muon->IsCombinedMuon()))
	{
	  leptons.erase(leptons.begin() + i);
	  --i;
	}
	else if (track->NrOfPixelHits() < 1 && track->NrOfSCTHits() < 6)
	{
	  leptons.erase(leptons.begin() + i);
	  --i;
	}
      }
    }
    if (leptons.size() < 1) continue;
    channels.clear();
    channels = findChannels(leptons);
    cutNr = record->addEvent(cutNr,weight,"Medium electrons, Combined muons with ID hits",channels);

    //Pt > 20 GeV
    if (_debug)
    {
      std::cout << "Requiring pT > 20 GeV" << std::endl;
    }
    for (int i = 0; i != (int)leptons.size(); ++i)
    {
      if (leptons[i]->Pt() < 20)
      {
	leptons.erase(leptons.begin() + i);
	--i;
      }
    }
    if (leptons.size() < 1) continue;
    channels.clear();
    channels = findChannels(leptons);
    cutNr = record->addEvent(cutNr,weight,"Pt > 20 GeV",channels);

    //(Muons only) Make sure MS track has Pt > 10 GeV
    if (_debug)
    {
      std::cout << "Checking MS track for muons" << std::endl;
    }
    for (int i = 0; i != (int)leptons.size(); ++i)
    {
      if (leptons[i]->LeptonPdgID() == 13)
      {
	MTree::Track* mtrack = ((MTree::Muon*)leptons[i])->GetStandAloneTrack();
	if (mtrack == 0)
	{
	  leptons.erase(leptons.begin() + i);
	  --i;
	}
	else if (mtrack->Pt() < 10)
	{
	  leptons.erase(leptons.begin() + i);
	  --i;
	}
      }
    }
    if (leptons.size() < 1) continue;
    channels.clear();
    channels = findChannels(leptons);
    cutNr = record->addEvent(cutNr,weight,"MS track has Pt > 10 GeV",channels);

    //Make medium lepton plots and save vector for Z analysis.
    for (int i = 0; i != (int)leptons.size(); ++i)
    {
      if (leptons[i]->LeptonPdgID() == 11)
      {
	MTree::Electron* elec = (MTree::Electron*)leptons[i];
	histos[44]->Fill(elec->Pt(),weight);
	histos[45]->Fill(elec->Eta(),weight);
	histos[46]->Fill(elec->Phi(),weight);
	histos[47]->Fill(elec->ETCone40()/elec->Pt(),weight);
	histos[48]->Fill(elec->TrackIso40()/elec->Pt(),weight);
      }
      else
      {
	MTree::Muon* muon = (MTree::Muon*)leptons[i];
	histos[49]->Fill(muon->Pt(),weight);
	histos[50]->Fill(muon->Eta(),weight);
	histos[51]->Fill(muon->Phi(),weight);
	histos[52]->Fill(muon->ETCone40()/muon->Pt(),weight);
	histos[53]->Fill(muon->TrackIso40()/muon->Pt(),weight);
	histos[54]->Fill(muon->MatchChi2OverDoF(),weight);
      }
    }
    medLeptons = leptons;

    //Tight requirements
    if (_debug)
    {
      std::cout << "Applying tight requirement" << std::endl;
    }
    for (int i = 0; i != (int)leptons.size(); ++i)
    {
      if (leptons[i]->LeptonPdgID() == 11)
      {
	MTree::Electron* electron = (MTree::Electron*)leptons[i];
	if (!(electron->IsTight()))
	{
	  leptons.erase(leptons.begin() + i);
	  --i;
	}
	else
	{
          histos[10]->Fill(electron->Pt(),weight);
          histos[11]->Fill(electron->Eta(),weight);
          histos[12]->Fill(electron->Phi(),weight);
          histos[13]->Fill(electron->ETCone40()/electron->Pt(),weight);
          histos[14]->Fill(electron->TrackIso40()/electron->Pt(),weight);
	}
      }
      else if (leptons[i]->LeptonPdgID() == 13)
      {
	MTree::Muon* muon = (MTree::Muon*)leptons[i];

        histos[15]->Fill(muon->Pt(),weight);
        histos[16]->Fill(muon->Eta(),weight);
        histos[17]->Fill(muon->Phi(),weight);
        histos[18]->Fill(muon->ETCone40()/muon->Pt(),weight);
        histos[19]->Fill(muon->TrackIso40()/muon->Pt(),weight);
	histos[31]->Fill(muon->MatchChi2OverDoF());
      }
    }
    if (leptons.size() < 1) continue;
    channels.clear();
    channels = findChannels(leptons);
    cutNr = record->addEvent(cutNr,weight,"Tight electrons",channels);

    //Overlap removal.
    leptons = removeOverlap(leptons);
    channels.clear();
    channels = findChannels(leptons);
    cutNr = record->addEvent(cutNr,weight,"Overlap removal",channels);

    //Only because Jonas said I should, fill loose "W" plots here.
    leptons = orderByPt(leptons);
    double delPhi = leptons[0]->DeltaPhi(*(_missingET));
    double mT = sqrt(2*leptons[0]->Pt()*_missingET->Pt()*(1-cos(delPhi)));
    if (leptons[0]->LeptonPdgID() == 11)
    {
      MTree::Electron* elec = (MTree::Electron*)leptons[0];
      histos[20]->Fill(mT,weight);
      histos[21]->Fill(_missingET->MET(),weight);
      histos2[0]->Fill(mT,elec->ETCone40()/elec->Pt());
      histos2[1]->Fill(mT,elec->TrackIso40()/elec->Pt());
      histos2[2]->Fill(mT,_missingET->MET());
      histos2[3]->Fill(mT,delPhi);
    }
    else if (leptons[0]->LeptonPdgID() == 13)
    {
      MTree::Muon* muon = (MTree::Muon*)leptons[0];
      histos[25]->Fill(mT,weight);
      histos[26]->Fill(_missingET->MET(),weight);
      histos2[8]->Fill(mT,muon->ETCone40()/muon->Pt());
      histos2[9]->Fill(mT,muon->TrackIso40()/muon->Pt());
      histos2[10]->Fill(mT,_missingET->MET());
      histos2[11]->Fill(mT,delPhi);
      histos2[16]->Fill(mT,muon->MatchChi2OverDoF());
    }

    //Split up into final states.
    if ((int)leptons.size() > 2)
    {
      if (_debug)
      {
	std::cout << "Analyzing event with > 2 tight leptons" << std::endl;
      }
      multiLeptons++;
      if (!isMC)
      {
	std::cout << "An event with more than two tight leptons!" << std::endl;
	printEverything();
      }
    }
    else if (_missingET->MET() > 20)
    {
      if ((int)leptons.size() > 1)
      {
	if (_debug)
	{
	  std::cout << "Analyzing 'WW' event" << std::endl;
	}
	wwCandidates++;
	//Coolness, it's a WW--print out everything.
	if (!isMC)
	{
	std::cout << "A WW candidate event!" << std::endl;
	std::cout << "Go write some real analylsis code for it!" << std::endl;
	//	printEverything();
	}
      }
      else
      {
	//It's a W.  Not quite so cool any more.
	//Make W.  Fill loose plots. Add event to record object.
	if (_debug)
	{
	  std::cout << "Analyzing 'W' event" << std::endl;
	}
	int wCut = 0;
	Wchannels.clear();
	WCandidate* cand;
	if (leptons[0]->LeptonPdgID() == 11)
	{
	  MTree::Electron* elec = (MTree::Electron*)leptons[0];
	  cand = new WCandidate(*_eventInfo,*elec,*_missingET);
// 	  histos[20]->Fill(cand->getMT(),weight);
// 	  histos[21]->Fill((cand->getMissingEt()).MET(),weight);
// 	  histos2[0]->Fill(cand->getMT(),elec->ETCone40()/elec->Pt(),weight);
// 	  histos2[1]->Fill(cand->getMT(),elec->TrackIso40()/elec->Pt(),weight);
// 	  histos2[2]->Fill(cand->getMT(),(cand->getMissingEt()).MET(),weight);
// 	  histos2[3]->Fill(cand->getMT(),fabs(cand->getDPhi()),weight);
	  Wchannels.push_back("E");
	}
	else
	{
	  MTree::Muon* muon = (MTree::Muon*)leptons[0];
	  cand = new WCandidate(*_eventInfo,*muon,*_missingET);
// 	  histos[25]->Fill(cand->getMT(),weight);
// 	  histos[26]->Fill((cand->getMissingEt()).MET(),weight);
// 	  histos2[8]->Fill(cand->getMT(),muon->ETCone40()/muon->Pt(),weight);
// 	  histos2[9]->Fill(cand->getMT(),muon->TrackIso40()/muon->Pt(),weight);
// 	  histos2[10]->Fill(cand->getMT(),(cand->getMissingEt()).MET(),weight);
// 	  histos2[11]->Fill(cand->getMT(),fabs(cand->getDPhi()),weight);
// 	  histos2[16]->Fill(cand->getMT(),muon->MatchChi2OverDoF());
	  Wchannels.push_back("Mu");
	}
	wCut = Wrecord->addEvent(wCut,weight,"Initial number of events");
	wCut = Wrecord->addEvent(wCut,weight,"W Candidate passed MET > 25",Wchannels);

	//Apply isolation requirements.
	bool caloIso = true;
	bool trackIso = true;
        if (cand->decayedToMuon())
        {
 	  MTree::Muon muon = cand->getMuon();

	  if (muon.ETCone40()/muon.Pt() > 0.3)
	  {
	    caloIso = false;
	  }
	  if (muon.TrackIso40()/muon.Pt() > 0.3)
          {
	    trackIso = false;
	  }
        }
        else
        {
	  MTree::Electron elec = cand->getElectron();

	  if (elec.ETCone40()/elec.Pt() > 0.3)
          {
	    caloIso = false;
	  }
	  if (elec.TrackIso40()/elec.Pt() > 0.3)
	  {
	    trackIso = false;
	  }
        }

	bool passMass = true;
	if (cand->getMT() < 40)
	{
	  passMass = false;
	}

	if (caloIso)
	{
	  wCut = Wrecord->addEvent(wCut,weight,"ETCone40/Pt < 0.3",Wchannels);
	  if (trackIso)
	  {
	    wCut = Wrecord->addEvent(wCut,weight,"TrackIso40/Pt < 0.3",Wchannels);

	    if (passMass)
	    {
	      wCut = Wrecord->addEvent(wCut,weight,"mT > 40 GeV",Wchannels);

	      //Put jet selections here.
	      int jetCut = 0;
	      Jetchannels.clear();
	      if (cand->decayedToMuon())
	      {
		Jetchannels.push_back("muon");
	      }
	      else
	      {
		Jetchannels.push_back("electron");
	      }

	      if (_jets->GetEntriesFast() > 0)
	      {
		jetCut = Jetrecord->addEvent(jetCut,weight,"Event has W and jets");
		jetCut = Jetrecord->addEvent(jetCut,weight,"Event has a jet",Jetchannels);
	      }

	      std::vector<MTree::Jet*> jets = findNonOverlapJets(leptons);
	      if ((int)jets.size() > 0)
	      {
		jetCut = Jetrecord->addEvent(jetCut,weight,"Events with non-overlapping jets",Jetchannels);
	      }

	      if (cand->decayedToMuon())
	      {
		for (int i = 0; i != (int)jets.size(); ++i)
		{
		  histos[33]->Fill(jets[i]->Pt(),weight);
		  histos[39]->Fill(jets[i]->Eta(),weight);
		}
		histos[32]->Fill((int)jets.size(),weight);
	      }
	      else
	      {
		for (int i = 0; i != (int)jets.size(); ++i)
		{
		  histos[43]->Fill(jets[i]->Pt(),weight);
		  histos[38]->Fill(jets[i]->Eta(),weight);
		}
		histos[42]->Fill((int)jets.size(),weight);
	      }

	      for (int i = 0; i != (int)jets.size(); ++i)
	      {
		if (jets[i]->Pt() < 15)
		{
		  jets.erase(jets.begin() + i);
		  --i;
		}
	      }
	      if ((int)jets.size() > 0)
	      {
		jetCut = Jetrecord->addEvent(jetCut,weight,"Jet Pt > 15 GeV",Jetchannels);
	      }

	      for (int i = 0; i != (int)jets.size(); ++i)
	      {
		if (fabs(jets[i]->Eta()) > 2.5)
		{
		  jets.erase(jets.begin() + i);
		  --i;
		}
	      }
	      if ((int)jets.size() > 0)
	      {
		jetCut = Jetrecord->addEvent(jetCut,weight,"Jet |Eta| < 2.5",Jetchannels);
	      }

            if (cand->decayedToMuon())
            {
	      MTree::Muon muon = cand->getMuon();
	      histos[27]->Fill(cand->getMT(),weight);
	      histos[28]->Fill((cand->getMissingEt()).MET(),weight);
	      histos2[12]->Fill(cand->getMT(),muon.ETCone40()/muon.Pt(),weight);
	      histos2[13]->Fill(cand->getMT(),muon.TrackIso40()/muon.Pt(),weight);
	      histos2[14]->Fill(cand->getMT(),(cand->getMissingEt()).MET(),weight);
	      histos2[15]->Fill(cand->getMT(),fabs(cand->getDPhi()),weight);
	      histos2[17]->Fill(cand->getMT(),muon.MatchChi2OverDoF());

	      histos[60]->Fill(muon.Pt(),weight);
	      histos[61]->Fill(muon.Eta(),weight);
	      histos[62]->Fill(muon.Phi(),weight);
	      histos[63]->Fill(muon.ETCone40()/muon.Pt(),weight);
	      histos[64]->Fill(muon.TrackIso40()/muon.Pt(),weight);
	      histos[65]->Fill(muon.MatchChi2OverDoF(),weight);

		for (int i = 0; i != (int)jets.size(); ++i)
		{
		  histos[37]->Fill(jets[i]->Pt(),weight);
		  histos[41]->Fill(jets[i]->Eta(),weight);
		}
		histos[36]->Fill((int)jets.size(),weight);
            }
            else
            {
 	      MTree::Electron elec = cand->getElectron();
	      histos[22]->Fill(cand->getMT(),weight);
	      histos[23]->Fill((cand->getMissingEt()).MET(),weight);
	      histos2[4]->Fill(cand->getMT(),elec.ETCone40()/elec.Pt(),weight);
	      histos2[5]->Fill(cand->getMT(),elec.TrackIso40()/elec.Pt(),weight);
	      histos2[6]->Fill(cand->getMT(),(cand->getMissingEt()).MET(),weight);
	      histos2[7]->Fill(cand->getMT(),fabs(cand->getDPhi()),weight);

	      histos[55]->Fill(elec.Pt(),weight);
	      histos[56]->Fill(elec.Eta(),weight);
	      histos[57]->Fill(elec.Phi(),weight);
	      histos[58]->Fill(elec.ETCone40()/elec.Pt(),weight);
	      histos[59]->Fill(elec.TrackIso40()/elec.Pt(),weight);

		for (int i = 0; i != (int)jets.size(); ++i)
		{
		  histos[35]->Fill(jets[i]->Pt(),weight);
		  histos[40]->Fill(jets[i]->Eta(),weight);
		}
		histos[34]->Fill((int)jets.size(),weight);
            }

	    //	    wCandsFinal.push_back(*cand);
	    delete cand;
	    }
	  }
	}
      }
    }//Closes code looking at events with MET > 20 GeV
    else
    {
      if (_debug)
      {
	std::cout << "Analyzing 'Z' event" << std::endl;
      }
      int zCut = 0;
      Zchannels.clear();
      leptons = orderByPt(leptons);
      if (leptons[0]->LeptonPdgID() == 11)
      {
	Zchannels.push_back("EE");
      }
      else if (leptons[0]->LeptonPdgID() == 13)
      {
	Zchannels.push_back("MuMu");
      }
      zCut = Zrecord->addEvent(zCut,weight,"Initial Total events");
      zCut = Zrecord->addEvent(zCut,weight,"Initial Z-like events",Zchannels);
      //Try to make a Z of one tight and one other tight or loose lepton.
      medLeptons = orderByPt(medLeptons);
      medLeptons = removeOverlap(medLeptons);

      int index1 = -10;
      int index2 = -10;
      double zmass = 91.1876;
      double mDiff = 1000;
      if ((int)medLeptons.size() > 1)
      {
	for (int i = 0; i != (int)medLeptons.size(); ++i)
	{
	  for (int j = i + 1; j != (int)medLeptons.size(); ++j)
	  {
	    if ((medLeptons[i]->Charge() != medLeptons[j]->Charge()) && (medLeptons[i]->LeptonPdgID() == medLeptons[j]->LeptonPdgID()))
	    {
	      TLorentzVector ll = *medLeptons[i] + *medLeptons[j];
	      if (fabs(ll.M() - zmass) < mDiff)
	      {
	        mDiff = fabs(ll.M() - zmass);
	        index1 = i;
	        index2 = j;
	      }
	    }
	  }
	}
      }
      if (index1 != index2)
      {
	ZCandidate* maybe;
	if (medLeptons[index1]->LeptonPdgID() == 11)
	{
	  maybe = new ZCandidate(*_eventInfo,*(MTree::Electron*)medLeptons[index1],*(MTree::Electron*)medLeptons[index2]);
	  MTree::Electron leadE = maybe->getLeadElectron();
	  MTree::Electron subE = maybe->getSubElectron();
	  histos[24]->Fill(maybe->getZMass(),weight);
	  histos[66]->Fill(leadE.Pt(),weight);
	  histos[68]->Fill(leadE.Eta(),weight);
	  histos[70]->Fill(leadE.Phi(),weight);
	  histos[72]->Fill(leadE.Charge(),weight);
	  histos[74]->Fill(leadE.ETCone40()/leadE.Pt(),weight);
	  histos[67]->Fill(subE.Pt(),weight);
	  histos[69]->Fill(subE.Eta(),weight);
	  histos[71]->Fill(subE.Phi(),weight);
	  histos[73]->Fill(subE.Charge(),weight);
	  histos[75]->Fill(subE.ETCone40()/subE.Pt(),weight);
	}
	else
	{
	  maybe = new ZCandidate(*_eventInfo, *(MTree::Muon*)medLeptons[index1],*(MTree::Muon*)medLeptons[index2]);
	  MTree::Muon leadM = maybe->getLeadMuon();
	  MTree::Muon subM = maybe->getSubMuon();
          histos[29]->Fill(maybe->getZMass(),weight);
	  histos[76]->Fill(leadM.Pt(),weight);
	  histos[78]->Fill(leadM.Eta(),weight);
	  histos[80]->Fill(leadM.Phi(),weight);
	  histos[82]->Fill(leadM.Charge(),weight);
	  histos[84]->Fill(leadM.ETCone40()/leadM.Pt(),weight);
	  histos[77]->Fill(subM.Pt(),weight);
	  histos[79]->Fill(subM.Eta(),weight);
	  histos[81]->Fill(subM.Phi(),weight);
	  histos[83]->Fill(subM.Charge(),weight);
	  histos[85]->Fill(subM.ETCone40()/subM.Pt(),weight);
	}
	zCut = Zrecord->addEvent(zCut,weight,"Possible Z lepton pairs",Zchannels);
	//	zCandsFinal.push_back(*maybe);
	delete maybe;
      }
    }//Closes Z else block


//     //for testing purposes only
//     if (eventNr > 200000)
//     {
//       eventNr = 100000000;
//     }

    if (_debug)
    {
      std::cout << "Reached end of loop" << std::endl;
    }

    }//Closes event loop
  }//Closes samples loop

//   std::string filenameW;
//   std::string filenameZ;
//   //  std::string filenameWW;
//   std::ofstream Wfile;
//   std::ofstream Zfile;
//   // std::ofstream WWfile;

//   filenameW = "event-summaries/" + sample + "_Wevents.txt";
//   const char* tagW = filenameW.c_str();
//   Wfile.open(tagW);
//   filenameZ = "event-summaries/" + sample + "_Zevents.txt";
//   const char* tagZ = filenameZ.c_str();
//   Zfile.open(tagZ);
//   filenameWW = "event-summaries/" + sample + "_WWevents.txt";
//   const char* tagWW = filenameWW.c_str();
//   WWfile.open(tagWW);

//   Wfile << "Events of Interest in sample " << sample << std::endl<<std::endl;
//   Zfile << "Events of Interest in sample " << sample << std::endl<<std::endl;
  //  WWfile << "Events of Interest in sample " << sample << std::endl<<std::endl;

//   if (!isMC)
//   {
//     for (int i = 0; i != (int)wCandsFinal.size(); ++i)
//     {
//       wCandsFinal[i].printInfo(Wfile);
//     }

//     for (int i = 0; i != (int)zCandsFinal.size(); ++i)
//     {
//       zCandsFinal[i].printInfo(Zfile);
//     }
//   }

//   for (int i = 0; i != (int)wwCandsFinal.size(); ++i)
//   {
//     wwCandsFinal[i].printInfo(WWfile);
//   }

//   std::cout<< "W Candidate events: " << wCandsFinal.size() << std::endl;
//   std::cout<< "Z Candidate events: " << zCandsFinal.size() << std::endl;
  std::cout << "WW Candidate events: " << wwCandidates << std::endl;
  std::cout << "Multi-Lepton events: " << multiLeptons << std::endl;
  //  std::cout<< "WW Candidate events: " << wwCandsFinal.size() << std::endl;

  record->print(sample + "_boson");
  Wrecord->print(sample + "_Ws");
  Zrecord->print(sample + "_Zs");
//   Wfile.close();
//   Zfile.close();
  //  WWfile.close();
}

ClassImp(DataAnalysis)
