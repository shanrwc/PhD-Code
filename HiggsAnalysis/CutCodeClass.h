#ifndef __CUTCODECLASS_H_
#define __CUTCODECLASS_H_

#include "BaseAnalysis.h"
#include "HWWConstants.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>

class CutCodeClass
{

 public:
  CutCodeClass();
  ~CutCodeClass();

  double getPtll(std::vector<MTree::Lepton*>& leptons);
  double getPtTotal(std::vector<MTree::Lepton*>& leptons, std::vector<MTree::Jet*>& jets, MTree::MissingET* met);
  double getTauTauMass(TLorentzVector& lep1, TLorentzVector& lep2, TLorentzVector& met);
  double getMT(std::vector<MTree::Lepton*>& leptons, MTree::MissingET* met);

  bool failJetPtSV0(std::vector<MTree::Jet*>& jets);
  bool failThirdCentralJet(std::vector<MTree::Jet*>& jets);

};

#endif
