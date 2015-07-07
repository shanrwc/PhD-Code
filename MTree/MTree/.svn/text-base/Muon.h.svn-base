#ifndef __MTREE_MUON_H
#define __MTREE_MUON_H

#include "TROOT.h"
#include "TObject.h"
#include "TLorentzVector.h"
#include "TMath.h"
#include "TError.h"
#include "TVector2.h"
#include "TVector3.h"
#include "TMatrix.h"
#include "TRotation.h"
#include "TRef.h"

#include "MTree/Lepton.h"
#include <iostream>

namespace MTree {

class Muon : public Lepton {
 public:
  Muon();
  ~Muon();
  
  void Initialize(double ETCone10, double ETCone20, double ETCone30, double ETCone40, 
		  double PTCone10, double PTCone20, double PTCone30, double PTCone40, 
		  double FitChi2, double FitChi2OverDoF, double MatchChi2, 
		  double MatchChi2OverDoF, double EnergyLoss, double EtCore, bool BestMatch,
		  bool IsCombinedMuon, bool IsStandAloneMuon, bool IsLowPtReconstructedMuon, bool IsCaloMuon,
		  int NrMDTHits, int NrCSCEtaHits, int NrCSCPhiHits, int NrRPCEtaHits,
		  int NrRPCPhiHits, int NrTGCEtaHits, int NrTGCPhiHits, int NrOfSegments);
  void InitializeTrackIsolation(double TrackIso10, double TrackIso20, double TrackIso30, double TrackIso40);
  void InitializeMuonQuality(bool IsLoose, bool IsMedium, bool IsTight, float IsMuonLikelihood);
  void SetMissingETWeights(float METWex, float METWey, float METWet);

  double ETCone10() const {return _ETCone10;}
  double ETCone20() const {return _ETCone20;}
  double ETCone30() const {return _ETCone30;}
  double ETCone40() const {return _ETCone40;}
  double PTCone10() const {return _PTCone10;}
  double PTCone20() const {return _PTCone20;}
  double PTCone30() const {return _PTCone30;}
  double PTCone40() const {return _PTCone40;}
  double FitChi2() const {return _FitChi2;}
  double FitChi2OverDoF() const {return _FitChi2OverDoF;}
  double MatchChi2() const {return _MatchChi2;}
  double MatchChi2OverDoF() const {return _MatchChi2OverDoF;}
  double EnergyLoss() const {return _EnergyLoss;}
  double EtCore() const {return _EtCore;}

  bool BestMatch() const {return _BestMatch;}
  bool IsCombinedMuon() const {return _IsCombinedMuon;}
  bool IsStandAloneMuon() const {return _IsStandAloneMuon;}
  bool IsLowPtReconstructedMuon() const {return _IsLowPtReconstructedMuon;} 
  bool IsCaloMuon() const {return _IsCaloMuon;}
  int NrMDTHits() const {return _NrMDTHits;}
  int NrCSCEtaHits() const {return _NrCSCEtaHits;}
  int NrCSCPhiHits() const {return _NrCSCPhiHits;}
  int NrRPCEtaHits() const {return _NrRPCEtaHits;}
  int NrRPCPhiHits() const {return _NrRPCPhiHits;}
  int NrTGCEtaHits() const {return _NrTGCEtaHits;}
  int NrTGCPhiHits() const {return _NrTGCPhiHits;}
  int NrOfSegments() const {return _NrOfSegments;}

  float METWex() const {return _METWex;}
  float METWey() const {return _METWey;}
  float METWet() const {return _METWet;}

  double TrackIso10() const {return _TrackIso10;}
  double TrackIso20() const {return _TrackIso20;}
  double TrackIso30() const {return _TrackIso30;}
  double TrackIso40() const {return _TrackIso40;}

  bool IsLoose() const {return _IsLoose;}
  bool IsMedium() const {return _IsMedium;}
  bool IsTight() const {return _IsTight;}
  float IsMuonLikelihood() const {return _IsMuonLikelihood;}

  void SetStandAloneTrack(MTree::Track* Track) {_StandAloneTrack = Track;}
  MTree::Track* GetStandAloneTrack() const;

  void PrintData(std::ostream& s) const;

  ClassDef(Muon,1);
  
 private:
  

  Double_t _ETCone10;
  Double_t _ETCone20;
  Double_t _ETCone30;
  Double_t _ETCone40;
  Double_t _PTCone10;
  Double_t _PTCone20;
  Double_t _PTCone30;
  Double_t _PTCone40;
  Double_t _FitChi2;
  Double_t _FitChi2OverDoF;
  Double_t _MatchChi2;
  Double_t _MatchChi2OverDoF;
  Double_t _EnergyLoss;
  Double_t _EtCore;
	       
  Bool_t _BestMatch;
  Bool_t _IsCombinedMuon;
  Bool_t _IsStandAloneMuon;
  Bool_t _IsLowPtReconstructedMuon;
  Bool_t _IsCaloMuon;
  Int_t _NrMDTHits;
  Int_t _NrCSCEtaHits;
  Int_t _NrCSCPhiHits;
  Int_t _NrRPCEtaHits;
  Int_t _NrRPCPhiHits;
  Int_t _NrTGCEtaHits;
  Int_t _NrTGCPhiHits;
  Int_t _NrOfSegments;

  Double_t _TrackIso10;
  Double_t _TrackIso20;
  Double_t _TrackIso30;
  Double_t _TrackIso40;

  Bool_t _IsLoose;
  Bool_t _IsMedium;
  Bool_t _IsTight;
  Float_t _IsMuonLikelihood;
  
  TRef _StandAloneTrack;

  Float_t _METWex;
  Float_t _METWey;
  Float_t _METWet;
};

}

#endif
