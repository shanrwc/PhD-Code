#ifndef __CUTFLOWCLASS_H_
#define __CUTFLOWCLASS_H_

#include "TH1D.h"

#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

class CutflowClass
{
 public:
  CutflowClass();
  ~CutflowClass();

  //Fills maps with empty histograms for cutflow
  void setChannels(std::vector<std::string> &channels);

  //Adds events to appropriate cutflows
  int addEvent(int cutNr, double xsec, std::vector<std::string> &channels);

  //Since these cutflows are only useful if they make it into the TFile . . .
  std::map<std::string, TH1D*>* getNumCutflows();
  std::map<std::string, TH1D*>* getXSecCutflows();

  void printXSecCheck();
 private:

  std::map<std::string, TH1D*> mapN;
  std::map<std::string, TH1D*> mapC;

  std::map<std::string, std::vector<double> > checkC;
};

#endif
