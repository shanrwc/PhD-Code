#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2.h"
#include "THStack.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TMath.h"
#include "TGraphAsymmErrors.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

void quickplot()
{
  TFile* f = new TFile("root-files/SUSY/Scale/data_scale.root");

  TH1D* met = (TH1D*)f->Get(
}
