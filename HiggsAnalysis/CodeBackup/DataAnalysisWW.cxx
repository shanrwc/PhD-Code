#include "DataAnalysis.h"

void DataAnalysis::wwCutflow(std::string sample, TH1D** histos, TH2D** histos2)
{
  if (_debug)
  {
    std::cout << "Calling wwCutflow" << std::endl;
  }

  //Set up record class objects to keep track of things.
  if (_debug)
  {
    std::cout << "Setting up records" << std::endl;
  }
  RecordClass* eRec = new RecordClass("electrons");
  RecordClass* mRec = new RecordClass("muons");

  RecordClass* eventRec = new RecordClass("events");
  std::vector<std::string> eventchan;
  eventchan.push_back("ee");
  eventchan.push_back("emu");
  eventchan.push_back("mumu");
  eventRec->setChannels(eventchan);

  int z0s[40];
  for (int i = 0; i != 40; ++i)
  {
    z0s[i] = 0;
  }

  ofstream outFile("Cut_1_WWee.txt");

  //Check if sample is MC or not. Set up GRL.
  if (_debug)
  {
    std::cout << "Setting up good run list" << std::endl;
  }
  bool isMC = false;
  if (sample.compare("h2ww160") == 0 || sample.compare("smww") == 0 || sample.compare("WWee") == 0 || sample.compare("WWmm") == 0 || sample.compare("WWem") == 0 || sample.compare("singleWenu") == 0 || sample.compare("singleWmnu") == 0)
  {
    isMC = true;
  }
  TString list;
  list = "/home/swalch/laboratory/macro-cosmos/HiggsAnalysis/GoodRunLists/GRL_7TeV_init.xml";
  Root::TGoodRunsListReader foo(list);
  foo.Interpret();
  Root::TGoodRunsList grl1 = foo.GetMergedGoodRunsList();

  //Grab vector of sample names to run over and loop over them.
  std::vector<std::string> samples = GetAllSamples(sample);
  std::vector<std::string>::iterator iter;
  std::vector<std::string>::iterator end = samples.end();

  for (iter = samples.begin(); iter != end; ++iter)
  {
    if (_debug)
    {
      std::cout << "Analyzing sample " << *iter << std::endl;
    }
    //Set up chain to loop over and other ROOT voodoo.
    std::string sampleName = *iter;
    TChain *chain = GetChain(sampleName);
    setBranchAddresses(chain);

    if (_debug)
    {
      std::cout << "Looking up weight and cross-section" << std::endl;
    }
    double sampleWeight = 0.0;
    double sampleXSec = 0.0;
    if (isMC)
    {
      sampleWeight = GetSampleWeight(sampleName);
      sampleXSec = GetCrossSection(sampleName);
      eventRec->setWeightSum(sampleWeight);
    }

    if(_debug)
    {
      std::cout << "Entering event loop" << std::endl;
    }
    int eventNr = 0;
    double weight = 1.0;
    while (chain->GetEntry(eventNr++))
    {
      int eventCut = 0;
      if (eventNr % 10000 == 0)
      {
	std::cout << "Analyzed " << eventNr << " events " << std::endl;
      }

      if (isMC)
      {
	weight = sampleXSec/sampleWeight*(_eventInfo->EventWeight());
      }

      eventCut = eventRec->addEvent(eventCut,weight,"Before Event Selection");

      //Good Run List
      if (_debug)
      {
	std::cout << "Checking good run list" << std::endl;
      }
      bool passed = true;
      if (sample == "data-skimmed")
      {
	int runNr = _eventInfo->RunNumber();
	int lumiBlock = _eventInfo->LumiBlock();
	passed = grl1.HasRunLumiBlock(runNr,lumiBlock);
      }
      if (!passed) continue;
      eventCut = eventRec->addEvent(eventCut,weight,"Passed Good Run list");


      //Trigger: e10_medium and mu10
      if (_debug)
      {
	std::cout << "Checking trigger requirements" << std::endl;
      }
//       if (!(_trigger->PassedL1Trigger("L1_EM5")) && !(_trigger->PassedL1Trigger("L1_MU10"))) continue;
//       eventCut = eventRec->addEvent(eventCut,weight,"Passed L1 trigger");

//       if (!(_trigger->PassedL2Trigger("L2_e10_medium")) && !(_trigger->PassedL2Trigger("L2_mu10"))) continue;
//       eventCut = eventRec->addEvent(eventCut,weight,"Passed L2 trigger");

//       if (!(_trigger->PassedEFTrigger("EF_e10_medium")) && !(_trigger->PassedEFTrigger("EF_mu10"))) continue;

      if (!((_trigger->PassedL1Trigger("L1_EM5") && _trigger->PassedL2Trigger("L2_e10_medium") && _trigger->PassedEFTrigger("EF_e10_medium")) || (_trigger->PassedL1Trigger("L1_MU10") && _trigger->PassedL2Trigger("L2_mu10") && _trigger->PassedEFTrigger("EF_mu10")))) continue;
      eventCut = eventRec->addEvent(eventCut,weight,"Passed trigger chain");

	//Produce vector of interesting leptons
      std::vector<MTree::Lepton*> leptons;
      int nEl = _electrons->GetEntries();
      for (int i = 0; i != nEl; ++i)
      {
	int eCut = 0;
	eCut = eRec->addEvent(eCut,weight,"Initial number of electrons");

	MTree::Electron* elec = (MTree::Electron*)_electrons->At(i);
	histos[34]->Fill(elec->Pt(),weight);
	histos[35]->Fill(elec->ClusterBECombEta(),weight);
	histos[36]->Fill(elec->Phi(),weight);
	histos[37]->Fill(elec->ETCone30()/elec->Pt(),weight);
	MTree::Track* track = elec->GetTrack();	
	if (track != 0)
	{
	  histos[38]->Fill(TMath::Abs(track->Vtx_D0()/track->Vtx_D0_Err()),weight);
	}
	int nTr = _tracks->GetEntries();
	int overlapTracks = 0;
	for (int j = 0; j != nTr; ++j)
	{
	  MTree::Track* arrow = (MTree::Track*)_tracks->At(j);
	  if (arrow == track) continue;
	  if (arrow->Pt() < 1) continue;
	  if (arrow->NrOfPixelHits() < 2) continue;
	  if (arrow->NrOfSCTHits() < 6) continue;
	  if (elec->DeltaR(*arrow) > 0.3) continue;

	  ++overlapTracks;
	}


	if (!(elec->Author() == 1) && !(elec->Author() == 3)) continue;
	eCut = eRec->addEvent(eCut,weight,"Author cut");
	if(elec->Pt() < 15) continue;
	eCut = eRec->addEvent(eCut,weight,"Pt > 15 GeV");
	if (TMath::Abs(elec->ClusterBECombEta()) > 2.47) continue;
	if (TMath::Abs(elec->ClusterBECombEta()) > 1.37 && TMath::Abs(elec->ClusterBECombEta()) < 1.52) continue;
	eCut = eRec->addEvent(eCut,weight,"Eta requirements");
	if (!(elec->IsTight())) continue;
	eCut = eRec->addEvent(eCut,weight,"Tight electron");
	if (track == 0) continue;
	if (TMath::Abs(track->Vtx_D0()/track->Vtx_D0_Err()) > 10) continue;
	eCut = eRec->addEvent(eCut,weight,"d0 significance < 10");
	if (elec->ETCone30()/elec->Pt() > 0.1) continue;
	eCut = eRec->addEvent(eCut,weight,"ETCone30/Pt < 0.1");

	if (overlapTracks > 3) continue;
	eCut = eRec->addEvent(eCut,weight,"Number of nearby tracks");

	leptons.push_back(elec);
	histos[23]->Fill(elec->Pt(),weight);
	histos[24]->Fill(elec->ClusterBECombEta(),weight);
	histos[25]->Fill(elec->Phi(),weight);
	histos[26]->Fill(elec->ETCone30()/elec->Pt(),weight);
	histos[27]->Fill(TMath::Abs(track->Vtx_D0()/track->Vtx_D0_Err()),weight);
      }

      if (_debug)
      {
	std::cout << "Found good electrons; now looking at muons" << std::endl;
      }

      int nMu = _muons->GetEntries();
      for (int i = 0; i != nMu; ++i)
      {
	int mCut = 0;
	mCut = mRec->addEvent(mCut,weight,"Initial number of muons");

	MTree::Muon* mu = (MTree::Muon*)_muons->At(i);
	histos[39]->Fill(mu->Pt(),weight);
	histos[40]->Fill(mu->Eta(),weight);
	histos[41]->Fill(mu->Phi(),weight);
	histos[42]->Fill(mu->ETCone30()/mu->Pt(),weight);
	MTree::Track* track = mu->GetTrack();
	if (track != 0)
	{
	  histos[43]->Fill(TMath::Abs(track->Vtx_D0()/track->Vtx_D0_Err()),weight);
	}
	histos[44]->Fill(mu->MatchChi2(),weight);

	if (!(mu->IsCombinedMuon())) continue;
	mCut = mRec->addEvent(mCut,weight,"Combined Muon");
	if (!(mu->Author() == 6)) continue;
	mCut = mRec->addEvent(mCut,weight,"STACO muon");
	if (mu->Pt() < 15) continue;
	mCut = mRec->addEvent(mCut,weight,"Pt > 15 GeV");
	if (TMath::Abs(mu->Eta()) > 2.5) continue;
	mCut = mRec->addEvent(mCut,weight,"|Eta| < 2.5");
	if (mu->MatchChi2() > 100) continue;
	mCut = mRec->addEvent(mCut,weight,"MatchChi2 < 100");
	if (track == 0) continue;
	if (TMath::Abs(track->Vtx_D0()/track->Vtx_D0_Err()) > 10) continue;
	mCut = mRec->addEvent(mCut,weight,"d0 significance < 10");
	if (mu->ETCone30()/mu->Pt() > 0.1) continue;
	mCut = mRec->addEvent(mCut,weight,"ETCone30/Pt < 0.1");

	int nTr = _tracks->GetEntries();
	int overlapTracks = 0;
	for (int j = 0; j != nTr; ++j)
	{
	  MTree::Track* arrow = (MTree::Track*)_tracks->At(j);
	  if (arrow == track) continue;
	  if (arrow->Pt() < 1) continue;
	  if (arrow->NrOfPixelHits() < 2) continue;
	  if (arrow->NrOfSCTHits() < 6) continue;
	  if (mu->DeltaR(*arrow) > 0.3) continue;

	  ++overlapTracks;
	}
	if (overlapTracks > 3) continue;
	mCut = mRec->addEvent(mCut,weight,"number of nearby tracks");

	leptons.push_back(mu);
	histos[28]->Fill(mu->Pt(),weight);
	histos[29]->Fill(mu->Eta(),weight);
	histos[30]->Fill(mu->Phi(),weight);
	histos[31]->Fill(mu->ETCone30()/mu->Pt(),weight);
	histos[32]->Fill(TMath::Abs(track->Vtx_D0()/track->Vtx_D0_Err()),weight);
	histos[33]->Fill(mu->MatchChi2(),weight);
      }

      //Lepton overlap removal
      if (_debug)
      {
	std::cout << "Lepton overlap removal" << std::endl;
      }
      for (int j = 0; j != (int)leptons.size(); ++j)
      {
	for (int k = j + 1; k != (int)leptons.size(); ++k)
	{
	  if ((leptons[j]->LeptonPdgID() != leptons[k]->LeptonPdgID()) && (TMath::Abs(leptons[j]->DeltaR(*leptons[k])) < 0.1))
	  {
	    if (leptons[j]->LeptonPdgID() == 11)
	    {
	      leptons.erase(leptons.begin() + j);
	      --j;
	      --k;
	    }
	    else
	    {
	      leptons.erase(leptons.begin() + k);
	      --k;
	    }
	  }
	  else if ((leptons[j]->LeptonPdgID() == leptons[k]->LeptonPdgID()) && (leptons[j]->LeptonPdgID() == 11) && (TMath::Abs(leptons[j]->DeltaR(*leptons[k])) < 0.1))
	  {
	    if (leptons[j]->Pt() < leptons[k]->Pt())
	    {
	      leptons.erase(leptons.begin() + j);
	      --j;
	      --k;
	    }
	    else
	    {
	      leptons.erase(leptons.begin() + k);
	      --k;
	    }
	  }
	}
      }

      if ((int)leptons.size() != 2) continue;
      eventCut = eventRec->addEvent(eventCut,weight,"Exactly 2 good leptons");

      eventchan.clear();
      eventchan = findWWChannels(leptons);
      eventCut = eventRec->addEvent(eventCut,weight,"2 lepton events",eventchan);

      if (_debug)
      {
	std::cout << "Applying event selections" << std::endl;
      }
      //Make vector of jets and set nice pointers.
      int njets = _jets->GetEntriesFast();
      int index1 = -10;
      int index2 = -10;
      double diff1 = 100;
      double diff2 = 100;
      std::vector<MTree::Jet*> jets;
      for (int j = 0; j != njets; ++j)
      {
	MTree::Jet* jet = (MTree::Jet*)_jets->At(j);
	if (jet->Pt() < 20) continue;
	if (TMath::Abs(jet->Eta()) > 3) continue;

	jets.push_back(jet);
      }
      if (_debug)
      {
	std::cout << "made vector of jets" << std::endl;
      }
      for (int j = 0; j != (int)jets.size(); ++j)
      {
	if (leptons[0]->LeptonPdgID() == 11)
	{
	  double temp = TMath::Abs(leptons[0]->DeltaR(*jets[j]));
	  if (temp < diff1)
	  {
	    diff1 = temp;
	    index1 = j;
	  }
	}
      }
      if ((index1 != -10) && (diff1 < 0.3))
      {
	jets.erase(jets.begin() + index1);
      }
      if (_debug)
      {
	std::cout << "removed first electron's jet" << std::endl;
      }
      for (int j = 0; j != (int)jets.size(); ++j)
      {
	if (leptons[1]->LeptonPdgID() == 11)
	{
	  double small = TMath::Abs(leptons[1]->DeltaR(*jets[j]));
	  if (small < diff2)
	  {
	    diff2 = small;
	    index2 = j;
	  }
	}
      }
      if (_debug)
      {
	std::cout << "found second electron's jet" << std::endl;
      }
      if ((index2 != -10) && (diff2 < 0.3))
      {
	jets.erase(jets.begin() + index2);
      }
      if (_debug)
      {
	std::cout << "removed second electron's jet" << std::endl;
      }
      TLorentzVector ll = *leptons[0] + *leptons[1];
      MTree::Track* track1 = leptons[0]->GetTrack();
      MTree::Track* track2 = leptons[1]->GetTrack();

      //Fill beaucoup des plots.
      if (_debug)
      {
	std::cout << "Filling plots" << std::endl;
      }
      if (eventchan[0] == "ee")
      {
        histos[0]->Fill(leptons[0]->DeltaR(*leptons[1]),weight);
	histos[5]->Fill(ll.Pt(),weight);
	histos[8]->Fill(TMath::Abs(track1->Vtx_D0() - track2->Vtx_D0()),weight);
	histos[11]->Fill(TMath::Abs(track1->Vtx_Z0() - track2->Vtx_Z0()),weight);
	histos[14]->Fill(ll.M(),weight);
	histos[17]->Fill((int)jets.size(),weight);
	histos[20]->Fill(_missingET->MET(),weight);
      }
      if (eventchan[0] == "emu")
      {
        histos[1]->Fill(leptons[0]->DeltaR(*leptons[1]),weight);
	double min = 100;
	for (int j = 0; j != (int)jets.size(); ++j)
	{
	  if ((leptons[0]->LeptonPdgID() == 13) && (TMath::Abs(leptons[0]->DeltaR((*jets[j]))) < min))
	  {
	    min = TMath::Abs(leptons[0]->DeltaR((*jets[j])));
	  }
	  else if ((leptons[1]->LeptonPdgID() == 13) && (TMath::Abs(leptons[1]->DeltaR((*jets[j]))) < min))
	  {
	    min = TMath::Abs(leptons[1]->DeltaR((*jets[j])));
	  }
	}
	histos[3]->Fill(min,weight);
	histos[6]->Fill(ll.Pt(),weight);
	histos[9]->Fill(TMath::Abs(track1->Vtx_D0() - track2->Vtx_D0()),weight);
	histos[12]->Fill(TMath::Abs(track1->Vtx_Z0() - track2->Vtx_Z0()),weight);
	histos[15]->Fill(ll.M(),weight);
	histos[18]->Fill((int)jets.size(),weight);
	histos[21]->Fill(_missingET->MET(),weight);
      }
      if (eventchan[0] == "mumu")
      {
        histos[2]->Fill(leptons[0]->DeltaR(*leptons[1]),weight);
	double min1 = 100;
	double min2 = 100;
	for (int j = 0; j != (int)jets.size(); ++j)
	{
	  if (TMath::Abs(leptons[0]->DeltaR(*jets[j])) < min1)
	  {
	    min1 = TMath::Abs(leptons[0]->DeltaR(*jets[j]));
	  }
	  if (TMath::Abs(leptons[1]->DeltaR(*jets[j])) < min2)
	  {
	    min2 = TMath::Abs(leptons[1]->DeltaR(*jets[j]));
	  }
	}
	histos[4]->Fill(min1,weight);
	histos[4]->Fill(min2,weight);
	histos[7]->Fill(ll.Pt(),weight);
	histos[10]->Fill(TMath::Abs(track1->Vtx_D0() - track2->Vtx_D0()),weight);
	histos[13]->Fill(TMath::Abs(track1->Vtx_Z0() - track2->Vtx_Z0()),weight);
	histos[16]->Fill(ll.M(),weight);
	histos[19]->Fill((int)jets.size(),weight);
	histos[22]->Fill(_missingET->MET(),weight);
      }

      if (_debug)
      {
	std::cout << "Checking charge" << std::endl;
      }
      if (leptons[0]->Charge() == leptons[1]->Charge()) continue;
      eventCut = eventRec->addEvent(eventCut,weight,"Oppositely charged leptons",eventchan);
      //      outFile << _eventInfo->EventNumber() << std::endl;

      if (_debug)
      {
	std::cout << "Checking dR(l,l)" << std::endl;
      }
      if (leptons[0]->DeltaR(*leptons[1]) < 0.1) continue;
      eventCut = eventRec->addEvent(eventCut,weight,"DeltaR(ll) > 0.1",eventchan);

      //Lepton-Jet checking will go here, when I sort that out in my head.
      if (_debug)
      {
	std::cout << "Electron-jet overlap removal" << std::endl;
      }
      bool overlap = false;
      for (int j = 0; j != (int)jets.size(); ++j)
      {
	if ((leptons[0]->LeptonPdgID() == 11)  && (TMath::Abs(leptons[0]->DeltaR(*jets[j])) < 0.3))
	{
	  overlap = true;
	}
	if ((leptons[1]->LeptonPdgID() == 11) && (TMath::Abs(leptons[1]->DeltaR(*jets[j])) < 0.3))
	{
	  overlap = true;
	}
      }
      if (overlap) continue;
      eventCut = eventRec->addEvent(eventCut,weight,"DeltaR(e,jet) > 0.3",eventchan);

      if (_debug)
      {
	std::cout << "Dilepton pt cut" << std::endl;
      }
      if (ll.Pt() < 30) continue;
      eventCut = eventRec->addEvent(eventCut,weight,"Dilepton pt > 30",eventchan);

      if (_debug)
      {
	std::cout << "D0 and Z0 cuts" << std::endl;
      }
      if (TMath::Abs(track1->Vtx_D0() - track2->Vtx_D0()) > 0.2) continue;
      eventCut = eventRec->addEvent(eventCut,weight,"Delta D0 < 0.2",eventchan);
      if (TMath::Abs(track1->Vtx_Z0() - track2->Vtx_Z0()) > 0.8) continue;
      eventCut = eventRec->addEvent(eventCut,weight,"Delta Z0 < 0.8",eventchan);

      if (_debug)
      {
	std::cout << "Dilepton mass cuts" << std::endl;
      }
      if ((leptons[0]->LeptonPdgID() == leptons[1]->LeptonPdgID()) && (ll.M() < 15)) continue;
      eventCut = eventRec->addEvent(eventCut,weight,"Same flavor dilepton mass > 15 GeV",eventchan);

      if (TMath::Abs(ll.M() - 91.1876) < 10) continue;
      eventCut = eventRec->addEvent(eventCut,weight,"Z veto",eventchan);

      if (_debug)
      {
	std::cout << "Jet multiplicity" << std::endl;
      }
      if ((int)jets.size() > 0 ) continue;
      eventCut = eventRec->addEvent(eventCut,weight,"Jet multiplicity",eventchan);

      if (_debug)
      {
	std::cout << "MissingET cut" << std::endl;
      }
      if(_missingET->MET() < 30) continue;
      eventCut = eventRec->addEvent(eventCut,weight,"Missing ET > 30 GeV",eventchan);

//       for (int n = 0; n != 40; ++n)
//       {
// 	if (TMath::Abs(track1->Vtx_Z0() - track2->Vtx_Z0()) <= n*0.05)
// 	{
// 	  z0s[n] += _eventInfo->EventWeight();
// 	}
//       }

      if (_debug)
      {
	std::cout << "Reached end of event loop" << std::endl;
      }
    }//closes event loop
  }//closes loop over all samples

//   for (int n = 0; n != 40; ++n)
//   {
//     std::cout << "Event weight with dZ0 up to "<< n*0.05 << ": " <<z0s[n] << std::endl; 
//   }

  eRec->print(sample + "_electrons");
  mRec->print(sample + "_muons");
  eventRec->print(sample + "_WW");
  outFile.close();
}

/////////////////////////////////////////////////////////////////////////////////////////

void DataAnalysis::wwFakeEst(std::string sample, TH1D** histos, TH2D** histos2)
{
  //Set up record keeping, good run list, anything else critical . . .
  if (_debug)
  {
    std::cout << "Calling wwFakeEst()" << std::endl;
  }

  RecordClass* elrecord = new RecordClass("elecs");
  RecordClass* evrecord = new RecordClass("events");
  RecordClass* murecord = new RecordClass("muons");

  std::vector<std::string> channels;
  channels.push_back("ee");
  channels.push_back("em");
  channels.push_back("me");
  channels.push_back("mm");
  evrecord->setChannels(channels);

  ofstream outfile;
  std::string tag = "event-summaries/" + sample + "_Dileptons.txt";
  outfile.open(tag.c_str());

  ofstream summary;
  std::string tag2 = "event-summaries/" + sample + "_FullInfo.txt";
  summary.open(tag2.c_str());

  ofstream byline;
  std::string headline = "event-summaries/" + sample + "_EventSum.txt";
  byline.open(headline.c_str());

  ofstream news;
  std::string screamer = "event-summaries/" + sample + "_aftermass.txt";
  news.open(screamer.c_str());

  ofstream special;
  std::string title = "event-summaries/" + sample + "_muonevents.txt";
  special.open(title.c_str());

  ofstream jfstream;
  std::string place = "event-summaries/" + sample + "_jf17events.txt";
  jfstream.open(place.c_str());

  ofstream zstream;
  std::string holder = "event-summaries/" + sample + "_zevents.txt";
  zstream.open(holder.c_str());

  bool isMC = true;
  if ((sample.find("data") != string::npos) || (sample.find("MwB") != string::npos) || (sample.find("L1Calo") != string::npos) || (sample.find("159224") != string::npos))
  {
    isMC = false;
  }

  if (_debug)
  {
    std::cout << "Setting up good run list" << std::endl;
  }
  TString list;
  list = "/home/swalch/macro-cosmos/HiggsAnalysis/GoodRunLists/GRL_7TeV_init.xml";
  Root::TGoodRunsListReader foo(list);
  foo.Interpret();
  Root::TGoodRunsList grl1 = foo.GetMergedGoodRunsList();

  //Loop over sub-samples
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
      std::cout << "Looking at weight and cross-section" << std::endl;
    }
    double sampleWeight = 0.0;
    double sampleXSec= 0.0;
    if (isMC)
    {
      sampleWeight = GetSampleWeight(sampleName);
      sampleXSec = GetCrossSection(sampleName);
      evrecord->setWeightSum(sampleWeight);
    }

    if (_debug)
    {
      std::cout << "Entering event loop" << std::endl;
    }
    int eventNr = 0;
    double weight = 1.0;
    while(chain->GetEntry(eventNr++))
    {
      if (eventNr % 10000 == 0)
      {
	std::cout << "Analyzed " << eventNr << " events." << std::endl;
      }
      int goodPVs = 0;
      int nPVs = _primaryVertices->GetEntries();
      for (int i = 0; i != nPVs; ++i)
      {
	MTree::PrimaryVertex* vertex = (MTree::PrimaryVertex*)_primaryVertices->At(i);
	if (vertex->NTracks() < 3) continue;
	if(TMath::Abs(vertex->Z()) > 150) continue;

	++goodPVs;
      }

      if (isMC)
      {
	weight = sampleXSec/sampleWeight*(_eventInfo->EventWeight());
	if (sample.find("shellZ") != string::npos || sample.find("singleW") != string::npos)
	{
	  std::cout << "reweighting by vertex" << std::endl;
	  if (goodPVs == 1)
	  {
	    weight = weight*4.07978;
	  }
	  if (goodPVs == 2)
	  {
	    weight = weight*1.06417;
	  }
	  if (goodPVs == 3)
	  {
	    weight = weight*0.334481;
	  }
	  if (goodPVs == 4)
	  {
	    weight = weight*0.117482;
	  }
	  if (goodPVs == 5)
	  {
	    weight = weight*0.0474461;
	  }
	  if (goodPVs == 6)
	  {
	    weight = weight*0.0157642;
	  }
	  if (goodPVs == 7)
	  {
	    weight = weight*0.00545309;
	  }
	  if (goodPVs == 8)
	  {
	    weight = weight*0.00709313;
	  }
	  if (goodPVs > 8)
	  {
	    weight = weight*0;
	  }
	}
	if (sample.find("JF17") != string::npos)
	{
	  if(IsJF17WZEvent())
	  {
            continue;
	    std::cout << "rejecting W/Z/t event!"<< std::endl;
	  }
	}
      }

      int cutNr = 0;
      cutNr = evrecord->addEvent(cutNr,weight,"Initial number of events");

      bool isEvent = false;
      if ((_eventInfo->EventNumber() == 5294971) || (_eventInfo->EventNumber() == 5168462))
      {
        isEvent = true;
	_eventInfo->PrintData(byline);
      }
      else continue;

      //Check Good Run List
      if (_debug)
      {
	std::cout << "Checking good run list" << std::endl;
      }
      bool passed = true;
      if (!isMC && (sample.compare("topmix") != 0))
      {
	int runNr = _eventInfo->RunNumber();
	int lumiBlock = _eventInfo->LumiBlock();
	passed = grl1.HasRunLumiBlock(runNr,lumiBlock);
      }
      if (!passed) continue;
      cutNr = evrecord->addEvent(cutNr,weight,"Passed good run list");
      if (isEvent)
      {
	byline << "passed GRL" << std::endl;
      }

      //Trigger selection
      if (_debug)
      {
	std::cout << "L1 Triggers" << std::endl;
      }
      bool passedEM = false;
      bool passedMU = false;
      std::string etrigger = "L1_EM10";
      std::string mtrigger = "L1_MU10";
      if (_trigger->PassedL1Trigger(etrigger))
      {
	passedEM = true;
      }
      if (_trigger->PassedL1Trigger(mtrigger))
      {
	passedMU = true;
      }
      if (!passedEM && !passedMU) continue;
      cutNr = evrecord->addEvent(cutNr,weight,"Passed an L1 trigger");
      if (isEvent)
      {
	byline << "passed trigger" << std::endl;
      }

      channels.clear();
      if (passedEM)
      {
	channels.push_back("ee");
      }
      if (passedEM || passedMU)
      {
	channels.push_back("em");
	channels.push_back("me");
      }
      if (passedMU)
      {
	channels.push_back("mm");
      }
      cutNr = evrecord->addEvent(cutNr,weight,"Passed an L1 trigger", channels);

      //Primary Vertex requirements
      if (_debug)
      {
	std::cout << "Checking PV" << std::endl;
      }
      bool goodPV = false;
      int npvs = _primaryVertices->GetEntries();
      for (int i = 0; i != npvs; ++i)
      {
	MTree::PrimaryVertex* vertex = (MTree::PrimaryVertex*)_primaryVertices->At(i);
	if (vertex->NTracks() < 3) continue;
	if(TMath::Abs(vertex->Z()) > 150) continue;

	goodPV = true;
      }
      if (!goodPV) continue;
      cutNr = evrecord->addEvent(cutNr,weight,"Passed PV cut",channels);
      if (isEvent)
      {
	byline << "passed PV" << std::endl;
      }

      //Jet cleaning cut
      if (_debug)
      {
	std::cout << "Checking for bad jets" << std::endl;
      }
      bool fail = false;
      int nJets = _jets->GetEntries();
      for (int i = 0; i != nJets; ++i)
      {
	MTree::Jet* jet = (MTree::Jet*)_jets->At(i);
	bool badJet = false;
	bool ptJet = false;

	if ((jet->N90() <= 5) && (jet->HECFrac() > 0.8))
	{
	  badJet = true;
	}
	if ((TMath::Abs(jet->JetQuality()) > 0.8) && (jet->JetEMFraction() > 0.95))
	{
	  badJet = true;
	}
	if (TMath::Abs(jet->JetTime()) >= 50)
	{
	  badJet = true;
	}
	if (jet->GetRawPt() > 10)
	{
	  ptJet = true;
	}
	if (badJet && ptJet)
	{
	  fail = true;
	}

      }
      if (fail) continue;
      cutNr = evrecord->addEvent(cutNr,weight,"Jet Cleaning cuts",channels);
      if (isEvent)
      {
	byline << "passed Jet cleaning" << std::endl;
      }

      //Select leptons; make sure there are two with opposite sign.
      //Loop over electrons and make vectors
      if (_debug)
      {
	std::cout << "Looping over electrons" << std::endl;
      }
      std::vector<MTree::Electron*> looseEs;
      std::vector<MTree::Electron*> mediumEs;
      int nEl = _electrons->GetEntries();
      for (int i = 0; i != nEl; ++i)
      {
	MTree::Electron* electron = (MTree::Electron*)_electrons->At(i);
	int cutE = 0;
	cutE = elrecord->addEvent(cutE,weight,"Initial number of electrons");

	if (electron->ClusterPt() < 10) continue;
	cutE = elrecord->addEvent(cutE,weight,"Pt > 10 GeV");
	if (isEvent)
	{
	  byline << "elec pt: " << electron->ClusterPt() << std::endl;
	}
	if (TMath::Abs(electron->ClusterEta()) > 2.47) continue;
	if ((TMath::Abs(electron->ClusterEta()) > 1.37) && (TMath::Abs(electron->ClusterEta()) < 1.52)) continue;
	cutE = elrecord->addEvent(cutE,weight,"|Eta| < 2.47 and not in crack region");
	if (isEvent)
	{
	  byline << "elec eta: " << electron->ClusterEta() << std::endl;
	}
	if ((electron->Author() != 1) && (electron->Author() != 3)) continue;
	cutE = elrecord->addEvent(cutE,weight,"Author = 1 or 3");
	if (isEvent)
	{
	  byline << "elec author: " << electron->Author() << std::endl;
	}
	int run;
	if (isMC)
	{
	  run = 159224;
	}
	else
	{
	  run = _eventInfo->RunNumber();
	}
	if ((egammaOQ::checkOQClusterElectron(run,electron->ClusterEta(),electron->ClusterPhi()) == 3)) continue;
	cutE = elrecord->addEvent(cutE,weight,"OTX Fiducial cut");
	if (isEvent)
	{
	  byline << "elec OTX: " << egammaOQ::checkOQClusterElectron(_eventInfo->RunNumber(),electron->ClusterEta(),electron->ClusterPhi()) << std::endl;
	}
	if (!(electron->IsLoose())) continue;
	cutE = elrecord->addEvent(cutE,weight,"Loose electrons");
	if (isEvent)
	{
	  byline << "elec is loose " << std::endl;
	  electron->PrintData(byline);
	}
	histos2[0]->Fill(electron->ClusterPt(),electron->ClusterEta(),weight);

	if (((electron->IsEM() & 190824287) == 0) || ((!(electron->ExpectHitInBLayer())) && ((electron->IsEM() & 190824285) == 0)))
	{
	if (isEvent)
	{
	  byline << "elec is robust tight" << std::endl;
	}
	  histos2[1]->Fill(electron->ClusterPt(),electron->ClusterEta(),weight);
	  if (electron->ClusterPt() > 20)
	  {
	    cutE = elrecord->addEvent(cutE,weight,"Robust tight, high pt electron");
	    mediumEs.push_back(electron);
	  }
	  else
	  {
	    looseEs.push_back(electron);
	  }
	  histos[0]->Fill(electron->ClusterPt(),weight);
	  histos[2]->Fill(electron->ClusterEta(),weight);
	}
	else
	{
	  looseEs.push_back(electron);
	  histos[1]->Fill(electron->ClusterPt(),weight);
	  histos[3]->Fill(electron->ClusterEta(),weight);
	}
      }

      if (_debug)
      {
	std::cout << "Looping over muons" << std::endl;
      }
      int nMu = _muons->GetEntries();
      std::vector<MTree::Muon*> looseMs;
      std::vector<MTree::Muon*> mediumMs;
      for (int i = 0; i != nMu; ++i)
      {
	MTree::Muon* muon = (MTree::Muon*)_muons->At(i);
	MTree::Track* idtrack = muon->GetTrack();
	MTree::Track* mstrack = muon->GetStandAloneTrack();
	if ((idtrack == 0) || (mstrack == 0)) continue;

	int cutM = 0;
	cutM = murecord->addEvent(cutM,weight,"Initial number of muons");

	if (muon->Pt() < 10) continue;
	cutM = murecord->addEvent(cutM,weight,"Pt > 10 GeV");
	if (isEvent)
	{
	  byline << "muon pt " << muon->Pt() << std::endl;
	}
	if (TMath::Abs(muon->Eta()) > 2.4) continue;
	cutM = murecord->addEvent(cutM,weight,"|Eta| < 2.4");
	if (isEvent)
	{
	  byline << "muon eta " << muon->Eta() << std::endl;
	}
	if (!(muon->IsCombinedMuon())) continue;
	if (muon->Author() != 6) continue;
	cutM = murecord->addEvent(cutM,weight,"STACO combined muons");
	if (isEvent)
	{
	  byline << "staco comb " << std::endl;
	}
	if (TMath::Abs(idtrack->Vtx_Z0()) > 10) continue;
	cutM = murecord->addEvent(cutM,weight,"|z0| < 10 mm");
	if (isEvent)
	{
	  byline << "muon Z0 " << std::endl;
	}
	if (mstrack->Pt() < 10) continue;
	cutM = murecord->addEvent(cutM,weight,"MS track pt > 10 GeV");

	if (TMath::Abs(idtrack->Pt() - mstrack->Pt()) > 15) continue;
	cutM = murecord->addEvent(cutM,weight,"delta track pt < 15 GeV");
	if (isEvent)
	{
	  byline << "ms track cuts " << std::endl;
	  muon->PrintData(byline);
	}
	histos2[2]->Fill(muon->Pt(),muon->Eta(),weight);

	if (muon->PTCone40()/muon->Pt() < 0.2)
	{
	if (isEvent)
	{
	  byline << "muon is isolated " << std::endl;
	}
	  if (muon->Pt() > 15)
	  {
	    mediumMs.push_back(muon);
	    cutM = murecord->addEvent(cutM,weight,"Isolated muon");
	  }
	  else
	  {
	    looseMs.push_back(muon);
	  }
	  histos[4]->Fill(muon->Pt(),weight);
	  histos[6]->Fill(muon->Eta(),weight);
	  histos[8]->Fill(muon->PTCone40()/muon->Pt(),weight);
	  histos[162]->Fill(muon->PTCone30()/muon->Pt(),weight);
	  histos[164]->Fill(muon->PTCone40(),weight);
	  histos[166]->Fill(muon->PTCone30(),weight);
	  histos[168]->Fill(muon->ETCone40()/muon->Pt(),weight);
	  histos[170]->Fill(muon->ETCone30()/muon->Pt(),weight);
	  histos[172]->Fill(muon->ETCone40(),weight);
	  histos[174]->Fill(muon->ETCone30(),weight);
	  histos2[3]->Fill(muon->Pt(),muon->Eta(),weight);
	}
	else
	{
	  looseMs.push_back(muon);
	  histos[5]->Fill(muon->Pt(),weight);
	  histos[7]->Fill(muon->Eta(),weight);
	  histos[9]->Fill(muon->PTCone40()/muon->Pt(),weight);
	  histos[163]->Fill(muon->PTCone30()/muon->Pt(),weight);
	  histos[165]->Fill(muon->PTCone40(),weight);
	  histos[167]->Fill(muon->PTCone30(),weight);
	  histos[169]->Fill(muon->ETCone40()/muon->Pt(),weight);
	  histos[171]->Fill(muon->ETCone30()/muon->Pt(),weight);
	  histos[173]->Fill(muon->ETCone40(),weight);
	  histos[175]->Fill(muon->ETCone30(),weight);
	}
      }

      //Overlap removal between muons and electrons
      //tight to tight
      if (_debug)
      {
	std::cout << "Removing overlapping e/mu leptons" << std::endl;
      }
      for (int i = 0; i != (int)mediumMs.size(); ++i)
      {
	for (int j = 0; j != (int)mediumEs.size(); ++j)
	{
	  if (getDeltaR(mediumMs[i],mediumEs[j]) < 0.1)
	  {
	    mediumEs.erase(mediumEs.begin() + j);
	    --j;
	  }
	}
      }

      //tight to loose
      if (_debug)
      {
	std::cout << "tight to loose em" << std::endl;
      }
      for (int i = 0; i != (int)looseMs.size(); ++i)
      {
	for (int j = 0; j != (int)mediumEs.size(); ++j)
	{
	  if (getDeltaR(looseMs[i],mediumEs[j]) < 0.1)
	  {
	    mediumEs.erase(mediumEs.begin() + j);
	    --j;
	  }
	}
      }

      //loose to tight
      if (_debug)
      {
	std::cout << "loose to tight em" << std::endl;
      }
      for (int i = 0; i != (int)mediumMs.size(); ++i)
      {
	for (int j = 0; j != (int)looseEs.size(); ++j)
	{
	  if (getDeltaR(mediumMs[i],looseEs[j]) < 0.1)
	  {
	    looseEs.erase(looseEs.begin() + j);
	    --j;
	  }
	}
      }

      //loose to loose
      if (_debug)
      {
	std::cout << "loose to loose em" << std::endl;
      }
      for (int i = 0; i != (int)looseMs.size(); ++i)
      {
	for (int j = 0; j != (int)looseEs.size(); ++j)
	{
	  if (getDeltaR(looseMs[i],looseEs[j]) < 0.1)
	  {
	    looseEs.erase(looseEs.begin() + j);
	    --j;
	  }
	}
      }

      // . . . okay, so there has to be a better way to do this.  I'll think about it.
      //On to electron to electron overlap removal.
      //within tight
      if (_debug)
      {
	std::cout << "Removing overlapping ee's" << std::endl;
      }
      for (int i = 0; i != (int)mediumEs.size(); ++i)
      {
	for (int j = i + 1; j != (int)mediumEs.size(); ++j)
	{
	  if (getDeltaR(mediumEs[i],mediumEs[j]) < 0.1)
	  {
	    if (mediumEs[i]->Pt() < mediumEs[j]->Pt())
	    {
	      swap(mediumEs[i],mediumEs[j]);
            }
	    mediumEs.erase(mediumEs.begin() + j);
	    --j;
	  }
	}
      }

      //within loose
      if (_debug)
      {
	std::cout << " . . . loose to loose . . ." << std::endl;
      }

      for (int i = 0; i != (int)looseEs.size(); ++i)
      {
	for (int j = i + 1; j != (int)looseEs.size(); ++j)
	{
	  if (getDeltaR(looseEs[i],looseEs[j]) < 0.1)
	  {
	    if (looseEs[i]->Pt() < looseEs[j]->Pt())
	    {
	      swap(looseEs[i],looseEs[j]);
            }
	    looseEs.erase(looseEs.begin() + j);
	    --j;
	  }
	}
      }

      //between the two
      if (_debug)
      {
	std::cout << " . . . tight to loose." << std::endl;
      }
      for (int i = 0; i != (int)mediumEs.size(); ++i)
      {
	for (int j = 0; j != (int)looseEs.size(); ++j)
	{
	  if ((i >= (int)mediumEs.size()) || (i < 0)) continue;
	  if (getDeltaR(mediumEs[i],looseEs[j]) < 0.1)
	  {
	    if (mediumEs[i]->Pt() > looseEs[j]->Pt())
	    {
	      looseEs.erase(looseEs.begin() + j);
	      --j;
	    }
	    else
	    {
	      mediumEs.erase(mediumEs.begin() + i);
	      --i;
	    }
	  }
	}
      }

      //Reject events with more than two leptons
      if (_debug)
      {
	std::cout << "Rejecting events that don't have exactly two leptons" << std::endl;
      }
      if (((int)mediumEs.size() + (int)looseEs.size() + (int)mediumMs.size() + (int)looseMs.size()) != 2) continue;
      if (isEvent)
      {
	byline << "had two leptons" << std::endl;
      }

      if (((int)mediumEs.size() + (int)mediumMs.size()) == 0) continue;
      if (isEvent)
      {
	byline << "and one was tight" << std::endl;
      }

      //Now figure out what channel this event belongs in. This is not as trivial as it should be.
      channels.clear();
      std::vector<MTree::Lepton*> leptons;
      if (((int)mediumEs.size() + (int)looseEs.size()) == 2)
      {
	channels.push_back("ee");
      }
      if (((int)mediumMs.size() + (int)looseMs.size()) == 2)
      {
	channels.push_back("mm");
      }
      if ((((int)mediumEs.size() + (int)looseMs.size()) == 2) && ((int)mediumEs.size() != 2))
      {
	channels.push_back("em");
      }
      if ((((int)mediumMs.size() + (int)looseEs.size()) == 2) && ((int)mediumMs.size() != 2))
      {
	channels.push_back("me");
      }
      if (((int)mediumMs.size() + (int)mediumEs.size() == 2) && ((int)mediumMs.size() == 1) && ((int)mediumEs.size() == 1))
      {
	if (mediumMs[0]->Pt() > mediumEs[0]->ClusterPt())
	{
	  channels.push_back("me");
	}
	else
	{
	  channels.push_back("em");
	}
      }
      if ((int)channels.size() != 1)
      {
	std::cout << "Something went wrong with the channel selection!" << std::endl;
	for (int j = 0; j != (int)channels.size(); ++j)
	{
	  std::cout << channels[j] << std::endl;
	}
	continue;
      }
      for(int i = 0; i != (int)mediumMs.size(); ++i)
      {
	leptons.push_back(mediumMs[i]);
      }
      for (int i = 0; i != (int)mediumEs.size(); ++i)
      {
	leptons.push_back(mediumEs[i]);
      }
      for(int i = 0; i != (int)looseMs.size(); ++i)
      {
	leptons.push_back(looseMs[i]);
      }
      for (int i = 0; i != (int)looseEs.size(); ++i)
      {
	leptons.push_back(looseEs[i]);
      }

      cutNr = evrecord->addEvent(cutNr,weight,"Two leptons, one of them tight",channels);

      //Check sign of leptons
      if (_debug)
      {
	std::cout << "Checking sign of leptons" << std::endl;
      }
      if (leptons[0]->Charge() == leptons[1]->Charge()) continue;
      cutNr = evrecord->addEvent(cutNr,weight,"2 opposite sign leptons",channels);
      if (isEvent)
      {
	byline << "leptons have opposite sign" << std::endl;
      }

      //Apply dilepton mass cuts
      if (_debug)
      {
	std::cout << "Making dilepton mass cuts" << std::endl;
      }
      TLorentzVector v1;
      if (leptons[0]->LeptonPdgID() == 11)
      {
	MTree::Electron* elec = (MTree::Electron*)leptons[0];
	double Eclus1 = elec->ClusterE();
        double theta1 = elec->Theta();
        double phi1 = elec->Phi();
	v1.SetPxPyPzE(Eclus1*TMath::Sin(theta1)*TMath::Cos(phi1), Eclus1*TMath::Sin(theta1)*TMath::Sin(phi1),Eclus1*TMath::Cos(theta1),Eclus1);
      }
      else
      {
	v1.SetPxPyPzE(leptons[0]->Px(),leptons[0]->Py(),leptons[0]->Pz(),leptons[0]->E());
      }
      TLorentzVector v2;
      if (leptons[1]->LeptonPdgID() == 11)
      {
	MTree::Electron* elec = (MTree::Electron*)leptons[1];
	double Eclus2 = elec->ClusterE();
        double theta2 = elec->Theta();
        double phi2 = elec->Phi();
        v2.SetPxPyPzE(Eclus2*TMath::Sin(theta2)*TMath::Cos(phi2), Eclus2*TMath::Sin(theta2)*TMath::Sin(phi2),Eclus2*TMath::Cos(theta2),Eclus2);
      }
      else
      {
	v2.SetPxPyPzE(leptons[1]->Px(),leptons[1]->Py(),leptons[1]->Pz(),leptons[1]->E());
      }

      //Pause a moment to fill lots of dilepton plots before cutting on anything.
      if ((channels[0].compare("ee") == 0) || (channels[0].compare("em") == 0))
      {
	_eventInfo->PrintData(jfstream);
	((MTree::Electron*)leptons[0])->PrintData(jfstream);
	if (leptons[1]->LeptonPdgID() == 11)
	{
	  ((MTree::Electron*)leptons[1])->PrintData(jfstream);
	}
	else
	{
	  ((MTree::Muon*)leptons[1])->PrintData(jfstream);
	}
	jfstream << "MET: " << getMET() << std::endl;
	jfstream << "Mass: " << (v1+v2).M() << std::endl << std::endl;
      }
      if (channels[0].compare("ee") == 0)
      {
	histos[58]->Fill(getMET(),weight);
	histos[59]->Fill(TMath::Abs(leptons[0]->DeltaPhi(*leptons[1])),weight);
	histos[60]->Fill((v1+v2).M(),weight);
	histos[61]->Fill(sqrt(2*(v1+v2).Pt()*_missingET->Pt()*(1-TMath::Cos((v1+v2).DeltaPhi(*_missingET)))),weight);
	histos[64]->Fill(((MTree::Electron*)leptons[0])->ClusterPt(),weight);
	histos[67]->Fill(((MTree::Electron*)leptons[1])->ClusterPt(),weight);
	histos[70]->Fill(((MTree::Electron*)leptons[0])->ClusterEta(),weight);
	histos[73]->Fill(((MTree::Electron*)leptons[1])->ClusterEta(),weight);
	histos[76]->Fill(((MTree::Electron*)leptons[0])->ETCone30()/((MTree::Electron*)leptons[0])->ClusterPt(),weight);
	histos[79]->Fill(((MTree::Electron*)leptons[1])->ETCone30()/((MTree::Electron*)leptons[1])->Pt(),weight);
      }
      else if (channels[0].compare("em") == 0)
      {
	histos[84]->Fill(getMET(),weight);
	histos[85]->Fill(TMath::Abs(leptons[0]->DeltaPhi(*leptons[1])),weight);
	histos[86]->Fill((v1+v2).M(),weight);
	histos[87]->Fill(sqrt(2*(v1+v2).Pt()*_missingET->Pt()*(1-TMath::Cos((v1+v2).DeltaPhi(*_missingET)))),weight);
	histos[90]->Fill(((MTree::Electron*)leptons[0])->ClusterPt(),weight);
	histos[93]->Fill(leptons[1]->Pt(),weight);
	histos[96]->Fill(((MTree::Electron*)leptons[0])->ClusterEta(),weight);
	histos[99]->Fill(leptons[1]->Eta(),weight);
	histos[102]->Fill(((MTree::Electron*)leptons[0])->ETCone30()/((MTree::Electron*)leptons[0])->ClusterPt(),weight);
	histos[105]->Fill(((MTree::Muon*)leptons[1])->PTCone40()/leptons[1]->Pt(),weight);
      }
      else if (channels[0].compare("me") == 0)
      {
	histos[110]->Fill(getMET(),weight);
	histos[111]->Fill(TMath::Abs(leptons[0]->DeltaPhi(*leptons[1])),weight);
	histos[112]->Fill((v1+v2).M(),weight);
	histos[113]->Fill(sqrt(2*(v1+v2).Pt()*_missingET->Pt()*(1-TMath::Cos((v1+v2).DeltaPhi(*_missingET)))),weight);
	histos[116]->Fill(leptons[0]->Pt(),weight);
	histos[119]->Fill(((MTree::Electron*)leptons[1])->ClusterPt(),weight);
	histos[122]->Fill(leptons[0]->Eta(),weight);
	histos[125]->Fill(((MTree::Electron*)leptons[1])->ClusterEta(),weight);
	histos[128]->Fill(((MTree::Muon*)leptons[0])->PTCone40()/leptons[0]->Pt(),weight);
	histos[131]->Fill(((MTree::Electron*)leptons[1])->ETCone30()/((MTree::Electron*)leptons[1])->ClusterPt(),weight);
      }
      else if (channels[0].compare("mm") == 0)
      {
	histos[136]->Fill(getMET(),weight);
	histos[137]->Fill(TMath::Abs(leptons[0]->DeltaPhi(*leptons[1])),weight);
	histos[138]->Fill((v1+v2).M(),weight);
	histos[139]->Fill(sqrt(2*(v1+v2).Pt()*_missingET->Pt()*(1-TMath::Cos((v1+v2).DeltaPhi(*_missingET)))),weight);
	histos[142]->Fill(leptons[0]->Pt(),weight);
	histos[145]->Fill(leptons[1]->Pt(),weight);
	histos[148]->Fill(leptons[0]->Eta(),weight);
	histos[151]->Fill(leptons[1]->Eta(),weight);
	histos[154]->Fill(((MTree::Muon*)leptons[0])->PTCone40()/leptons[0]->Pt(),weight);
	histos[157]->Fill(((MTree::Muon*)leptons[1])->PTCone40()/leptons[0]->Pt(),weight);
      }

      if ((leptons[0]->LeptonPdgID() == leptons[1]->LeptonPdgID()) && ((v1 + v2).M() < 15)) continue;
      cutNr = evrecord->addEvent(cutNr,weight,"Mll > 15",channels);
      if (isEvent)
      {
	byline << "mll > 15" << std::endl;
      }

      if ((leptons[0]->LeptonPdgID() == leptons[1]->LeptonPdgID()) && (TMath::Abs(91.1876 - (v1 + v2).M()) < 10)) continue;
      cutNr = evrecord->addEvent(cutNr,weight,"|mll - mZ| > 10",channels);
      if (isEvent)
      {
	byline << "mll not in z window" << std::endl;
      }
      news << "channel: " << channels[0] << " Event Number: " << _eventInfo->EventNumber() << " Run Number: " << _eventInfo->RunNumber() << std::endl;
      //FIILL SPECIAL MET PLOT HERE, JUST FOR THE NOTE
      if (channels[0].compare("ee") == 0)
      {
	histos[180]->Fill(getMET(),weight);
      }
      else if (channels[0].compare("em") == 0)
      {
	histos[181]->Fill(getMET(),weight);
      }
      else if (channels[0].compare("me") == 0)
      {
	histos[182]->Fill(getMET(),weight);
      }
      else if (channels[0].compare("mm") == 0)
      {
	histos[183]->Fill(getMET(),weight);
      }
      else
      {
	std::cout << "Screaming at the inhumanity of it all!" << std::endl;
      }

      //Cut on missing ET.
      if (_debug)
      {
	std::cout << "Cutting on missingET" << std::endl;
      }
      if (isEvent)
      {
	byline << "met > 25" << std::endl;
	byline << getMET() << std::endl;
	_missingET->PrintData(byline);
      }
      if (getMET() < 25) continue;
      cutNr = evrecord->addEvent(cutNr,weight,"MET > 25",channels);

      //Quickly, fill njets plots . . . 
      std::vector<MTree::Jet*> jets;
      int njets = _jets->GetEntries();
      for (int l = 0; l != njets; ++l)
      {
        MTree::Jet* jet = (MTree::Jet*)_jets->At(l);
        if (jet->Pt() < 20) continue;
        if (TMath::Abs(jet->Eta()) > 2.8) continue;
	TLorentzVector clus1;
	if (leptons[0]->LeptonPdgID() == 11)
	{
	  MTree::Electron* elec = (MTree::Electron*)leptons[0];
	  clus1.SetPtEtaPhiM(elec->ClusterPt(),elec->ClusterEta(),elec->ClusterPhi(),elec->ClusterM());
	}
	else
	{
	  clus1.SetPtEtaPhiM(leptons[0]->Pt(),leptons[0]->Eta(),leptons[0]->Phi(),leptons[0]->M());
	}
	TLorentzVector clus2;
	if (leptons[1]->LeptonPdgID() == 11)
	{
	  MTree::Electron* elec = (MTree::Electron*)leptons[1];
	  clus2.SetPtEtaPhiM(elec->ClusterPt(),elec->ClusterEta(),elec->ClusterPhi(),elec->ClusterM());
	}
	else
	{
	  clus2.SetPtEtaPhiM(leptons[1]->Pt(),leptons[1]->Eta(),leptons[1]->Phi(),leptons[1]->M());
	}

        if ((leptons[0]->LeptonPdgID() == 11) && (TMath::Abs(clus1.DeltaR(*jet)) < 0.3)) continue;
        if ((leptons[1]->LeptonPdgID() == 11) && (TMath::Abs(clus2.DeltaR(*jet)) < 0.3)) continue;
	if (isEvent)
	{
	  jet->PrintData(byline);
	}
	jets.push_back(jet);
      }

      if (channels[0].compare("ee") == 0)
      {
	histos[176]->Fill((int)jets.size(),weight);
      }
      else if (channels[0].compare("em") == 0)
      {
	histos[177]->Fill((int)jets.size(),weight);
      }
      else if (channels[0].compare("me") == 0)
      {
	histos[178]->Fill((int)jets.size(),weight);
      }
      else if (channels[0].compare("mm") == 0)
      {
	histos[179]->Fill((int)jets.size(),weight);
      }
      else
      {
	std::cout << "Screaming at the inhumanity of it all!" << std::endl;
      }

      if (!isMC)
      {
	special << channels[0] << std::endl;
	_eventInfo->PrintData(special);
	if (leptons[0]->LeptonPdgID() == 11)
	{
	  ((MTree::Electron*)leptons[0])->PrintData(special);
	}
	else
	{
	  ((MTree::Muon*)leptons[0])->PrintData(special);
	}
	if (leptons[1]->LeptonPdgID() == 11)
	{
	  ((MTree::Electron*)leptons[1])->PrintData(special);
	}
	else
	{
	  ((MTree::Muon*)leptons[1])->PrintData(special);
	}
	special << "Njets: " << (int)jets.size() << std::endl;
	for (int k = 0; k != (int)jets.size(); ++k)
	{
	  jets[k]->PrintData(special);
	}
      }

      //So I can add the njet veto.
      if ((int)jets.size() > 1) continue;
      cutNr = evrecord->addEvent(cutNr,weight,"Njets < 2",channels);

      //Fill histos, now made very complicated by all the darn channels.
      double leadID;
      double leadpt;
      double leadeta;
      if (leptons[0]->LeptonPdgID() == 11)
      {
	leadID = 11;
	leadpt = ((MTree::Electron*)leptons[0])->ClusterPt();
	leadeta = ((MTree::Electron*)leptons[0])->ClusterEta();
      }
      else
      {
	leadID = 13;
	leadpt = leptons[0]->Pt();
	leadeta = leptons[0]->Eta();
      }
      double subID;
      double subpt;
      double subeta;
      if (leptons[1]->LeptonPdgID() == 11)
      {
	subID = 11;
	subpt = ((MTree::Electron*)leptons[1])->ClusterPt();
	subeta = ((MTree::Electron*)leptons[1])->ClusterEta();
      }
      else
      {
	subID = 13;
	subpt = leptons[1]->Pt();
	subeta = leptons[1]->Eta();
      }

      double effS = getSigEff(leadID, leadpt, leadeta, subID, subpt, subeta);
      double effB = getBackEff(leadID, leadpt, leadeta, subID, subpt, subeta);

      double sigL = -effB/(effS-effB);
      double backL = effS/(effS-effB);

      if (channels[0].compare("ee") == 0)
      {
	histos[10]->Fill(getMET(),weight);
	histos[34]->Fill(getMET(),weight*sigL);
	histos[35]->Fill(getMET(),weight*backL);
	histos[12]->Fill(TMath::Abs(leptons[0]->DeltaPhi(*leptons[1])),weight);
	histos[36]->Fill(TMath::Abs(leptons[0]->DeltaPhi(*leptons[1])),weight*sigL);
        histos[37]->Fill(TMath::Abs(leptons[0]->DeltaPhi(*leptons[1])),weight*backL);
	histos[14]->Fill((v1+v2).M(),weight);
	histos[38]->Fill((v1+v2).M(),weight*sigL);
	histos[39]->Fill((v1+v2).M(),weight*backL);
	histos[62]->Fill(sqrt(2*(v1+v2).Pt()*_missingET->Pt()*(1-TMath::Cos((v1+v2).DeltaPhi(*_missingET)))),weight);
	histos[82]->Fill(sqrt(2*(v1+v2).Pt()*_missingET->Pt()*(1-TMath::Cos((v1+v2).DeltaPhi(*_missingET)))),weight*sigL);
	histos[83]->Fill(sqrt(2*(v1+v2).Pt()*_missingET->Pt()*(1-TMath::Cos((v1+v2).DeltaPhi(*_missingET)))),weight*backL);
	histos[65]->Fill(((MTree::Electron*)leptons[0])->ClusterPt(),weight);
	histos[68]->Fill(((MTree::Electron*)leptons[1])->ClusterPt(),weight);
	histos[71]->Fill(((MTree::Electron*)leptons[0])->ClusterEta(),weight);
	histos[74]->Fill(((MTree::Electron*)leptons[1])->ClusterEta(),weight);
	histos[77]->Fill(((MTree::Electron*)leptons[0])->ETCone30()/((MTree::Electron*)leptons[0])->ClusterPt(),weight);
	histos[80]->Fill(((MTree::Electron*)leptons[1])->ETCone30()/((MTree::Electron*)leptons[1])->Pt(),weight);
      }
      else if (channels[0].compare("em") == 0)
      {
	histos[16]->Fill(getMET(),weight);
	histos[40]->Fill(getMET(),weight*sigL);
	histos[41]->Fill(getMET(),weight*backL);
	histos[18]->Fill(TMath::Abs(leptons[0]->DeltaPhi(*leptons[1])),weight);
	histos[42]->Fill(TMath::Abs(leptons[0]->DeltaPhi(*leptons[1])),weight*sigL);
        histos[43]->Fill(TMath::Abs(leptons[0]->DeltaPhi(*leptons[1])),weight*backL);
	histos[20]->Fill((v1+v2).M(),weight);
	histos[44]->Fill((v1+v2).M(),weight*sigL);
	histos[45]->Fill((v1+v2).M(),weight*backL);
	histos[88]->Fill(sqrt(2*(v1+v2).Pt()*_missingET->Pt()*(1-TMath::Cos((v1+v2).DeltaPhi(*_missingET)))),weight);
	histos[108]->Fill(sqrt(2*(v1+v2).Pt()*_missingET->Pt()*(1-TMath::Cos((v1+v2).DeltaPhi(*_missingET)))),weight*sigL);
	histos[109]->Fill(sqrt(2*(v1+v2).Pt()*_missingET->Pt()*(1-TMath::Cos((v1+v2).DeltaPhi(*_missingET)))),weight*backL);
	histos[91]->Fill(((MTree::Electron*)leptons[0])->ClusterPt(),weight);
	histos[94]->Fill(leptons[1]->Pt(),weight);
	histos[97]->Fill(((MTree::Electron*)leptons[0])->ClusterEta(),weight);
	histos[100]->Fill(leptons[1]->Eta(),weight);
	histos[103]->Fill(((MTree::Electron*)leptons[0])->ETCone30()/((MTree::Electron*)leptons[0])->ClusterPt(),weight);
	histos[106]->Fill(((MTree::Muon*)leptons[1])->PTCone40()/leptons[1]->Pt(),weight);
      }
      else if (channels[0].compare("me") == 0)
      {
	histos[22]->Fill(getMET(),weight);
	histos[46]->Fill(getMET(),weight*sigL);
	histos[47]->Fill(getMET(),weight*backL);
	histos[24]->Fill(TMath::Abs(leptons[0]->DeltaPhi(*leptons[1])),weight);
	histos[48]->Fill(TMath::Abs(leptons[0]->DeltaPhi(*leptons[1])),weight*sigL);
        histos[49]->Fill(TMath::Abs(leptons[0]->DeltaPhi(*leptons[1])),weight*backL);
	histos[26]->Fill((v1+v2).M(),weight);
	histos[50]->Fill((v1+v2).M(),weight*sigL);
	histos[51]->Fill((v1+v2).M(),weight*backL);
	histos[114]->Fill(sqrt(2*(v1+v2).Pt()*_missingET->Pt()*(1-TMath::Cos((v1+v2).DeltaPhi(*_missingET)))),weight);
	histos[134]->Fill(sqrt(2*(v1+v2).Pt()*_missingET->Pt()*(1-TMath::Cos((v1+v2).DeltaPhi(*_missingET)))),weight*sigL);
	histos[135]->Fill(sqrt(2*(v1+v2).Pt()*_missingET->Pt()*(1-TMath::Cos((v1+v2).DeltaPhi(*_missingET)))),weight*backL);
	histos[117]->Fill(leptons[0]->Pt(),weight);
	histos[120]->Fill(((MTree::Electron*)leptons[1])->ClusterPt(),weight);
	histos[123]->Fill(leptons[0]->Eta(),weight);
	histos[126]->Fill(((MTree::Electron*)leptons[1])->ClusterEta(),weight);
	histos[129]->Fill(((MTree::Muon*)leptons[0])->PTCone40()/leptons[0]->Pt(),weight);
	histos[132]->Fill(((MTree::Electron*)leptons[1])->ETCone30()/((MTree::Electron*)leptons[1])->ClusterPt(),weight);
      }
      else if (channels[0].compare("mm") == 0)
      {
	histos[28]->Fill(getMET(),weight);
	histos[52]->Fill(getMET(),weight*sigL);
	histos[53]->Fill(getMET(),weight*backL);
	histos[30]->Fill(TMath::Abs(leptons[0]->DeltaPhi(*leptons[1])),weight);
	histos[54]->Fill(TMath::Abs(leptons[0]->DeltaPhi(*leptons[1])),weight*sigL);
        histos[55]->Fill(TMath::Abs(leptons[0]->DeltaPhi(*leptons[1])),weight*backL);
	histos[32]->Fill((v1+v2).M(),weight);
	histos[56]->Fill((v1+v2).M(),weight*sigL);
	histos[57]->Fill((v1+v2).M(),weight*backL);
	histos[140]->Fill(sqrt(2*(v1+v2).Pt()*_missingET->Pt()*(1-TMath::Cos((v1+v2).DeltaPhi(*_missingET)))),weight);
	histos[160]->Fill(sqrt(2*(v1+v2).Pt()*_missingET->Pt()*(1-TMath::Cos((v1+v2).DeltaPhi(*_missingET)))),weight*sigL);
	histos[161]->Fill(sqrt(2*(v1+v2).Pt()*_missingET->Pt()*(1-TMath::Cos((v1+v2).DeltaPhi(*_missingET)))),weight*backL);
	histos[143]->Fill(leptons[0]->Pt(),weight);
	histos[146]->Fill(leptons[1]->Pt(),weight);
	histos[149]->Fill(leptons[0]->Eta(),weight);
	histos[152]->Fill(leptons[1]->Eta(),weight);
	histos[155]->Fill(((MTree::Muon*)leptons[0])->PTCone40()/leptons[0]->Pt(),weight);
	histos[158]->Fill(((MTree::Muon*)leptons[1])->PTCone40()/leptons[0]->Pt(),weight);
      }
      else
      {
	std::cout << "Ya gots a weeeiiiiird channel here" << std::endl;
	std::cout << "Thissa it: " << channels[0] << std::endl;
      }

      if (!isMC)
      {
        outfile << channels[0] << std::endl;
        outfile << _eventInfo->RunNumber() << std::endl;
        outfile << _eventInfo->LumiBlock() << std::endl;
        outfile << _eventInfo->EventNumber() << std::endl << std::endl;

        summary << channels[0] << std::endl;
        summary << _eventInfo->RunNumber() << std::endl;
        summary << _eventInfo->LumiBlock() << std::endl;
        summary << _eventInfo->EventNumber() << std::endl << std::endl;
	if (leptons[0]->LeptonPdgID() == 11)
	{
	  ((MTree::Electron*)leptons[0])->PrintData(summary);
        }
	else
	{
	  ((MTree::Muon*)leptons[0])->PrintData(summary);
	}
	if (leptons[1]->LeptonPdgID() == 11)
	{
	  ((MTree::Electron*)leptons[1])->PrintData(summary);
	}
	else
	{
	  ((MTree::Muon*)leptons[1])->PrintData(summary);
	}

	for (int l = 0; l != (int)jets.size(); ++l)
	{
	  jets[l]->PrintData(summary);
	}
      }

      //For me, make sure I've got two "tight" leptons, or leading, or what have you.
      int nTight = 0;
      for (int i = 0; i != (int)leptons.size(); ++i)
      {
	if (leptons[i]->LeptonPdgID() == 11)
	{
	  MTree::Electron* electron = (MTree::Electron*)leptons[i];
	  if (((electron->IsEM() & 190824287) == 0) || ((!(electron->ExpectHitInBLayer())) && ((electron->IsEM() & 190824285) == 0)))
	  {
	    ++nTight;
	  }
	}
	else
	{
	  if ((((MTree::Muon*)leptons[i])->PTCone40()/leptons[i]->Pt()) < 0.2)
	  {
	    ++nTight;
	  }
	}
      }
      if (nTight != 2) continue;
      cutNr = evrecord->addEvent(cutNr,weight,"Two tight/leading leptons",channels);

      double sigT = 1/(effS - effB);
      double backT = -1/(effS - effB);

      if (channels[0].compare("ee") == 0)
      {
	histos[11]->Fill(getMET(),weight);
	histos[34]->Fill(getMET(),weight*sigT);
	histos[35]->Fill(getMET(),weight*backT);
	histos[13]->Fill(TMath::Abs(leptons[0]->DeltaPhi(*leptons[1])),weight);
	histos[36]->Fill(TMath::Abs(leptons[0]->DeltaPhi(*leptons[1])),weight*sigT);
	histos[37]->Fill(TMath::Abs(leptons[0]->DeltaPhi(*leptons[1])),weight*backT);
	histos[15]->Fill((v1+v2).M(),weight);
	histos[38]->Fill((v1+v2).M(),weight*sigT);
	histos[39]->Fill((v1+v2).M(),weight*backT);
	histos[63]->Fill(sqrt(2*(v1+v2).Pt()*_missingET->Pt()*(1-TMath::Cos((v1+v2).DeltaPhi(*_missingET)))),weight);
	histos[82]->Fill(sqrt(2*(v1+v2).Pt()*_missingET->Pt()*(1-TMath::Cos((v1+v2).DeltaPhi(*_missingET)))),weight*sigT);
	histos[83]->Fill(sqrt(2*(v1+v2).Pt()*_missingET->Pt()*(1-TMath::Cos((v1+v2).DeltaPhi(*_missingET)))),weight*backT);
	histos[66]->Fill(((MTree::Electron*)leptons[0])->ClusterPt(),weight);
	histos[69]->Fill(((MTree::Electron*)leptons[1])->ClusterPt(),weight);
	histos[72]->Fill(((MTree::Electron*)leptons[0])->ClusterEta(),weight);
	histos[75]->Fill(((MTree::Electron*)leptons[1])->ClusterEta(),weight);
	histos[78]->Fill(((MTree::Electron*)leptons[0])->ETCone30()/((MTree::Electron*)leptons[0])->ClusterPt(),weight);
	histos[81]->Fill(((MTree::Electron*)leptons[1])->ETCone30()/((MTree::Electron*)leptons[1])->ClusterPt(),weight);
      }
      else if (channels[0].compare("em") == 0)
      {
	histos[17]->Fill(getMET(),weight);
	histos[40]->Fill(getMET(),weight*sigT);
	histos[41]->Fill(getMET(),weight*backT);
	histos[19]->Fill(TMath::Abs(leptons[0]->DeltaPhi(*leptons[1])),weight);
	histos[42]->Fill(TMath::Abs(leptons[0]->DeltaPhi(*leptons[1])),weight*sigT);
	histos[43]->Fill(TMath::Abs(leptons[0]->DeltaPhi(*leptons[1])),weight*backT);
	histos[21]->Fill((v1+v2).M(),weight);
	histos[44]->Fill((v1+v2).M(),weight*sigT);
	histos[45]->Fill((v1+v2).M(),weight*backT);
	histos[89]->Fill(sqrt(2*(v1+v2).Pt()*_missingET->Pt()*(1-TMath::Cos((v1+v2).DeltaPhi(*_missingET)))),weight);
	histos[108]->Fill(sqrt(2*(v1+v2).Pt()*_missingET->Pt()*(1-TMath::Cos((v1+v2).DeltaPhi(*_missingET)))),weight*sigT);
	histos[109]->Fill(sqrt(2*(v1+v2).Pt()*_missingET->Pt()*(1-TMath::Cos((v1+v2).DeltaPhi(*_missingET)))),weight*backT);
	histos[92]->Fill(((MTree::Electron*)leptons[0])->ClusterPt(),weight);
	histos[95]->Fill(leptons[1]->Pt(),weight);
	histos[98]->Fill(((MTree::Electron*)leptons[0])->ClusterEta(),weight);
	histos[101]->Fill(leptons[1]->Eta(),weight);
	histos[104]->Fill(((MTree::Electron*)leptons[0])->ETCone30()/((MTree::Electron*)leptons[0])->ClusterPt(),weight);
	histos[107]->Fill(((MTree::Muon*)leptons[1])->PTCone40()/leptons[1]->Pt(),weight);
      }
      else if (channels[0].compare("me") == 0)
      {
	histos[23]->Fill(getMET(),weight);
	histos[46]->Fill(getMET(),weight*sigT);
	histos[47]->Fill(getMET(),weight*backT);
	histos[25]->Fill(TMath::Abs(leptons[0]->DeltaPhi(*leptons[1])),weight);
	histos[48]->Fill(TMath::Abs(leptons[0]->DeltaPhi(*leptons[1])),weight*sigT);
	histos[49]->Fill(TMath::Abs(leptons[0]->DeltaPhi(*leptons[1])),weight*backT);
	histos[27]->Fill((v1+v2).M(),weight);
	histos[50]->Fill((v1+v2).M(),weight*sigT);
	histos[51]->Fill((v1+v2).M(),weight*backT);
	histos[115]->Fill(sqrt(2*(v1+v2).Pt()*_missingET->Pt()*(1-TMath::Cos((v1+v2).DeltaPhi(*_missingET)))),weight);
	histos[134]->Fill(sqrt(2*(v1+v2).Pt()*_missingET->Pt()*(1-TMath::Cos((v1+v2).DeltaPhi(*_missingET)))),weight*sigT);
	histos[135]->Fill(sqrt(2*(v1+v2).Pt()*_missingET->Pt()*(1-TMath::Cos((v1+v2).DeltaPhi(*_missingET)))),weight*backT);
	histos[118]->Fill(leptons[0]->Pt(),weight);
	histos[121]->Fill(((MTree::Electron*)leptons[1])->ClusterPt(),weight);
	histos[124]->Fill(leptons[0]->Eta(),weight);
	histos[127]->Fill(((MTree::Electron*)leptons[1])->ClusterEta(),weight);
	histos[130]->Fill(((MTree::Muon*)leptons[0])->PTCone40()/leptons[0]->Pt(),weight);
	histos[133]->Fill(((MTree::Electron*)leptons[1])->ETCone30()/((MTree::Electron*)leptons[1])->ClusterPt(),weight);
      }
      else if (channels[0].compare("mm") == 0)
      {
	histos[29]->Fill(getMET(),weight);
	histos[52]->Fill(getMET(),weight*sigT);
	histos[53]->Fill(getMET(),weight*backT);
	histos[31]->Fill(TMath::Abs(leptons[0]->DeltaPhi(*leptons[1])),weight);
	histos[54]->Fill(TMath::Abs(leptons[0]->DeltaPhi(*leptons[1])),weight*sigT);
	histos[55]->Fill(TMath::Abs(leptons[0]->DeltaPhi(*leptons[1])),weight*backT);
	histos[33]->Fill((v1+v2).M(),weight);
	histos[56]->Fill((v1+v2).M(),weight*sigT);
	histos[57]->Fill((v1+v2).M(),weight*backT);
	histos[141]->Fill(sqrt(2*(v1+v2).Pt()*_missingET->Pt()*(1-TMath::Cos((v1+v2).DeltaPhi(*_missingET)))),weight);
	histos[160]->Fill(sqrt(2*(v1+v2).Pt()*_missingET->Pt()*(1-TMath::Cos((v1+v2).DeltaPhi(*_missingET)))),weight*sigT);
	histos[161]->Fill(sqrt(2*(v1+v2).Pt()*_missingET->Pt()*(1-TMath::Cos((v1+v2).DeltaPhi(*_missingET)))),weight*backT);
	histos[144]->Fill(leptons[0]->Pt(),weight);
	histos[147]->Fill(leptons[1]->Pt(),weight);
	histos[150]->Fill(leptons[0]->Eta(),weight);
	histos[153]->Fill(leptons[1]->Eta(),weight);
	histos[156]->Fill(((MTree::Muon*)leptons[0])->PTCone40()/leptons[0]->Pt(),weight);
	histos[159]->Fill(((MTree::Muon*)leptons[1])->PTCone40()/leptons[0]->Pt(),weight);
      }
      else
      {
	std::cout << "Ya gots a weeeiiiiird channel here.  Again." << std::endl;
	std::cout << "Thissa it: " << channels[0] << std::endl;
      }

      if (_debug)
      {
	std::cout << "Reached end of event loop" << std::endl;
      }
//       if (eventNr > 50000)
//       {
// 	eventNr = 15000000;
//       }
    }//Closes loop over events
  }//Closes loop over samples

  elrecord->print(sample + "_elcut");
  evrecord->print(sample + "_evcut");
  murecord->print(sample + "_mucut");

  outfile.close();
  summary.close();
  byline.close();
  news.close();
  special.close();
  jfstream.close();
  zstream.close();
}

//////////////////////////////////////////////////////////////////////////////////

void DataAnalysis::wwFakeEstBack(std::string sample, TH1D** histos, TH2D** histos2)
{
  //Set up record keeping, good run list, anything else critical . . .
  if (_debug)
  {
    std::cout << "Calling wwFakeEst()" << std::endl;
  }

  RecordClass* elrecord = new RecordClass("elecs");
  RecordClass* evrecord = new RecordClass("events");
  RecordClass* murecord = new RecordClass("muons");

  ofstream outfile;
  std::string tag = "event-summaries/" + sample + "_faketest.txt";
  outfile.open(tag.c_str());

  bool isMC = true;
  if ((sample.find("data") != string::npos) || (sample.compare("topmix") == 0))
  {
    isMC = false;
  }

  if (_debug)
  {
    std::cout << "Setting up good run list" << std::endl;
  }
  TString list;
  list = "/home/swalch/laboratory/macro-cosmos/HiggsAnalysis/GoodRunLists/GRL_7TeV_init.xml";
  Root::TGoodRunsListReader foo(list);
  foo.Interpret();
  Root::TGoodRunsList grl1 = foo.GetMergedGoodRunsList();

  //Loop over sub-samples
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
      std::cout << "Looking at weight and cross-section" << std::endl;
    }
    double sampleWeight = 0.0;
    double sampleXSec= 0.0;
    if (isMC)
    {
      sampleWeight = GetSampleWeight(sampleName);
      sampleXSec = GetCrossSection(sampleName);
      evrecord->setWeightSum(sampleWeight);
    }

    if (_debug)
    {
      std::cout << "Entering event loop" << std::endl;
    }
    int eventNr = 0;
    double weight = 1.0;
    while(chain->GetEntry(eventNr++))
    {
      if (eventNr % 10000 == 0)
      {
	std::cout << "Analyzed " << eventNr << " events." << std::endl;
      }
      int cutNr = 0;
      cutNr = evrecord->addEvent(cutNr,weight,"Initial number of events");

      if (isMC)
      {
	weight = sampleXSec/sampleWeight*(_eventInfo->EventWeight());
      }

      //Check Good Run List
//       if (_debug)
//       {
// 	std::cout << "Checking good run list" << std::endl;
//       }
//       bool passed = true;
//       if (!isMC)
//       {
// 	int runNr = _eventInfo->RunNumber();
// 	int lumiBlock = _eventInfo->LumiBlock();
// 	passed = grl1.HasRunLumiBlock(runNr,lumiBlock);
//       }
//       if (!passed) continue;
//       cutNr = evrecord->addEvent(cutNr,weight,"Passed good run list");

      //Trigger selection
      if (_debug)
      {
	std::cout << "L1_EM5 Trigger" << std::endl;
      }
      std::string etrigger = "L1_EM5";
      if (!(_trigger->PassedL1Trigger(etrigger)))  continue;
      cutNr = evrecord->addEvent(cutNr,weight,"Passed L1_EM5");

      //Loop over electrons and make vectors
      if (_debug)
      {
	std::cout << "Looping over electrons" << std::endl;
      }
      std::vector<MTree::Lepton*> temp;
      std::vector<MTree::Electron*> looseEs;
      std::vector<MTree::Muon*> muons;
      int nEl = _electrons->GetEntries();
      for (int i = 0; i != nEl; ++i)
      {
	MTree::Electron* electron = (MTree::Electron*)_electrons->At(i);
	temp.push_back(electron);
      }

      int nMu = _muons->GetEntries();
      for (int i = 0; i != nMu; ++i)
      {
	MTree::Muon* muon = (MTree::Muon*)_muons->At(i);
	if (!(muon->IsCombinedMuon())) continue;
	if (muon->Author() != 6) continue;
	temp.push_back(muon);
      }

      temp = removeOverlap(temp);

      for (int i = 0; i != (int)temp.size(); ++i)
      {
	if (temp[i]->LeptonPdgID() == 11)
	{
	MTree::Electron* electron = (MTree::Electron*)temp[i];
	MTree::Track* track = electron->GetTrack();
	histos[0]->Fill(electron->Pt(),weight);
	histos[1]->Fill(electron->Eta(),weight);
	if (track != 0)
	{
	  histos[2]->Fill(TMath::Abs(track->Vtx_D0()/track->Vtx_D0_Err()),weight);
	}
	histos[27]->Fill(electron->ETCone30()/electron->Pt(),weight);
	histos[29]->Fill(electron->TrackIso30()/electron->Pt(),weight);

	int cutE = 0;
	cutE = elrecord->addEvent(cutE,weight,"Initial number of electrons");

	if (electron->Pt() < 15) continue;
	cutE = elrecord->addEvent(cutE,weight,"Pt > 15 GeV");
	if (TMath::Abs(electron->Eta()) > 2.47) continue;
	if ((TMath::Abs(electron->Eta()) > 1.37) && (TMath::Abs(electron->Eta()) < 1.52)) continue;
	cutE = elrecord->addEvent(cutE,weight,"|Eta| < 2.47 and not in crack region");
	if ((electron->Author() != 1) && (electron->Author() != 3)) continue;
	cutE = elrecord->addEvent(cutE,weight,"Author = 1 or 3");
	if (TMath::Abs(electron->Charge()) != 1) continue;
	cutE = elrecord->addEvent(cutE,weight,"Making sure charge is 1 or -1");
	if (track == 0) continue;
	cutE = elrecord->addEvent(cutE,weight,"Electron has a track");
	if (TMath::Abs(track->Vtx_D0()/track->Vtx_D0_Err()) > 10) continue;
	cutE = elrecord->addEvent(cutE,weight,"d0_sig < 10");
	if (!(electron->IsLoose())) continue;
	cutE = elrecord->addEvent(cutE,weight,"Loose electrons");

	looseEs.push_back(electron);
	histos[8]->Fill(electron->Pt(),weight);

	histos[3]->Fill(electron->Pt(),weight);
	histos[4]->Fill(electron->Eta(),weight);
	histos[5]->Fill(TMath::Abs(track->Vtx_D0()/track->Vtx_D0_Err()),weight);
	histos[28]->Fill(electron->ETCone30()/electron->Pt(),weight);
	histos[30]->Fill(electron->TrackIso30()/electron->Pt(),weight);
	}
	else if (temp[i]->LeptonPdgID() == 13)
	{
	  MTree::Muon* muon = (MTree::Muon*)temp[i];
	  int cutM = 0;
	  cutM = murecord->addEvent(cutM,weight,"Initial number of STACO comb muons");

	  if (muon->Pt() < 15) continue;
	  cutM = murecord->addEvent(cutM,weight,"Pt > 15 GeV");
	  if (TMath::Abs(muon->Eta()) > 2.5) continue;
	  cutM = murecord->addEvent(cutM,weight,"|eta| < 2.5");
	  MTree::Track* track = muon->GetTrack();
	  if (track == 0) continue;
	  cutM = murecord->addEvent(cutM,weight,"Muon has a track");
	  if (TMath::Abs(track->Vtx_D0()/track->Vtx_D0_Err()) > 10) continue;
	  cutM = murecord->addEvent(cutM,weight,"d0_sig < 10");

	  muons.push_back(muon);
	}
      }

      //Find tight muon highest in pt.
      if (_debug)
      {
	std::cout << "Reordering by pt" << std::endl;
      }
      MTree::Muon* muon1 = 0;
      for (int i = 0; i < (int)muons.size(); i++)
      {
        for (int j = i + 1; j < (int)muons.size(); j++)
        { 
          if (muons[j]->Pt() > muons[i]->Pt())
          {
	    swap(muons[i],muons[j]);
          }
        }
      }

      if (_debug)
      {
	std::cout << "Finding muon" << std::endl;
      }
      if (muons.size() == 0) continue;
      muon1 = muons[0];

      cutNr = evrecord->addEvent(cutNr,weight,"Events with a good muon");

      //Oh, and there better be a second electron as well.
      if (looseEs.size() == 0) continue;
      cutNr = evrecord->addEvent(cutNr,weight,"Events with a muon and at least one electron");

      //Fill histograms here.
      histos[6]->Fill(_missingET->MET(),weight);
      histos[9]->Fill((*muon1 + *looseEs[0]).M(),weight);
      histos[11]->Fill(muon1->Pt(),weight);
      histos[13]->Fill(looseEs[0]->Pt(),weight);
      histos[15]->Fill(TMath::Abs(muon1->DeltaPhi(*looseEs[0])),weight);
      histos[31]->Fill(muon1->ETCone30()/muon1->Pt(),weight);
      histos[32]->Fill(looseEs[0]->ETCone30()/looseEs[0]->Pt(),weight);
      histos[33]->Fill(muon1->TrackIso30()/muon1->Pt(),weight);
      histos[34]->Fill(looseEs[0]->TrackIso30()/looseEs[0]->Pt(),weight);

      //Cut on missingET.
      if (_debug)
      {
	std::cout << "Cutting on MET" << std::endl;
      }
//       if (sample.find("Z") == string::npos)
//       {
        if (_missingET->MET() < 30) continue;
//       }
      cutNr = evrecord->addEvent(cutNr,weight,"MET > 30 GeV");

      //Limit this to zero jet events.
      int njets = _jets->GetEntries();
      int goodJets = 0;
      for (int i = 0; i != njets; ++i)
      {
	MTree::Jet* jet = (MTree::Jet*)_jets->At(i);
	histos[18]->Fill(jet->Pt(),weight);
	histos[20]->Fill(jet->Eta(),weight);
	if (jet->Pt() < 20) continue;
	if (TMath::Abs(jet->Eta()) > 3) continue;
	if (TMath::Abs(muon1->DeltaR(*jet)) < 0.3) continue;
	bool free = true;
	for (int j = 0; j != (int)looseEs.size(); ++j)
	{
	  if (TMath::Abs(looseEs[j]->DeltaR(*jet)) < 0.3)
	  {
	    free = false;
	  }
	}
	if (free)
	{
	  ++goodJets;
	  histos[19]->Fill(jet->Pt(),weight);
	  histos[21]->Fill(jet->Eta(),weight);
	}
      }
      histos[17]->Fill(goodJets,weight);
      if (goodJets > 0) continue;
      cutNr = evrecord->addEvent(cutNr,weight,"Zero jet events");

      //Check for at least one opposite sign lepton.
      if (_debug)
      {
	std::cout << "Finding a second lepton" << std::endl;
      }
      for (int i = 0; i != (int)looseEs.size(); ++i)
      {
	if (muon1->Charge() == looseEs[i]->Charge())
	{
	  looseEs.erase(looseEs.begin() + i);
	  --i;
	}
      }
      if (looseEs.size() < 1) continue;
      cutNr = evrecord->addEvent(cutNr,weight,"Events with a tight-loose electron pair");

      //Reject Z events
      if (_debug)
      {
	std::cout << "Rejecting Z bosons" << std::endl;
      }
//       if (sample.find("Z") == string::npos)
//       {
	double mass = (*muon1 + *looseEs[0]).M();
	if ((mass > 81) && (mass < 101)) continue;
//       }
      cutNr = evrecord->addEvent(cutNr,weight,"Mass not in Z window");

      //Fill more histograms
      histos[22]->Fill(_missingET->MET(),weight);
      histos[23]->Fill((*muon1 + *looseEs[0]).M(),weight);
      histos[24]->Fill(muon1->Pt(),weight);
      histos[25]->Fill(looseEs[0]->Pt(),weight);
      histos[26]->Fill(TMath::Abs(muon1->DeltaPhi(*looseEs[0])),weight);
      histos[35]->Fill(muon1->ETCone30()/muon1->Pt(),weight);
      histos[36]->Fill(looseEs[0]->ETCone30()/looseEs[0]->Pt(),weight);
      histos[37]->Fill(muon1->TrackIso30()/muon1->Pt(),weight);
      histos[38]->Fill(looseEs[0]->TrackIso30()/looseEs[0]->Pt(),weight);

      //Check to see if second lepton is also tight.
      if (_debug)
      {
	std::cout << "checking tightness of second lepton" << std::endl;
      }
      bool istight = false;
      for (int i = 0; i != (int)looseEs.size(); ++i)
      {
	if (looseEs[i]->IsTight())
	{
	  istight = true;
	}
	else
	{
	  looseEs.erase(looseEs.begin() + i);
	  --i;
	}
      }

      if (!istight) continue;

      cutNr = evrecord->addEvent(cutNr,weight,"Events with a tight-tight pair");
      histos[7]->Fill(_missingET->MET(),weight);
      histos[10]->Fill((*muon1+*looseEs[0]).M(),weight);
      histos[12]->Fill(muon1->Pt(),weight);
      histos[14]->Fill(looseEs[0]->Pt(),weight);
      histos[16]->Fill(TMath::Abs(muon1->DeltaPhi(*looseEs[0])),weight);
      histos[39]->Fill(muon1->ETCone30()/muon1->Pt(),weight);
      histos[40]->Fill(looseEs[0]->ETCone30()/looseEs[0]->Pt(),weight);
      histos[41]->Fill(muon1->TrackIso30()/muon1->Pt(),weight);
      histos[42]->Fill(looseEs[0]->TrackIso30()/looseEs[0]->Pt(),weight);

      if (_debug)
      {
	std::cout << "Reached end of event loop" << std::endl;
      }
    }//Closes loop over events
  }//Closes loop over samples

  elrecord->print(sample + "_elcut");
  evrecord->print(sample + "_evcut");
  murecord->print(sample + "_mucut");
}

/////////////////////////////////////////////////////////////////////////////////////////

void DataAnalysis::wFakeEst(std::string sample, TH1D** histos, TH2D** histos2)
{
  //Set up record keeping, good run list, anything else critical . . .
  if (_debug)
  {
    std::cout << "Calling wFakeEst()" << std::endl;
  }

  RecordClass* elrecord = new RecordClass("elecs");
  RecordClass* evrecord = new RecordClass("events");

  bool isMC = true;
  if ((sample.find("data") != string::npos) || (sample.compare("topmix") == 0))
  {
    isMC = false;
  }

  if (_debug)
  {
    std::cout << "Setting up good run list" << std::endl;
  }
  TString list;
  list = "/home/swalch/laboratory/macro-cosmos/HiggsAnalysis/GoodRunLists/GRL_7TeV_init.xml";
  Root::TGoodRunsListReader foo(list);
  foo.Interpret();
  Root::TGoodRunsList grl1 = foo.GetMergedGoodRunsList();

  //Loop over sub-samples
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
      std::cout << "Looking at weight and cross-section" << std::endl;
    }
    double sampleWeight = 0.0;
    double sampleXSec= 0.0;
    if (isMC)
    {
      sampleWeight = GetSampleWeight(sampleName);
      sampleXSec = GetCrossSection(sampleName);
      evrecord->setWeightSum(sampleWeight);
    }

    if (_debug)
    {
      std::cout << "Entering event loop" << std::endl;
    }
    int eventNr = 0;
    double weight = 1.0;
    while(chain->GetEntry(eventNr++))
    {
      if (eventNr % 10000 == 0)
      {
	std::cout << "Analyzed " << eventNr << " events." << std::endl;
      }
      if (isMC)
      {
	weight = sampleXSec/sampleWeight*(_eventInfo->EventWeight());
      }

      int cutNr = 0;
      cutNr = evrecord->addEvent(cutNr,weight,"Initial number of events");


      //Reject W or Z events in JF17
      bool badWZ = false;
      if ((sample.compare("JF17") == 0) && (IsJF17WZEvent()))
      {
	badWZ = true;
      }
      if (badWZ) continue;
      cutNr = evrecord->addEvent(cutNr, weight, "Not a JF17 WZ event");

      //Check Good Run List
      if (_debug)
      {
	std::cout << "Checking good run list" << std::endl;
      }
      bool passed = true;
      if ((!isMC) && sample.compare("topmix") != 0)
      {
	int runNr = _eventInfo->RunNumber();
	int lumiBlock = _eventInfo->LumiBlock();
	passed = grl1.HasRunLumiBlock(runNr,lumiBlock);
      }
      if (!passed) continue;
      cutNr = evrecord->addEvent(cutNr,weight,"Passed good run list");

      //Trigger selection
      if (_debug)
      {
	std::cout << "L1_EM5 Trigger" << std::endl;
      }
      std::string etrigger = "L1_EM5";
      if (!(_trigger->PassedL1Trigger(etrigger)))  continue;
      cutNr = evrecord->addEvent(cutNr,weight,"Passed L1_EM5");

      //Make and store vector of muons for overlap removal.
      if (_debug)
      {
	std::cout << "Making vector of muons" << std::endl;
      }
      std::vector<MTree::Muon*> muons;
      int nMu = _muons->GetEntries();
      for (int i = 0; i != nMu; ++i)
      {
	MTree::Muon* muon = (MTree::Muon*)_muons->At(i);
	if (muon->Author() != 6) continue;
	if (!(muon->IsCombinedMuon())) continue;

	muons.push_back(muon);
      }

      //Make a vector of good electrons, up to loose selection.
      if (_debug)
      {
	std::cout << "Applying electron selections" << std::endl;
      }
      std::vector<MTree::Lepton*> electrons;
      int nEl = _electrons->GetEntries();
      for (int i = 0; i != nEl; ++i)
      {
	MTree::Electron* elec = (MTree::Electron*)_electrons->At(i);
	int cutEl = 0;
	cutEl = elrecord->addEvent(cutEl,weight,"Initial number of electron objects");
	MTree::Track* track = elec->GetTrack();

	if (elec->Pt() < 15) continue;
	cutEl = elrecord->addEvent(cutEl,weight,"pT > 15 GeV");

	double eta = TMath::Abs(elec->Eta());
	if (eta > 2.47) continue;
	if ((eta > 1.37) && (eta < 1.52)) continue;
	cutEl = elrecord->addEvent(cutEl,weight,"|eta| < 2.47 and not in crack region");

	if ((elec->Author() != 1) && (elec->Author() != 3)) continue;
	cutEl = elrecord->addEvent(cutEl,weight,"Author of 1 or 3");

	if (track == 0) continue;
	if (TMath::Abs(track->Vtx_D0()/track->Vtx_D0_Err()) > 10) continue;
	cutEl = elrecord->addEvent(cutEl,weight,"d0_err < 10");

	if (!(elec->IsLoose())) continue;
	cutEl = elrecord->addEvent(cutEl,weight,"IsEM Loose");

	electrons.push_back(elec);
	histos[5]->Fill(elec->Pt(),weight);

	if (!(elec->IsTight())) continue;
	cutEl = elrecord->addEvent(cutEl,weight,"IsEM Tight");

	histos[6]->Fill(elec->Pt(),weight);
	histos[7]->Fill(elec->Eta(),weight);
	histos[8]->Fill(track->Vtx_D0()/track->Vtx_D0_Err(),weight);
	histos[9]->Fill(elec->ETCone30()/elec->Pt(),weight);
	histos[10]->Fill(elec->TrackIso30()/elec->Pt(),weight);
      }
      if (electrons.size() == 0) continue;
      cutNr = evrecord->addEvent(cutNr,weight,">= one good electron");

      //Perform overlap removal.
      if (_debug)
      {
	std::cout << "Remove overlapping leptons" << std::endl;
      }
      electrons = removeOverlap(electrons);
      for (int i = 0; i != (int)muons.size(); ++i)
      {
	for (int j = 0; j != (int)electrons.size(); ++j)
	{
	  if (muons[i]->GetTrack() == electrons[j]->GetTrack())
	  {
	    electrons.erase(electrons.begin() + j);
	    --j;
	  }
	}
      }

      //Kick out event with > 1 electron at this point.
      if (_debug)
      {
	std::cout << "Veto events with > 1 electron" << std::endl;
      }
      if (electrons.size() != 1) continue;
      cutNr = evrecord->addEvent(cutNr,weight,"Events with exactly 1 loose electron");

      MTree::Electron* electron = (MTree::Electron*)electrons[0];
      double met = _missingET->MET();
      double dphi = electron->DeltaPhi(*_missingET);
      double mT = sqrt(2*electron->Pt()*_missingET->Pt()*(1-cos(dphi)));
      double wpt = (*electron + *(_missingET)).Pt();

      //Apply the rest of the event selections.
      if (_debug)
      {
	std::cout << "Applying MET selection" << std::endl;
      }
      if (met < 25) continue;
      cutNr = evrecord->addEvent(cutNr,weight,"MET > 25 GeV");

      histos[11]->Fill(met,weight);
      histos[12]->Fill(mT,weight);
      histos[13]->Fill(TMath::Abs(dphi),weight);
      histos[17]->Fill(electron->Pt(),weight);
      histos[18]->Fill(electron->ETCone30()/electron->Pt(),weight);
      histos[19]->Fill(electron->TrackIso30()/electron->Pt(),weight);
      histos[23]->Fill(wpt,weight);

//       double effS = getSigEff(electron->Pt(),electron->Eta());
//       double effB = getBackEff(electron->Pt(),electron->Eta());

//       double sigL = effB/(effB-effS);
//       double backL = effS/(effS-effB);

//       histos[0]->Fill(met,weight*sigL);
//       histos[1]->Fill(met,weight*backL);
//       histos[2]->Fill(mT,weight*sigL);
//       histos[3]->Fill(mT,weight*backL);
//       histos[4]->Fill(electron->Pt(),weight*sigL);
//       histos[25]->Fill(electron->Pt(),weight*backL);
//       histos[26]->Fill(wpt,weight*sigL);
//       histos[27]->Fill(wpt,weight*backL);

      //Count the number of tight electrons.
      if (_debug)
      {
	std::cout << "Applying tight selection" << std::endl;
      }
      if (!(electron->IsTight())) continue;
      cutNr = evrecord->addEvent(cutNr,weight,"Tight electron");

      histos[14]->Fill(met,weight);
      histos[15]->Fill(mT,weight);
      histos[16]->Fill(TMath::Abs(dphi),weight);
      histos[20]->Fill(electron->Pt(),weight);
      histos[21]->Fill(electron->ETCone30()/electron->Pt(),weight);
      histos[22]->Fill(electron->TrackIso30()/electron->Pt(),weight);
      histos[24]->Fill(wpt,weight);

//       double sigT = 1/(effS-effB);
//       double backT = 1/(effB-effS);

//       histos[0]->Fill(met,weight*sigT);
//       histos[1]->Fill(met,weight*backT);
//       histos[2]->Fill(mT,weight*sigT);
//       histos[3]->Fill(mT,weight*backT);
//       histos[4]->Fill(electron->Pt(),weight*sigT);
//       histos[25]->Fill(electron->Pt(),weight*backT);
//       histos[26]->Fill(wpt,weight*sigT);
//       histos[27]->Fill(wpt,weight*backT);

      if (_debug)
      {
	std::cout << "Reached end of event loop" << std::endl;
      }
//       delete cand;
      muons.clear();
      electrons.clear();
    }//Closes loop over events
  }//Closes loop over samples

  elrecord->print(sample + "_Wfake");
  evrecord->print(sample + "_WfakeEl");

}
