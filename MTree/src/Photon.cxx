#include "MTree/Photon.h"

namespace MTree {

  Photon::Photon() : TLorentzVector(0.0, 0.0, 0.0, 0.0), _Author(0), _EOverP(0.0), _ETCone20(0.0), _ETCone30(0.0), 
                     _ETCone40(0.0), _ETCone45(0.0), _ETCone40PtCorr(0.0), _ETCone40EDCorr(0.0), _ETCone40Corr(0.0),
                     _E233(0.0), _E237(0.0), _E277(0.0), _EtHad(0.0), 
                     _F1(0.0), _F1Core(0.0), _Fracs1(0.0), _Iso(0.0), _ZVertex(0.0), _ErrZ(0.0), _IsConvertedPhoton(0), 
                     _ConvTrackMatch(0), _ConvAngleMatch(0), _IsEM(0), _ElectronWeight(0.0), _BgWeight(0.0), _NeuralNet(0.0), 
		     _HMatrix(0.0), _HMatrix5(0.0), _IsolationLikelihood(0.0), _AdaBoost(0.0), _PhotonWeight(0.0), 
		     _BgPhotonWeight(0.0), _ClusterEta(0.0), _ClusterPhi(0.0), _ClusterE(0.0), _ClusterM(0.0), _ClusterPt(0.0), 
		     _ClusterEtaEMB2(0.0), _ClusterEtaEME2(0.0), _ClusterPhiEMB2(0.0), _ClusterPhiEME2(0.0), 
		     _ClusterBECombEta(0.0), _ClusterBECombPhi(0.0), _ClusterBECombE(0.0), _IsLoose(0), _IsLooseAR(0),
                     _IsTight(0), _IsTightIso(0), _IsTightAR(0), _IsTightARIso(0), _METWex(0.0), _METWey(0.0), _METWet(0.0),
                     _ETHad1(0.0), _EMaxs1(0.0), _EMaxs2(0.0), _EMin(0.0), _Fracm(0.0), _WTot(0.0), _W1(0.0) {
}

Photon::~Photon() {

}

void Photon::Initialize(double Pt, double Eta, double Phi, double Mass, int Author, 
			double EOverP, double ETCone20, double ETCone30, double ETCone40, 
			double ETCone45, int IsEM, float E233, float E237, float E277, float EtHad, float F1, 
			float F1Core, float Fracs1, float Iso, bool TrackRefit, bool ConvTrackMatch, 
			bool ConvAngleMatch, float ZVertex, float ErrZ, double ElectronWeight, double BgWeight, 
			double NeuralNet, double HMatrix, double HMatrix5, double IsolationLikelihood, 
			double AdaBoost, double PhotonWeight, double BgPhotonWeight) {
  SetPtEtaPhiM(Pt, Eta, Phi, Mass);
  _Author = Author;
  _EOverP = EOverP;
  _ETCone20 = ETCone20;
  _ETCone30 = ETCone30;
  _ETCone40 = ETCone40;
  _ETCone45 = ETCone45;
  _IsEM = IsEM;
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

void Photon::SetMissingETWeights(float METWex, float METWey, float METWet)
{
  _METWex = METWex;
  _METWey = METWey;
  _METWet = METWet;
}

void Photon::InitializeClusterVariables(float ClusterEta, float ClusterPhi, float ClusterE, float ClusterM, float ClusterPt, 
                                        float ClusterEtaEMB2, float ClusterEtaEME2, float ClusterPhiEMB2, float ClusterPhiEME2, 
                                        float ClusterBECombEta, float ClusterBECombPhi, float ClusterBECombE)
{
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
void Photon::InitializeExtraClusterVariables(float ETHad1, float WEta2, float EMaxs1, float EMaxs2, float EMin,
                                             float Fracm, float WTot, float W1) 
{
  _ETHad1 = ETHad1;
  _WEta2 = WEta2; 
  _EMaxs1 = EMaxs1;
  _EMaxs2 = EMaxs2;
  _EMin = EMin;
  _Fracm = Fracm;
  _WTot = WTot;
  _W1 = W1;
}

void Photon::InitializeConversionVariables(int ConvFlag, bool IsConv, int NConv, 
                                           int NSingleTrackConv, int NDoubleTrackConv)
{
  _ConvFlag = ConvFlag;
  _IsConv = IsConv;
  _NConv = NConv;
  _NSingleTrackConv = NSingleTrackConv;
  _NDoubleTrackConv = NDoubleTrackConv;
}

void Photon::InitializeCorrectedIsolation(float ETCone40PtCorr, float ETCone40EDCorr, float ETCone40Corr)
{
  _ETCone40PtCorr = ETCone40PtCorr;
  _ETCone40EDCorr = ETCone40EDCorr;
  _ETCone40Corr = ETCone40Corr;
}
void Photon::InitializePhotonFlags(bool IsLoose, bool IsLooseAR, bool IsTight, bool IsTightIso, bool IsTightAR, bool IsTightARIso) {
  _IsLoose = IsLoose;
  _IsLooseAR = IsLooseAR;
  _IsTight = IsTight;
  _IsTightIso = IsTightIso;
  _IsTightAR = IsTightAR;
  _IsTightARIso = IsTightARIso;
}

double Photon::ElectronLikelihood() const {
  if (_BgWeight == 0.0) return -999.0;
  return TMath::Log(_ElectronWeight / _BgWeight);
}

double Photon::PhotonLikelihood() const {
  if (_BgPhotonWeight == 0.0) return -999.0;
  return TMath::Log(_PhotonWeight / _BgPhotonWeight);
}

void Photon::PrintData(std::ostream& s) const {
  s << "*****\n* Photon\n*****" << std::endl;
  s << "Photon 4-vector [E,px,py,pz]   = [" << this->E() << ", " << this->Px() << ", " << this->Py()
    << ", " << this->Pz() << "]" << std::endl;
  s << "Photon 4-vector [pT,eta,phi,m] = [" << this->Pt() << ", " << this->Eta() << ", " << this->Phi()
    << ", " << this->M() << "]" << std::endl;
  s <<   "EOverP         : " << _EOverP 
    << "\nAuthor         : " << _Author 
    << "\nETCone20       : " << _ETCone20 
    << "\nETCone30       : " << _ETCone30 
    << "\nETCone40       : " << _ETCone40 
    << "\nETCone45       : " << _ETCone45 
    << "\nETCone40PtCorr : " << _ETCone40PtCorr
    << "\nETCone40EDCorr : " << _ETCone40EDCorr
    << "\nETCone40Corr   : " << _ETCone40Corr
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
    << "\nIsLooseAR      : " << _IsLooseAR
    << "\nIsTight        : " << _IsTight
    << "\nIsTightIso     : " << _IsTightIso
    << "\nIsTightAR      : " << _IsTightAR
    << "\nIsTightARIso   : " << _IsTightARIso
    << "\nMETWex         : " << _METWex
    << "\nMETWey         : " << _METWey
    << "\nMETWet         : " << _METWet
    << "\nETHad1         : " << _ETHad1
    << "\nWEta2          : " << _WEta2
    << "\nEMaxs1         : " << _EMaxs1
    << "\nEMaxs2         : " << _EMaxs2
    << "\nEMin           : " << _EMin
    << "\nFracm          : " << _Fracm
    << "\nWTot           : " << _WTot
    << "\nW1             : " << _W1
    << "\nConvFlag       : " << _ConvFlag
    << "\nIsConv         : " << _IsConv
    << "\nNConv          : " << _NConv
    << "\nNSingleTrackCon: " << _NSingleTrackConv
    << "\nNDoubleTrackCon: " << _NDoubleTrackConv
    << std::endl;
  s << "*****\n" << std::endl;
}

}
 
ClassImp(MTree::Photon)

