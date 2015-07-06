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

std::vector<std::string> getSourceFiles(std::string sample)
{
  std::vector<std::string> files;

  if (sample == "smWW")
  {
    files.push_back("WWee");
    files.push_back("WWem");
    files.push_back("WWet");
    files.push_back("WWme");
    files.push_back("WWmm");
    files.push_back("WWmt");
    files.push_back("WWte");
    files.push_back("WWtm");
    files.push_back("WWtt");
    files.push_back("ggWWee");
    files.push_back("ggWWem");
    files.push_back("ggWWet");
    files.push_back("ggWWme");
    files.push_back("ggWWmm");
    files.push_back("ggWWmt");
    files.push_back("ggWWte");
    files.push_back("ggWWtm");
    files.push_back("ggWWtt");
  }
  else
  {
    files.push_back(sample);
  }

  return(files);
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void writeHWW(bool xsec = true)
{
  std::vector<std::string> channels;
  channels.push_back("ee");
  channels.push_back("em");
  channels.push_back("mm");

  std::vector<std::string> jetbins;
  jetbins.push_back("zeroj");
  jetbins.push_back("onej");
  jetbins.push_back("twoj");

  std::vector<std::string> samples;
  samples.push_back("smWW");
//   samples.push_back("smWW_old");

  std::map<std::string, std::vector<std::string> > sources;
  for (int i = 0; i != (int)samples.size(); ++i)
  {
    sources[samples[i]] = getSourceFiles(samples[i]);
  }


  //Loop over particles and jetbins
  for (int nChan = 0; nChan != (int)channels.size(); ++nChan)
  {
    for (int nJet = 0; nJet != (int)jetbins.size(); ++nJet)
    {
      //Set up cutnames column of output file
      std::vector<std::string> result;

      std::string cutfile = "macros/" + jetbins[nJet] + "cutnames.txt";
      ifstream cfile (cutfile.c_str());
      std::string line;
      if (cfile.is_open())
      {
        while(cfile.good())
        {
	  getline(cfile,line);
	  result.push_back(EvenString(line,30));
        }
      }
      else
      {
        std::cout << "cut names file is not open!" << std::endl;
      }
      cfile.close();

      //This is necessary to remove an annoying line of zeros from the cutflow.
      result.pop_back();

      //In each iteration, iterate over a map of source files
      //Map contains string/sample name associated with a vector or list of files
      std::map<std::string, std::vector<std::string> >::iterator iter;
      std::map<std::string, std::vector<std::string> >::iterator end = sources.end();
      for (iter = sources.begin(); iter != end; ++iter)
      {
	std::string sampleName = iter->first;
	result[0] += EvenString(sampleName,14);

	std::vector<std::string> allFiles = iter->second;
	std::string type;
        if (xsec)
        {
          type = "_C";
        }
        else
        {
	  type = "_N";
        }

	double nEvents[result.size() - 1];
	for (int k = 0; k != (int)result.size() - 1; ++k)
	{
	  nEvents[k] = 0.0;
	}

	for (int j = 0; j != (int)allFiles.size(); ++j)
	{
	  TFile* file = new TFile(("root-files/hww/" + allFiles[j] + "_hwwstudy.root").c_str());
	  TH1D* hist = (TH1D*)file->Get((channels[nChan] + "_" + jetbins[nJet] + type).c_str());

	  for (int k = 1; k < (int)result.size(); ++k)
	  {
	    double num = hist->GetBinContent(k);
	    if (xsec && sampleName.find("data") == std::string::npos)
	    {
	      num *= LUMINOSITY*GetXSection(allFiles[j])/GetSampleWeight(allFiles[j]);
	    }

	    nEvents[k] += num;
	  }
	}
	for (int j = 1; j < (int)result.size(); ++j)
	{
	  result[j] += EvenString(MakeString(nEvents[j-1]),14);
	}

      }//closes iteration over map of source files

      //Now, put all those nice strings into nice text files.
      std::string finalfile = "macros/textfiles/" + channels[nChan] + "_" + jetbins[nJet] + "_cutflow.txt";
      std::ofstream outfile;
      outfile.open(finalfile.c_str());
      for (int i = 0; i != (int)result.size(); ++i)
      {
        outfile << result[i] << std::endl;
      }
      outfile.close();

    }//closes loop over jet bins
  }//closes loop particle channels
}//close main function
