#ifndef __MTREE_JET_H
#define __MTREE_JET_H

#include "TROOT.h"
#include "TArrayD.h"
#include "TObject.h"
#include "TLorentzVector.h"
#include "TClonesArray.h"
#include "TMath.h"
#include "TError.h"
#include "TVectorD.h"
#include "TVector2.h"
#include "TVector3.h"
#include "TMatrix.h"
#include "TRotation.h"
#include <iostream>
#include <vector>
#include <map>

namespace MTree {

class Jet : public TLorentzVector {
 public:
  Jet();
  ~Jet();
  
  void Initialize(double Pt, double Eta, double Phi, double Mass, float IP3D, float SV0, 
		  float SV1, float SV2, float Weight, float CSIP, float JetProb,
		  float JetFitTag, float JetFitCOMB, float JetFitTagNN, float JetFitCOMBNN);
  void SetTrueFlavor(int TrueFlavor) {_TrueFlavor = TrueFlavor;}
  void SetJetFlavor(int JetFlavor) {_JetFlavor = JetFlavor;}
  void SetLeadingPartonPdgID(int LeadingPartonPdgID) {_LeadingPartonPdgID = LeadingPartonPdgID;}
  void SetLeadingPartonPt(double LeadingPartonPt) {_LeadingPartonPt = LeadingPartonPt;}

  void SetJetQualities(float JetEMFraction, /*float LArQuality, float HECQuality, float JetTiming,
		       int N90,*/ int N90Constituents, /*float NegativeE,*/
		       int SamplingMax, float SamplingMaxFrac, float HECFrac, float TileGap3Frac);
  void SetJetQualityFlags(bool IsGood, bool IsBad, bool IsUgly);
  void SetRawEnergies(float RawE, float RawPx, float RawPy, float RawPz/*, float CScaleE, float CScalePx, 
		      float CScalePy, float CScalePz, float CalE, float CalPx, float CalPy, float CalPz,
		      float EmJESFactor, float GCWJESFactor*/);
  void SetJVF(/*float JetVertexFraction, int NTracks, float SumPtTracks,*/ float newJetVertexFraction, int newNTracks, float newSumPtTracks);
  void SetMissingETWeights(float METWex, float METWey, float METWet);

//Many jet moment variables are moved to a map<string, double>.
//These are commented out, and the moment string is listed next to it.

  float IP3D() const {return _IP3D;}
  float SV0() const {return _SV0;}
  float SV1() const {return _SV1;}
  float SV2() const {return _SV2;}
  float Weight() const {return _Weight;}
  float CSIP() const {return _CSIP;}
  float JetProb() const {return _JetProb;}
  float JetFitTag() const {return _JetFitTag;}
  float JetFitCOMB() const {return _JetFitCOMB;}
  float JetFitTagNN() const {return _JetFitTagNN;}
  float JetFitCOMBNN() const {return _JetFitCOMBNN;}
  float METWex() const {return _METWex;}
  float METWey() const {return _METWey;}
  float METWet() const {return _METWet;}

  float JetEMFraction() const {return _JetEMFraction;}
  //float JetQuality() const {return LArQuality();} LArQuality
  //float LArQuality() const {return _LArQuality;} LArQuality
  //float HECQuality() const {return _HECQuality;} HECQuality
/*   float JetTime() const {return _JetTime;} */
  //float JetTiming() const {return _JetTiming;} Timing
  //int N90() const {return _N90;} n90
  int N90Tool() const {return _N90Tool;}
  int N90Constituents() const {return _N90Constituents;}
  //float NegativeE() const {return _NegativeE;} NegativeE
  int SamplingMax() const {return _SamplingMax;}
  float SamplingMaxFrac() const {return _SamplingMaxFrac;}
  float HECFrac() const {return _HECFrac;}
  float TileGap3Frac() const {return _TileGap3Frac;}
  //float JetVertexFraction() const {return _JetVertexFraction;} JVF
  //float SumPtTracks() const {return _SumPtTracks;} sumPtTrk
  //int NTracks() const {return _NTracks;} nTrk
  float newJetVertexFraction() const {return _newJetVertexFraction;}
  float newSumPtTracks() const {return _newSumPtTracks;}
  int newNTracks() const {return _newNTracks;}

  bool IsGood() const {return _IsGood;}
  bool IsBad() const {return _IsBad;}
  bool IsUgly() const {return _IsUgly;}

  float GetRawE() const {return _RawE;}
  float GetRawPt() const {return TMath::Sqrt(_RawPx*_RawPx + _RawPy*_RawPy);}
  float GetRawEta() const {return (-1)*TMath::Log(TMath::Tan(0.5*TMath::ATan(GetRawPt()/_RawPz)));}
  float GetRawPx() const {return _RawPx;}
  float GetRawPy() const {return _RawPy;}
  float GetRawPz() const {return _RawPz;}
/*   float GetCScaleE() const {return _CScaleE;} */
/*   float GetCScalePx() const {return _CScalePx;} */
/*   float GetCScalePy() const {return _CScalePy;} */
/*   float GetCScalePz() const {return _CScalePz;} */
/*   float GetCalE() const {return _CalE;} */
/*   float GetCalPx() const {return _CalPx;} */
/*   float GetCalPy() const {return _CalPy;} */
/*   float GetCalPz() const {return _CalPz;} */

  //float GetEmJESFactor() const {return _EmJESFactor;} EMJES
  //float GetGCWJESFactor() const {return _GCWJESFactor;} GCWJES

  int TrueFlavor() const {return _TrueFlavor;}
  int JetFlavor() const {return _JetFlavor;}

  int LeadingPartonPdgID() const {return _LeadingPartonPdgID;}
  double LeadingPartonPt() const {return _LeadingPartonPt;}

  void SetJetMoment(std::string name, float val) {_jetMoments[name] = val;}
  float JetMoment(std::string name) const {return _jetMoments[name];}

  void PrintData(std::ostream& s = std::cout) const;

  ClassDef(Jet,1);
  
 private:

  Float_t _IP3D;
  Float_t _SV0;
  Float_t _SV1;
  Float_t _SV2;
  Float_t _Weight;
  Float_t _CSIP;
  Float_t _JetProb;
  Float_t _JetFitTag;
  Float_t _JetFitCOMB;
  Float_t _JetFitTagNN;
  Float_t _JetFitCOMBNN;


  Float_t _JetEMFraction;
/*   Float_t _LArQuality; */
/*   Float_t _HECQuality; */
/*   Float_t _JetTime; */
/*   Float_t _JetTiming; */
/*   Int_t _N90; */
  Int_t _N90Tool;
  Int_t _N90Constituents;
/*   Float_t _NegativeE; */
  Int_t _SamplingMax;
  Float_t _SamplingMaxFrac; 
  Float_t _HECFrac;
  Float_t _TileGap3Frac;

  Bool_t _IsGood;
  Bool_t _IsBad;
  Bool_t _IsUgly;

  Float_t _RawE;
  Float_t _RawPx;
  Float_t _RawPy;
  Float_t _RawPz;
  Float_t _CScaleE;
  Float_t _CScalePx;
  Float_t _CScalePy;
  Float_t _CScalePz;
  Float_t _CalE;
  Float_t _CalPx;
  Float_t _CalPy;
  Float_t _CalPz;

/*   Float_t _EmJESFactor; */
/*   Float_t _GCWJESFactor; */

  Int_t _JetFlavor;
  Int_t _TrueFlavor;
  Int_t _LeadingPartonPdgID;
  Double_t _LeadingPartonPt;

/*   Double_t _JetVertexFraction; */
/*   Int_t _NTracks; */
/*   Double_t _SumPtTracks; */
  Double_t _newJetVertexFraction;
  Int_t _newNTracks;
  Double_t _newSumPtTracks;

  Float_t _METWex;
  Float_t _METWey;
  Float_t _METWet;

  mutable std::map<std::string, float> _jetMoments;
  
};

}

#endif
  
