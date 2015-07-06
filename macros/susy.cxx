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

void susy(std::string sample = "su4", bool debug = false)
{
  SusyAnalysis* analysis = new SusyAnalysis();
  if (debug)
  {
    analysis->SetDebug();
  }

  std::string filename = "root-files/SUSY/" + sample + "_susy.root";
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

  properties.clear();
  properties.push_back("met");
  properties.push_back("mll");
  properties.push_back("njets");

  binNs.clear();
  binNs.push_back(100);
  binNs.push_back(125);
  binNs.push_back(10);

  lowEnd.clear();
  lowEnd.push_back(0);
  lowEnd.push_back(0);
  lowEnd.push_back(-0.5);

  highEnd.clear();
  highEnd.push_back(200);
  highEnd.push_back(250);
  highEnd.push_back(9.5);

  std::vector<std::string> metN;
  metN.push_back("30");
//   metN.push_back("65");
  metN.push_back("100");

  std::vector<std::string> half;
  half.push_back("sig");
  half.push_back("back");

  std::vector<std::string> sign;
  sign.push_back("os");
  sign.push_back("ss");

  std::vector< std::vector< std::vector< std::vector< std::vector<TH1D*> > > > > MMhistos((int)properties.size(),std::vector< std::vector< std::vector< std::vector<TH1D*> > > > ((int)channels.size(), std::vector< std::vector< std::vector<TH1D*> > > ((int)sign.size(), std::vector< std::vector<TH1D*> > ((int)half.size(), std::vector<TH1D*>((int)metN.size(),test)))));

  for (int i = 0; i != (int)properties.size(); ++i)
  {
    for (int j = 0; j != (int)channels.size(); ++j)
    {
      for (int k = 0; k != (int)sign.size(); ++k)
      {
	for (int l = 0; l != (int)half.size(); ++l)
	{
	  for (int m = 0; m != (int)metN.size(); ++m)
	  {
	    std::string label = "mm_" + properties[i] + "_" + channels[j] + "_" + sign[k] + "_" + half[l] + "_" + metN[m];

	    MMhistos[i][j][k][l][m] = new TH1D(label.c_str(),label.c_str(),binNs[i],lowEnd[i],highEnd[i]);
	  }
	}
      }
    }
  }

  TH1D* histos[10];
  TH2D* histos2[10];

  histos[0] = new TH1D("qcd_cr_medium","qcd_cr_medium",2,-0.5,1.5);

  histos2[0] = new TH2D("elec_eff_l","elec_eff_l",14,10,80,4,-3,3);
  histos2[0]->Sumw2();
  histos2[1] = new TH2D("elec_eff_t","elec_eff_t",14,10,80,4,-3,3);
  histos2[1]->Sumw2();
  histos2[2] = new TH2D("mu_eff_l","mu_eff_l",14,10,80,6,-3.3,3.3);
  histos2[2]->Sumw2();
  histos2[3] = new TH2D("mu_eff_t","mu_eff_t",14,10,80,6,-3.3,3.3);
  histos2[3]->Sumw2();

  analysis->SusyCutflow(sample, histos, histos2, oshistos, sshistos, MMhistos);

  file->Write();
  file->Close();
}
