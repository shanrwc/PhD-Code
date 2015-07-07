#include "TopologicalVarsClass.h"

TopologicalVarsClass::TopologicalVarsClass()
{
  lepton1_pt = 0;
  lepton1_eta = 0;
  lepton1_phi = 0;
  lepton2_pt = 0;
  lepton2_eta = 0;
  lepton2_phi = 0;
  dilep_pt = 0;
  missingET = 0;
  mll = 0;
  mllnu = 0;
  deltaPhi = 0;
  n_jets = 0;
  xsec = 0;
  sampleType = 0;
}

TopologicalVarsClass::~TopologicalVarsClass()
{
  ;
}

double TopologicalVarsClass::getLepOnePt() const
{
  return(lepton1_pt);
}

double TopologicalVarsClass::getLepTwoPt() const
{
  return(lepton2_pt);
}

double TopologicalVarsClass::getDiLepPt() const
{
  return(dilep_pt);
}

double TopologicalVarsClass::getMissingET() const
{
  return(missingET);
}

double TopologicalVarsClass::getMLL() const
{
  return(mll);
}

double TopologicalVarsClass::getMLLNu() const
{
  return(mllnu);
}

double TopologicalVarsClass::getDeltaPhi() const
{
  return(deltaPhi);
}

double TopologicalVarsClass::getNJets() const
{
  return(n_jets);
}

double TopologicalVarsClass::getXSection() const
{
  return(xsec);
}

double TopologicalVarsClass::getSampleType() const
{
  return(sampleType);
}

void TopologicalVarsClass::calculateVars(MTree::MissingET* MET, 
                                         MTree::Lepton* lepton1,
                                         MTree::Lepton* lepton2,
					 double eventWeight,
					 string name, double nJets)
{
  //Set all lepton variables.
  if (lepton1->LeptonPdgID() == lepton2->LeptonPdgID())
  {
    if (lepton1->Pt() > lepton2->Pt())
    {
      lepton1_pt = lepton1->Pt();
      lepton1_eta = lepton1->Eta();
      lepton1_phi = lepton1->Phi();
      lepton2_pt = lepton2->Pt();
      lepton2_eta = lepton2->Eta();
      lepton2_phi = lepton2->Phi();
    }
    else
    {
      lepton1_pt = lepton2->Pt();
      lepton1_eta = lepton2->Eta();
      lepton1_phi = lepton2->Phi();
      lepton2_pt = lepton1->Pt();
      lepton2_eta = lepton1->Eta();
      lepton2_phi = lepton1->Phi();
    }
  }
  else
  {
    if (fabs(lepton1->LeptonPdgID()) < fabs(lepton2->LeptonPdgID()))
    {
      lepton1_pt = lepton1->Pt();
      lepton1_eta = lepton1->Eta();
      lepton1_phi = lepton1->Phi();
      lepton2_pt = lepton2->Pt();
      lepton2_eta = lepton2->Eta();
      lepton2_phi = lepton2->Phi();
    } 
    else
    {
      lepton1_pt = lepton2->Pt();
      lepton1_eta = lepton2->Eta();
      lepton1_phi = lepton2->Phi();
      lepton2_pt = lepton1->Pt();
      lepton2_eta = lepton1->Eta();
      lepton2_phi = lepton1->Phi();
    }
  }

  //Set sampleType variable.
  if (name == "ggf2l170higgs")
  {
    sampleType = 1;
  }
  if (name == "2lsmww")
  {
    sampleType = 2;
  }
  if (name == "2lttbar")
  {
    sampleType = 3;
  }

  //Set xsec variable. 
  xsec = eventWeight;

  //Set missingET variable.
  missingET = MET->MET();

  //Set invariant lepton mass variable and dilepton pt.
  TLorentzVector ll = (TLorentzVector)(*lepton1 + *lepton2);
  mll = ll.M();
  dilep_pt = ll.Pt();

  //Set other invariant lepton mass variable.
  double ptm = MET->Pt();
  double dPhi = ll.DeltaPhi(*MET);

  mllnu = sqrt(2*ll.Pt()*ptm*(1-cos(dPhi)));

  //Set deltaPhi variable.
  deltaPhi = lepton1->DeltaPhi(*lepton2);

  //Set nJets variable.
  n_jets = nJets;
}

void TopologicalVarsClass::clear()
{
  lepton1_pt = 0;
  lepton1_eta = 0;
  lepton1_phi = 0;
  lepton2_pt = 0;
  lepton2_eta = 0;
  lepton2_phi = 0;
  dilep_pt = 0;
  missingET = 0;
  mll = 0;
  mllnu = 0;
  deltaPhi = 0;
  n_jets = 0;
  xsec = 0;
  sampleType = 0;
}

ClassImp(TopologicalVarsClass)
