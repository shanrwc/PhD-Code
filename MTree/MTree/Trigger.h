#ifndef __MTREE_TRIGGER_H
#define __MTREE_TRIGGER_H

#include "TROOT.h"
#include "TObject.h"
#include "TMath.h"
#include "TClonesArray.h"
#include "TString.h"
#include "TObjString.h"

#include <vector>
#include <string>
#include <iostream>

namespace MTree {

class Trigger : public TObject {
 public:
  Trigger();
  ~Trigger();

  void SetL1TriggersPassed(const std::vector<std::string>& TriggerNames, 
			   const std::vector<std::string>& VetoStrings);
  void SetL2TriggersPassed(const std::vector<std::string>& TriggerNames, 
			   const std::vector<std::string>& VetoStrings);
  void SetEFTriggersPassed(const std::vector<std::string>& TriggerNames, 
			   const std::vector<std::string>& VetoStrings);

  std::vector<std::string> GetL1TriggersPassed() const;
  std::vector<std::string> GetL2TriggersPassed() const;
  std::vector<std::string> GetEFTriggersPassed() const;

  bool PassedTrigger(std::string TriggerName)   const; // works for any level of trigger
  bool PassedL1Trigger(std::string TriggerName) const; // works only for L1 triggers
  bool PassedL2Trigger(std::string TriggerName) const; // works only for L2 triggers
  bool PassedEFTrigger(std::string TriggerName) const; // works only for EF triggers
  
  // These functions return true if the event passes any trigger in TriggerNames (logical OR)
  bool PassedAnyOfL1Triggers(std::vector<std::string>& TriggerNames) const; 
  bool PassedAnyOfL2Triggers(std::vector<std::string>& TriggerNames) const; 
  bool PassedAnyOfEFTriggers(std::vector<std::string>& TriggerNames) const; 

  void PrintData(std::ostream& s) const;

  void ClearVariables();

  ClassDef(Trigger,1);
  
 private:

  TClonesArray* _L1TriggersPassed;
  TClonesArray* _L2TriggersPassed;
  TClonesArray* _EFTriggersPassed;

};

}

#endif
