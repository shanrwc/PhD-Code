#ifndef __MTREE_HLTTRIGGERROI_H
#define __MTREE_HLTTRIGGERROI_H

#include "TROOT.h"
#include "TObject.h"
#include "TLorentzVector.h"
#include "TMath.h"
#include "TError.h"
#include "TVector2.h"
#include "TVector3.h"
#include "TMatrix.h"
#include "TRotation.h"
#include "TClonesArray.h"
#include "TString.h"
#include "TObjString.h"
#include <vector>
#include <string>
#include <iostream>

namespace MTree {

class HLTTriggerROI : public TLorentzVector {
 public:
  HLTTriggerROI();
  ~HLTTriggerROI();
  
  void Initialize(double Pt, double Eta, double Phi, double EtaWidth, double PhiWidth, int ROIWord);
  void SetROINames(const std::vector<std::string>& HLTNames);
  void AddROIName(const std::string& HLTName);

  double EtaWidth() const {return _EtaWidth;}
  double PhiWidth() const {return _PhiWidth;}
  int ROIWord() const {return _ROIWord;}

  std::vector<std::string> GetHLTNames() const;
  bool PassedTrigger(std::string HLTName) const;

  void PrintData(std::ostream& s) const;

  ClassDef(HLTTriggerROI,1);
  
 private:

  Double_t _EtaWidth;
  Double_t _PhiWidth;
  Int_t _ROIWord;
  TClonesArray* _HLTNames;

};

}

#endif
