#ifndef __ZCONTROL_H_
#define __ZCONTROL_H_

#include "BaseAnalysis.h"
#include "CutCodeClass.h"
#include "HWWConstants.h"

#include <iostream>
#include <string>
#include <map>
#include <vector>

class ZControlClass
{
 public:
  ZControlClass();
  ~ZControlClass();

  void addEvent(std::string channel, std::vector<MTree::Lepton*> leptons, std::vector<MTree::Jet*> jets, MTree::MissingET* met, double mll, double mT, double Npv, double mu, double weight);

  void printRegions(std::string sample);
 private:
  CutCodeClass* code;

  std::map<std::string, double> numberA;
  std::map<std::string, double> numberB;
  std::map<std::string, double> numberC;
  std::map<std::string, double> numberD;

  //plots for sanity checks
  TH2D* ttmet1;
  TH2D* ttmll1;
  TH2D* ttmet2;
  TH2D* ttmll2;

  TH1D* metlpv;
  TH1D* methpv;

  //These maps store mT distributions of events in the Z-window that pass
  //all topological cuts.
  std::map<std::string,TH1D*> cMetAft;
  std::map<std::string,TH1D*> dMetAft;

  std::map<std::string, TH1D*> metAmap;
  std::map<std::string, TH1D*> mllAmap;
  std::map<std::string, TH1D*> metBmap;
  std::map<std::string, TH1D*> mllBmap;
  std::map<std::string, TH1D*> metCmap;
  std::map<std::string, TH1D*> mllCmap;
  std::map<std::string, TH1D*> metDmap;
  std::map<std::string, TH1D*> mllDmap;


  std::map<std::string, TH2D*> mllmet;
  std::map<std::string, TH2D*> npvmet;
  std::map<std::string, TH2D*> mumet;
  std::map<std::string, TH2D*> npvnjet;

  std::map<std::string, TH1D*> breg;
  std::map<std::string, TH1D*> dreg;
  std::map<std::string, TH1D*> bscan;
  std::map<std::string, TH1D*> dscan;

  std::map<std::string, TH1D*> bwidth;
  std::map<std::string, TH1D*> dwidth;

  std::map<std::string, TH1D*> diffMetB;
  std::map<std::string, TH1D*> diffMetD;

  //nPV plots and plots of met for low and high nPVs (>= 6 vs < 6)
  std::map<std::string, TH1D*> npvmap;
  std::map<std::string, TH1D*> metlPV;
  std::map<std::string, TH1D*> methPV;

  std::string getChannel(std::vector<MTree::Lepton*> leptons, int nJets);
  void fill1DHistos(double mll, double met, std::string chan,double weight);
  void scanZWindows(double mll, double met, std::string chan,double weight);
  void varyMETCut(double met, std::string chan, double weight);
};

#endif
