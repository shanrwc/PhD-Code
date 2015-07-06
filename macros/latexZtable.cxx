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

#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "HiggsAnalysis/HWWConstants.h"

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
  sstr.precision(1);
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

void latexZtable()
{
//   double lumin = 1.035;

  std::vector<std::string> channels;
  channels.push_back("e");
//   channels.push_back("em");
  channels.push_back("m");

  std::vector<std::string> jetbins;
  jetbins.push_back("0");
  jetbins.push_back("1");
  jetbins.push_back("2");

  std::vector<std::string> sources;
  sources.push_back("data");
  sources.push_back("smWW");
  sources.push_back("smW");
  sources.push_back("smEW");
  sources.push_back("ttbar");
  sources.push_back("sTop");
  sources.push_back("smZ");


//   std::string finalfile = "macros/latexfiles/zregion_table.txt";
//   std::ofstream outfile;
//   outfile.open(finalfile.c_str());
//   outfile << "\\begin{tabular}{|c|c|c|c|c|c|c|c|c|c|c|c|c|}" << std::endl;
//   outfile << "\\hline" << std::endl;
//   outfile << "& \\multicolumn{2}{|c|}{ee 0 jet} & \\multicolumn{2}{c|}{ee 1 jet} & \\multicolumn{2}{c|}{ee 2 jet} & \\multicolumn{2}{c|}{$\\mu\\mu$ 0 jet} & \\multicolumn{2}{c|}{$\\mu\\mu$ 1 jet} & \\multicolumn{2}{c|}{$\\mu\\mu$ 2 jet} \\\\" << std::endl;
//   outfile << "\\hline" << std::endl;
//   outfile << "& B & D & B & D & B & D & B & D & B & D & B & D \\\\" << std::endl;
//   outfile << "\\hline" << std::endl;

//   for (int i = 0; i != (int)sources.size(); ++i)
//   {
//     if (sources[i] == "smZ")
//     {
//       outfile << "\\hline" << std::endl;
//     }
//     outfile << "\\" << sources[i];
//     outfile.precision(1);
//     TFile* file = new TFile(("root-files/hww/" + sources[i] + "_hwwstudy.root").c_str());
//     for (int j = 0; j != (int)channels.size(); ++j)
//     {
//       for (int k = 0; k != (int)jetbins.size(); ++k)
//       {
// 	TH1D* num = (TH1D*)file->Get((channels[j] + "_" + jetbins[k] + "_breg").c_str());
// 	TH1D* den = (TH1D*)file->Get((channels[j] + "_" + jetbins[k] + "_dreg").c_str());

// 	if (sources[i].find("data") == string::npos)
// 	{
// 	  num->Scale(LUMINOSITY);
// 	  den->Scale(LUMINOSITY);
// 	}
// 	double errB = 0;
// 	double intB = num->IntegralAndError(1,1,errB);
// 	outfile << fixed << "& " << intB ;
	
// 	double errD = 0;
// 	double intD = den->IntegralAndError(1,1,errD);
// 	outfile << fixed << "& " << intD ;
//       }
//     }
//     outfile << "\\\\" << std::endl;
//     outfile << "\\hline" << std::endl;
//   }

//   outfile << "\\hline" << std::endl;
//   outfile << "\\end{tabular}" << std::endl;
//   outfile.close();


  for (int nChan = 0; nChan != (int)channels.size(); ++nChan)
  {
    for (int nJet = 0; nJet != (int)jetbins.size(); ++nJet)
    {
      //Set up output text file
      std::string filename = "macros/textfiles/aftPt_"+channels[nChan] + "_" + jetbins[nJet] + ".txt";
      std::ofstream outfile;
      outfile.open(filename.c_str());
      outfile << "\\begin{tabular}{|c|c|c|c|c|}" << std::endl;
      outfile << "\\hline" << std::endl;
      outfile << "& A & B & C & D \\\\" << std::endl;
      outfile << "\\hline" << std::endl;

      //Loop over source files
      for (int nIn = 0; nIn != (int)inputs.size(); ++nIn)
      {
	TFile* file = new TFile(("root-files/hww/" + inputs[nIn] + "_hwwstudy.root.").c_str());

      }
    }//closes for loop over jet bins
  }//closes for loop over channels

}//closes main function
