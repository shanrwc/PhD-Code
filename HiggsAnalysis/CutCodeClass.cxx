#include "CutCodeClass.h"

//########################################################################################

CutCodeClass::CutCodeClass()
{
  ;
}

//########################################################################################

CutCodeClass::~CutCodeClass()
{
  ;
}

//#######################################################################################

double CutCodeClass::getPtll(std::vector<MTree::Lepton*>& leptons)
{
  double llx = leptons[0]->Px() + leptons[1]->Px();
  double lly = leptons[0]->Py() + leptons[1]->Py();
  double ptll = sqrt(llx*llx + lly*lly);

  return(ptll);
}

//########################################################################################

double CutCodeClass::getPtTotal(std::vector<MTree::Lepton*>& leptons, std::vector<MTree::Jet*>& jets, MTree::MissingET* met)
{

  TLorentzVector total;
  total = (TLorentzVector)*met;

  for (int i = 0; i != (int)leptons.size(); ++i)
  {
    total +=(TLorentzVector)*leptons[i];
  }
  for (int i = 0; i != (int)jets.size(); ++i)
  {
    total += (TLorentzVector)*jets[i];
  }

//   double xterms = 0.0;
//   double yterms = 0.0;

//   for (int i = 0; i != (int)leptons.size(); ++i)
//   {
//     xterms += leptons[i]->Px();
//     yterms += leptons[i]->Py();
//   }
//   for (int i = 0; i != (int)jets.size(); ++i)
//   {
//     xterms += jets[i]->Px();
//     yterms += jets[i]->Py();
//   }
//   xterms += met->Px();
//   yterms += met->Py();

//   double ptTot = sqrt(xterms*xterms + yterms*yterms);

  double ptTot = total.Pt();
  return(ptTot);
}

//#######################################################################################

double CutCodeClass::getTauTauMass(TLorentzVector& lep1, TLorentzVector& lep2, TLorentzVector& met)
{
  double dphi1 = met.DeltaPhi(lep1);
  double dphi2 = met.DeltaPhi(lep2);

  double lep1_pt = lep1.Pt();
  double lep2_pt = lep2.Pt();
  double met_et = sqrt(met.Px()*met.Px() + met.Py()*met.Py());

  double ratio1 = TMath::Sin(dphi2) != 0.0 ? TMath::Sin(dphi1) / TMath::Sin(dphi2) : 0.0;
  double angleTerm = TMath::Cos(dphi1) - ratio1 * TMath::Cos(dphi2);

  double v1 = angleTerm != 0.0 ? met_et / angleTerm : 0.0;
  double v2 = -v1 * ratio1;

  double denom1 = lep1_pt + v1;
  double denom2 = lep2_pt + v2;

  double x1 = denom1 != 0.0 ? lep1_pt / denom1 : 0.0;
  double x2 = denom2 != 0.0 ? lep2_pt / denom2 : 0.0;


  if ((x1 > 0.0 && x2 > 0.0) /*&& (x1 < 1.0 && x2 < 1.0)*/) {
    return (lep1+lep2).M() / TMath::Sqrt(x1*x2);
  }

  return 0.;
}

//#######################################################################################

double CutCodeClass::getMT(std::vector<MTree::Lepton*>& leptons, MTree::MissingET* MET)
{
  if ((int)leptons.size() > 1)
  {
    double met = MET->Pt();

    TLorentzVector dilep = (TLorentzVector)*leptons[0] + (TLorentzVector)*leptons[1];
    double pTll = dilep.Pt();
    double mll = dilep.M();
    double ET = sqrt(pTll*pTll + mll*mll);

    double sumPt = (*MET+dilep).Pt();
    //    double sumPt = sqrt((MET.Px()+dilep.Px())*(MET.Px()+dilep.Px()) + ((MET.Py()+dilep.Py())*(MET.Py()+dilep.Py())));

    double mT = sqrt((ET+met)*(ET+met) - (sumPt*sumPt));

    return(mT);
  }
  else
  {
    return(0);
  }
}

//#######################################################################################

bool CutCodeClass::failJetPtSV0(std::vector<MTree::Jet*>& jets)
{
  bool fail = false;

  for (int i = 0; i != (int)jets.size(); ++i)
  {
    if (jets[i]->Pt() > 25 && jets[i]->JetFitCOMBNN() > 0.35)
    {
      fail = true;
    }
  }

  return(fail);
}

//#######################################################################################

bool CutCodeClass::failThirdCentralJet(std::vector<MTree::Jet*>& jets)
{
  bool fail = false;

  for (int i = 2; i != (int)jets.size(); ++i)
  {
    if (jets[i]->Pt() > 25 && TMath::Abs(jets[i]->Eta()) < 3.2)
    {
      fail = true;
    }
  }

  return(fail);
}

//#######################################################################################
