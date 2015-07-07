#include "DataAnalysis.h"

int DataAnalysis::maxNofPVTracks()
{
  int i;
  int nPVs = _primaryVertices->GetEntries();
  int maxNTracks = -10;

  for (i = 0; i != nPVs; ++i)
  {
    int nTracks = ((MTree::PrimaryVertex*)_primaryVertices->At(i))->NTracks();
    if (nTracks > maxNTracks)
    {
      maxNTracks = nTracks;
    }
  }

  return(maxNTracks);
}

double DataAnalysis::zPVmaxTracks()
{
  int i;
  int nPVs = _primaryVertices->GetEntries();
  int maxNTracks = -10;
  int index = -10;

  for (i = 0; i != nPVs; ++i)
  {
    int nTracks = ((MTree::PrimaryVertex*)_primaryVertices->At(i))->NTracks();
    if (nTracks > maxNTracks)
    {
      maxNTracks = nTracks;
      index = i;
    }
  }
  MTree::PrimaryVertex* vert = (MTree::PrimaryVertex*)_primaryVertices->At(index);
  return (vert->Z());
}

std::vector<MTree::Lepton*> DataAnalysis::looseLeptons(std::vector<MTree::Lepton*> leptons)
{
  int i;
  std::vector<MTree::Lepton*> looseOnes;
  for (i = 0; i != (int)leptons.size(); ++i)
  {
    bool pass = true;

    if (leptons[i]->LeptonPdgID() == 11)
    {
      MTree::Electron* electron;
      electron = (MTree::Electron*)leptons[i];

      if (electron->Pt() < 2.5)
      {
	pass = false;
      }
      if (pass && fabs(electron->Eta()) > 2.5)
      {
	pass = false;
      }
      if (pass && fabs(electron->Eta()) > 1.37 && fabs(electron->Eta()) < 1.52)
      {
	pass = false;
      }
      if (pass && electron->Author() != 1 && electron->Author() != 3) 
      {
	pass = false;
      }
      if (pass && electron->GetTrack() == 0)
      {
	pass = false;
      }
      if (pass && !(electron->IsLoose()))
      {
	pass = false;
      }
    }
    if (leptons[i]->LeptonPdgID() == 13)
    {
      MTree::Muon* muon;
      muon = (MTree::Muon*)leptons[i];
    
      if (muon->P() < 4)
      {
	pass = false;
      } 
      if (pass && muon->Pt() < 2.5)
      {
	pass = false;
      }
      if (pass && fabs(muon->Eta()) > 2.5)
      {
	pass = false;
      }
      if (pass && muon->GetTrack() == 0) 
      {
	pass = false;
      }
      if (pass && !(muon->IsCombinedMuon())) 
      {
	pass = false;
      }
      if (pass && muon->Author() != 12)
      {
	pass = false;
      }
    }

    if (pass)
    {
      looseOnes.push_back(leptons[i]);
    }
  }
  return(looseOnes);
}

std::vector<MTree::Lepton*> DataAnalysis::mediumLeptons(std::vector<MTree::Lepton*> leptons)
{
  int i;
  std::vector<MTree::Lepton*> mediumOnes;
  for(i = 0; i != (int) leptons.size(); ++i)
  {
    bool pass = true;
    if (leptons[i]->LeptonPdgID() == 11)
    {
      MTree::Electron* electron = (MTree::Electron*)leptons[i];
      if (!(electron->IsMedium()))
      {
	pass = false;
      }
    }
   
    if (leptons[i]->LeptonPdgID() == 13)
    {
      MTree::Muon* muon = (MTree::Muon*)leptons[i];
      MTree::Track* track = muon->GetTrack();

      if (track->NrOfPixelHits() < 1)
      {
	pass = false;
      }
      if (pass && track->NrOfSCTHits() < 6)
      {
	pass = false;
      }
    }
   
    if (pass)
    {
      mediumOnes.push_back(leptons[i]);
    }
  }
  return(mediumOnes);
}

void DataAnalysis::skimEvents(std::string chainName) 
{
  std::vector<std::string> samples = GetAllSamples(chainName);
  std::vector<std::string>::iterator iter;
  std::vector<std::string>::iterator end = samples.end();

  RecordClass* record = new RecordClass("skim");

  for (iter = samples.begin(); iter != end; ++iter)
  {
    std::string name = *iter;

  TChain* tree = GetChain(name);
  setBranchAddresses(tree);

  std::cout << "Skimming lepton events in the " << name << " chain." << std::endl;
  char fileName[200];
  std::string tag = "/home/swalch/data-skimmed/"+chainName +"/" + name + "_skim.root";
  sprintf(fileName, tag.c_str() ,chainName.c_str());

  TFile* outFile = new TFile(fileName, "RECREATE");
  TTree* newtree = tree->CloneTree(0);
  int entry = 0;
  while (tree->GetEntry(entry++)) 
  {
    int cutNr = 0;

    if (entry % 10000 == 0) std::cout << "Skimmed " << entry << " events" << std::endl;
    cutNr = record->addEvent(cutNr, "Initial Number of Events");

    int hpte = 0;
    int hptm = 0;
    int NrElectrons = _electrons->GetEntries();
    for (int i = 0; i != NrElectrons; ++i)
    {
      MTree::Electron* electron = (MTree::Electron*)_electrons->At(i);
      if (electron->ClusterPt() < 10) continue;
      if (TMath::Abs(electron->ClusterEta()) > 2.47) continue;
      if ((TMath::Abs(electron->ClusterEta()) > 1.37) && (TMath::Abs(electron->ClusterEta()) < 1.52)) continue;
      if ((electron->Author() != 1) && (electron->Author() != 3)) continue;
      ++hpte;
    }

    int NrMuons = _muons->GetEntries();
    for (int i = 0; i != NrMuons; ++i)
    {
      MTree::Muon* muon = (MTree::Muon*)_muons->At(i);
      if (muon->Pt() < 10) continue;
      if (TMath::Abs(muon->Eta()) > 2.4) continue;
      if (!(muon->IsCombinedMuon())) continue;
      if (muon->Author() != 6) continue;
      ++hptm;
    }

    if ((hpte + hptm) < 1) continue;
    cutNr = record->addEvent(cutNr, "Events with a half-decent electron or muon");

    // End of cuts
    newtree->Fill();
    outFile = newtree->GetCurrentFile();
  }
  outFile->Write();
  std::cout << "Results written to file" << std::endl;
  outFile->Close();
  }
  std::cout << "Finished skimming events" << std::endl;

  record->print(chainName + "_skim",true);
  std::cout << "Exiting function now" << std::endl;
} 

void DataAnalysis::skimEvents2(std::string chainName) 
{
  std::vector<std::string> samples = GetAllSamples(chainName);
  std::vector<std::string>::iterator iter;
  std::vector<std::string>::iterator end = samples.end();

  RecordClass* record = new RecordClass("skim");

  for (iter = samples.begin(); iter != end; ++iter)
  {
    std::string name = *iter;

  TChain* tree = GetChain(name);
  setBranchAddresses(tree);

  std::cout << "Skimming lepton events in the " << name << " chain." << std::endl;
  char fileName[200];
  std::string tag = "/home/swalch/data-skimmed/"+chainName +"/" + name + "_dilep.root";
  sprintf(fileName, tag.c_str() ,chainName.c_str());

  TFile* outFile = new TFile(fileName, "RECREATE");
  TTree* newtree = tree->CloneTree(0);
  int entry = 0;
  while (tree->GetEntry(entry++)) 
  {
    int cutNr = 0;

    if (entry % 10000 == 0) std::cout << "Skimmed " << entry << " events" << std::endl;
    cutNr = record->addEvent(cutNr, "Initial Number of Events");

    int hpte = 0;
    int hptm = 0;
    int NrElectrons = _electrons->GetEntries();
    for (int i = 0; i != NrElectrons; ++i)
    {
      MTree::Electron* electron = (MTree::Electron*)_electrons->At(i);
      if (electron->ClusterPt() < 10) continue;
      if (TMath::Abs(electron->ClusterEta()) > 2.47) continue;
      if ((TMath::Abs(electron->ClusterEta()) > 1.37) && (TMath::Abs(electron->ClusterEta()) < 1.52)) continue;
      if ((electron->Author() != 1) && (electron->Author() != 3)) continue;
      ++hpte;
    }

    int NrMuons = _muons->GetEntries();
    for (int i = 0; i != NrMuons; ++i)
    {
      MTree::Muon* muon = (MTree::Muon*)_muons->At(i);
      if (muon->Pt() < 10) continue;
      if (TMath::Abs(muon->Eta()) > 2.4) continue;
      if (!(muon->IsCombinedMuon())) continue;
      if (muon->Author() != 6) continue;
      ++hptm;
    }

    if ((hpte + hptm) < 2) continue;
    cutNr = record->addEvent(cutNr, "Events with two leptons");

    // End of cuts
    newtree->Fill();
    outFile = newtree->GetCurrentFile();
  }
  outFile->Write();
  std::cout << "Results written to file" << std::endl;
  outFile->Close();
  }
  std::cout << "Finished skimming events" << std::endl;

  record->print(chainName + "_dilep",true);
  std::cout << "Exiting function now" << std::endl;
} 

std::vector<MTree::Lepton*> DataAnalysis::orderByPt(std::vector<MTree::Lepton*> leptons)
{
  for (int i = 0; i < (int)leptons.size(); i++)
  {
    for (int j = i + 1; j < (int)leptons.size(); j++)
    {
      if (leptons[j]->Pt() > leptons[i]->Pt())
      {
	swap(leptons[i],leptons[j]);
      }
    }
  }

  return(leptons);
}

std::vector<MTree::Lepton*> DataAnalysis::removeOverlap(std::vector<MTree::Lepton*> leptons)
{
  //Order leptons vector by pt.  Just because it will make erasing  the overlaps 
  //one-directional/easier.
  leptons = orderByPt(leptons);
  std::vector<MTree::Lepton*> copy = leptons;
  bool check = false;

  //Loop over leptons vector.  If a pair has the same flavor and a dR < 0.1, remove the 
  //lepton with lower pt.

  for (int i = 0; i != (int)leptons.size(); ++i)
  {
    for (int j = i + 1; j != (int)leptons.size(); ++j)
    {
      if (TMath::Abs(leptons[i]->DeltaR(*leptons[j])) < 0.1)
      {
	if (leptons[i]->LeptonPdgID() == leptons[j]->LeptonPdgID())
	{
	  if (leptons[i]->Pt() <= leptons[j]->Pt())
	  {
	    check = true;
	  }
	  leptons.erase(leptons.begin() + j);
	  --j;
	}
	else
	{
	  if (leptons[i]->LeptonPdgID() == 11)
	  {
	    swap(leptons[i],leptons[j]);
	  }
	  leptons.erase(leptons.begin() + j);
	  --j;
	}
      }
    }
  }

//   if (check)
//   {
//     std::cout << "Original vector:" << std::endl;
//     for (int i = 0; i != (int)copy.size(); ++i)
//     {
//       std::cout << copy[i]->Pt() << std::endl;
//     }
//     std::cout << "New vector: " << std::endl;
//     for (int i = 0; i != (int)leptons.size(); ++i)
//     {
//       std::cout << leptons[i]->Pt() << std::endl;
//     }
//   }

  return (leptons);
}

std::vector<MTree::Jet*> DataAnalysis::findNonOverlapJets(std::vector<MTree::Lepton*> leptons)
{
  //Loop over all jets.  Compare each jet to all leptons.  If dR between jet and every lepton is 
  //greater than 0.1, add to vector.
  std::vector<MTree::Jet*> jets;
  int nJets = _jets->GetEntriesFast();

  for (int i = 0; i != nJets; ++i)
  {
    MTree::Jet* jet = (MTree::Jet*)_jets->At(i);
    bool pass = true;
    for (int j = 0; j != (int)leptons.size(); ++j)
    {
      if (fabs(jet->DeltaR(*leptons[j])) < 0.1)
      {
	pass = false;
      }
    }
    if (pass)
    {
      jets.push_back(jet);
    }
  }

  return (jets);
}

std::vector<std::string> DataAnalysis::findChannels(std::vector<MTree::Lepton*> leptons)
{
  int nE = 0;
  int nM = 0;

  for (int i = 0; i != (int)leptons.size(); ++i)
  {
    if (leptons[i]->LeptonPdgID() == 11)
    {
      ++nE;
    }
    else if (leptons[i]->LeptonPdgID() == 13)
    {
      ++nM;
    }
    else
    {
      std::cout << "findChannels() found something that isn't a lepton!" << std::endl;
    }
  }

  std::vector<std::string> channels;
  if (nE > 0)
  {
    channels.push_back("e");
  }
  if (nM > 0)
  {
    channels.push_back("mu");
  }
  if (nE > 0 && nM > 0)
  {
    channels.push_back("emu");
  }
  if (nE > 1)
  {
    channels.push_back("ee");
  }
  if (nM > 1)
  {
    channels.push_back("mumu");
  }

  return(channels);
}

std::vector<std::string> DataAnalysis::findWWChannels(std::vector<MTree::Lepton*> leptons)
{
  int nE = 0;
  int nM = 0;

  for (int i = 0; i != (int)leptons.size(); ++i)
  {
    if (leptons[i]->LeptonPdgID() == 11)
    {
      ++nE;
    }
    else if (leptons[i]->LeptonPdgID() == 13)
    {
      ++nM;
    }
    else
    {
      std::cout << "findChannels() found something that isn't a lepton!" << std::endl;
    }
  }

  std::vector<std::string> channels;
  if (nE > 0 && nM > 0)
  {
    channels.push_back("emu");
  }
  if (nE > 1)
  {
    channels.push_back("ee");
  }
  if (nM > 1)
  {
    channels.push_back("mumu");
  }

  return(channels);
}

std::vector<std::string> DataAnalysis::findChannels(std::vector<WCandidate> ws)
{
  std::vector<std::string> channels;
  for (int i = 0; i != (int)ws.size(); ++i)
  {
    if (ws[i].decayedToMuon())
    {
      channels.push_back("mu");
    }
    else
    {
      channels.push_back("e");
    }
  }

  if (channels.size() > 1)
  {
    std::cout << "This event has multiple Ws!" << std::endl;
  }

  return (channels);
}

std::vector<std::string> DataAnalysis::findChannels(std::vector<ZCandidate> zs)
{
  std::vector<std::string> channels;
  for (int i = 0; i != (int)zs.size(); ++i)
  {
    if (zs[i].decayedtoMuons())
    {
      channels.push_back("mumu");
    }
    else
    {
      channels.push_back("ee");
    }
  }

  if (channels.size() > 1)
  {
    std::cout << "This event has multiple Zs!" << std::endl;
  }

  return (channels);
}

double DataAnalysis::getSigEff(int leadid, double pt1, double eta1, int subid, double pt2, double eta2)
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
    leff = true_elec_eff->GetBinContent(true_elec_eff->FindBin(pt1, eta1));
  }
  else
  {
    leff = true_muon_eff->GetBinContent(true_muon_eff->FindBin(pt1, eta1));
  }

  if (subid == 11)
  {
    seff = true_elec_eff->GetBinContent(true_elec_eff->FindBin(pt2, eta2));
  }
  else
  {
    seff = true_muon_eff->GetBinContent(true_muon_eff->FindBin(pt2, eta2));
  }

  //Do calculation
  double event_efficiency = 0;
  if (leadid == subid)
  {
    event_efficiency = (leff*seff)/(leff+seff-(leff*seff));
  }
  else
  {
    event_efficiency = seff;
  }

  return(event_efficiency);
}

double DataAnalysis::getBackEff(int leadid, double pt1, double eta1, int subid, double pt2, double eta2)
{
  //See note above.

  //'Cept, I'm going to cheat for now and skip the converting from lepton to signal.
  //'Cause it don't change much.

  double back_efficiency = 0;
  if (subid == 11)
  {
    back_efficiency = fake_elec_eff->GetBinContent(fake_elec_eff->FindBin(pt2, eta2));
  }
  else
  {
    back_efficiency = fake_muon_eff->GetBinContent(fake_muon_eff->FindBin(pt2, eta2));
  }

  return(back_efficiency);
}

bool DataAnalysis::IsJF17WZEvent()
{
  int nTruthParticles = _truthParticles->GetEntriesFast();
  for (int i = 0; i != nTruthParticles; ++i)
  {
    MTree::TruthParticle* truth = (MTree::TruthParticle*)_truthParticles->At(i);
    if ((TMath::Abs(truth->PdgID()) == 23) || (TMath::Abs(truth->PdgID()) == 24) || (TMath::Abs(truth->PdgID()) == 6))
    {
      return (true);
    }
  }
  return (false);
}

void DataAnalysis::printEverything()
{
  std::string filename;
  std::stringstream ss;
  ss << _eventInfo->EventNumber();
  std::string num = ss.str();

  filename = "event-summaries/" + num + ".txt";

  std::ofstream file;
  file.open(filename.c_str());

  _eventInfo->PrintData(file);
  int nPVs = _primaryVertices->GetEntries();
  for (int i = 0; i != nPVs; ++i)
  {
    MTree::PrimaryVertex* pv = (MTree::PrimaryVertex*)_primaryVertices->At(i);
    pv->PrintData(file);
  }
  int nEl = _electrons->GetEntries();
  for (int i = 0; i != nEl; ++i)
  { 
    MTree::Electron* elec = (MTree::Electron*)_electrons->At(i);
    elec->PrintData(file);
    elec->GetTrack()->PrintData(file);
  }
  int nMu = _muons->GetEntries();
  for (int i = 0; i != nMu; ++i)
  {
    MTree::Muon* muon = (MTree::Muon*)_muons->At(i);
    muon->PrintData(file);
    muon->GetTrack()->PrintData(file);
  }
  int nJets = _jets->GetEntries();
  for (int i = 0; i != nJets; ++i)
  {
    MTree::Jet* jet = ((MTree::Jet*)_jets->At(i));
    jet->PrintData(file);
  }
  _missingET->PrintData(file);

  file.close();
}

double DataAnalysis::getMET()
{
  std::map<std::string,MTree::MissingET> terms;

  int nMET = _missingETTerms->GetEntries();

  double lhtx = 0;
  double lhty = 0;
  double mbx = 0;
  double mby = 0;
  double rmtx = 0;
  double rmty = 0;

  for (int i = 0; i != nMET; ++i)
  {
    MTree::MissingET* term = (MTree::MissingET*)_missingETTerms->At(i);
    std::string name = term->GetTerm();

    if (name.compare("MET_LocHadTopo") == 0)
    {
      lhtx = term->Ex();
      lhty = term->Ey();
    }
    if (name.compare("MET_MuonBoy") == 0)
    {
      mbx = term->Ex();
      mby = term->Ey();
    }
    if (name.compare("MET_RefMuon_Track") == 0)
    {
      rmtx = term->Ex();
      rmty = term->Ey();
    }
  }

  double xterms = lhtx + mbx - rmtx;
  double yterms = lhty + mby - rmty;

  double met = sqrt(xterms*xterms + yterms*yterms);

  return(met);
}

double DataAnalysis::getDeltaR(MTree::Lepton* lepton1, MTree::Lepton* lepton2)
{
  //Cook up two TLorentzVectors.
  TLorentzVector v1;
  if (lepton1->LeptonPdgID() == 11)
  {
    MTree::Electron* elec = (MTree::Electron*)lepton1;
    v1.SetPtEtaPhiM(elec->ClusterPt(),elec->ClusterEta(),elec->ClusterPhi(),elec->ClusterM());
  }
  else
  {
    v1.SetPtEtaPhiM(lepton1->Pt(),lepton1->Eta(),lepton1->Phi(),lepton1->M());
  }

  TLorentzVector v2;
  if (lepton1->LeptonPdgID() == 11)
  {
    MTree::Electron* elec = (MTree::Electron*)lepton2;
    v2.SetPtEtaPhiM(elec->ClusterPt(),elec->ClusterEta(),elec->ClusterPhi(),elec->ClusterM());
  }
  else
  {
    v2.SetPtEtaPhiM(lepton2->Pt(),lepton2->Eta(),lepton2->Phi(),lepton2->M());
  }

  //Calculate DeltaR between the two special vectors and return that.
  double deltaR = TMath::Abs(v1.DeltaR(v2));
  return(deltaR);
}
