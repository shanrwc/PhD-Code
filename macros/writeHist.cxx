#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2.h"
#include "THStack.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

template<typename T>
std::string makeString(T const& value)
{
  std::stringstream sstr;
  sstr << value;
  return sstr.str();
}

std::string evenString(std::string input, int length)
{
  while ((int)input.length() < length)
  {
    input += " ";
  }

  return(input);
}

void writeHist()
{

  std::vector<std::string> hists;
  hists.push_back("eCutflow");
  hists.push_back("mCutflow");
  hists.push_back("jCutflow");

  //for now, require all hists come from one TFile.  That can of course be changed.
//   TFile* f = new TFile("root-files/hww/data_hwwstudy.root");
  TFile* f = new TFile("root-files/hww/data183581_hwwstudy.root");

  for (int nHist = 0; nHist != (int)hists.size(); ++nHist)
  {
    //Get the histogram
    TH1D* hist = (TH1D*)f->Get(hists[nHist].c_str());

    //Put info in vector of strings
    std::vector<std::string> results;

    for (int i = 1; i != hist->GetNbinsX(); ++i)
    {
      std::string entry = makeString(hist->GetXaxis()->GetBinLabel(i)) + " " + makeString(hist->GetBinContent(i));
      results.push_back(entry);
    }

    //Write info to file
    std::string filename = "macros/textfiles/" + makeString(hist->GetName()) + "_" + hists[nHist] + ".txt";
    std::ofstream outfile;
    outfile.open(filename.c_str());
    for (int i = 0; i != (int)results.size(); ++i)
    {
      outfile << results[i] << std::endl;
    }
    outfile.close();
  }

}
