#ifndef __WWCONTROL_H_
#define __WWCONTROL_H_

#include "BaseAnalysis.h"
#include "CutCodeClass.h"
#include "HWWConstants.h"

#include <iostream>
#include <string>
#include <map>
#include <vector>

class WWControlClass
{
 public:
  WWControlClass();
  ~WWControlClass();

  void add0JetEvent(std::vector<MTree::Lepton*> leptons, double mll, double weight);

  void add1JetEvent(std::vector<MTree::Lepton*> leptons, std::vector<MTree::Jet*> jets, 
		    MTree::MissingET* metForWW, double mll, double weight);

  void printWWResults();

 private:
  CutCodeClass* code;

  std::string getChannel(std::vector<MTree::Lepton*> leptons);

  TH1D* ee0ww;
  TH1D* em0ww;
  TH1D* mm0ww;
  TH1D* ee1ww;
  TH1D* em1ww;
  TH1D* mm1ww;
};

#endif
