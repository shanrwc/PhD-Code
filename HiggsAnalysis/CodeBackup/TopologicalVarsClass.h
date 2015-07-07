#ifndef _TOPOLOGICALVARSCLASS_HPP_
#define _TOPOLOGICALVARSCLASS_HPP_

#include "TROOT.h"
#include "TObject.h"

#include "MTree/Lepton.h"
#include "MTree/MissingET.h"

#include <iostream>
#include <vector>
#include <string>
using namespace std;

class TopologicalVarsClass : public TObject
{ 
  public:
  //Constructor and Destructor
  TopologicalVarsClass();
  ~TopologicalVarsClass();

  //ROOT gibberish
  ClassDef(TopologicalVarsClass,2);

  //Getter functions
  double getLepOnePt() const;
  double getLepTwoPt() const;
  double getDiLepPt() const;
  double getMissingET() const;
  double getMLL() const;
  double getMLLNu() const;
  double getDeltaPhi() const;
  double getNJets() const;
  double getXSection() const;
  double getSampleType() const;

  //Setter or Calculator functions
  void calculateVars(MTree::MissingET* MET, MTree::Lepton* lepton1, MTree::Lepton* lepton2, double eventWeight, string name, double nJets);
  void clear();

  private:
  //The actual quantities
  double lepton1_pt;
  double lepton1_eta;
  double lepton1_phi;
  double lepton2_pt;
  double lepton2_eta;
  double lepton2_phi;
  double dilep_pt;
  double missingET;
  double mll;
  double mllnu;
  double deltaPhi;
  double n_jets;
  double xsec;
  double sampleType;

};
#endif
