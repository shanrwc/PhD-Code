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
#include "TMath.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>

#include "HiggsAnalysis/HWWConstants.h"
#include "macros/SampleInfoFunctions.cxx"

std::vector<std::string> splitString(std::string phrase)
{
  std::stringstream ss(phrase);
  std::string buffer;
  std::vector<std::string> pieces;

  while(ss >> buffer)
  {
    pieces.push_back(buffer);
  }

  return(pieces);
}

template<typename T>
std::string makeString(T const& value)
{
  std::stringstream sstr;
  sstr.setf(ios::floatfield, ios::fixed);
  sstr.precision(2);
  sstr << value;
  return sstr.str();
}

double makeDouble(std::string input)
{
  std::istringstream stm;
  stm.str(input);
  double num;
  stm >> num;

  return(num);
}

std::string evenString(std::string input, int length)
{
  while ((int)input.length() < length)
  {
    input += " ";
  }

  return(input);
}

void writeHWW(bool xsec = true)
{
  double lumin = 1.035;
//   double lumin = 1;

  std::vector<std::string> channels;
  channels.push_back("ee");
  channels.push_back("em");
  channels.push_back("mm");

  std::vector<std::string> jetbins;
  jetbins.push_back("zeroj");
  jetbins.push_back("onej");
  jetbins.push_back("twoj");

  std::vector<std::string> sources;
//   sources.push_back("testhww150");
//   sources.push_back("WWee");
  sources.push_back("data");
  sources.push_back("smWW");
  sources.push_back("smZ");
  sources.push_back("smW");
  sources.push_back("smEW");
  sources.push_back("ttbar");
  sources.push_back("sTop");
  sources.push_back("totalBack");
  sources.push_back("hOneFifty");
  sources.push_back("pythiaZ");

//   sources.push_back("WWee");
//   sources.push_back("WWee_old");
//   sources.push_back("WWem");
//   sources.push_back("WWet");
//   sources.push_back("WWme");
//   sources.push_back("WWmm");
//   sources.push_back("WWmt");
//   sources.push_back("WWte");
//   sources.push_back("WWtm");
//   sources.push_back("WWtt");
//   sources.push_back("ggWWee");
//   sources.push_back("ggWWem");
//   sources.push_back("ggWWet");
//   sources.push_back("ggWWme");
//   sources.push_back("ggWWmm");
//   sources.push_back("ggWWmt");
//   sources.push_back("ggWWte");
//   sources.push_back("ggWWtm");
//   sources.push_back("ggWWtt");

  for (int nChan = 0; nChan != (int)channels.size(); ++nChan)
  {
    for (int nJet = 0; nJet != (int)jetbins.size(); ++nJet)
    {
      //Make vector to put results in and put cut names in that vector
      std::vector<std::string> result;

      std::string cutfile = "macros/" + jetbins[nJet] + "cutnames.txt";
      ifstream cfile (cutfile.c_str());
      std::string line;
      if (cfile.is_open())
      {
        while(cfile.good())
        {
	  getline(cfile,line);
	  result.push_back(evenString(line,30));
        }
      }
      else
      {
        std::cout << "cut names file is not open!" << std::endl;
      }
      cfile.close();

      //This is necessary to remove an annoying line of zeros from the cutflow.
      result.pop_back();

      //Now loop over source files and get results to put in vector of . . . results.
      for (int nS = 0; nS != (int)sources.size(); ++nS)
      {
        TFile* file = new TFile(("root-files/hww/" + sources[nS] + "_hwwstudy.root").c_str());

        result[0] += evenString(sources[nS],14);
        std::string type;
        if (xsec)
        {
          type = "_C";
        }
        else
        {
	  type = "_N";
        }
        TH1D* hist = (TH1D*)file->Get((channels[nChan] + "_" + jetbins[nJet] + type).c_str());
        for (int i = 1; i < (int)result.size(); ++i)
        {
	  double num = hist->GetBinContent(i);
	  if (xsec && i != 1 && (sources[nS].find("data") == string::npos) && (sources[nS].find("hww") == string::npos) && (sources[nS].find("test") == string::npos))
	  {
	    //	    if (sources[nS] != "smWW" && sources[nS].find("old") == string::npos) num *= LUMINOSITY*GetXSection(sources[nS])/GetSampleWeight(sources[nS]);
	   num *= LUMINOSITY;
	  }

	  result[i] += evenString(makeString(num),14);
	}
      }

      //Now, put all those nice strings into nice text files.
      std::string finalfile = "macros/textfiles/" + channels[nChan] + "_" + jetbins[nJet] + "_cutflow.txt";
      std::ofstream outfile;
      outfile.open(finalfile.c_str());
      for (int i = 0; i != (int)result.size(); ++i)
      {
        outfile << result[i] << std::endl;
      }
      outfile.close();
    }
  }//closes loop over channels

}
