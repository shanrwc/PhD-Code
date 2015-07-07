#ifndef EGAMMAREC_EGAMMAPIDDEFS_H
#define EGAMMAREC_EGAMMAPIDDEFS_H
/*****************************************************************************
Name    : egammaPIDdefs.h
Package : offline/Reconstruction/egammaRec
Author  : Dirk Zerwas
Created : December 2004
Note    : 

DESCRIPTION:

This file defines the bit-occupation of the isEM flag of egamma
and the enum definition of the Identification results
*****************************************************************************/

namespace egammaPID { 

  enum BitDef {
    // Cluster based egamma
    ClusterEtaRange        =  0,
    ClusterHadronicLeakage =  1,
    ClusterMiddleEnergy    =  4, 
    ClusterMiddleEratio37  =  5,
    ClusterMiddleEratio33  =  6,
    ClusterMiddleWidth     =  7,
    ClusterStripsEratio    =  8,
    ClusterStripsDeltaEmax2=  9,
    ClusterStripsDeltaE    = 10,
    ClusterStripsWtot      = 11,
    ClusterStripsFracm     = 12,
    ClusterStripsWeta1c    = 13,
    ClusterIsolation       = 14,
    //Track based egamma
    TrackBlayer            = 16,
    TrackPixel             = 17,
    TrackSi                = 18,
    TrackA0                = 19,
    TrackMatchEta          = 20,
    TrackMatchPhi          = 21,
    TrackMatchEoverP       = 22,
    TrackTRThits           = 24,
    TrackTRTratio          = 25,
    TrackTRTratio90        = 26
  };

  enum PID {
    IsEM                = 0,
    ElectronWeight,
    BgWeight,
    NeuralNet,
    Hmatrix,
    Hmatrix5,
    SofteIsEM,
    SofteElectronWeight,
    SofteBgWeight,
    SofteNeuralNet,
    IsolationLikelihood,
    AdaBoost,
    PhotonWeight,
    BgPhotonWeight,
    LastEgammaPID
  };

  const double EgPidUndefined = -999.;
  /// for simplification
  const unsigned int HADLEAKETA = 
    0x1 << ClusterEtaRange        | 
    0x1 << ClusterHadronicLeakage;
  const unsigned int CALOSTRIPS=
    0x1 << ClusterStripsEratio     |
    0x1 << ClusterStripsDeltaEmax2 |
    0x1 << ClusterStripsDeltaE     |
    0x1 << ClusterStripsWtot       |
    0x1 << ClusterStripsFracm      |
    0x1 << ClusterStripsWeta1c    ;
  const unsigned int CALOMIDDLE=    
    0x1 << ClusterMiddleEnergy     |
    0x1 << ClusterMiddleEratio37   |
    0x1 << ClusterMiddleEratio33   |
    0x1 << ClusterMiddleWidth     ;
  const unsigned int CALOISO = 0x1 << ClusterIsolation ;
  const unsigned int CALONOISO = HADLEAKETA | CALOSTRIPS | CALOMIDDLE ;
  const unsigned int CALO = CALONOISO | CALOISO ;
  const unsigned int TRACKINGNOBLAYER =     
    0x1 << TrackPixel   |
    0x1 << TrackSi      |
    0x1 << TrackA0     ;
  const unsigned int TRACKING = TRACKINGNOBLAYER |  0x1 << TrackBlayer;
  const unsigned int TRACKMATCHDETA =
    0x1 << TrackMatchEta;
  const unsigned int TRACKMATCH =
    0x1 << TrackMatchEta      |
    0x1 << TrackMatchPhi      |
    0x1 << TrackMatchEoverP  ;
  const unsigned int TRACKMATCHNOEOVERP =
    0x1 << TrackMatchEta      |
    0x1 << TrackMatchPhi     ;
  const unsigned int TRT = 
    0x1 << TrackTRThits   |
    0x1 << TrackTRTratio ;
  const unsigned int TRT90 = 
    0x1 << TrackTRThits   |
    0x1 << TrackTRTratio90 ;
  const unsigned int ALLNOTRT= TRACKING | TRACKMATCH | CALO;
  const unsigned int ALL= ALLNOTRT | TRT;

  const unsigned int ElectronLoose = CALOMIDDLE | HADLEAKETA;
  const unsigned int ElectronMedium= CALO | TRACKINGNOBLAYER | TRACKMATCHDETA;
  const unsigned int ElectronMediumNoIso = CALONOISO | TRACKINGNOBLAYER | TRACKMATCHDETA ;
  const unsigned int ElectronTight = ALL;
  const unsigned int ElectronTightTRTNoIso = TRACKING | TRACKMATCH | CALONOISO | TRT90 ;
  const unsigned int ElectronTightNoIsolation = ElectronTightTRTNoIso;

  const unsigned int PhotonTight = CALO;

}
#endif // EGAMMAREC_EGAMMAPIDDEFS_H
