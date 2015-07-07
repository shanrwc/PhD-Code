#ifndef __WIRESAGANALYSIS_H
#define __WIRESAGANALYSIS_H

#include "WireSagBase.h"

class WireSagAnalysis : public WireSagBase
{
 public:
  WireSagAnalysis();
  ~WireSagAnalysis();

  ClassDef(WireSagAnalysis,0);

  //Main Analysis functions
  void WireSagEst(std::string sample, std::vector<std::vector<std::vector<TH1D*> > > histos, TH2D** histos2);

  void WireSagComparison(std::string sample1, std::string sample2, TH1D** histos);

  //Helper functions
  int findTower(double eta, double phi);
  double getX(double eta, double phi);
  double getY(double eta, double phi);

 private:
};

#endif
