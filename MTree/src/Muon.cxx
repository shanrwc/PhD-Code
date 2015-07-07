#include "MTree/Muon.h"

namespace MTree {

Muon::Muon() : _ETCone10(0.0), _ETCone20(0.0), _ETCone30(0.0), _ETCone40(0.0), _PTCone10(0.0), 
	       _PTCone20(0.0), _PTCone30(0.0), _PTCone40(0.0), _FitChi2(0.0),
	       _FitChi2OverDoF(0.0), _MatchChi2(0.0), _MatchChi2OverDoF(0.0), _EnergyLoss(0.0), _EtCore(0.0),
	       _BestMatch(0), _IsCombinedMuon(0), _IsStandAloneMuon(0), _IsLowPtReconstructedMuon(0), 
	       _IsCaloMuon(0), _NrMDTHits(0), _NrCSCEtaHits(0), _NrCSCPhiHits(0), _NrRPCEtaHits(0), _NrRPCPhiHits(0),
	       _NrTGCEtaHits(0), _NrTGCPhiHits(0), _NrOfSegments(0), _TrackIso10(0.0), _TrackIso20(0.0), 
	       _TrackIso30(0.0), _TrackIso40(0.0), _IsLoose(0), _IsMedium(0), _IsTight(0), _IsMuonLikelihood(0.0),
	       _StandAloneTrack(0), _METWex(0.0), _METWey(0.0), _METWet(0.0)  {
}

Muon::~Muon() {

}

MTree::Track* Muon::GetStandAloneTrack() const
{
  if (!_StandAloneTrack.IsValid()) return 0;
  MTree::Track* saTrack = (MTree::Track*) _StandAloneTrack.GetObject();
  return saTrack;
}

void Muon::Initialize(double ETCone10, double ETCone20, double ETCone30, double ETCone40, 
		      double PTCone10, double PTCone20, double PTCone30, double PTCone40, 
		      double FitChi2, double FitChi2OverDoF, double MatchChi2, 
		      double MatchChi2OverDoF, double EnergyLoss, double EtCore, bool BestMatch, 
		      bool IsCombinedMuon, bool IsStandAloneMuon, bool IsLowPtReconstructedMuon, bool IsCaloMuon,
		      int NrMDTHits, int NrCSCEtaHits, int NrCSCPhiHits, int NrRPCEtaHits, int NrRPCPhiHits, 
		      int NrTGCEtaHits, int NrTGCPhiHits, int NrOfSegments) {
  _ETCone10 = ETCone10;
  _ETCone20 = ETCone20;
  _ETCone30 = ETCone30;
  _ETCone40 = ETCone40;
  _PTCone10 = PTCone10;
  _PTCone20 = PTCone20;
  _PTCone30 = PTCone30;
  _PTCone40 = PTCone40;
  _FitChi2 = FitChi2;
  _FitChi2OverDoF = FitChi2OverDoF;
  _MatchChi2 = MatchChi2;
  _MatchChi2OverDoF = MatchChi2OverDoF;
  _EnergyLoss = EnergyLoss;
  _EtCore = EtCore;
  _BestMatch = BestMatch;
  _IsCombinedMuon = IsCombinedMuon;
  _IsStandAloneMuon = IsStandAloneMuon;
  _IsLowPtReconstructedMuon = IsLowPtReconstructedMuon;
  _IsCaloMuon = IsCaloMuon;
  _NrMDTHits = NrMDTHits;
  _NrCSCEtaHits = NrCSCEtaHits;
  _NrCSCPhiHits = NrCSCPhiHits;
  _NrRPCEtaHits = NrRPCEtaHits;
  _NrRPCPhiHits = NrRPCPhiHits;
  _NrTGCEtaHits = NrTGCEtaHits;
  _NrTGCPhiHits = NrTGCPhiHits;
  _NrOfSegments = NrOfSegments;
}
  
void Muon::InitializeTrackIsolation(double TrackIso10, double TrackIso20, double TrackIso30, double TrackIso40) {
  _TrackIso10 = TrackIso10;
  _TrackIso20 = TrackIso20;
  _TrackIso30 = TrackIso30;
  _TrackIso40 = TrackIso40;
}

void Muon::InitializeMuonQuality(bool IsLoose, bool IsMedium, bool IsTight, float IsMuonLikelihood) {
  _IsLoose = IsLoose;
  _IsMedium = IsMedium;
  _IsTight = IsTight;
  _IsMuonLikelihood = IsMuonLikelihood;
}

void Muon::SetMissingETWeights(float METWex, float METWey, float METWet)
{
  _METWex = METWex;
  _METWey = METWey;
  _METWet = METWet;
}

void Muon::PrintData(std::ostream& s) const {
  s << "*****\n* Muon\n*****" << std::endl;
  PrintLeptonData(s);
  MTree::Track* saTrack = GetStandAloneTrack();
  if (saTrack)
    {
      s << "StandAlone Track four vector [E,px,py,pz]   = [" << saTrack->E() << ", " << saTrack->Px()
	<< ", " << saTrack->Py() << ", " << saTrack->Pz() << "]" << std::endl;
      s << "StandAlone Track four vector [pT,eta,phi,m] = [" << saTrack->Pt() << ", " << saTrack->Eta()
	<< ", " << saTrack->Phi() << ", " << saTrack->M() << "]" << std::endl;
    }
  s <<   "ETCone10        : " << _ETCone10 
    << "\nETCone20        : " << _ETCone20 
    << "\nETCone30        : " << _ETCone30 
    << "\nETCone40        : " << _ETCone40 
    << "\nPTCone10        : " << _PTCone10 
    << "\nPTCone20        : " << _PTCone20 
    << "\nPTCone30        : " << _PTCone30 
    << "\nPTCone40        : " << _PTCone40 
    << "\nFit Chi2        : " << _FitChi2 
    << "\nFit Chi2/NDOF   : " << _FitChi2OverDoF   
    << "\nMatch Chi2      : " << _MatchChi2 
    << "\nMatch Chi2/NDOF : " << _MatchChi2OverDoF 
    << "\nEnergyLoss      : " << _EnergyLoss 
    << "\nEtCore          : " << _EtCore
    << "\nBest Match      : " << _BestMatch
    << "\nCombined Muon   : " << _IsCombinedMuon   
    << "\nStand Alone Muon: " << _IsStandAloneMuon 
    << "\nLow pT Muon     : " << _IsLowPtReconstructedMuon  
    << "\nCalo Muon       : " << _IsCaloMuon
    << "\nNr MDT Hits     : " << _NrMDTHits 
    << "\nNr CSC Eta Hits : " << _NrCSCEtaHits
    << "\nNr CSC Phi Hits : " << _NrCSCPhiHits 
    << "\nNr RPC Eta Hits : " << _NrRPCEtaHits
    << "\nNr RPC Phi Hits : " << _NrRPCPhiHits 
    << "\nNr TGC Eta Hits : " << _NrTGCEtaHits
    << "\nNr TGC Phi Hits : " << _NrTGCPhiHits 
    << "\nNr of Segments  : " << _NrOfSegments 
    << "\nTrackIso10      : " << _TrackIso10
    << "\nTrackIso20      : " << _TrackIso20
    << "\nTrackIso30      : " << _TrackIso30
    << "\nTrackIso40      : " << _TrackIso40	    
    << "\nIsLoose         : " << _IsLoose	    
    << "\nIsMedium        : " << _IsMedium	    
    << "\nIsTight         : " << _IsTight	    
    << "\nIsMuonLikelihood: " << _IsMuonLikelihood
    << "\nMETWex               : " << _METWex
    << "\nMETWey               : " << _METWey
    << "\nMETWet               : " << _METWet
    << std::endl;
  s << "*****\n" << std::endl;
}

}

ClassImp(MTree::Muon)
