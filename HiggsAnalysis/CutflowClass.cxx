#include "CutflowClass.h"

CutflowClass::CutflowClass()
{
  ;
}

//#######################################################################################

CutflowClass::~CutflowClass()
{
//   mapN.clear();
//   mapC.clear();
}

//#######################################################################################

void CutflowClass::setChannels(std::vector<std::string> &channels)
{
  std::vector<double> zeros;
  for (int i = 0; i != 35; ++i)
  {
    zeros.push_back(0.0);
  }

  for (int i = 0; i != (int)channels.size(); ++i)
  {
    TH1D* tempN = new TH1D((channels[i] + "_N").c_str(),(channels[i] + "_N").c_str(),35,-0.5,34.5);
    TH1D* tempC = new TH1D((channels[i] + "_C").c_str(),(channels[i] + "_C").c_str(),35,-0.5,34.5);

    mapN[channels[i]] = tempN;
    mapC[channels[i]] = tempC;

    checkC[channels[i]] = zeros;
  }
}

//#######################################################################################

int CutflowClass::addEvent(int cutNr, double xsec, std::vector<std::string> &channels)
{
  for (int i = 0; i != (int)channels.size(); ++i)
  {
    mapN[channels[i]]->Fill(cutNr,1.0);
    mapC[channels[i]]->Fill(cutNr,xsec);

    (checkC[channels[i]])[cutNr] += xsec;
  }

  ++cutNr;
  return(cutNr);
}

//#######################################################################################

std::map<std::string, TH1D*>* CutflowClass::getNumCutflows()
{
  return(&mapN);
}

//#######################################################################################

std::map<std::string, TH1D*>* CutflowClass::getXSecCutflows()
{
  return(&mapC);
}

//#######################################################################################

void CutflowClass::printXSecCheck()
{
  std::string filename = "cutflow_xcheck.txt";
  std::ofstream outfile;
  outfile.open(filename.c_str());

  std::map<std::string, std::vector<double> >::iterator iter;
  std::map<std::string, std::vector<double> >::iterator end = checkC.end();

  for (iter = checkC.begin(); iter != end; ++iter)
  {
    outfile << iter->first << std::endl;
    std::vector<double> results = iter->second;
    for (int i = 0; i != (int)results.size(); ++i)
    {
      outfile << results[i] << std::endl;
    }
    outfile << std::endl;
  }

  outfile.close();
}

//#######################################################################################
