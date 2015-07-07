#include "RecordClass.h"

RecordClass::RecordClass(string label)
{
  tag = label;

  histN = new TH1D((tag + "_main_eventNr").c_str(),"main_eventNr",20,0,20);
  histC = new TH1D((tag + "_main_weightNr").c_str(),"main_weightNr",20,0,20);

  weightSum = 0;
  unlinkedChannels = false;
}

RecordClass::~RecordClass()
{
  delete histN;
  delete histC;

  mapN.clear();
  mapC.clear();
}

void RecordClass::setChannels(string name1, string name2,  string name3)
{
  if (name1 != "")
  {
    string name1N = name1 + tag + "N";
    string name1W = name1 + tag + "W";
    TH1D* histN1 = new TH1D(name1N.c_str(),name1N.c_str(),20,-0.5,19.5);
    TH1D* histW1 = new TH1D(name1W.c_str(),name1W.c_str(),20,-0.5,19.5);
    mapN[name1] = histN1;
    mapC[name1] = histW1;
  }

  if (name2 != "")
  {
    string name2N = name2 + tag + "N";
    string name2W = name2 + tag + "W";
    TH1D* histN2 = new TH1D(name2N.c_str(),name2N.c_str(),20,-0.5,19.5);
    TH1D* histW2 = new TH1D(name2W.c_str(),name2W.c_str(),20,-05,19.5);
    mapN[name2] = histN2;
    mapC[name2] = histW2;
  }

  if (name3 != "")
  {
    string name3N = name3 + tag + "N";
    string name3W = name3 + tag + "W";
    TH1D* histN3 = new TH1D(name3N.c_str(),name3N.c_str(),20,-0.5,19.5);
    TH1D* histW3 = new TH1D(name3W.c_str(),name3W.c_str(),20,-0.5,19.5);
    mapN[name3] = histN3;
    mapC[name3] = histW3;
  }
}

void RecordClass::setChannels(vector<string> names)
{
  vector<string>::iterator iter;
  vector<string>::iterator end = names.end();

  for (iter = names.begin(); iter != end; ++iter)
  {
    string name = *iter;
    string nameN = name + tag + "N";
    string nameW = name + tag + "W";
    TH1D* histN = new TH1D(nameN.c_str(),nameN.c_str(),20,-0.5,19.5);
    TH1D* histC = new TH1D(nameW.c_str(),nameW.c_str(),20,-0.5,19.5);
    mapN[name] = histN;
    mapC[name] = histC;
  }
}

void RecordClass::setWeightSum(double weight)
{
  weightSum += weight;
}

void RecordClass::unlinkChannels()
{
  unlinkedChannels = true;
}

int RecordClass::addEvent(int cutNr, double xsec, string cut_name)
{
  //1. If this cut hasn't been reached before, add cut_name to list.
  if(cutNr + 1 > (int)cutNames.size())
  {
    cutNames.push_back(cut_name);
  }

  histN->Fill(double(cutNr),1);
  histC->Fill(double(cutNr),xsec);

  //2. Iterate counter and return it.
  cutNr++;
  return(cutNr);
}

int RecordClass::addEvent(int cutNr, double xsec, string cut_name, vector<string> channels)
{
  int i;

  //1. If this cut hasn't been reached before, add cut_name to list.
  if (cutNr + 1 > (int)cutNames.size())
  {
    cutNames.push_back(cut_name);
  }

  //2. Iterate channels listed in vector.
  for (i = 0; i < (int)channels.size(); i++)
  {
    string name = channels[i];
    map<string,TH1D*>::iterator iter = mapN.find(name);
    map<string,TH1D*>::iterator iterC = mapC.find(name);
    if (iter != mapN.end())
    {
      (iter->second)->Fill(double(cutNr),1);
      (iterC->second)->Fill(double(cutNr),xsec);
    }

  }

  //3. Iterate counter and return it.
  cutNr++;
  return(cutNr);
}

int RecordClass::addEvent(int cutNr, string cut_name)
{
  //1. If this cut hasn't been reached before, add cut_name to list.
  if(cutNr + 1 > (int)cutNames.size())
  {
    cutNames.push_back(cut_name);
  }

  histN->Fill(double(cutNr),1);

  //2. Iterate counter and return it.
  cutNr++;
  return(cutNr);
}

int RecordClass::addEvent(int cutNr, string cut_name, vector<string> channels)
{
  int i;

  //1. If this cut hasn't been reached before, add cut_name to list.
  if (cutNr + 1 > (int)cutNames.size())
  {
    cutNames.push_back(cut_name);
  }

  //2. Iterate channels listed in vector.
  for (i = 0; i < (int)channels.size(); i++)
  {
    string name = channels[i];
    map<string,TH1D*>::iterator iter = mapN.find(name);
    if (iter != mapN.end())
    {
      (iter->second)->Fill(double(cutNr),1);
    }
  }

  //3. Iterate counter and return it.
  cutNr++;
  return(cutNr);
}

void RecordClass::print(string sampleName, bool isData)
{
  int i =0;
  int j =0;
  string filename;
  ofstream outfile;

  filename = "cutflows/" + sampleName + ".txt";
  const char* tag = filename.c_str();
  outfile.open(tag);

  if (!isData)
  {
    outfile << setprecision(8) << "Cut: " << sampleName << endl;
    outfile << "Total number of Events: " << histN->GetBinContent(1) << endl;
    outfile << "Total Weight Sum: " << weightSum << endl;
    outfile << "Total cross-section: "<< histC->GetBinContent(1) << endl << endl;

    while(histN->GetBinContent(i+2) > 0)
    {
      outfile << "Cut: " << cutNames[i+1] << endl;
      outfile << "Events passed: " << histN->GetBinContent(i+2) << endl;
      outfile << "Weight passed: " << histC->GetBinContent(i+2)/histC->GetBinContent(1)*weightSum << endl;
      outfile << "Cross-section: " << histC->GetBinContent(i+2) << endl;
      outfile << "Relative Efficiency: " << histC->GetBinContent(i+2)/histC->GetBinContent(i+1) << endl;
      outfile << "Total Efficiency: " << histC->GetBinContent(i+2)/histC->GetBinContent(1) << endl;
      outfile << endl;
      i++;
    }

    map<string,TH1D*>::iterator iter;
    map<string,TH1D*>::iterator end = mapN.end();
    for (iter = mapN.begin(); iter != end; ++iter)
    {
      string name = iter->first;
      if (name == "") continue;
      TH1D* histN_1 = iter->second;
      TH1D* histW_1 = mapC[name];
      j = 0;

      while (histN_1->GetBinContent(i+j+2) > 0)
      {
        if (j == 0)
        {
          outfile << "Channel: " << name << endl << endl;
        }
        outfile << "Cut: " << cutNames[i+j+1] << endl;
        outfile << "Events passed: " << histN_1->GetBinContent(i+j+2) << endl;
        outfile << "Weight passed: " << histW_1->GetBinContent(i+j+2)/histC->GetBinContent(1)*weightSum << endl;
        outfile << "Cross-section: " << histW_1->GetBinContent(i+j+2) << endl;
	if (unlinkedChannels) 
	{
	  if (j != 0)
	  {
	    outfile << "Relative Efficiency: " << histW_1->GetBinContent(i+j+2)/histW_1->GetBinContent(i+j+1) << endl; 
            outfile << "Total Efficiency: " << histW_1->GetBinContent(i+j+2)/histW_1->GetBinContent(i+2) << endl;
	  }
	}
	else
	{       
          if (j == 0)
          {
            outfile << "Relative Efficiency: " << histW_1->GetBinContent(i+j+2)/histC->GetBinContent(i+1) << endl;
          }
          else
          {
            outfile << "Relative Efficiency: " << histW_1->GetBinContent(i+j+2)/histW_1->GetBinContent(i+j+1) << endl;
          }
          outfile << "Total Efficiency: " << histW_1->GetBinContent(i+j+2)/histC->GetBinContent(1) << endl;
	}
        outfile << endl;
        j++;
      }
    }

  }
  else
  {
    outfile << setprecision(8) << "Cut: " << sampleName << endl;
    outfile << "Total number of Events: " << histN->GetBinContent(1) << endl;

    while(histN->GetBinContent(i+2) > 0)
    {
      outfile << "Cut: " << cutNames[i+1] << endl;
      outfile << "Events passed: " << histN->GetBinContent(i+2) << endl;
      outfile << "Relative Efficiency: " << histN->GetBinContent(i+2)/histN->GetBinContent(i+1) << endl;
      outfile << "Total Efficiency: " << histN->GetBinContent(i+2)/histN->GetBinContent(1) << endl;
      outfile << endl;
      i++;
    }

    map<string,TH1D*>::iterator iter;
    map<string,TH1D*>::iterator end = mapN.end();
    for (iter = mapN.begin(); iter != end; ++iter)
    {
      string name = iter->first;
      if (name == "") continue;
      TH1D* histN_1 = iter->second;
      j = 0;

      while (histN_1->GetBinContent(i+j+2) > 0)
      {
        if (j == 0)
        {
          outfile << "Channel: " << name << endl << endl;
        }
        outfile << "Cut: " << cutNames[i+j+1] << endl;
        outfile << "Events passed: " << histN_1->GetBinContent(i+j+2) << endl;
	if (unlinkedChannels)
	{
	  if (j != 0)
	  {
	    outfile << "Relative Efficiency: " << histN_1->GetBinContent(i+j+2)/histN_1->GetBinContent(i+j+1) << endl; 
            outfile << "Total Efficiency: " << histN_1->GetBinContent(i+j+2)/histN_1->GetBinContent(i+2) << endl;
	  }
	}
	else
	{
          if (j == 0)
          {
            outfile << "Relative Efficiency: " << histN_1->GetBinContent(i+j+2)/histN->GetBinContent(i+1) << endl;
          }
          else
          {
            outfile << "Relative Efficiency: " << histN_1->GetBinContent(i+j+2)/histN_1->GetBinContent(i+j+1) << endl;
          }
          outfile << "Total Efficiency: " << histN_1->GetBinContent(i+j+2)/histN->GetBinContent(1) << endl;
	}
        outfile << endl;
        j++;
      }
    }
  }

  outfile << "EOF" << endl;
  outfile.close();
}

//ClassImp(RecordClass)
