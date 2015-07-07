#ifndef __MTREE_MISSINGET_H
#define __MTREE_MISSINGET_H

#include "TROOT.h"
#include "TObject.h"
#include "TLorentzVector.h"
#include "TMath.h"
#include "TError.h"
#include "TVector2.h"
#include "TVector3.h"
#include "TMatrix.h"
#include "TRotation.h"
#include "TObjString.h"

#include <string>
#include <iostream>

namespace MTree {

class MissingET : public TLorentzVector {
 public:
  MissingET();
  ~MissingET();
  
  void Initialize(double met, double metx, double mety, double SumET, TObjString term);
  void InitializeRegions(float ExCentralRegion, float ExEndcapRegion, float ExForwardRegion,
			 float EyCentralRegion, float EyEndcapRegion, float EyForwardRegion,
			 float SumETCentralRegion, float SumETEndcapRegion, float SumETForwardRegion);
  double MET() const {return E();}
  double Ey() const {return Py();}
  double Ex() const {return Px();}
  double SumET() const {return _SumET;}
  std::string GetTerm() const;
  void ClearVariables();

  float GetExCentralRegion() const {return _ExCentralRegion;}
  float GetExEndcapRegion() const {return _ExEndcapRegion;}
  float GetExForwardRegion() const {return _ExForwardRegion;}

  float GetEyCentralRegion() const {return _EyCentralRegion;}
  float GetEyEndcapRegion() const {return _EyEndcapRegion;}
  float GetEyForwardRegion() const {return _EyForwardRegion;}

  float GetSumETCentralRegion() const {return _SumETCentralRegion;}
  float GetSumETEndcapRegion() const {return _SumETEndcapRegion;}
  float GetSumETForwardRegion() const {return _SumETForwardRegion;}

  void PrintData(std::ostream& s) const;

  ClassDef(MissingET,1);
  
 private:
  Double_t _SumET;
  TObjString _METTerm;

  Float_t _ExCentralRegion;
  Float_t _ExEndcapRegion;
  Float_t _ExForwardRegion;

  Float_t _EyCentralRegion;
  Float_t _EyEndcapRegion;
  Float_t _EyForwardRegion;

  Float_t _SumETCentralRegion;
  Float_t _SumETEndcapRegion;
  Float_t _SumETForwardRegion;
  
  
};

}

#endif
