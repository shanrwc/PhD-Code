#ifndef _ENDCAPMAP_HPP_
#define _ENDCAPMAP_HPP_

#include "TH1D.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TLegend.h"

#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>

class EndCapMap
{
 private:
  //labeling string
  std::string variable;
  std::string sample;

  //Grand high map of histograms
  std::map<std::string, TH1D*> histos;

  //Converts an integer to a string.  Because for some, inexplicable reasons,
  //c++ doesn't already have this.
  std::string makeString(int number);

  //Converts track's location to name-string
  std::string getLocString(int charge, double eta, double phi);

  //Fits a gaussian to all the plots in the darn map
  void fitHisto(TH1D* histo, double &mean, double &error, double &variance, double &varerr);

 public:
  //constructor
  EndCapMap(std::string Sample, std::string tag);

  //destructor
  ~EndCapMap();

  //Function to add event
  void addEvent(int charge, double eta, double phi, double var, double weight);

  //Functions that makes graphs-the masterpieces of this class.
  void graphFittedMeans(int etagroup, int phigroup);
  void graphChamberCenters(int etagroup);
  void graphDistributions(int etagroup, int phigroup);

};

#endif
