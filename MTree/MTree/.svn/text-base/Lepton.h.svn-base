#ifndef __MTREE_LEPTON_H
#define __MTREE_LEPTON_H

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

#include "MTree/Track.h"
#include <iostream>

namespace MTree {

class Lepton : public TLorentzVector {
 public:
  Lepton();
  ~Lepton();
  
  void InitializeLepton(double Pt, double Eta, double Phi, double Mass, 
			int Author, int Charge, int LeptonPdgID);

  void SetTrack(MTree::Track* Track) {_Track = Track;}

  int Author() const {return _Author;}
  int Charge() const {return _Charge;}
  int LeptonPdgID() const {return _LeptonPdgID;}
  
  MTree::Track* GetTrack() const;

  void PrintLeptonData(std::ostream& s) const;

  ClassDef(Lepton,1);
  
 protected:

  Int_t _Author;
  Int_t _Charge;
  Int_t _LeptonPdgID;

  TRef _Track;
};

}

#endif
