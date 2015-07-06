#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

void qcdCutflow()
{
  const double lumin = 0.0157;

  TFile* real = new TFile("root-files/SUSY/susy/data_susy.root");
  TFile* suz = new TFile("root-files/SUSY/susy/su4_susy.root");
  TFile* ww = new TFile("root-files/SUSY/susy/smww_susy.root");
  TFile* wlv = new TFile("root-files/SUSY/susy/Wlv_susy.root");
  TFile* zll = new TFile("root-files/SUSY/susy/Zll_susy.root");
  TFile* tt = new TFile("root-files/SUSY/susy/tt_susy.root");
  TFile* jxm = new TFile("root-files/SUSY/susy/jetmuons_susy.root");

  TFile* lz = new TFile("root-files/SUSY/Loose/Zll_loose.root");
  TFile* lreal = new TFile("root-files/SUSY/Loose/data_loose.root");

  TH1D* cr_med = (TH1D*)real->Get("qcd_cr_medium");
  double medCr = cr_med->Integral();
  TH1D* cr_z = (TH1D*)zll->Get("qcd_cr_medium");
  cr_z->Scale(lumin);
  double zCr = cr_z->Integral();
  TH1D* cr_qcd = (TH1D*)jxm->Get("qcd_cr_medium");
  cr_qcd->Scale(lumin);
  double qcdCr = cr_qcd->Integral();

  TH1D* cr_los = (TH1D*)lreal->Get("qcd_cr_loose");
  double losCr = cr_los->Integral();

  std::vector<TFile*> files;
  files.push_back(real);
  files.push_back(suz);
  files.push_back(ww);
  files.push_back(wlv);
  files.push_back(zll);
  files.push_back(tt);
  files.push_back(jxm);

  std::vector<std::string> fileNames;
  fileNames.push_back("Data                ");
  fileNames.push_back("SU4                 ");
  fileNames.push_back("WW                  ");
  fileNames.push_back("W+jets              ");
  fileNames.push_back("Z+jets              ");
  fileNames.push_back("ttbar               ");
  fileNames.push_back("QCD                 ");

  std::vector<std::string> signs;
  signs.push_back("os");
//   signs.push_back("ss");

  std::vector<std::string> channels;
  channels.push_back("ee");
  channels.push_back("em");
  channels.push_back("me");
  channels.push_back("mm");

  std::vector<std::string> phase;
  phase.push_back("0");
  phase.push_back("1");
  phase.push_back("2");
  phase.push_back("3");
  phase.push_back("4");
  phase.push_back("5");
  phase.push_back("6");
  phase.push_back("7");

  std::vector <std::string> phaseName;
  phaseName.push_back("2 leptons      ");
  phaseName.push_back("mll > 5        ");
  phaseName.push_back("met > 30       ");
  phaseName.push_back("met > 30 + iso ");
  phaseName.push_back("met > 65       ");
  phaseName.push_back("met > 65 + iso ");
  phaseName.push_back("met > 100      ");
  phaseName.push_back("met > 100 + iso");


  for (int i = 0; i != (int)signs.size(); ++i)
  {
    //Create appropriate text file here.
    std::string filename = "cutflows/" + signs[i] + ".histo.cutflow.txt";
    ofstream outfile;
    outfile.open(filename.c_str());
    outfile << "                    ee        em        me        mm        Total" << std::endl;
    for (int j = 0; j != (int)phase.size(); ++j)
    {
      outfile << phaseName[j] << std::endl << std::endl;
      for (int k = 0; k != (int)files.size(); ++k)
      {
	//Set up numbers to store total
	double total = 0;
	outfile << fileNames[k];
	for (int m = 0; m != (int)channels.size(); ++m)
	{
	  std::string label = "njets_" + channels[m] + "_" + signs[i] + "_" + phase[j];
	  TFile* f = files[k];
	  TH1D* histo;

	  if (k == 0)
	  {
	    histo = (TH1D*)f->Get(label.c_str());
	  }
	  else if ((k > 0) && (k < 6))
	  {
	    histo = (TH1D*)f->Get(label.c_str());
	    histo->Scale(lumin);
	  }
	  else
	  {
	    if (m == 0)
	    {
	      TH1D* ldata = (TH1D*)lreal->Get(label.c_str());
	      TH1D* lzzs = (TH1D*)lz->Get(label.c_str());
	      lzzs->Scale(lumin);

	      ldata->Add(lzzs,-1);
              ldata->Scale(medCr/(medCr+losCr));
	      histo  = (TH1D*)ldata->Clone();
	    }
	    else
	    {
	      histo = (TH1D*)f->Get(label.c_str());
	      histo->Scale(lumin);
	      histo->Scale((medCr-zCr)/qcdCr);
	    }
	  }

	  outfile << histo->Integral() << "   ";
	  if (k == 0)
	  {
	    outfile << "   ";
	  }
	  total += histo->Integral();
	}//Closes loop over channels
	//Print out total over channels, too
	outfile << total << std::endl;
      }//Closes loop over files
      outfile << std::endl;
    }//Closes loop over phases
    //Close text file here
    outfile.close();
    std::cout << "made text file" << std::endl;
  }//Closes loop over os and ss

}//Closes function
