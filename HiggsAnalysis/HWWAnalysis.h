#ifndef __HWWANALYSIS_H_
#define __HWWANALYSIS_H_

#include "BaseAnalysis.h"
#include "HObjectSelector.h"
#include "CutflowClass.h"
#include "CutlistClass.h"
#include "HHistosClass.h"
#include "ZControlClass.h"
#include "CutCodeClass.h"
#include "WWEventClass.h"
#include "ggFReweighting.h"

#include "HWWConstants.h"

#include "CalibrationDataInterface/CalibrationDataInterfaceBase.h"
#include "CalibrationDataInterface/CalibrationDataInterfaceROOT.h"

class HWWAnalysis : public BaseAnalysis
{
 public:
  HWWAnalysis();
  ~HWWAnalysis();

  ClassDef(HWWAnalysis,0);

  //Functions the macro calls go here
  void h2wwCutflow(std::string sample, TFile* file, bool lowMassm, double hMass, bool comset);
  void Initialize(std::string sample, TFile* file);
  void fillPVrefs(std::string sample);

 private:
  //Functions and objects the class uses go here
  CutCodeClass* code;
  HObjectSelector* selector;
  CutflowClass* cutflow;
  CutlistClass* cutlist;
  HHistosClass* histos;
  ZControlClass* zregion;
  WWEventClass* wwevent;

  ggFReweighting* reweight;

  Analysis::CalibrationDataInterfaceROOT* calib;
  Analysis::CalibrationDataVariables* ajet;

  TPileupReweighting* pileup;

  MTree::MissingET* metForWW;
  MTree::MissingET* metRel;

  //Book-keeping functionality
  std::vector<std::string> findChannels(std::vector<bool> triggers);
  std::vector<std::string> findChannels(std::vector<MTree::Lepton*> &leptons);
  std::vector<std::string> findChannels(std::vector<MTree::Lepton*> &leptons, int nJets);

  //Random helpful stuff
  double primVtxWeight(int nGoodPVs);
  bool usePVWeight(std::string sample);
  double getHiggsPtWeight(std::string sample);
  double getBJetWeight(std::vector<MTree::Jet*> jets);

  template<class T>
  void orderByPt(std::vector<T*> &vec);

  template<class T>
  void removeOverlap(double cut, std::vector<T*> &vec);

  template<class T, class S>
  void removeOverlap(double cut, std::vector<T*> &keepers, std::vector<S*> &leavers);

  void correctMET(std::vector<MTree::Lepton*> leptons, std::vector<MTree::Jet*> jets);

  //Selection criteria
  void muonjetOverlap(double cut, std::vector<MTree::Muon*> &muons, std::vector<MTree::Jet*> &jets);

  void selectGoodPVs(bool &goodPV, int &nGoodPVs, int eventNumber);
  bool badJetsPresent(std::vector<MTree::Lepton*> leptons, std::string& result);
  bool passDeadFEB(bool isMC, double& evWeight,int runNumber, int eventNumber);

  bool IsLArHole(float eta, float phi,
		 float etamin = -0.1, float etamax = 1.5,
		 float phimin = -0.9, float phimax = -0.5)
  {
    if (eta < etamin || eta > etamax) return false;
    if (phi < phimin || phi > phimax) return false;
    return true;     
  }

  //If the function below returns true, event needs to be vetoed.
  bool IsLArHoleVeto(float jet_pt, 
		     float jet_eta,                
		     float jet_phi,                
		     float jet_BCH_CORR_JET, 
		     float jet_BCH_CORR_CELL,
		     bool kIsData,                  //data or mc flag
		     float threshold=40000.)
  {
    if (!IsLArHole(jet_eta,jet_phi)) return false;
    float pt = threshold;
    // Correct threshold cut by missing energy in LAr hole in data.
    // BCH_CORR_CELL is necessary to take into account dead tile module near LAr hole.
    if (kIsData) pt = pt*(1. - jet_BCH_CORR_JET)/(1. - jet_BCH_CORR_CELL);
    if (jet_pt <= pt) return false;
    return true;
  }

  int febEs;
  int badEs;
  int febJs;
  int badJs;

  TH1D* Ptll;
  TH1D* Mll;

};


template<class T>
void HWWAnalysis::orderByPt(std::vector<T*> &vec)
{
  for (int i = 0; i != (int)vec.size(); ++i)
  {
    for (int j = i + 1; j != (int)vec.size(); ++j)
    {
      if (vec[j]->Pt() > vec[i]->Pt())
      {
	swap(vec[i],vec[j]);
      }
    }
  }
}

template<class T, class S>
void HWWAnalysis::removeOverlap(double cut, std::vector<T*> &keepers, std::vector<S*> &leavers)
{
   for (int i = 0; i < (int)keepers.size(); ++i)
   {
     for (int j = 0; j < (int)leavers.size(); ++j)
     {
       if (keepers[i]->DeltaR(*leavers[j]) < cut)
       {
 	leavers.erase(leavers.begin() + j);
 	--j;
       }
     }
   }
}

template<class T>
void HWWAnalysis::removeOverlap(double cut, std::vector<T*> &vec)
{
  orderByPt(vec);

  for (int i = 0; i != (int)vec.size(); ++i)
  {
    for (int j = i + 1; j != (int)vec.size(); ++j)
    {
      if (vec[i]->DeltaR(*vec[j]) < cut)
      {
	if (vec[j]->Pt() < vec[i]->Pt())
	{
	  vec.erase(vec.begin() + j);
	  --j;
	}
	else
	{
	  std::cout << "Something is wrong with the orderByPt function!" << std::endl;
	  std::cout << "Overlap removal has not worked correctly!" << std::endl;
	}
      }
    }
  }
}

#endif 
