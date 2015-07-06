#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <iostream>

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

#include "HiggsAnalysis/HWWAnalysis.h"

void listSampleNames()
{
  HWWAnalysis* analysis;
  analysis = new HWWAnalysis();
  analysis->GetAllNames();
}
