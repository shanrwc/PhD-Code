#include "SusyAnalysis.h"

bool SusyAnalysis::firstTriggerCheck(std::string sample, int runNumber)
{
  bool passTrigger = false;
  bool passE = false;
  bool passM = false;

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

  passTriggers(runNumber,passE,passM);

  if ((ECaloStream && passE) || (MuonStream && passM) || (MCStream && (passE || passM)))
  {
    passTrigger = true;
  }

  return(passTrigger);
}

std::vector<MTree::PrimaryVertex*> SusyAnalysis::getPVs()
{
  std::vector<MTree::PrimaryVertex*> primaryVs;
  int nPVs = _primaryVertices->GetEntries();
  for (int i = 0; i != nPVs; ++i)
  {
    MTree::PrimaryVertex* vertex = (MTree::PrimaryVertex*)_primaryVertices->At(i);
    if (vertex->NTracks() < 5) continue;
    primaryVs.push_back(vertex);
  }

  return(primaryVs);
}

double SusyAnalysis::getPileUpWeight(int nPVs)
{
  double weight = 0;
  if (nPVs == 1)
  {
    weight = 2.21276;
  }
  if (nPVs == 2)
  {
    weight = 1.18542;
  }
  if (nPVs == 3)
  {
    weight = 0.689077;
  }
  if (nPVs == 4)
  {
    weight = 0.437053;
  }
  if (nPVs == 5)
  {
    weight = 0.293771;
  }
  if (nPVs > 5)
  {
    weight = 0.172771;
  }

  return(weight);
}

bool SusyAnalysis::badJetsPresent()
{
  bool failCleaning = false;
  int nJets = _jets->GetEntries();
  for (int i = 0; i != nJets; ++i)
  {
    MTree::Jet* jet = (MTree::Jet*)_jets->At(i);

//     if (jet->Pt() < 20) continue;
    if (jet->GetRawPt()*(jet->GetEmJESFactor()) < 20) continue;

    if ((TMath::Abs(jet->JetQuality()) > 0.8) && (jet->JetEMFraction() > 0.95))
    {
      failCleaning = true;
    }

    if ((jet->HECFrac() > 0.8) && (jet->N90() <= 5))
    {
      failCleaning = true;
    }
    if ((jet->HECFrac() > 0.5) && (TMath::Abs(jet->JetQuality()) > 0.5))
    {
      failCleaning = true;
    }

    if (TMath::Abs(jet->JetTime()) > 25)
    {
      failCleaning = true;
    }
    if (jet->JetEMFraction() < 0.05)
    {
      failCleaning = true;
    }
    if ((jet->SamplingMaxFrac() > 0.99) && (TMath::Abs(jet->Eta()) < 2))
    {
      failCleaning = true;
    }

  }

  return(failCleaning);
}

bool SusyAnalysis::cosmicJetPresent(std::vector<MTree::Jet*> jets)
{
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

  return(failJetPhot);
}

std::vector<MTree::Electron*> SusyAnalysis::getMediumElectrons(int runNumber)
{
  std::vector<MTree::Electron*> electrons;

  int nElectrons = _electrons->GetEntries();
  for (int i = 0; i != nElectrons; ++i)
  {
    MTree::Electron* elec = (MTree::Electron*)_electrons->At(i);

    if (elec->Pt() < 10) continue;
    if (TMath::Abs(elec->ClusterEta()) > 2.47) continue;
    if (elec->Author() != 1 && elec->Author() != 3) continue;

//     if (runNumber == 100000)
//     {
//       runNumber = 166497;
//     }
//     if (egammaOQ::checkOQClusterElectron(runNumber,elec->ClusterEta(),elec->ClusterPhi()) == 3) continue;

    double et = 1000*elec->ClusterE()/TMath::CosH(TMath::Abs(elec->ClusterBECombEta()));
    if (!(isRobustMedium(elec->IsEM(),elec->ClusterBECombEta(),et,elec->E237()/elec->E277(),elec->WEta2()))) continue;

    int trackBLayer = 16;
    const unsigned BLAYER = 0x1 << trackBLayer;
    bool hasBLayerHit = (elec->IsEM() & BLAYER) == 0;
    if (elec->ExpectHitInBLayer() && !hasBLayerHit) continue;

    if ((TMath::Abs(elec->ClusterEta()) > 1.37) && (TMath::Abs(elec->ClusterEta()) < 1.52)) continue;
    if (elec->ETCone20() > 10) continue;

    electrons.push_back(elec);
  }

  return(electrons);
}

std::vector<MTree::Electron*> SusyAnalysis::getLooseElectrons(int runNumber)
{
  std::vector<MTree::Electron*> electrons;

  int nElectrons = _electrons->GetEntries();
  for (int i = 0; i != nElectrons; ++i)
  {
    MTree::Electron* elec = (MTree::Electron*)_electrons->At(i);

    if (elec->Pt() < 10) continue;
    if (TMath::Abs(elec->ClusterEta()) > 2.47) continue;
    if ((TMath::Abs(elec->ClusterEta()) > 1.37) && (TMath::Abs(elec->ClusterEta()) < 1.52)) continue;
    if (elec->Author() != 1 && elec->Author() != 3) continue;

//     if (runNumber == 100000)
//     {
//       runNumber = 166497;
//     }
//     if (egammaOQ::checkOQClusterElectron(runNumber,elec->ClusterEta(),elec->ClusterPhi()) == 3) continue;

    double et = 1000*elec->ClusterE()/TMath::CosH(TMath::Abs(elec->ClusterBECombEta()));

    bool medium = isRobustMedium(elec->IsEM(),elec->ClusterBECombEta(),et,elec->E237()/elec->E277(),elec->WEta2());
    bool loose = isRobustLoose(elec->IsEM(),elec->ClusterBECombEta(),et,elec->E237()/elec->E277(),elec->WEta2());

    int trackBLayer = 16;
    const unsigned BLAYER = 0x1 << trackBLayer;
    bool hasBLayerHit = (elec->IsEM() & BLAYER) == 0;
    if (elec->ExpectHitInBLayer() && !hasBLayerHit) continue;

    if (elec->ETCone20() > 10) continue;

    if ((loose) && (!medium))
    {
      electrons.push_back(elec);
    }
  }

  return(electrons);
}

std::vector<MTree::Electron*> SusyAnalysis::getCrackElectrons(int runNumber,bool wantLoose = false)
{
  std::vector<MTree::Electron*> crackElectrons;
  int nElectrons = _electrons->GetEntries();
  for (int i = 0; i != nElectrons; ++i)
  {
    MTree::Electron* elec = (MTree::Electron*)_electrons->At(i);

    if (elec->Pt() < 10) continue;
    if (TMath::Abs(elec->ClusterEta()) > 2.47) continue;
    if (elec->Author() != 1 && elec->Author() != 3) continue;

//     if (runNumber == 100000)
//     {
//       runNumber = 166497;
//     }
//     if (egammaOQ::checkOQClusterElectron(runNumber,elec->ClusterEta(),elec->ClusterPhi()) == 3) continue;

    double et = 1000*elec->ClusterE()/TMath::CosH(TMath::Abs(elec->ClusterBECombEta()));
    bool medium = isRobustMedium(elec->IsEM(),elec->ClusterBECombEta(),et,elec->E237()/elec->E277(),elec->WEta2());
    bool loose = isRobustLoose(elec->IsEM(),elec->ClusterBECombEta(),et,elec->E237()/elec->E277(),elec->WEta2());

    if (!loose) continue;

    if (!wantLoose && !medium) continue;

    int trackBLayer = 16;
    const unsigned BLAYER = 0x1 << trackBLayer;
    bool hasBLayerHit = (elec->IsEM() & BLAYER) == 0;
    if (elec->ExpectHitInBLayer() && !hasBLayerHit) continue;

    if ((TMath::Abs(elec->ClusterEta()) > 1.37) && (TMath::Abs(elec->ClusterEta()) < 1.52))
    {
      crackElectrons.push_back(elec);
    }
  }

  return(crackElectrons);
}

std::vector<MTree::Muon*> SusyAnalysis::getGoodMuons()
{
  std::vector<MTree::Muon*> muons;

  int nMuons = _muons->GetEntries();
  for (int i = 0; i != nMuons; ++i)
  {
    MTree::Muon* muon = (MTree::Muon*)_muons->At(i);

    if (muon->Pt() <= 10) continue;

    if (TMath::Abs(muon->Eta()) >= 2.4) continue;

    if (muon->Author() != 6) continue;
    if (!(muon->IsCombinedMuon())) continue;

    if (muon->MatchChi2() < 0 || muon->MatchChi2() > 100) continue;
    if (muon->ETCone20() > 10) continue;
    //    if (muon->ETCone20() >= 1.8) continue;

    //    if (muon->MatchChi2() >= 150) continue;


    MTree::Track* trackID = muon->GetTrack();
//     MTree::Track* trackMS = muon->GetStandAloneTrack();
    if (trackID == 0) continue;
//     if (trackMS == 0) continue;
//     if ((trackMS->Pt() < 50) && (((trackMS->P() - trackID->P())/trackID->P()) < -0.4)) continue;

    muons.push_back(muon);
  }

  return(muons);
}

std::vector<MTree::Jet*> SusyAnalysis::getJets()
{
  std::vector<MTree::Jet*> jets;
  int nJets = _jets->GetEntries();
  for (int i = 0; i != nJets; ++i)
  {
    MTree::Jet* jet = (MTree::Jet*)_jets->At(i);

    double jetpT = (jet->GetRawPt())*(jet->GetEmJESFactor());
    if (jetpT < 30) continue;
    if (TMath::Abs(jet->Eta()) > 2.5) continue;

    jets.push_back(jet);
  }

  return(jets);
}

std::vector<MTree::Photon*> SusyAnalysis::getPhotons()
{
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

  return(photons);
}

bool SusyAnalysis::OTXProblem(int runNumber, std::vector<MTree::Electron*> electrons)
{
  if (runNumber <= 100000)
  {
    runNumber = 166497;
  }

  bool OTXVeto = false;
  for (int i = 0; i != (int)electrons.size(); ++i)
  {
    if (egammaOQ::checkOQClusterElectron(runNumber,electrons[i]->ClusterEta(),electrons[i]->ClusterPhi()) == 3)
    {
      OTXVeto = true;
    }
  }

  return(OTXVeto);
}

bool SusyAnalysis::hasCosmicMuon(std::vector<MTree::Muon*> muons)
{
  bool CosmicVeto = false;
  for (int i = 0; i != (int)muons.size(); ++i)
  {
    MTree::Track* track = muons[i]->GetTrack();
    if (TMath::Abs(track->Vtx_Z0()) >= 10)
    {
      CosmicVeto = true;
    }
  }
  return(CosmicVeto);
}
