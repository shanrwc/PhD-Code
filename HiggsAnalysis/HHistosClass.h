#ifndef __HHISTOSCLASS_H_
#define __HHISTOSCLASS_H_

#include "BaseAnalysis.h"
#include "TH1D.h"

#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

class HHistosClass
{
 public:
  HHistosClass();
  ~HHistosClass();

  int addEvent(double xsec, int cutNr, std::vector<std::string>& channels, 
                std::vector<MTree::Lepton*>& leptons, std::vector<MTree::Jet*>& jets, 
	       MTree::MissingET* metRel, double MT, double mll);

 private:

  //This fills a map with empty histograms
  void initialize(std::string name, std::map<int,std::vector<TH1D*> >* pointer);

  //This takes a channel name and returns the corresponding vector of histos
  std::vector<TH1D*> getHistos(std::string channel, int cutNr);

  std::map<int,std::vector<TH1D*> >* ee0jet;
  std::map<int,std::vector<TH1D*> >* ee1jet;
  std::map<int,std::vector<TH1D*> >* ee2jet;
  std::map<int,std::vector<TH1D*> >* em0jet;
  std::map<int,std::vector<TH1D*> >* em1jet;
  std::map<int,std::vector<TH1D*> >* em2jet;
  std::map<int,std::vector<TH1D*> >* mm0jet;
  std::map<int,std::vector<TH1D*> >* mm1jet;
  std::map<int,std::vector<TH1D*> >* mm2jet;


  //c++ needs this to be easier . . .
  template<typename T>
  std::string makeString(T const& value);

};

template<typename T>
std::string HHistosClass::makeString(T const& value)
{
  std::stringstream sstr;
  sstr << value;
  return sstr.str();
}

#endif
