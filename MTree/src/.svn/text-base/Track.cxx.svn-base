#include "MTree/Track.h"

namespace MTree {

Track::Track() : TLorentzVector(0.0, 0.0, 0.0, 0.0), _Charge(0), _NDOF(0), _Chi2(0.0),
		 _D0(0.0), _D0_Err(0.0), _Z0(0.0), _Z0_Err(0.0), _Vtx_D0(0.0), _Vtx_D0_Err(0.0), 
		 _Vtx_Z0(0.0), _Vtx_Z0_Err(0.0), _Prim_Vtx_D0(0.0), _Prim_Vtx_D0_Err(0.0), 
		 _Prim_Vtx_Z0(0.0), _Prim_Vtx_Z0_Err(0.0), _ExpectBLayerHit(0), _NrOfBLayerHits(0), 
                 _NrOfPixelHits(0), _NrOfPixelHoles(0), _NrOfPixelDeadSensors(0), _NrOfSCTHits(0), 
                 _NrOfSCTHoles(0), _NrOfSCTDeadSensors(0),
		 _NrOfTRTHits(0), _NrOfTRTOutliers(0), _NrOfTRTHighThresholdHits(0), _ParticleOrigin(0), 
		 _HitMask(0), _PrimaryVertex(0), _TrackAuthor(0) {
}

Track::~Track() {

}

void Track::Initialize(double Pt, double Eta, double Phi, int Charge, int NDOF, float Chi2,
		       float D0, float D0_Err, float Z0, float Z0_Err, float Vtx_D0, float Vtx_D0_Err, 
		       float Vtx_Z0, float Vtx_Z0_Err, float Prim_Vtx_D0, float Prim_Vtx_D0_Err, 
		       float Prim_Vtx_Z0, float Prim_Vtx_Z0_Err, bool ExpectBLayerHit, int NrOfBLayerHits, 
                       int NrOfPixelHits, int NrOfPixelHoles, int NrOfPixelDeadSensors, 
		       int NrOfSCTHits, int NrOfSCTHoles, int NrOfSCTDeadSensors, int NrOfTRTHits, 
                       int NrOfTRTOutliers, int NrOfTRTHighThresholdHits, int ParticleOrigin,
		       int HitMask) {
  SetPtEtaPhiM(Pt, Eta, Phi, 0.0);
  _Charge = Charge;
  _NDOF = NDOF;
  _Chi2 = Chi2;
  _D0 = D0;
  _D0_Err = D0_Err;
  _Z0 = Z0;
  _Z0_Err = Z0_Err;
  _Vtx_D0 = Vtx_D0;
  _Vtx_D0_Err = Vtx_D0_Err;
  _Vtx_Z0 = Vtx_Z0;
  _Vtx_Z0_Err = Vtx_Z0_Err;
  _Prim_Vtx_D0 = Prim_Vtx_D0;
  _Prim_Vtx_D0_Err = Prim_Vtx_D0_Err;
  _Prim_Vtx_Z0 = Prim_Vtx_Z0;
  _Prim_Vtx_Z0_Err = Prim_Vtx_Z0_Err;
  _NrOfBLayerHits = NrOfBLayerHits;
  _NrOfPixelHits = NrOfPixelHits;
  _NrOfSCTHits = NrOfSCTHits;
  _NrOfTRTHits = NrOfTRTHits;
  _NrOfTRTOutliers = NrOfTRTOutliers;
  _NrOfTRTHighThresholdHits = NrOfTRTHighThresholdHits;
  _ParticleOrigin = ParticleOrigin;
  _HitMask = HitMask;
  _ExpectBLayerHit      = ExpectBLayerHit;
  _NrOfPixelHoles       = NrOfPixelHoles;
  _NrOfPixelDeadSensors = NrOfPixelDeadSensors;
  _NrOfSCTHoles         = NrOfSCTHoles;
  _NrOfSCTDeadSensors   = NrOfSCTDeadSensors;
}

 
MTree::PrimaryVertex* Track::GetPrimaryVertex() const {
  if (!_PrimaryVertex.IsValid()) return 0;
  MTree::PrimaryVertex* vertex = (MTree::PrimaryVertex*) _PrimaryVertex.GetObject();
  return vertex;
}

std::vector<int> Track::DecodedHitMask() const {
  std::vector<int> hits;
  hits.reserve(21);
  for (int i = 0; i != 21; ++i) {
    int test = 1 << i;
    int result = test & _HitMask;
    if (result != 0) hits.push_back(1);
    else hits.push_back(0);      
  }
  return hits;
}

void Track::PrintData(std::ostream& s) const {

  std::vector<std::string> TrackAuthors;
  TrackAuthors.push_back(std::string("ID"));
  TrackAuthors.push_back(std::string("Muonboy"));
  TrackAuthors.push_back(std::string("Moore"));
  TrackAuthors.push_back(std::string("MuTag"));

  s << "*****\n* Track\n*****" << std::endl;
  s << "Track 4-vector [E,px,py,pz]   = [" << this->E() << ", " << this->Px() << ", " << this->Py()
    << ", " << this->Pz() << "]" << std::endl;
  s << "Track 4-vector [pT,eta,phi,m] = [" << this->Pt() << ", " << this->Eta() << ", " << this->Phi()
    << ", " << this->M() << "]" << std::endl;
  s <<   "Charge         : " << _Charge 
    << "\nTrackAuthor    : " << _TrackAuthor << " (" << TrackAuthors[_TrackAuthor] << " track)"  
    << "\nChi2           : " << _Chi2  
    << "\nNDOF           : " << _NDOF
    << "\nChi2/NDOF      : " << (_NDOF != 0 ? _Chi2 / _NDOF : 0.0) 
    << "\nd_0            : " << _D0 << " +- " << _D0_Err    
    << "\nz_0            : " << _Z0 << " +- " << _Z0_Err 
    << "\nVertex d_0     : " << _Vtx_D0 << " +- " << _Vtx_D0_Err    
    << "\nVertex z_0     : " << _Vtx_Z0 << " +- " << _Vtx_Z0_Err 
    << "\nVertex 0 d_0   : " << _Prim_Vtx_D0 << " +- " << _Prim_Vtx_D0_Err    
    << "\nVertex 0 z_0   : " << _Prim_Vtx_Z0 << " +- " << _Prim_Vtx_Z0_Err 
    << "\nBLayer Hits    : " << _NrOfBLayerHits 
    << "\nPixel Hits     : " << _NrOfPixelHits 
    << "\nSCT Hits       : " << _NrOfSCTHits 
    << "\nTRT Hits       : " << _NrOfTRTHits 
    << "\nTRT Outliers   : " << _NrOfTRTOutliers 
    << "\nTRT HT Hits    : " << _NrOfTRTHighThresholdHits 
    << "\nParticle Origin: " << _ParticleOrigin 
    << "\nHit Mask       : " << _HitMask
    << std::endl;
  std::vector<int> hits = DecodedHitMask();
  s << "Decoding the hit mask. This track has hits in the following layers: " << std::endl;
  s << "Pixel Barrel 0: " << hits[0] << std::endl;
  s << "Pixel Barrel 1: " << hits[1] << std::endl;
  s << "Pixel Barrel 2: " << hits[2] << std::endl;
  s << "Pixel Endcap 0: " << hits[3] << std::endl;
  s << "Pixel Endcap 1: " << hits[4] << std::endl;
  s << "Pixel Endcap 2: " << hits[5] << std::endl;
  s << "SCT Barrel 0  : " << hits[6] << std::endl;
  s << "SCT Barrel 1  : " << hits[7] << std::endl;
  s << "SCT Barrel 2  : " << hits[8] << std::endl;
  s << "SCT Barrel 3  : " << hits[9] << std::endl;
  s << "SCT Disk   0  : " << hits[10] << std::endl;
  s << "SCT Disk   1  : " << hits[11] << std::endl;
  s << "SCT Disk   2  : " << hits[12] << std::endl;
  s << "SCT Disk   3  : " << hits[13] << std::endl;
  s << "SCT Disk   4  : " << hits[14] << std::endl;
  s << "SCT Disk   5  : " << hits[15] << std::endl;
  s << "SCT Disk   6  : " << hits[16] << std::endl;
  s << "SCT Disk   7  : " << hits[17] << std::endl;
  s << "SCT Disk   8  : " << hits[18] << std::endl;
  s << "TRT Barrel    : " << hits[19] << std::endl;
  s << "TRT Endcap    : " << hits[20] << std::endl;
  MTree::PrimaryVertex* vertex = GetPrimaryVertex();
  s << "Vertex Address: " << vertex << std::endl;
  s << "*****\n" << std::endl;
}

}

ClassImp(MTree::Track)
