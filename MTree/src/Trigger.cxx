#include "MTree/Trigger.h"

namespace MTree {

Trigger::Trigger() {

  _L1TriggersPassed = new TClonesArray("TObjString", 50);
  _L2TriggersPassed = new TClonesArray("TObjString", 50);
  _EFTriggersPassed = new TClonesArray("TObjString", 50);
}

Trigger::~Trigger() {

}
 
void Trigger::SetL1TriggersPassed(const std::vector<std::string>& TriggerNames, 
				  const std::vector<std::string>& VetoStrings) {
//   int triggerCounter = 0;
//   std::vector<std::string>::const_iterator i = TriggerNames.begin(), e = TriggerNames.end();
//   std::vector<std::string>::const_iterator iVS, eVS = VetoStrings.end();
//   for (; i != e; ++i) {
//     std::string trigger(*i);
//     bool isVetoed = false;
//     for (iVS = VetoStrings.begin(); iVS != eVS; ++iVS) {
//       if (trigger.find(*iVS) != std::string::npos) {
// 	isVetoed = true;
// 	break;
//       }
//     }
//     if (isVetoed) continue;
//     new((*_L1TriggersPassed)[triggerCounter++]) TObjString((*i).c_str());
//   }  


  int triggerCounter = 0;
  int nrTrigs = TriggerNames.size();
  int nrVetos = VetoStrings.size();
  for (int i=0;i<nrTrigs;i++)
  {
    std::string trig = TriggerNames[i];
    int pos = trig.find('_');

    std::string searchString = trig.substr(pos+1, trig.find('_', pos+1)-pos-1);
    //std::cout << "search string: " << searchString << std::endl;

    bool veto = false;
    for (int j=0;j<nrVetos;j++)
    {
      if (searchString.find(VetoStrings[j]) != std::string::npos)
      {
	veto = true;
	//std::cout << "Vetoing trigger: " << trig << ", due to string: " << VetoStrings[j] << std::endl;
	break;
      }
    }

    if (veto)
    {
      continue;
    }
    new((*_L1TriggersPassed)[triggerCounter++]) TObjString(trig.c_str());
  }

}

void Trigger::SetL2TriggersPassed(const std::vector<std::string>& TriggerNames, 
				  const std::vector<std::string>& VetoStrings) {
//   int triggerCounter = 0;
//   std::vector<std::string>::const_iterator i = TriggerNames.begin(), e = TriggerNames.end();
//   std::vector<std::string>::const_iterator iVS, eVS = VetoStrings.end();
//   for (; i != e; ++i) {
//     std::string trigger(*i);
//     bool isVetoed = false;
//     for (iVS = VetoStrings.begin(); iVS != eVS; ++iVS) {
//       if (trigger.find(*iVS) != std::string::npos) {
// 	isVetoed = true;
// 	break;
//       }
//     }
//     if (isVetoed) continue;
//     new((*_L2TriggersPassed)[triggerCounter++]) TObjString((*i).c_str());
//   }  

  int triggerCounter = 0;
  int nrTrigs = TriggerNames.size();
  int nrVetos = VetoStrings.size();
  for (int i=0;i<nrTrigs;i++)
  {
    std::string trig = TriggerNames[i];
    int pos = trig.find('_');

    std::string searchString = trig.substr(pos+1, trig.find('_', pos+1)-pos-1);
    //std::cout << "search string: " << searchString << std::endl;

    bool veto = false;
    for (int j=0;j<nrVetos;j++)
    {
      if (searchString.find(VetoStrings[j]) != std::string::npos)
      {
	veto = true;
	//std::cout << "Vetoing trigger: " << trig << ", due to string: " << VetoStrings[j] << std::endl;
	break;
      }
    }

    if (veto)
    {
      continue;
    }
    new((*_L2TriggersPassed)[triggerCounter++]) TObjString(trig.c_str());
  }
}

void Trigger::SetEFTriggersPassed(const std::vector<std::string>& TriggerNames, 
				  const std::vector<std::string>& VetoStrings) {
//   int triggerCounter = 0;
//   std::vector<std::string>::const_iterator i = TriggerNames.begin(), e = TriggerNames.end();
//   std::vector<std::string>::const_iterator iVS, eVS = VetoStrings.end();
//   for (; i != e; ++i) {
//     std::string trigger(*i);
//     bool isVetoed = false;
//     for (iVS = VetoStrings.begin(); iVS != eVS; ++iVS) {
//       if (trigger.find(*iVS) != std::string::npos) {
// 	isVetoed = true;
// 	//std::cout << "Found veto: " << *iVS << std::endl;
// 	break;
//       }
//     }
//     //if (isVetoed) std::cout << "Vetoing trigger: " << trigger << std::endl;
//     if (isVetoed) continue;
//     new((*_EFTriggersPassed)[triggerCounter++]) TObjString((*i).c_str());
//   }  






  int triggerCounter = 0;
  int nrTrigs = TriggerNames.size();
  int nrVetos = VetoStrings.size();
  for (int i=0;i<nrTrigs;i++)
  {
    std::string trig = TriggerNames[i];
    int pos = trig.find('_');

//if trig is EF_mu40_MSonly_barrel,
//searchString is mu40
    std::string searchString = trig.substr(pos+1, trig.find('_', pos+1)-pos-1);
    //std::cout << "search string: " << searchString << std::endl;

    bool veto = false;
    for (int j=0;j<nrVetos;j++)
    {
      if (searchString.find(VetoStrings[j]) != std::string::npos)
      {
	veto = true;
	//std::cout << "Vetoing trigger: " << trig << ", due to string: " << VetoStrings[j] << std::endl;
	break;
      }
    }

    if (veto)
    {
      continue;
    }
    new((*_EFTriggersPassed)[triggerCounter++]) TObjString(trig.c_str());
  }

}

std::vector<std::string> Trigger::GetL1TriggersPassed() const {
  std::vector<std::string> triggers;
  int NrTriggers = _L1TriggersPassed->GetEntriesFast();
  for (int i = 0; i != NrTriggers; ++i) {
    triggers.push_back(std::string(((TObjString*) _L1TriggersPassed->At(i))->GetString().Data()));
  }
  return triggers;
}

std::vector<std::string> Trigger::GetL2TriggersPassed() const {
  std::vector<std::string> triggers;
  int NrTriggers = _L2TriggersPassed->GetEntriesFast();
  for (int i = 0; i != NrTriggers; ++i) {
    triggers.push_back(std::string(((TObjString*) _L2TriggersPassed->At(i))->GetString().Data()));
  }
  return triggers;
}

std::vector<std::string> Trigger::GetEFTriggersPassed() const {
  std::vector<std::string> triggers;
  int NrTriggers = _EFTriggersPassed->GetEntriesFast();
  for (int i = 0; i != NrTriggers; ++i) {
    triggers.push_back(std::string(((TObjString*) _EFTriggersPassed->At(i))->GetString().Data()));
  }
  return triggers;
}

void Trigger::ClearVariables() {
  _L1TriggersPassed->Clear();
  _L2TriggersPassed->Clear();
  _EFTriggersPassed->Clear();  
}

bool Trigger::PassedTrigger(std::string TriggerName) const {
  return PassedL1Trigger(TriggerName) || PassedL2Trigger(TriggerName) || PassedEFTrigger(TriggerName);
}

bool Trigger::PassedL1Trigger(std::string TriggerName) const {
  int nrTriggers = _L1TriggersPassed->GetEntriesFast();
  for (int i = 0; i != nrTriggers; ++i) {
    std::string trigger(((TObjString*) _L1TriggersPassed->At(i))->GetString().Data());
    if (trigger == TriggerName) return true;
  }
  return false;
}

bool Trigger::PassedL2Trigger(std::string TriggerName) const {
  int nrTriggers = _L2TriggersPassed->GetEntriesFast();
  for (int i = 0; i != nrTriggers; ++i) {
    std::string trigger(((TObjString*) _L2TriggersPassed->At(i))->GetString().Data());
    if (trigger == TriggerName) return true;
  }
  return false;
}

bool Trigger::PassedEFTrigger(std::string TriggerName) const {
  int nrTriggers = _EFTriggersPassed->GetEntriesFast();
  for (int i = 0; i != nrTriggers; ++i) {
    std::string trigger(((TObjString*) _EFTriggersPassed->At(i))->GetString().Data());
    if (trigger == TriggerName) return true;
  }
  return false;
}

bool Trigger::PassedAnyOfL1Triggers(std::vector<std::string>& TriggerNames) const {
  int nrTriggers = _L1TriggersPassed->GetEntriesFast();
  for (int i = 0; i != nrTriggers; ++i) {
    std::string trigger(((TObjString*) _L1TriggersPassed->At(i))->GetString().Data());
    if (find(TriggerNames.begin(), TriggerNames.end(), trigger) != TriggerNames.end()) return true;
  }
  return false;
}


bool Trigger::PassedAnyOfL2Triggers(std::vector<std::string>& TriggerNames) const {
  int nrTriggers = _L2TriggersPassed->GetEntriesFast();
  for (int i = 0; i != nrTriggers; ++i) {
    std::string trigger(((TObjString*) _L2TriggersPassed->At(i))->GetString().Data());
    if (find(TriggerNames.begin(), TriggerNames.end(), trigger) != TriggerNames.end()) return true;
  }
  return false;
}

bool Trigger::PassedAnyOfEFTriggers(std::vector<std::string>& TriggerNames) const {
  int nrTriggers = _EFTriggersPassed->GetEntriesFast();
  for (int i = 0; i != nrTriggers; ++i) {
    std::string trigger(((TObjString*) _EFTriggersPassed->At(i))->GetString().Data());
    if (find(TriggerNames.begin(), TriggerNames.end(), trigger) != TriggerNames.end()) return true;
  }
  return false;
}

void Trigger::PrintData(std::ostream& s) const {
  s << "*****\n* Triggers Passed\n*****" << std::endl;

  s << "L1 Triggers:\n" << "------------" << std::endl;
  int nrTriggers = _L1TriggersPassed->GetEntriesFast();
  for (int i = 0; i != nrTriggers; ++i) s << ((TObjString*) _L1TriggersPassed->At(i))->GetString().Data() 
					  << std::endl;

  s << "\nL2 Triggers:\n" << "------------" << std::endl;
  nrTriggers = _L2TriggersPassed->GetEntriesFast();
  for (int i = 0; i != nrTriggers; ++i) s << ((TObjString*) _L2TriggersPassed->At(i))->GetString().Data() 
					  << std::endl;

  s << "\nEF Triggers:\n" << "------------" << std::endl;
  nrTriggers = _EFTriggersPassed->GetEntriesFast();
  for (int i = 0; i != nrTriggers; ++i) s << ((TObjString*) _EFTriggersPassed->At(i))->GetString().Data() 
					  << std::endl;

  s << "*****\n" << std::endl;
}

}

ClassImp(MTree::Trigger)
