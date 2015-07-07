#ifndef __MTREE_ELECTRON_H
#define __MTREE_ELECTRON_H

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

class Electron : public Lepton {
 public:
  Electron();
  ~Electron();
  
  void Initialize(double EOverP, double ETCone20, double ETCone30, double ETCone40, 
		  double ETCone45, unsigned long int IsEM, unsigned long int IsGoodOQ, bool PassedOQ, 
		  float E233, float E237, float E277, float EtHad, 
		  float F1, float F1Core, float Fracs1, float Iso, bool TrackRefit, 
		  bool ConvTrackMatch, bool ConvAngleMatch, float ZVertex, float ErrZ, 
		  double ElectronWeight, double BgWeight, double NeuralNet, double HMatrix,
		  double HMatrix5, double IsolationLikelihood, double AdaBoost, 
		  double PhotonWeight, double BgPhotonWeight);
  void InitializeTrackIsolation(double TrackIso10, double TrackIso20, double TrackIso30, double TrackIso40, 
                                double PTCone20, double PTCone30, double PTCone40);
  void InitializeClusterVariables(float ClusterEta, float ClusterPhi, float ClusterE, float ClusterM, 
				  float ClusterPt, float ClusterEtaEMB2, float ClusterEtaEME2, 
				  float ClusterPhiEMB2, float ClusterPhiEME2, float ClusterBECombEta, 
				  float ClusterBECombPhi, float ClusterBECombE);
  void InitializeExtraVariables(float EtHad1, float WEta2, float E2Tsts1, float EMins1, float WTots1, 
				float WEta1, float DeltaEta0, float DeltaEta1, float DeltaEta2,  
				float DeltaEta3, float DeltaPhi0, float DeltaPhi1, float DeltaPhi2, 
				float DeltaPhi3, int NConversions, float EtaP);
  void InitializeElectronFlags(bool IsLoose, bool IsMedium, bool IsMediumIso, bool IsTight, 
			       bool IsTightIso, bool ExpectHitInBLayer);
  void InitializeEBoost(double EBoost) {_EBoost = EBoost;}
  void InitializeTruthClassifiers(int TruthParticleType, int TruthParticleOrigin);
  
  void SetMissingETWeights(float METWex, float METWey, float METWet);
  
  double EOverP() const {return _EOverP;}
  double ETCone20() const {return _ETCone20;}
  double ETCone30() const {return _ETCone30;}
  double ETCone40() const {return _ETCone40;}
  double ETCone45() const {return _ETCone45;}
  float E233() const {return _E233;}
  float E237() const {return _E237;}
  float E277() const {return _E277;}
  float EtHad() const {return _EtHad;}
  float F1() const {return _F1;}
  float F1Core() const {return _F1Core;}
  float Fracs1() const {return _Fracs1;}
  float Iso() const {return _Iso;}
  bool TrackRefit() const {return _TrackRefit;}
  float ZVertex() const {return _ZVertex;}
  float ErrZ() const {return _ErrZ;}
  float METWex() const {return _METWex;}
  float METWey() const {return _METWey;}
  float METWet() const {return _METWet;}

  bool IsConvertedPhoton() const {return _IsConvertedPhoton;}
  bool ConvTrackMatch() const {return _ConvTrackMatch;}
  bool ConvAngleMatch() const {return _ConvAngleMatch;}

  unsigned long int IsEM() const {return _IsEM;}
  unsigned long int IsGoodOQ() const {return _IsGoodOQ;}
  bool PassedOQ() const {return _PassedOQ;}
  double ElectronWeight() const {return _ElectronWeight;}
  double BgWeight() const {return _BgWeight;}
  double ElectronLikelihood() const;
  double NeuralNet() const {return _NeuralNet;}
  double HMatrix() const {return _HMatrix;}
  double HMatrix5() const {return _HMatrix5;}
  double IsolationLikelihood() const {return _IsolationLikelihood;}
  double AdaBoost() const {return _AdaBoost;}
  double PhotonWeight() const {return _PhotonWeight;}
  double BgPhotonWeight() const {return _BgPhotonWeight;}
  double PhotonLikelihood() const;
  double EBoost() const {return _EBoost;}
  
  float EtHad1() const {return _EtHad1;}
  float WEta2() const {return _WEta2;}
  float E2Tsts1() const {return _E2Tsts1;}
  float EMins1() const {return _EMins1;}
  float WTots1() const {return _WTots1;}
  float WEta1() const {return _WEta1;}
  float DeltaEta0() const {return _DeltaEta0;}
  float DeltaEta1() const {return _DeltaEta1;}
  float DeltaEta2() const {return _DeltaEta2;}
  float DeltaEta3() const {return _DeltaEta3;}
  float DeltaPhi0() const {return _DeltaPhi0;}
  float DeltaPhi1() const {return _DeltaPhi1;}
  float DeltaPhi2() const {return _DeltaPhi2;}
  float DeltaPhi3() const {return _DeltaPhi3;}
  float EtaP() const {return _EtaP;}

  double TrackIso10() const {return _TrackIso10;}
  double TrackIso20() const {return _TrackIso20;}
  double TrackIso30() const {return _TrackIso30;}
  double TrackIso40() const {return _TrackIso40;}
  double PTCone20() const {return _PTCone20;}
  double PTCone30() const {return _PTCone30;}
  double PTCone40() const {return _PTCone40;}

  float ClusterEta() const {return _ClusterEta;}
  float ClusterPhi() const {return _ClusterPhi;}
  float ClusterE() const {return _ClusterE;}
  float ClusterM() const {return _ClusterM;}
  float ClusterPt() const {return _ClusterPt;}
  float ClusterEtaEMB2() const {return _ClusterEtaEMB2;}
  float ClusterEtaEME2() const {return _ClusterEtaEME2;}
  float ClusterPhiEMB2() const {return _ClusterPhiEMB2;}
  float ClusterPhiEME2() const {return _ClusterPhiEME2;}
  float ClusterBECombEta() const {return _ClusterBECombEta;}
  float ClusterBECombPhi() const {return _ClusterBECombPhi;}
  float ClusterBECombE() const {return _ClusterBECombE;}

  int TruthParticleType() const {return _TruthParticleType;}
  int TruthParticleOrigin() const {return _TruthParticleOrigin;}

  bool IsLoose() const {return _IsLoose;}
  bool IsMedium() const {return _IsMedium;}
  bool IsMediumIso() const {return _IsMediumIso;}
  bool IsTight() const {return _IsTight;}
  bool IsTightIso() const {return _IsTightIso;}
  bool ExpectHitInBLayer() const {return _ExpectHitInBLayer;}

  int NConversions() const {return _NConversions;}
  
  void PrintData(std::ostream& s) const;

  ClassDef(Electron,1);
  
 private:

  Double_t _EOverP;
  Double_t _ETCone20;
  Double_t _ETCone30;
  Double_t _ETCone40;
  Double_t _ETCone45;
  Float_t _E233;
  Float_t _E237;
  Float_t _E277;
  Float_t _EtHad;
  Float_t _F1;
  Float_t _F1Core;
  Float_t _Fracs1;
  Float_t _Iso;
  Bool_t _TrackRefit;
  Float_t _ZVertex;
  Float_t _ErrZ;

  Bool_t _IsConvertedPhoton;
  Bool_t _ConvTrackMatch;
  Bool_t _ConvAngleMatch;

  ULong_t _IsEM;
  ULong_t _IsGoodOQ;
  Bool_t _PassedOQ;
  Double_t _ElectronWeight;
  Double_t _BgWeight;
  Double_t _NeuralNet;
  Double_t _HMatrix;
  Double_t _HMatrix5;
  Double_t _IsolationLikelihood;
  Double_t _AdaBoost;
  Double_t _PhotonWeight;
  Double_t _BgPhotonWeight;
  Double_t _EBoost;

  Float_t _EtHad1;
  Float_t _WEta2;
  Float_t _E2Tsts1;
  Float_t _EMins1;
  Float_t _WTots1;
  Float_t _WEta1;
  Float_t _DeltaEta0;
  Float_t _DeltaEta1;
  Float_t _DeltaEta2;
  Float_t _DeltaEta3;
  Float_t _DeltaPhi0;
  Float_t _DeltaPhi1;
  Float_t _DeltaPhi2;
  Float_t _DeltaPhi3;
  Float_t _EtaP;

  Float_t _TrackIso10;
  Float_t _TrackIso20;
  Float_t _TrackIso30;
  Float_t _TrackIso40;
  Float_t _PTCone10;
  Float_t _PTCone20;
  Float_t _PTCone30;
  Float_t _PTCone40;

  Float_t _ClusterEta;
  Float_t _ClusterPhi;
  Float_t _ClusterE;
  Float_t _ClusterM;
  Float_t _ClusterPt;
  Float_t _ClusterEtaEMB2;
  Float_t _ClusterEtaEME2;
  Float_t _ClusterPhiEMB2;
  Float_t _ClusterPhiEME2;
  Float_t _ClusterBECombEta;
  Float_t _ClusterBECombPhi;
  Float_t _ClusterBECombE;

  Bool_t _IsLoose;
  Bool_t _IsMedium;
  Bool_t _IsMediumIso;
  Bool_t _IsTight;
  Bool_t _IsTightIso;
  Bool_t _ExpectHitInBLayer;

  Int_t _TruthParticleType;
  Int_t _TruthParticleOrigin;

  Int_t _NConversions;

  Float_t _METWex;
  Float_t _METWey;
  Float_t _METWet;

};

}

#endif
