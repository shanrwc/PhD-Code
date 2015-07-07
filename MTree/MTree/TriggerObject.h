#ifndef __MTREE_TRIGGEROBJECT_H
#define __MTREE_TRIGGEROBJECT_H

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

#include <iostream>
#include <set>

namespace MTree {

  class TriggerObject : public TObject {

  public:
    TriggerObject();
    ~TriggerObject();
    
    double Pt() const {return _Pt;}
    double Eta() const {return _Eta;}
    double Phi() const {return _Phi;}

    void Initialize (double pt, double eta, double phi);

    void SetAuthor(int author) {_Authors.insert(author);}
    //author = 1 => from egammaContainer
    //author = 2 => from CombinedMuonFeature
    //author = 3 => from TrigMuonEFInfoContainer Combined Track
    //author = 4 => from MuonFeature
    //author = 5 => from TrigMuonEFInfoContainer Extrapolated Track
    //author = 6 => from TrigMuonEFInfoContainer MS Track

    void AddTriggerName(std::string name) {_triggerNames.insert(name);}
    std::set<std::string>& TriggerNames() {return _triggerNames;}
    std::set<int>& Authors() {return _Authors;}
    bool PassedTrigger(std::string trigName);

    void PrintData(std::ostream& s) const;
    bool IsSame(double pt, double eta, double phi);

    ClassDef(TriggerObject,1);

  private:
    
    std::set<std::string> _triggerNames;
    std::set<int> _Authors;
    double _Pt;
    double _Eta;
    double _Phi;
  };

}

#endif
