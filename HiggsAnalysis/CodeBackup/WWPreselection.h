#ifndef __WWPRESELECTION_H
#define __WWPRESELECTION_H

#include "BaseAnalysis.h"

class WWPreselection : public BaseAnalysis {
 public:  
  WWPreselection();
  ~WWPreselection();

  ClassDef(WWPreselection,0);
 
  //Analysis functions
  void wwTMVASelection(std::string sample, TH1D** histos, TH2D** histos2D);

  std::vector<std::string> wwLeptonChannel(std::vector<MTree::Lepton*> leptons);
  std::vector<MTree::Lepton*> wwLeptonOverlapRemoval(double range, std::vector<MTree::Lepton*> leptons);
  std::vector<MTree::Jet*> wwJetOverlapRemoval(double range);
  bool wwL1Trigger(MTree::Trigger* trigger);
  bool wwL2Trigger(MTree::Trigger* trigger);
  bool wwEFTrigger(MTree::Trigger* trigger);
  std::vector<MTree::Lepton*> wwPreselection(std::vector<MTree::Lepton*> leptons);
  std::vector<MTree::Lepton*> wwLeptonSelection(std::vector<MTree::Lepton*> leptons);
  std::vector<MTree::Lepton*> wwCharge(std::vector<MTree::Lepton*> leptons);
  int wwJetCounter(std::vector<MTree::Jet*> jets);

 private:
  double GetWWTransverseMass();
  double GetWTransverseMass(MTree::Lepton* lepton);
  void getX1X2(double& x1, double& x2);

  MTree::Lepton* _Leptons[2];
  MTree::Jet* _Jets[2];

};

#endif
