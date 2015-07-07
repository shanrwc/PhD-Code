#ifndef __MTREE_PHOTON_H
#define __MTREE_PHOTON_H

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

#include <iostream>

namespace MTree {

class Photon : public TLorentzVector {
 public:
  Photon();
  ~Photon();
  
  void Initialize(double Pt, double Eta, double Phi, double Mass, int Author, 
		  double EOverP, double ETCone20, double ETCone30, double ETCone40, 
		  double ETCone45, int IsEM, float E233, float E237, float E277, float EtHad, 
		  float F1, float F1Core, float Fracs1, float Iso, bool TrackRefit, 
		  bool ConvTrackMatch, bool ConvAngleMatch, float ZVertex, float ErrZ, 
		  double ElectronWeight, double BgWeight, double NeuralNet, double HMatrix,
		  double HMatrix5, double IsolationLikelihood, double AdaBoost, 
		  double PhotonWeight, double BgPhotonWeight);
  void InitializeClusterVariables(float ClusterEta, float ClusterPhi, float ClusterE, float ClusterM, 
				  float ClusterPt, float ClusterEtaEMB2, float ClusterEtaEME2, 
				  float ClusterPhiEMB2, float ClusterPhiEME2, float ClusterBECombEta, 
				  float ClusterBECombPhi, float ClusterBECombE);
  void InitializeExtraClusterVariables(float ETHad1, float WEta2, float EMaxs1, float EMaxs2, float EMin,
                                       float Fracm, float WTot, float W1);
  void InitializeConversionVariables(int ConvFlag, bool IsConv, int NConv, int NSingleTrackConv, int NDoubleTrackConv);
  void InitializeCorrectedIsolation(float ETCone40PtCorr, 
                                    float ETCone40EDCorr, 
                                    float ETCone40Corr);
  void InitializePhotonFlags(bool IsLoose, bool IsLooseAR, bool IsTight, bool IsTightIso, bool IsTightAR, bool IsTightARIso);
  void SetMissingETWeights(float METWex, float METWey, float METWet);
  int Author() const {return _Author;}
  double EOverP() const {return _EOverP;}
  double ETCone20() const {return _ETCone20;}
  double ETCone30() const {return _ETCone30;}
  double ETCone40() const {return _ETCone40;}
  double ETCone45() const {return _ETCone45;}
  double ETCone40PtCorr() const {return _ETCone40PtCorr;}
  double ETCone40EDCorr() const {return _ETCone40EDCorr;}
  double ETCone40Corr() const {return _ETCone40Corr;}
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
  float ETHad1() const {return _ETHad1;}
  float WEta2() const {return _WEta2;}
  float EMaxs1() const {return _EMaxs1;}
  float EMaxs2() const {return _EMaxs2;}
  float EMin() const {return _EMin;}
  float Fracm() const {return _Fracm;}
  float WTot() const {return _WTot;}
  float W1() const {return _W1;}

  int ConvFlag() const {return _ConvFlag;}
  bool IsConv() const {return _IsConv;}
  int NConv() const {return _NConv;}
  int NSingleTrackConv() const {return _NSingleTrackConv;}
  int NDoubleTrackConv() const {return _NDoubleTrackConv;}

  bool IsConvertedPhoton() const {return _IsConvertedPhoton;}
  bool ConvTrackMatch() const {return _ConvTrackMatch;}
  bool ConvAngleMatch() const {return _ConvAngleMatch;}

  int IsEM() const {return _IsEM;}
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

  bool IsLoose() const {return _IsLoose;}
  bool IsLooseAR() const {return _IsLooseAR;}
  bool IsTight() const {return _IsTight;}
  bool IsTightIso() const {return _IsTightIso;}
  bool IsTightAR() const {return _IsTightAR;}
  bool IsTightARIso() const {return _IsTightARIso;}

  void PrintData(std::ostream& s) const;

  ClassDef(Photon,1);
  
 private:

  Int_t _Author;
  Double_t _EOverP;
  Double_t _ETCone20;
  Double_t _ETCone30;
  Double_t _ETCone40;
  Double_t _ETCone45;
  Double_t _ETCone40PtCorr;
  Double_t _ETCone40EDCorr;
  Double_t _ETCone40Corr;
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

  Int_t _IsEM;
  Double_t _ElectronWeight;
  Double_t _BgWeight;
  Double_t _NeuralNet;
  Double_t _HMatrix;
  Double_t _HMatrix5;
  Double_t _IsolationLikelihood;
  Double_t _AdaBoost;
  Double_t _PhotonWeight;
  Double_t _BgPhotonWeight;

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
  Bool_t _IsLooseAR;
  Bool_t _IsTight;
  Bool_t _IsTightIso;
  Bool_t _IsTightAR;
  Bool_t _IsTightARIso;

  Float_t _METWex;
  Float_t _METWey;
  Float_t _METWet;

  Float_t _ETHad1;
  Float_t _WEta2;
  Float_t _EMaxs1;
  Float_t _EMaxs2;
  Float_t _EMin;
  Float_t _Fracm;
  Float_t _WTot;
  Float_t _W1;

  Int_t _ConvFlag;
  Bool_t _IsConv;
  Int_t _NConv;
  Int_t _NSingleTrackConv;
  Int_t _NDoubleTrackConv;
  
};

}

#endif
