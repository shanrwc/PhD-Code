#ifndef __TREESKIMMER_H_
#define __TREESKIMMER_H_

#include "BaseAnalysis.h"

class TreeSkimmer : public BaseAnalysis
{
 public:
  TreeSkimmer();
  ~TreeSkimmer();

  ClassDef(TreeSkimmer,0);

 private:

  void dileptonSkim(std::string chainName);
};

#endif
