#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <iostream>

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

#include "HiggsAnalysis/SusyAnalysis.h"

std::string makeString(int number)
{
  std::stringstream ss;
  ss << number;
  return ss.str();
}

void qcdEst(std::string sample = "dataAEGamma", bool debug = false)
{
  SusyAnalysis* analysis = new SusyAnalysis();
  if (debug)
  {
    analysis->SetDebug();
  }

  std::string filename = "root-files/SUSY/" + sample + "_loose.root";
  TFile* file = new TFile(TString(filename.c_str()),"RECREATE");

  std::vector<std::string> properties;
  properties.push_back("leadpT");
  properties.push_back("subpT");
  properties.push_back("leadeta");
  properties.push_back("subeta");
  properties.push_back("leadphi");
  properties.push_back("subphi");
  properties.push_back("met");
  properties.push_back("mll");
  properties.push_back("njets");
  properties.push_back("nphots");
  properties.push_back("leadiso");
  properties.push_back("subiso");

  std::vector<int> binNs;
  binNs.push_back(200);
  binNs.push_back(200);
  binNs.push_back(60);
  binNs.push_back(60);
  binNs.push_back(60);
  binNs.push_back(60);
  binNs.push_back(100);
  binNs.push_back(125);
  binNs.push_back(10);
  binNs.push_back(5); 
  binNs.push_back(50);
  binNs.push_back(50);

  std::vector<double> lowEnd;
  lowEnd.push_back(0);
  lowEnd.push_back(0);
  lowEnd.push_back(-5);
  lowEnd.push_back(-5);
  lowEnd.push_back(-3.5);
  lowEnd.push_back(-3.5);
  lowEnd.push_back(0);
  lowEnd.push_back(0);
  lowEnd.push_back(-0.5);
  lowEnd.push_back(-0.5);
  lowEnd.push_back(0);
  lowEnd.push_back(0);

  std::vector<double> highEnd;
  highEnd.push_back(200);
  highEnd.push_back(200);
  highEnd.push_back(5);
  highEnd.push_back(5);
  highEnd.push_back(3.5);
  highEnd.push_back(3.5);
  highEnd.push_back(200);
  highEnd.push_back(250);
  highEnd.push_back(9.5);
  highEnd.push_back(4.5);
  highEnd.push_back(2.5);
  highEnd.push_back(2.5);

  const int OSStages = 8;
  const int SSStages = 5;

  std::vector<std::string> channels;
  channels.push_back("ee");
  channels.push_back("em");
  channels.push_back("me");
  channels.push_back("mm");

  TH1D* test = new TH1D("a","a",10,0,10);

  std::vector< std::vector< std::vector<TH1D*> > > oshistos((int)properties.size(),std::vector< std::vector<TH1D*> > ( (int)channels.size(), std::vector<TH1D*>(OSStages,test)));
  std::vector< std::vector< std::vector<TH1D*> > > sshistos((int)properties.size(),std::vector< std::vector<TH1D*> > ((int)channels.size(),std::vector<TH1D*>(SSStages,test)));

  for (int i = 0; i != (int)properties.size(); ++i)
  {
    for (int j = 0; j != (int)channels.size(); ++j)
    {
      for (int k = 0; k != OSStages; ++k)
      {
	std::string label = properties[i] + "_" + channels[j] + "_os_" + makeString(k);

	oshistos[i][j][k] = new TH1D(label.c_str(),label.c_str(),binNs[i],lowEnd[i],highEnd[i]);
      }
    }
  }

  for (int i = 0; i != (int)properties.size(); ++i)
  {
    for (int j = 0; j != (int)channels.size(); ++j)
    {
      for (int k = 0; k != SSStages; ++k)
      {
	std::string label = properties[i] + "_" + channels[j] + "_ss_" + makeString(k);

	sshistos[i][j][k] = new TH1D(label.c_str(),label.c_str(),binNs[i],lowEnd[i],highEnd[i]);
      }
    }
  }

  const int n_hists = 10;
  TH1D* histos[n_hists];

  //Declare histos here
  histos[0] = new TH1D("qcd_cr_loose","qcd_cr_loose",2,-0.5,1.5);

  analysis->qcdEstimate(sample, histos, oshistos, sshistos);

  file->Write();
  file->Close();
}
