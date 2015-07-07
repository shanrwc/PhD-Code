#include "MutagMETcorrection.h"

#include <TVector2.h>

MutagMETcorrection::MutagMETcorrection(bool inGeV)
{
  if(inGeV) GeV = 1.;
  else GeV = 1000.;
  reset();
}

MutagMETcorrection::~MutagMETcorrection()
{
}

void MutagMETcorrection::reset()
{
  m_sumtl.SetPtEtaPhiM(0,0,0,0);
}

// Add non smeared values
void MutagMETcorrection::addMuon(const int isLowPtReconstructed, const float pt, const float eta, const float phi, const float etcore, const float elossPar, const float thetaTrk, const float ptcone20)
{
  // Only select MuTag muons
  if(isLowPtReconstructed != 1) return;
  // MuTag muons inside transition region are already taken into account in MET
  if(fabs(eta) > 1 && fabs(eta) < 1.3) return;
  // Remove low pt MuTag muons as major source of background
  if(pt < 12*GeV) return;
  TLorentzVector tmp;
  if(ptcone20/pt < 0.1) {
    if(0.8*pt < etcore) return;
    tmp.SetPtEtaPhiM(pt-etcore,eta,phi,105.658/(1000./GeV));
  } else {
    tmp.SetPtEtaPhiM(pt-elossPar*sin(thetaTrk),eta,phi,105.658/(1000./GeV));
  }
  m_sumtl+=tmp;
}

TLorentzVector MutagMETcorrection::getEtMissCorrection() const
{
  return m_sumtl;
}

void MutagMETcorrection::getEtMissCorrection(float &metc_et, float &metc_phi) const
{
  metc_et = m_sumtl.Pt();
  metc_phi = m_sumtl.Phi();
}

// Add non smeared values
TLorentzVector MutagMETcorrection::getEtMissCorrection(const vector<int> *isLowPtReconstructed, const vector<float> *pt, const vector<float> *eta, const vector<float> *phi, const vector<float> *etcore, const vector<float> *elossPar,  const vector<float> *thetaTrk, const vector<float> *ptcone20)
{
  return getEtMissCorrection(*isLowPtReconstructed,*pt,*eta,*phi,*etcore,*elossPar,*thetaTrk,*ptcone20);
}

// Add non smeared values
TLorentzVector MutagMETcorrection::getEtMissCorrection(const vector<int> &isLowPtReconstructed, const vector<float> &pt, const vector<float> &eta, const vector<float> &phi, const vector<float> &etcore, const vector<float> &elossPar, const vector<float> &thetaTrk, const vector<float> &ptcone20)
{
  reset();
  for(unsigned int i=0; i<pt.size(); i++) {
    addMuon(isLowPtReconstructed.at(i),pt.at(i),eta.at(i),phi.at(i),etcore.at(i),elossPar.at(i),thetaTrk.at(i),ptcone20.at(i));
  }
  return m_sumtl;
}

// Get the MET corrected
TLorentzVector MutagMETcorrection::getEtMissCorrected(const TLorentzVector &met)
{
  TLorentzVector corr;
  corr.SetPtEtaPhiM(m_sumtl.Pt(),0,m_sumtl.Phi(),m_sumtl.M());
  return met-corr;
}

pair<float,float> MutagMETcorrection::getEtMissCorrected(const float met_et, const float met_phi)
{
  float met_px = met_et*cos(met_phi);
  float met_py = met_et*sin(met_phi);
  TVector2 met(met_px,met_py);
  TVector2 corr(m_sumtl.Px(),m_sumtl.Py());
  pair<float,float> metp((met-corr).Mod(),(met-corr).Phi());
  return metp;
}
