#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <iostream>

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

#include "HiggsAnalysis/HWWAnalysis.h"

void fillPVs(std::string sample, bool debug = false)
{
  HWWAnalysis* analysis = new HWWAnalysis();

  if(debug)
  {
    analysis->SetDebug();
  }

  analysis->fillPVrefs(sample);
}
