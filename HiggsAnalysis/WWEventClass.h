#ifndef __WWEVENTCLASS_H_
#define __WWEVENTCLASS_H_

#include "BaseAnalysis.h"

class WWEventClass
{
 public:
  WWEventClass();
  ~WWEventClass();

  void printEvent(MTree::EventInfo* _eventInfo, std::vector<MTree::Lepton*> leptons, std::vector<MTree::Jet*> jets, MTree::MissingET* metWW, MTree::MissingET* metRel, double mT, double mll);

 private:
};

template<typename T>
std::string makeString(T const& value)
{
  std::stringstream sstr;
  sstr << value;
  return sstr.str();
}

#endif
