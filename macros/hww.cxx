#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <iostream>

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

#include "HiggsAnalysis/HWWAnalysis.h"

std::string makeString(int number)
{
  std::stringstream ss;
  ss << number;
  return ss.str();
}

void hww(std::string sample = "testhww150", bool debug = false)
{
  std::string filename = "root-files/hww/" + sample + "_hwwstudy.root";
  TFile* file = new TFile(TString(filename.c_str()),"RECREATE");
  HWWAnalysis* analysis;
  analysis = new HWWAnalysis();
  if (debug)
  {
    analysis->SetDebug();
  }

  bool comSet = false;
  if (sample == "test" || sample.find("test") != string::npos)
  {
    std::cout << "running with 'commissioning' settings" << std::endl;
    comSet = true;
  }

//The third argument's that a bool is for low mass (true) or high mass (false) settings.
  analysis->h2wwCutflow(sample,file,true,150,comSet);

  file->Write();
  file->Close();
}
