#ifndef __HOBJECTSELECTOR_H_
#define __HOBJECTSELECTOR_H_

#include "BaseAnalysis.h"
#include "robustIsEMDefs.h"
#include "HWWConstants.h"
#include "SmearingClass.h"
#include "EnergyRescaler.h"
#include "egammaSFclass.h"
#include "StacoCBScaleEffFactors.h"
#include "egammaOQ.h"
#include "MutagMETcorrection.h"

class HObjectSelector
{
 public:
  HObjectSelector();
  ~HObjectSelector();

  ClassDef(HObjectSelector,0);

  //Function that checks triggers
  //Yea . . . this maybe doesn't belong here, but this is the class that already
  //contains that RndmFromEvtID class . . .
  //Triggers are objects, too, right . . .
  // . . . just go with it.
  bool checkTriggers(int nE, int nM, std::vector<bool>& triggers, int runNumber, int eventNumber, bool isMC, bool& numMatch);
  bool elecTrigMatch(MTree::Electron* elec, std::vector<MTree::TriggerObject*> trigObjs);
  bool muonTrigHypo(std::string trigName, double pt, double eta);
  bool muonTrigMatch(MTree::Muon* muon, std::vector<MTree::TriggerObject*> trigObjs, std::vector<MTree::L1TriggerROI*> L1TrigObjs);
  double getTrigSF(std::vector<MTree::Lepton*> leptons);

  //Function that selects electrons for analysis
  void selectElectrons(bool isMC, int runNumber, int eventNumber, std::vector<MTree::Electron*> &eobjects, double& scale, std::vector<MTree::TriggerObject*> trigObjs, std::vector<MTree::Muon*> muons);
  std::vector<MTree::Electron*> getFEBElectrons();
  bool allElecsPassFEB(int runNumber, std::vector<MTree::Electron*> elecs, bool print);

  //Function that selects muons for analysis
  void selectMuons(bool isMC, int runNumber, int eventNumber, std::vector<MTree::Muon*> &mobjects, double &scale, std::vector<MTree::TriggerObject*> trigObjs, std::vector<MTree::L1TriggerROI*> L1TrigObjs);

  //Function(s) that selects jets for analysis
  void selectJets(int runNumber, int eventNumber, bool isMC, std::vector<MTree::Jet*>& jobjects, std::vector<MTree::Muon*> muons);
  std::vector<MTree::Jet*> getFEBJets();
  bool badOrUglyJet(MTree::Jet* jet);

  //Getter functions
  bool getETrigPass(bool isMC, int runNumber, double& weight);
  bool getMTrigPass(bool isMC, int runNumber, double& weight);
  double getDelMuonPx();
  double getDelMuonPy();
  double getDelElecPx();
  double getDelElecPy();
  double getDelJetPx();
  double getDelJetPy();
  TH1D* getECutflow();
  TH1D* getMCutflow();
  TH1D* getJCutflow();
  TLorentzVector getMuTagCorMet(const TLorentzVector met);

  void initJESTool();

 private:
  //Cutflow histos
  TH1D* eCut;
  TH1D* mCut;
  TH1D* jCut;

  //Interesting distributions
 
  bool passE20trigger;
  bool passE22trigger;
  bool passM11trigger;
  bool passM18trigger;
  bool passM40trigger;
  bool passM18mtrigger;
  bool passM40mtrigger;

  double mScale;
  double dpxMuon;
  double dpyMuon;

  double eScale;
  double dpxElec;
  double dpyElec;

  double dpxJet;
  double dpyJet;

  std::vector<MTree::Electron*> febElecs;
  std::vector<MTree::Jet*> febJets;

  SmearingClass* mcp_smear;
  EnergyRescaler* eRescaler;
  egammaSFclass* eSFactor;
  StacoCBScaleEffFactors* mSFactor;
  JESUncertaintyProvider* myJES;
  egammaOQ* eOQcheck;
  MutagMETcorrection* metCorrect;

  std::string filename;
  std::ofstream outfile;

  template<class T, class S>
    bool overlapLep(double cut, std::vector<T*> &keepers, S* lepton);

  template<class T>
  void orderByPt(std::vector<T*> &vec);

  template<class T>
  void removeOverlap(double cut, std::vector<T*> &vec);

  template<class T, class S>
  void removeOverlap(double cut, std::vector<T*> &keepers, std::vector<S*> &leavers);
};

template<class T, class S>
bool HObjectSelector::overlapLep(double cut, std::vector<T*> &keepers, S* lepton)
{
  bool overlaps = false;

  for (int i = 0; i != (int)keepers.size(); ++i)
  {
    if (keepers[i]->DeltaR(*lepton) < cut)
    {
      overlaps = true;
    }
  }

  return(overlaps);
}

template<class T>
void HObjectSelector::orderByPt(std::vector<T*> &vec)
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
void HObjectSelector::removeOverlap(double cut, std::vector<T*> &keepers, std::vector<S*> &leavers)
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
void HObjectSelector::removeOverlap(double cut, std::vector<T*> &vec)
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
