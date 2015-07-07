#ifndef __GGFREWEIGHTING_H
#define __GGFREWEIGHTING_H

#include <vector>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <unistd.h>
#include <stdlib.h>
#include <iomanip>

#include "TMath.h"
#include "TFile.h"
#include "TH1.h"

class ggFReweighting {
 public :  
  ggFReweighting(std::string generator, int higgsMass, std::string option = "Mean", std::string inputFilePath = "./");

  ~ggFReweighting();
  
  void initialize(std::string generator, int higgsMass, std::string option = "Mean");
  
  int higgsMass();
  
  std::string option();

  std::string generator();

  double getWeight(double higgsPt);
  
  double getStatError(double higgsPt);

  std::pair<double, double> getWeightAndStatError(double higgsPt);

  void printInfo();

 private :
  std::string _generator;
  int _higgsMass;
  TH1D* _weightHist;
  std::string _inputFilePath;
  std::set<std::string> _generatorLib;
  std::set<int> _higgsMassLib;
  std::string _option;
};

void initializeGeneratorLib(std::set<std::string> &generatorLib);

void initializeHiggsMassLib(std::set<int> &higgsMassLib, std::string generator);

#endif


  
