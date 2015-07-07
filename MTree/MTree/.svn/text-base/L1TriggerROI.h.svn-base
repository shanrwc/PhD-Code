#ifndef __MTREE_L1TRIGGERROI_H
#define __MTREE_L1TRIGGERROI_H

#include "TROOT.h"
#include "TObject.h"
#include "TLorentzVector.h"
#include "TMath.h"
#include "TError.h"
#include "TVector2.h"
#include "TVector3.h"
#include "TMatrix.h"
#include "TRotation.h"

#include <iostream>

namespace MTree {

class L1TriggerROI : public TLorentzVector {
 public:
  L1TriggerROI();
  ~L1TriggerROI();
  
  void Initialize(double Pt, double Eta, double Phi, double EtaWidth, double PhiWidth, int ROIWord);

  double EtaWidth() const {return _EtaWidth;}
  double PhiWidth() const {return _PhiWidth;}
  int ROIWord() const {return _ROIWord;}

  void PrintData(std::ostream& s) const;

  ClassDef(L1TriggerROI,1);
  
 private:

  Double_t _EtaWidth;
  Double_t _PhiWidth;
  Int_t _ROIWord;
};

}

#endif
