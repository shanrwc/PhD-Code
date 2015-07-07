#ifndef MutagMETcorrection_h
#define MutagMETcorrection_h

///////////////////////////////////////////////////////////
//  Code to correct MET to take into account MuTag muons //
//                                                       //
// contact: eve.le.menedeu@cern.ch                       //
///////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
// How to use it ?
//
// Two ways:
//  - enter the vector of muon parameters and get the correction value
//    that you need to substract to the original MET with getEtMissCorrection
//    MET - correction, what is available with getEtMissCorrected function
//
//  - doing yourself the loop on muons for each event:
//      1. reset()  -> initialize the TLorentzVector of the sum of MuTag muons
//      2. in the loop on muons:
//         addMuon(isLowPtReconstructed,pt,eta,phi,etcore,eloss,theta,ptcone20) 
//                  -> add the muons to the TLorentzVector
//      3. after the loop use getEtMissCorrection to get the correction
//      4. correct the MET by MET-correction or using getEtMissCorrected
//
// Enter the value before smearing inside the function. MET smearing correction 
// should be applied with all the other muons.
//
// Be consistent with unit: if you announce MeV (inGeV=false) all need to be enter
// in MeV. In that case MET should also be in MeV.
//
// The correction should be applied to MC and data.
//
////////////////////////////////////////////////////////////////////////////////////



#include <vector>
#include <utility>
#include <TLorentzVector.h>

using std::vector;
using std::pair;

class MutagMETcorrection
{
public:
  MutagMETcorrection(bool inGeV=false);
  ~MutagMETcorrection();

  void reset();
  void addMuon(const int isLowPtReconstructed, const float pt, const float eta, const float phi, const float etcore, const float elossPar, const float thetaTrk, const float ptcone20);

  TLorentzVector getEtMissCorrection() const;
  void getEtMissCorrection(float &metc_et, float &metc_phi) const;
  TLorentzVector getEtMissCorrection(const vector<int> *isLowPtReconstructed, const vector<float> *pt, const vector<float> *eta, const vector<float> *phi, const vector<float> *etcore, const vector<float> *elossPar, const vector<float> *thetaTrk, const vector<float> *ptcone20);
  TLorentzVector getEtMissCorrection(const vector<int> &isLowPtReconstructed, const vector<float> &pt, const vector<float> &eta, const vector<float> &phi, const vector<float> &etcore, const vector<float> &elossPar, const vector<float> &thetaTrk, const vector<float> &ptcone20);

  TLorentzVector getEtMissCorrected(const TLorentzVector &met);
  pair<float,float> getEtMissCorrected(const float met_et, const float met_phi);

private:
  float GeV;
  TLorentzVector m_sumtl;
};

#endif
