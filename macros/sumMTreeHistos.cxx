#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TClonesArray.h"
#include "TH1D.h"

#include "MTree/Electron.h"
#include "MTree/Photon.h"
#include "MTree/EventInfo.h"
#include "MTree/Trigger.h"
#include "MTree/Jet.h"
#include "MTree/MissingET.h"
#include "MTree/Muon.h"
#include "MTree/Track.h"
#include "MTree/PrimaryVertex.h"
#include "MTree/TruthParticle.h"
#include "MTree/L1TriggerROI.h"
#include "MTree/HLTTriggerROI.h"
#include "MTree/TriggerObject.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>

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

void sumMTreeHistos(std::string family, std::string runNum = 0)
{
  std::vector<std::string> runNumbers;
  if (runNum.compare("0") == 0)
  {
    if (family == "smz")
    {
      runNumbers.push_back("107650");
      runNumbers.push_back("107651");
      runNumbers.push_back("107652");
      runNumbers.push_back("107653");
      runNumbers.push_back("107654");
      runNumbers.push_back("107655");
      runNumbers.push_back("107660");
      runNumbers.push_back("107661");
      runNumbers.push_back("107662");
      runNumbers.push_back("107663");
      runNumbers.push_back("107664");
      runNumbers.push_back("107665");
      runNumbers.push_back("106052");
      runNumbers.push_back("106046");
    }
    else if (family == "top")
    {
//       runNumbers.push_back("105200");
      runNumbers.push_back("117360");
      runNumbers.push_back("117361");
      runNumbers.push_back("117362");
      runNumbers.push_back("108343");
      runNumbers.push_back("108344");
      runNumbers.push_back("108345");
      runNumbers.push_back("105500");
    }
    else if (family == "smdy")
    {
      runNumbers.push_back("116250");
      runNumbers.push_back("116251");
      runNumbers.push_back("116252");
      runNumbers.push_back("116253");
      runNumbers.push_back("116254");
      runNumbers.push_back("116255");
      runNumbers.push_back("116260");
      runNumbers.push_back("116261");
      runNumbers.push_back("116262");
      runNumbers.push_back("116263");
      runNumbers.push_back("116264");
      runNumbers.push_back("116265");
      runNumbers.push_back("107055");
      runNumbers.push_back("105148");
      runNumbers.push_back("108320");
      runNumbers.push_back("113700");
      runNumbers.push_back("113701");
    }
    else if (family == "higgs")
    {
      runNumbers.push_back("116707");
      runNumbers.push_back("125008");
    }
    else if (family == "smww")
    {
      runNumbers.push_back("105921");
      runNumbers.push_back("105922");
      runNumbers.push_back("105923");
      runNumbers.push_back("105924");
      runNumbers.push_back("105925"); 
      runNumbers.push_back("105926");
      runNumbers.push_back("105927");
      runNumbers.push_back("105928");
      runNumbers.push_back("105929");
      runNumbers.push_back("106011");
      runNumbers.push_back("106012");
      runNumbers.push_back("106013");
      runNumbers.push_back("106014");
      runNumbers.push_back("106015");
      runNumbers.push_back("106016");
      runNumbers.push_back("106017");
      runNumbers.push_back("106018");
      runNumbers.push_back("106019");
    }
    else if (family == "smew")
    {
      runNumbers.push_back("105940");
      runNumbers.push_back("105941");
      runNumbers.push_back("105942");
      runNumbers.push_back("105970");
      runNumbers.push_back("105971");
      runNumbers.push_back("105972");
      runNumbers.push_back("106024");
      runNumbers.push_back("106025");
      runNumbers.push_back("106026");
      runNumbers.push_back("106027");
      runNumbers.push_back("106028");
      runNumbers.push_back("106029");
      runNumbers.push_back("113190");
      runNumbers.push_back("113191");
      runNumbers.push_back("105930");
      runNumbers.push_back("105931");
      runNumbers.push_back("105932");
      runNumbers.push_back("106036");
      runNumbers.push_back("106037");
      runNumbers.push_back("113192");
      runNumbers.push_back("113193");
      runNumbers.push_back("116600");
      runNumbers.push_back("106001");
      runNumbers.push_back("106002");
      runNumbers.push_back("106003");
      runNumbers.push_back("108288");
      runNumbers.push_back("108289");
      runNumbers.push_back("108290");
      runNumbers.push_back("118615");
      runNumbers.push_back("118616");
      runNumbers.push_back("118617");
      runNumbers.push_back("118618");
    }
    else if (family == "smw")
    {
      runNumbers.push_back("107680");
      runNumbers.push_back("107681");
      runNumbers.push_back("107682");
      runNumbers.push_back("107683");
      runNumbers.push_back("107684");
      runNumbers.push_back("107685");
      runNumbers.push_back("107690");
      runNumbers.push_back("107691");
      runNumbers.push_back("107692");
      runNumbers.push_back("107693");
      runNumbers.push_back("107694");
      runNumbers.push_back("107695");
      runNumbers.push_back("106022");
    }
    else
    {
      std::cout << "Family of runs is unknown!" << std::endl;
      return;
    }
  }
  else
  {
    runNumbers.push_back(runNum);
  }

  for (int nNum = 0; nNum != (int)runNumbers.size(); ++nNum)
  {
    std::string runNumber = runNumbers[nNum];

    //Make vector of the file names/paths to all MTrees in this sample.
    std::vector<std::string> run;

    std::string inputFile = "HiggsAnalysis/samples/" + family + ".txt";
    ifstream infile(inputFile.c_str());
    std::string line;
    if (infile.is_open())
    {
      while (infile.good())
      {
        getline(infile,line);
        if (line.find(runNumber) != string::npos)
	{ 
// 	  std::cout << "Found substring " << runNumber << std::endl;
          std::vector<std::string> holder = splitString(line);
          if ((int)holder.size() < 3) continue;
          run.push_back((holder[1] + "/" + holder[2]).c_str());
	}
      }
    }

    //Make an .root file for mu histogram
    std::string outRoot = "/net/s3_dataa/MTree/MTree-16-01-15/AOD/mc10b/pv-hists/mc/" + runNumber + ".root";
    TFile* file = new TFile(TString(outRoot.c_str()),"RECREATE");

    //Loop over input files, opening each and grabbing mu histo and skim histo.
    //Add them to globally defined sums.
    TH1D* muHist = 0;
    TH1D* effHist = 0;

    for (int i = 0; i != (int)run.size(); ++i)
    {
      TFile* temp = new TFile(TString(run[i].c_str()));

      TH1D* histoMu = (TH1D*)temp->Get("mu");
      TH1D* histoEff = (TH1D*)temp->Get("logHist");

      file->cd();
      if (i == 0)
      {
        muHist = (TH1D*)histoMu->Clone();
        effHist = (TH1D*)histoEff->Clone();
      }
      else
      {
        muHist->Add(histoMu);
        effHist->Add(histoEff);
      }
      file->cd();
      temp->Close();
    }

    if (muHist == 0 || effHist == 0)
    {
      std::cout << "ERROR: failed to find histograms for run " << runNumber << std::endl;
      return;
    }

    //Create text file and put output of effHist in it.
    std::string txtfileName = "/net/s3_dataa/MTree/MTree-16-01-15/AOD/mc10b/logs/mc/" + runNumber + ".txt";
    std::ofstream txtfile;
    txtfile.open(txtfileName.c_str());
    txtfile << effHist->GetXaxis()->GetBinLabel(1) << ": " << effHist->GetBinContent(1) << std::endl;
    txtfile << effHist->GetXaxis()->GetBinLabel(2) << ": " << effHist->GetBinContent(2) << std::endl;
    txtfile << effHist->GetXaxis()->GetBinLabel(3) << ": " << effHist->GetBinContent(3) << std::endl;
    txtfile << effHist->GetXaxis()->GetBinLabel(4) << ": " << effHist->GetBinContent(4) << std::endl;
    txtfile.close();

    file->cd();
    file->Write();
    file->Close();
  }
}//closes function
