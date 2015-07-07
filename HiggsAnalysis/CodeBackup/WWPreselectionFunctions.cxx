#include "WWPreselection.h"

std::vector<std::string> WWPreselection::wwLeptonChannel(std::vector<MTree::Lepton*> candidates)
{
  int nEl = 0;
  int nMu = 0;
  std::vector<std::string> channels;

  std::vector<MTree::Lepton*>::iterator iter = candidates.begin();
  std::vector<MTree::Lepton*>::iterator end = candidates.end();

  while (iter != end)
  {
    int ID = (*iter)->LeptonPdgID();

    if (ID == 11)
    {
      nEl++;
    }
    if (ID == 13)
    {
      nMu++;
    }
    if ((ID != 11) && (ID != 13))
    {
      std::cout << "Something's wrong with the wwChannel function" << std::endl;
    }
    ++iter;
  }

  if (nEl > 1)
  {
    channels.push_back("ee");
  }
  if ((nEl > 0) && (nMu > 0))
  {
    channels.push_back("emu");
  }
  if (nMu > 1)
  {
    channels.push_back("mumu");
  }

  return(channels);
}

std::vector<MTree::Lepton*> WWPreselection::wwLeptonOverlapRemoval(double range, std::vector<MTree::Lepton*> candidates)
{
  int i;
  int j;
  int sizeE;
  int sizeM;

  std::vector<MTree::Electron*> elecs;
  std::vector<MTree::Muon*> muons;
  std::vector<MTree::Lepton*> leptons;

  int size = (int)candidates.size();
  //1. Make vectors of electrons and muons.
  for (i = 0; i < size; ++i)
  {
    if (candidates[i]->LeptonPdgID() == 11)
    {
      elecs.push_back((MTree::Electron*)candidates[i]);
    }
    if (candidates[i]->LeptonPdgID() == 13)
    {
      muons.push_back((MTree::Muon*)candidates[i]);
    }
  }

  //2. Redorder muon vector by pt.
  sizeM = (int) muons.size();
  for (i = 0; i < sizeM; ++i)
  {
    for (j = i + 1 ; j < sizeM; ++j)
    {
      if (muons[j]->Pt() > muons[i]->Pt())
      {
	swap(muons[i],muons[j]);
      }
    }
  }

  //3. Loop over muons, removing any that overlap with leading muon.
  //Note that loop starts at one so leading muon isn't compared to itself.
  for (i = 1; i < sizeM; ++i)
  {
    if (muons[i]->DeltaR(*muons[0]) < range)
    {
      muons.erase(muons.begin() + i);
      i--;
    }
  }

  //4. Loop over both electrons and muons, removing any electrons that overlap
  //with a muon.
  sizeM = (int)muons.size();
  sizeE = (int)elecs.size();

  for (i = 0; i < sizeM; ++i)
  {
    for (j = 0; j < sizeE; ++j)
    {
      if (muons[i]->DeltaR(*elecs[j]) < range)
      {
	elecs.erase(elecs.begin() + j);
	j--;
      }
    }
  }

  //5. Reorder electrons by pt.
  sizeE = (int)elecs.size();

  for (i = 0; i < sizeE; ++i)
  {
    for (j = i + 1; j < sizeE; ++j)
    {
      if (elecs[j]->Pt() > elecs[i]->Pt())
      {
	swap(elecs[i],elecs[j]);
      }
    } 
  }

  //6. Loop over electrons, removing any that overlap with leading electron.
  for (i = 1; i < sizeE; ++i)
  {
    if (elecs[i]->DeltaR(*elecs[0]) < range)
    {
      elecs.erase(elecs.begin() + i);
      i--;
    }
  }

  //7. Assemble and return lepton vector.
  for (i = 0; i < sizeM; ++i)
  {
    leptons.push_back((MTree::Lepton*)muons[i]);
  }  
  sizeE = (int)elecs.size();
  for (i = 0; i < sizeE; ++i)
  {
    leptons.push_back((MTree::Lepton*)elecs[i]);
  }
  return(leptons);
}

std::vector<MTree::Jet*> WWPreselection::wwJetOverlapRemoval(double range)
{
  int i;
  int n_jets = _jets->GetEntriesFast();

  MTree::Jet* jet;
  std::vector<MTree::Jet*> jets;

  for (i = 0; i < n_jets; ++i)
  {
    bool far1 = false;
    bool far2 = false;

    jet = (MTree::Jet*)_jets->At(i);

    if (_Leptons[0]->DeltaR(*jet) > range)
    {
      far1 = true;
    } 
    if (_Leptons[1]->DeltaR(*jet) > range)
    {
      far2 = true;
    }

    if (far1 && far2)
    {
      jets.push_back(jet);
    }
  }

  return(jets);
}

bool WWPreselection::wwL1Trigger(MTree::Trigger* trigger)
{
  bool passEL = false;
  bool passMU = false;
  bool pass;

  std::string L1El = "L1_EM7";
  std::string L1Mu = "L1_MU10";

  if (trigger->PassedL1Trigger(L1El))
  {
    passEL = true;
  }
  if (trigger->PassedL1Trigger(L1Mu))
  {
    passMU = true;
  }

  if (passEL || passMU)
  {
    pass = true;
  }
  else
  {
    pass = false;
  }

  return(pass);
}

bool WWPreselection::wwL2Trigger(MTree::Trigger* trigger)
{
  bool passEL = false;
  bool passMU = false;
  bool pass;

  std::string L2El = "L2_e10_medium";
  std::string L2Mu = "L2_mu10";

  if (trigger->PassedL2Trigger(L2El))
  {
    passEL = true;
  }
  if (trigger->PassedL2Trigger(L2Mu))
  {
    passMU = true;
  }

  if (passEL || passMU)
  {
    pass = true;
  }
  else
  {
    pass = false;
  }

  return(pass);
}

bool WWPreselection::wwEFTrigger(MTree::Trigger* trigger)
{
  bool passEL = false;
  bool passMU = false;
  bool pass;

  std::string EFEl = "EF_e10_medium";
  std::string EFMu = "EF_mu10";

  if (trigger->PassedEFTrigger(EFEl))
  {
    passEL = true;
  }
  if (trigger->PassedEFTrigger(EFMu))
  {
    passMU = true;
  }

  if (passEL || passMU)
  {
    pass = true;
  }
  else
  {
    pass = false;
  }

  return(pass);
}

std::vector<MTree::Lepton*> WWPreselection::wwPreselection(std::vector<MTree::Lepton*> candidates)
{
  std::vector<MTree::Lepton*> leptons;
  std::vector<MTree::Lepton*>::iterator iter;
  std::vector<MTree::Lepton*>::iterator end = candidates.end();

  for (iter = candidates.begin(); iter != end; ++iter)
  {
    MTree::Lepton* lepton = *iter;
    if (fabs(lepton->LeptonPdgID()) == 11)
    {
      if ((((MTree::Electron*)lepton)->Author() & 0x1) == 0) continue;
      if (!((MTree::Electron*)lepton)->IsLoose()) continue;
      if (lepton->Pt() < 10) continue;
      leptons.push_back(lepton);
    }
    if (fabs(lepton->LeptonPdgID()) == 13)
    {
      if (((MTree::Muon*)lepton)->Author() != 6) continue;
      if (!(((MTree::Muon*)lepton)->IsCombinedMuon())) continue;
      if (lepton->Pt() < 10) continue;
      leptons.push_back(lepton);
    }
  }
  return(leptons);
}

std::vector<MTree::Lepton*> WWPreselection::wwLeptonSelection(std::vector<MTree::Lepton*> candidates)
{
  std::vector<MTree::Electron*> electrons;
  std::vector<MTree::Muon*> muons;
  std::vector<MTree::Lepton*> leptons;

  std::vector<MTree::Lepton*>::iterator iter;
  std::vector<MTree::Lepton*>::iterator end = candidates.end();

  //1. Make vectors of electrons and muons.
  for (iter = candidates.begin(); iter != end; ++iter)
  {
    MTree::Lepton* lepton = *iter;
    if (lepton->LeptonPdgID() == 11)
    {
      electrons.push_back((MTree::Electron*)lepton);
    }
    if (lepton->LeptonPdgID() == 13)
    {
      muons.push_back((MTree::Muon*)lepton);
    }
  }

  //2. Apply requirements and add passing leptons to vector.
  std::vector<MTree::Electron*>::iterator iterEl;
  std::vector<MTree::Electron*>::iterator endEl = electrons.end();

  for (iterEl = electrons.begin(); iterEl != endEl; ++iterEl)
  {
    MTree::Electron* elec = *iterEl;

    if (elec ->Pt() < 15) continue;
    if (fabs(elec->Eta()) > 2.47) continue;
    if ((fabs(elec->Eta()) > 1.37) && (fabs(elec->Eta()) < 1.52)) continue;
    if ((elec->IsEM() & egammaPID::ElectronTightNoIsolation) != 0) continue;
    if (elec->ETCone30()/elec->Pt() > 0.2) continue;
    if (elec->TrackIso30()/elec->Pt() > 0.1) continue;
    if (elec->ETCone30() > 10) continue;
    if (elec->TrackIso30() > 10) continue;

    MTree::Track* track = elec->GetTrack();
    if (!track) continue;
    if (fabs(track->Vtx_D0()/track->Vtx_D0_Err()) > 10) continue;

    leptons.push_back(elec);
  }

  std::vector<MTree::Muon*>::iterator iterMu;
  std::vector<MTree::Muon*>::iterator endMu = muons.end();

  for (iterMu = muons.begin(); iterMu != endMu; ++iterMu)
  {
    MTree::Muon* muon = *iterMu;

    if (muon->Pt() < 15) continue;
    if (fabs(muon->Eta()) > 2.5) continue;
    if (muon->ETCone30()/muon->Pt() > 0.2) continue;
    if (muon->PTCone30()/muon->Pt() > 0.1) continue;
    if (muon->ETCone30() > 10) continue;
    if (muon->PTCone30() > 10) continue;

    MTree::Track* track = muon->GetTrack();
    if (!track) continue;
    if (fabs(track->Vtx_D0()/track->Vtx_D0_Err()) > 10) continue;

    if (muon->MatchChi2() > 100) continue;

    leptons.push_back(muon);
  }

  return(leptons);
}

std::vector<MTree::Lepton*> WWPreselection::wwCharge(std::vector<MTree::Lepton*> candidates)
{
  double charge = 0.0;

  std::vector<MTree::Lepton*>::iterator iter;
  std::vector<MTree::Lepton*>::iterator end = candidates.end();

  for (iter = candidates.begin(); iter != end; ++iter)
  {
    charge += (*iter)->Charge();
  }

  if (fabs(charge) == (int)candidates.size())
  {
    candidates.clear();
  }

  return(candidates);
}

int WWPreselection::wwJetCounter(std::vector<MTree::Jet*> jets)
{
  int pass = 0;

  std::vector<MTree::Jet*>::iterator iter;
  std::vector<MTree::Jet*>::iterator end = jets.end();

  for (iter = jets.begin(); iter != end; ++iter)
  {
    MTree::Jet* jet = *iter;
    if (jet->Pt() > 20)
    {
      if (fabs(jet->Eta()) < 3)
      {
	++pass;
      }
    }
  }
  return(pass);
}
