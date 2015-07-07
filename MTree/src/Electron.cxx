#include "MTree/Electron.h"

namespace MTree {

Electron::Electron() : Lepton(), _EOverP(0.0), _ETCone20(0.0), _ETCone30(0.0), _ETCone40(0.0), _ETCone45(0.0), 
		       _E233(0.0), _E237(0.0), _E277(0.0), _EtHad(0.0), _F1(0.0), _F1Core(0.0), _Fracs1(0.0), 
		       _Iso(0.0), _ZVertex(0.0), _ErrZ(0.0), _IsConvertedPhoton(0), _ConvTrackMatch(0), 
		       _ConvAngleMatch(0), _IsEM(0), _IsGoodOQ(0), _ElectronWeight(0.0), _BgWeight(0.0), _NeuralNet(0.0), 
		       _HMatrix(0.0), _HMatrix5(0.0), _IsolationLikelihood(0.0), _AdaBoost(0.0), _PhotonWeight(0.0), 
		       _BgPhotonWeight(0.0), _EBoost(0.0), _EtHad1(0.0), _WEta2(0.0), _E2Tsts1(0.0), _EMins1(0.0), 
		       _WTots1(0.0), _WEta1(0.0), _DeltaEta0(0.0), _DeltaEta1(0.0), 
		       _DeltaEta2(0.0), _DeltaEta3(0.0), _DeltaPhi0(0.0), _DeltaPhi1(0.0), _DeltaPhi2(0.0), 
		       _DeltaPhi3(0.0), _EtaP(0.0), _TrackIso10(0.0), _TrackIso20(0.0), _TrackIso30(0.0), _TrackIso40(0.0),
		       _PTCone20(0.0), _PTCone30(0.0), _PTCone40(0.0), 
		       _ClusterEta(0.0), _ClusterPhi(0.0), _ClusterE(0.0), _ClusterM(0.0), _ClusterPt(0.0), 
		       _ClusterEtaEMB2(0.0), _ClusterEtaEME2(0.0), _ClusterPhiEMB2(0.0), _ClusterPhiEME2(0.0), 
		       _ClusterBECombEta(0.0), _ClusterBECombPhi(0.0), _ClusterBECombE(0.0), _IsLoose(0), 
		       _IsMedium(0), _IsMediumIso(0), _IsTight(0), _IsTightIso(0), _ExpectHitInBLayer(0), 
		       _TruthParticleType(0), _TruthParticleOrigin(0), _NConversions(0), 
                       _METWex(0.0), _METWey(0.0), _METWet(0.0) {
}

Electron::~Electron() {

}

void Electron::Initialize(double EOverP, double ETCone20, double ETCone30, double ETCone40, 
			  double ETCone45, unsigned long int IsEM, unsigned long int IsGoodOQ, bool PassedOQ,
			  float E233, float E237, float E277, float EtHad, float F1, 
			  float F1Core, float Fracs1, float Iso, bool TrackRefit, bool ConvTrackMatch, 
			  bool ConvAngleMatch, float ZVertex, float ErrZ, double ElectronWeight, double BgWeight, 
			  double NeuralNet, double HMatrix, double HMatrix5, double IsolationLikelihood, 
			  double AdaBoost, double PhotonWeight, double BgPhotonWeight) {
  _EOverP = EOverP;
  _ETCone20 = ETCone20;
  _ETCone30 = ETCone30;
  _ETCone40 = ETCone40;
  _ETCone45 = ETCone45;
  _IsEM = IsEM;
  _IsGoodOQ = IsGoodOQ;
  _PassedOQ = PassedOQ;
  _E233 = E233;
  _E237 = E237;
  _E277 = E277;
  _EtHad = EtHad;
  _F1 = F1;
  _F1Core = F1Core;
  _Fracs1 = Fracs1;
  _Iso = Iso;
  _TrackRefit = TrackRefit;
  _ConvTrackMatch = ConvTrackMatch;
  _ConvAngleMatch = ConvAngleMatch;
  _ZVertex = ZVertex;
  _ErrZ = ErrZ;
  _ElectronWeight = ElectronWeight;
  _BgWeight = BgWeight;
  _NeuralNet = NeuralNet;
  _HMatrix = HMatrix;
  _HMatrix5 = HMatrix5;
  _IsolationLikelihood = IsolationLikelihood;
  _AdaBoost = AdaBoost;
  _PhotonWeight = PhotonWeight;
  _BgPhotonWeight = BgPhotonWeight;
}

  void Electron::InitializeTrackIsolation(double TrackIso10, double TrackIso20, double TrackIso30, double TrackIso40, 
					  double PTCone20, double PTCone30, double PTCone40) {
  _TrackIso10 = TrackIso10;
  _TrackIso20 = TrackIso20;
  _TrackIso30 = TrackIso30;
  _TrackIso40 = TrackIso40;
  _PTCone20 = PTCone20;
  _PTCone30 = PTCone30;
  _PTCone40 = PTCone40;
}

void Electron::InitializeClusterVariables(float ClusterEta, float ClusterPhi, float ClusterE, float ClusterM, float ClusterPt, float ClusterEtaEMB2, float ClusterEtaEME2, float ClusterPhiEMB2, float ClusterPhiEME2, float ClusterBECombEta, float ClusterBECombPhi, float ClusterBECombE) {
  _ClusterEta = ClusterEta;
  _ClusterPhi = ClusterPhi;
  _ClusterE = ClusterE;
  _ClusterM = ClusterM;
  _ClusterPt = ClusterPt;
  _ClusterEtaEMB2 = ClusterEtaEMB2;
  _ClusterEtaEME2 = ClusterEtaEME2;
  _ClusterPhiEMB2 = ClusterPhiEMB2;
  _ClusterPhiEME2 = ClusterPhiEME2;
  _ClusterBECombEta = ClusterBECombEta;
  _ClusterBECombPhi = ClusterBECombPhi;
  _ClusterBECombE = ClusterBECombE;
}

void Electron::InitializeExtraVariables(float EtHad1, float WEta2, float E2Tsts1, float EMins1, float WTots1, 
					float WEta1, float DeltaEta0, float DeltaEta1, float DeltaEta2, 
					float DeltaEta3, float DeltaPhi0, float DeltaPhi1, 
					float DeltaPhi2, float DeltaPhi3, int NConversions, float EtaP) {
  _EtHad1 = EtHad1;
  _WEta2 = WEta2;
  _E2Tsts1 = E2Tsts1;
  _EMins1 = EMins1;
  _WTots1 = WTots1;
  _WEta1 = WEta1;
  _DeltaEta0 = DeltaEta0;
  _DeltaEta1 = DeltaEta1;
  _DeltaEta2 = DeltaEta2;
  _DeltaEta3 = DeltaEta3;
  _DeltaPhi0 = DeltaPhi0;
  _DeltaPhi1 = DeltaPhi1;
  _DeltaPhi2 = DeltaPhi2;
  _DeltaPhi3 = DeltaPhi3;
  _NConversions = NConversions;
  _EtaP = EtaP;
}

  void Electron::InitializeElectronFlags(bool IsLoose, bool IsMedium, bool IsMediumIso, bool IsTight, bool IsTightIso, bool ExpectHitInBLayer) {
  _IsLoose = IsLoose;
  _IsMedium = IsMedium;
  _IsMediumIso = IsMediumIso;
  _IsTight = IsTight;
  _IsTightIso = IsTightIso;
  _ExpectHitInBLayer = ExpectHitInBLayer;
}

void Electron::InitializeTruthClassifiers(int TruthParticleType, int TruthParticleOrigin) {
  _TruthParticleType = TruthParticleType;
  _TruthParticleOrigin = TruthParticleOrigin;
}

double Electron::ElectronLikelihood() const {
  if (_BgWeight == 0.0) return -999.0;
  return TMath::Log(_ElectronWeight / _BgWeight);
}

double Electron::PhotonLikelihood() const {
  if (_BgPhotonWeight == 0.0) return -999.0;
  return TMath::Log(_PhotonWeight / _BgPhotonWeight);
}

void Electron::SetMissingETWeights(float METWex, float METWey, float METWet)
{
  _METWex = METWex;
  _METWey = METWey;
  _METWet = METWet;
}

void Electron::PrintData(std::ostream& s) const {
  s << "*****\n* Electron\n*****" << std::endl;
  PrintLeptonData(s);
  s <<   "EOverP         : " << _EOverP 
    << "\nETCone20       : " << _ETCone20 
    << "\nETCone30       : " << _ETCone30 
    << "\nETCone40       : " << _ETCone40 
    << "\nETCone45       : " << _ETCone45 
    << "\nE233           : " << _E233 
    << "\nE237           : " << _E237 
    << "\nE277           : " << _E277 
    << "\nEtHad          : " << _EtHad 
    << "\nF1             : " << _F1 
    << "\nF1Core         : " << _F1Core 
    << "\nFracs1         : " << _Fracs1 
    << "\nIso            : " << _Iso 
    << "\nTrackRefit     : " << _TrackRefit
    << "\nConvTrackMatch : " << _ConvTrackMatch 
    << "\nConvAngleMatch : " << _ConvAngleMatch
    << "\nZVertex        : " << _ZVertex 
    << "\nErrZ           : " << _ErrZ
    << "\nIsConvertedPhot: " << _IsConvertedPhoton 
    << "\nIsEM           : " << _IsEM 
    << "\nIsGoodOQ       : " << _IsGoodOQ 
    << "\nPassedOQ       : " << _PassedOQ 
    << "\nElectron Weight: " << _ElectronWeight 
    << "\nBg Weight      : " << _BgWeight
    << "\nElectron LHood : " << ElectronLikelihood() 
    << "\nNeural Net     : " << _NeuralNet
    << "\nHMatrix        : " << _HMatrix 
    << "\nHMatrix5       : " << _HMatrix5 
    << "\nIsoLikelihood  : " << _IsolationLikelihood 
    << "\nAdaBoost       : " << _AdaBoost
    << "\nPhoton Weight  : " << _PhotonWeight 
    << "\nBgPhoton Weight: " << _BgPhotonWeight
    << "\nPhoton LHood   : " << PhotonLikelihood()
    << "\nEBoost         : " << _EBoost
    << "\nEtHad1         : " << _EtHad1
    << "\nWEta2          : " << _WEta2
    << "\nE2Tsts1        : " << _E2Tsts1
    << "\nEMins1         : " << _EMins1
    << "\nWTots1         : " << _WTots1
    << "\nWEta1          : " << _WEta1
    << "\nDeltaEta0      : " << _DeltaEta0
    << "\nDeltaEta1      : " << _DeltaEta1
    << "\nDeltaEta2      : " << _DeltaEta2
    << "\nDeltaEta3      : " << _DeltaEta3
    << "\nDeltaPhi0      : " << _DeltaPhi0
    << "\nDeltaPhi1      : " << _DeltaPhi1
    << "\nDeltaPhi2      : " << _DeltaPhi2
    << "\nDeltaPhi3      : " << _DeltaPhi3
    << "\nTrackIso10     : " << _TrackIso10
    << "\nTrackIso20     : " << _TrackIso20
    << "\nTrackIso30     : " << _TrackIso30
    << "\nTrackIso40     : " << _TrackIso40
    << "\nPTCone20       : " << _PTCone20
    << "\nPTCone30       : " << _PTCone30
    << "\nPTCone40       : " << _PTCone40	    
    << "\nClusterEta     : " << _ClusterEta	    
    << "\nClusterPhi     : " << _ClusterPhi	    
    << "\nClusterE       : " << _ClusterE
    << "\nClusterM       : " << _ClusterM
    << "\nClusterPt      : " << _ClusterPt
    << "\nClusterEtaEMB2 : " << _ClusterEtaEMB2
    << "\nClusterEtaEME2 : " << _ClusterEtaEME2
    << "\nClusterPhiEMB2 : " << _ClusterPhiEMB2
    << "\nClusterPhiEME2 : " << _ClusterPhiEME2
    << "\nClustBECombEta : " << _ClusterBECombEta	    
    << "\nClustBECombPhi : " << _ClusterBECombPhi	    
    << "\nClusterBECombE : " << _ClusterBECombE
    << "\nIsLoose        : " << _IsLoose
    << "\nIsMedium       : " << _IsMedium
    << "\nIsMediumIso    : " << _IsMediumIso
    << "\nIsTight        : " << _IsTight
    << "\nIsTightIso     : " << _IsTightIso
    << "\nExpectBLayer   : " << _ExpectHitInBLayer
    << "\nTruthType      : " << _TruthParticleType
    << "\nTruthOrigin    : " << _TruthParticleOrigin
    << "\nNConversions   : " << _NConversions
    << "\nMETWex         : " << _METWex
    << "\nMETWey         : " << _METWey
    << "\nMETWet         : " << _METWet
    << std::endl;
  s << "*****\n" << std::endl;
}

}
 
ClassImp(MTree::Electron)

