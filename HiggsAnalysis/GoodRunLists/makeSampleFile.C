#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

int atoi(string str);
string itoa(int i);
struct entryInfo
{
  int runNr;
  vector<string> vecStartLB;
  vector<string> vecEndLB;
  double lumi;

  int getRunNr() const {return runNr;}

  bool operator<(const entryInfo& rhs) const
  {
    return runNr < rhs.getRunNr();
  }

  entryInfo operator&(entryInfo& rhs)
  {
    entryInfo merged;
    merged.runNr = runNr;

    int nrLB_L = vecStartLB.size();
    int nrLB_R = rhs.vecStartLB.size();

    if (nrLB_L == 0 || nrLB_R == 0)
    {
      return merged;
    }

    int maxLB = min(atoi(vecEndLB[nrLB_L-1]), atoi(rhs.vecEndLB[nrLB_R-1]));
    int minLB = max(atoi(vecStartLB[0]), atoi(rhs.vecStartLB[0]));

    bool inLB = false;
    for (int i=minLB;i<=maxLB+1;i++)
    {
      //cout << "scanning lb: " << i << endl;
      if (includesLB(i) && rhs.includesLB(i))
      {
	//cout << "found one" << endl;
	if (!inLB)
	{
	  merged.vecStartLB.push_back(itoa(i));
	  //cout << "found start lb: " << i << endl;
	}
	inLB = true;
      }
      else
      {
	if (inLB)
	{
	  merged.vecEndLB.push_back(itoa(i-1));
	  //cout << "found end lb: " << i << endl;
	}
	inLB = false;
      }
    }

    return merged;
  }

  bool includesLB(int LB)
  {
    int nrLB = vecStartLB.size();
    for (int i=0;i<nrLB;i++)
    {
      if (atoi(vecStartLB[i]) <= LB && LB <= atoi(vecEndLB[i])) return true; 
    }
    return false;
  }
};

void convertXML(string xmlFileName, map<int, entryInfo>& entryMap);
void convertLumi(string lumiFileName, map<int, entryInfo>& entryMap);
void writeOutFile(string outFileName, string folder, map<int, entryInfo>& entryMap);
void mergeXML(string xmlFileName1, string xmlFileName2, string outFileName);
void writeXML(string outFileName, map<int, entryInfo>& entryMap);

int atoi(string str)
{
  stringstream strs;
  strs << str;
  int i;
  strs >> i;
  return i;
}

string itoa(int i)
{
  stringstream strs;
  strs << i;
  string str;
  strs >> str;
  return str;
}

void mergeXML(string xmlFileName1, string xmlFileName2, string outFileName)
{
  map<int, entryInfo> entryMap1;
  map<int, entryInfo> entryMap2;
  convertXML(xmlFileName1, entryMap1);
  convertXML(xmlFileName2, entryMap2);


  map<int, entryInfo> mergedEntryMap;
  for (map<int, entryInfo>::iterator itr=entryMap1.begin();itr!=entryMap1.end();itr++)
  {
    int runNr = itr->first;
    if (entryMap2[runNr].vecStartLB.size())
    {
      mergedEntryMap[runNr] = itr->second & entryMap2[runNr];
    }
  }
  writeXML(outFileName, mergedEntryMap);
}

void writeXML(string outFileName, map<int, entryInfo>& entryMap)
{
  ofstream outFile(outFileName.c_str());
  outFile << "<?xml version=\"1.0\"?>\n"
	  << "<LumiRangeCollection>\n"
	  << "  <NamedLumiRange>\n"
	  << "    <Name>Merged</Name>\n";
  for (map<int, entryInfo>::iterator itr=entryMap.begin();itr!=entryMap.end();itr++)
  {
    outFile << "    <LumiBlockCollection>\n";
    outFile << "      <Run>" << itr->first << "</Run>\n";
    int nrLB = itr->second.vecStartLB.size();
    for (int i=0;i<nrLB;i++)
    {
      outFile << "      <LBRange Start=\"" << itr->second.vecStartLB[i] << "\" End=\"" << itr->second.vecEndLB[i] << "\"/>\n";
    }
    outFile << "    </LumiBlockCollection>\n";
  }
  outFile << "  </NamedLumiRange>\n";
  outFile << "</LumiRangeCollection>\n";

  outFile.close();
}

void makeSampleFile(string xmlFileName, string lumiFileName, string outFileName, string folder)
{
  map<int, entryInfo> entryMap;
  convertXML(xmlFileName, entryMap);
  convertLumi(lumiFileName, entryMap);
  writeOutFile(outFileName, folder, entryMap);
}

void addEntry(int runNr, vector<string>& vecStartLB, vector<string>& vecEndLB, 
	      map<int, entryInfo>& entryMap)
{
  cout << "Adding entry for run: " << runNr << endl;
  cout << "Nr LBs: " << vecStartLB.size() << endl;
  entryInfo info;
  info.runNr = runNr;
  info.vecStartLB = vecStartLB;
  info.vecEndLB = vecEndLB;

  if (entryMap[runNr].vecStartLB.size())
    {
      cout << "Possibly overwriting entry: " << runNr << "! " << endl;
    }

  entryMap[runNr] = info;
}

void writeOutFile(string outFileName, string folder, map<int, entryInfo>& entryMap)
{
  map<int, entryInfo>::iterator mitr  = entryMap.begin();
  map<int, entryInfo>::iterator mitrE = entryMap.end();

  //convert map into ordered set
  set<entryInfo> entrySet;
  for (; mitr!=mitrE; mitr++)
    {
      entrySet.insert((*mitr).second);
    }

  set<entryInfo>::iterator sitr = entrySet.begin();
  set<entryInfo>::iterator sitrE = entrySet.end();


  ofstream outFile(outFileName.c_str());
  for (; sitr!=sitrE; sitr++)
    {
      entryInfo info = *sitr;

      outFile << "list " << folder << (folder==""?"":"/") << outFileName << "/" << info.runNr << endl;
      outFile << "lumi " << info.lumi << " invnb" << endl;

      vector<string> vecStartLB = info.vecStartLB;
      vector<string> vecEndLB = info.vecEndLB;

      int nrLBs = vecStartLB.size();
      for (int i=0; i<nrLBs; i++)
	{
	  outFile << "block " << vecStartLB[i] << " " << vecEndLB[i] << endl;
	}
      outFile << endl;
    }
  outFile.close();
}

void convertLumi(string lumiFileName, map<int, entryInfo>& entryMap)
{
  ifstream inFile(lumiFileName.c_str());
  if (inFile.fail())
    {
      cout << "Couldn't open lumi file: " << lumiFileName << endl;
      exit(1);
    }

  while (!inFile.eof())
    {
      //get runNr
      int runNr;
      inFile >> runNr;
      
      
      //skip to prescaled lumi
      string junk;
      for (int i=0; i<6; i++) inFile >> junk;
      
      double lumi;
      inFile >> lumi;
      
      //kill the rest of the line
      getline(inFile, junk);

      if (!entryMap[runNr].vecStartLB.size())
	{
	  cout << "entryInfo for run: " << runNr << " doesn't exit! Skipping.." << endl;
	  continue;
	}

      entryMap[runNr].lumi = lumi;
    }
  inFile.close();
}

void convertXML(string xmlFileName, map<int, entryInfo>& entryMap)
{
  ifstream xmlFile(xmlFileName.c_str());

  if (xmlFile.fail())
    {
      cout << "Couldn't open file: " << xmlFileName << endl;
      exit(1);
    }

  //ofstream outFile(outFileName.c_str());

  while (!xmlFile.eof())
    {
      string line;
      getline(xmlFile, line);
      if (line.find("<LumiBlockCollection>") != string::npos)
	{
	  string run;
	  getline(xmlFile, run);
	  int firstPos = run.find("<Run>") + string("<Run>").size();
	  int lastPos = run.find("</Run>");
	  string runNr = "";

	  for (int i=firstPos; i<lastPos; i++)
	    {
	      runNr+=run[i];
	    }
	  //cout << runNr << endl;

	  vector<string> startNrs;
	  vector<string> endNrs;

	  getline(xmlFile, line);
	  while (line.find("</LumiBlockCollection>") == string::npos)
	    {
	      
	      string start = "";
	      
	      firstPos = line.find("Start") + string("Start=\"").size();
	      lastPos = line.find("\" End");
	      
	      for (int i=firstPos; i<lastPos; i++)
		{
		  start += line[i];
		}
	      
	      firstPos = lastPos + string("\" End=\"").size();
	      lastPos = line.find("\"/>");
	      
	      string end = "";
	      
	      for (int i=firstPos; i<lastPos; i++)
		{
		  end += line[i];
		}
	      
	      //cout << start << ", " << end << endl;

	      startNrs.push_back(start);
	      endNrs.push_back(end);

	      getline(xmlFile, line);
	    }

	  int runNrInt = atoi(runNr.c_str());
	  addEntry(runNrInt, startNrs, endNrs, entryMap);

	}
    }

  xmlFile.close();
}
