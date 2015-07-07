#ifndef __MTREE_TRACK_H
#define __MTREE_TRACK_H

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

#include <vector>
#include <iostream>
#include "MTree/PrimaryVertex.h"

namespace MTree {

class Track : public TLorentzVector {
 public:
  Track();
  ~Track();
  
  void Initialize(double Pt, double Eta, double Phi, int Charge, int NDOF, float Chi2,
		  float D0, float D0_Err, float Z0, float Z0_Err, float Vtx_D0, float Vtx_D0_Err, 
		  float Vtx_Z0, float Vtx_Z0_Err, float Prim_Vtx_D0, float Prim_Vtx_D0_Err, 
		  float Prim_Vtx_Z0, float Prim_Vtx_Z0_Err, bool ExpectBLayerHit, int NrOfBLayerHits, 
                  int NrOfPixelHits, int NrOfPixelHoles, int NrOfPixelDeadSensors, 
		  int NrOfSCTHits, int NrOfSCTHoles, int NrOfSCTDeadSensors, int NrOfTRTHits, 
                  int NrOfTRTOutliers, int NrOfTRTHighThresholdHits, int ParticleOrigin,
		  int HitMask);
  void SetPrimaryVertex(MTree::PrimaryVertex* Vertex) {_PrimaryVertex = Vertex;}
  void SetTrackAuthor(int TrackAuthor) {_TrackAuthor = TrackAuthor;}
  int Charge() const {return _Charge;}
  int NDOF() const {return _NDOF;}

  float Chi2() const {return _Chi2;}

  float D0() const {return _D0;}
  float D0_Err() const {return _D0_Err;}
  float Z0() const {return _Z0;}
  float Z0_Err() const {return _Z0_Err;}
  float Vtx_D0() const {return _Vtx_D0;}
  float Vtx_D0_Err() const {return _Vtx_D0_Err;}
  float Vtx_Z0() const {return _Vtx_Z0;}
  float Vtx_Z0_Err() const {return _Vtx_Z0_Err;}
  float Prim_Vtx_D0() const {return _Prim_Vtx_D0;}
  float Prim_Vtx_D0_Err() const {return _Prim_Vtx_D0_Err;}
  float Prim_Vtx_Z0() const {return _Prim_Vtx_Z0;}
  float Prim_Vtx_Z0_Err() const {return _Prim_Vtx_Z0_Err;}

  bool ExpectBLayerHit()         const {return _ExpectBLayerHit;}
  int NrOfBLayerHits()           const {return _NrOfBLayerHits;}
  int NrOfPixelHits()            const {return _NrOfPixelHits;}
  int NrOfPixelHoles()           const {return _NrOfPixelHoles;}
  int NrOfPixelDeadSensors()     const {return _NrOfPixelDeadSensors;}
  int NrOfSCTHits()              const {return _NrOfSCTHits;}
  int NrOfSCTHoles()             const {return _NrOfSCTHoles;}
  int NrOfSCTDeadSensors()       const {return _NrOfSCTDeadSensors;}
  int NrOfTRTHits()              const {return _NrOfTRTHits;}
  int NrOfTRTOutliers()          const {return _NrOfTRTOutliers;}
  int NrOfTRTHighThresholdHits() const {return _NrOfTRTHighThresholdHits;}

  int ParticleOrigin() const {return _ParticleOrigin;}
  MTree::PrimaryVertex* GetPrimaryVertex() const;
  
  //Look in PrintData to understand the meaning of the bit mask (decoded or not)
  int HitMask() const {return _HitMask;}
  std::vector<int> DecodedHitMask() const;

  // 0 = ID Track
  // 1 = Muonboy
  // 2 = Moore
  // 3 = MuTag
  int TrackAuthor() const {return _TrackAuthor;}
  
  void PrintData(std::ostream& s) const;

  ClassDef(Track,1);
  
 private:

  Int_t _Charge;
  Int_t _NDOF;

  Float_t _Chi2;

  Float_t _D0;
  Float_t _D0_Err;
  Float_t _Z0;
  Float_t _Z0_Err;
  Float_t _Vtx_D0;
  Float_t _Vtx_D0_Err;
  Float_t _Vtx_Z0;
  Float_t _Vtx_Z0_Err;
  Float_t _Prim_Vtx_D0;
  Float_t _Prim_Vtx_D0_Err;
  Float_t _Prim_Vtx_Z0;
  Float_t _Prim_Vtx_Z0_Err;

  Bool_t _ExpectBLayerHit;
  Int_t _NrOfBLayerHits;
  Int_t _NrOfPixelHits;
  Int_t _NrOfPixelHoles;
  Int_t _NrOfPixelDeadSensors;
  Int_t _NrOfSCTHits;
  Int_t _NrOfSCTHoles;
  Int_t _NrOfSCTDeadSensors;
  Int_t _NrOfTRTHits;
  Int_t _NrOfTRTOutliers;
  Int_t _NrOfTRTHighThresholdHits;

  Int_t _ParticleOrigin;

  Int_t _HitMask;

  TRef _PrimaryVertex;
  Int_t _TrackAuthor;
};

}

#endif
