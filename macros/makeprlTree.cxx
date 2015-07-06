#include "HiggsAnalysis/TreeMakerClass.h"

#include "TFile.h"
#include "TChain.h"
#include "TTree.h"

void makeprlTree()
{
  TreeMakerClass field;

  double lumin = 1.41;

  field.makeHwwTree(lumin);
}
