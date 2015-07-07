#ifndef __MTREE_TRUTHPARTICLE_H
#define __MTREE_TRUTHPARTICLE_H

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
#include "TRefArray.h"
#include "TClonesArray.h"

#include <iostream>

namespace MTree {

class TruthParticle : public TLorentzVector {
 public:
  TruthParticle();
  ~TruthParticle();
  
  void Initialize(double Pt, double Eta, double Phi, double M, int PdgID, int Status,
		  int ParentPdgID = 0, int GrandParentPdgID = 0);
  void SetParentPdgID(int ParentPdgID) {_ParentPdgID = ParentPdgID;}
  void SetGrandParentPdgID(int GrandParentPdgID) {_GrandParentPdgID = GrandParentPdgID;}
  void SetParent(MTree::TruthParticle* Parent) {_Parent = Parent;}
  void AddDaughter(MTree::TruthParticle* Daughter) {_Daughters.Add(Daughter);}
  void AddDaughterAt(MTree::TruthParticle* Daughter, int Index) {_Daughters.AddAt(Daughter, Index);}
  void RemoveDaughter(MTree::TruthParticle* Daughter) {_Daughters.Remove(Daughter);}

  int PdgID() const {return _PdgID;}
  int ParentPdgID() const {return _ParentPdgID;}
  int GrandParentPdgID() const {return _GrandParentPdgID;}
  int Status() const {return _Status;}

  MTree::TruthParticle* GetParent() const;

  int GetNrOfDaughters() const {return _Daughters.GetEntriesFast();}
  MTree::TruthParticle* GetDaughter(int index) const; 

  void PrintData(std::ostream& s) const;

  ClassDef(TruthParticle,1);
  
 private:

  Int_t _PdgID;
  Int_t _ParentPdgID;
  Int_t _GrandParentPdgID;
  
  Int_t _Status;

  TRef _Parent;
  TRefArray _Daughters;

};

}

#endif
