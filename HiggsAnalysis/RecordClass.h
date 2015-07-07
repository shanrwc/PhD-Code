#ifndef _RECORDCLASS_HPP_
#define _RECORDCLASS_HPP_

#include "TH1D.h"

#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

using namespace std;

class RecordClass
{

private:

  double weightSum;
  vector<string> cutNames;
  bool unlinkedChannels;

  //Histograms that will maintain statistics for a full analysis. 
  TH1D* histN;
  TH1D* histC;

  //To accomodate arbitrarily many parallel channels . . .
  map <string, TH1D*> mapN;
  map <string, TH1D*> mapC;

  string tag;

public:

  //constructor
  RecordClass(string tag);

  //destructor
  ~RecordClass();

  //Unexplained ROOT weirdness
  //ClassDef(RecordClass,0);

  //This initializes the strings that will differentiate between channels.
  void setChannels(string name1 = "", string name2 = "", string name3 = "");
  void setChannels(vector<string> names);

  //This adds weight to the weightSum variable.
  void setWeightSum(double weight);

  //This changes a flag so the channel counter are independent of the base one (if, for 
  //example, you want to count events and leptons).
  void unlinkChannels();

  //These iterate the appropriate counters.  The first function will iterate 
  //the analysis-wide counters; the second will iterate the counters of the 
  //channels listed in the vector.  The last two do the same, but for the
  //the counter histograms only (used for real data).
  int addEvent(int cutNr, double xsec, string cut_name);
  int addEvent(int cutNr, double xsec, string cut_name, vector<string> channels);
  int addEvent(int cutNr, string cut_name);
  int addEvent(int cutNr, string cut_name, vector<string> channels);

  //This writes all the stored information to a text-file for later viewing and 
  //analysis.
  void print(string sampleName, bool isData = false);
};

#endif
