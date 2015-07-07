#ifndef __CONVERTD3PDTOMTREE_H__
#define __CONVERTD3PDTOMTREE_H__

#include "TChain.h"
#include "TBranch.h"
#include "TObjArray.h"
#include "TClonesArray.h"
#include "TLeaf.h"
#include "TLeafObject.h"
#include "TH1.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TLorentzVector.h"

#include "MTree/Electron.h"
#include "MTree/Photon.h"
#include "MTree/Muon.h"
#include "MTree/Jet.h"
#include "MTree/Track.h"
#include "MTree/PrimaryVertex.h"
#include "MTree/Trigger.h"
#include "MTree/EventInfo.h"
#include "MTree/MissingET.h"
#include "MTree/L1TriggerROI.h"
#include "MTree/HLTTriggerROI.h"
#include "MTree/TruthParticle.h"

#include "SUSYD3PDClass_MC.h"

#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

class ConvertD3PDToMTreeMC
{
  public:
    ConvertD3PDToMTreeMC(TChain *inputChain);
    ~ConvertD3PDToMTreeMC() { ; }
    bool AddD3PDToChain(const std::string& filename) { return 0; }
    bool AddD3PDsToChain(const std::vector< std::string > filenames) { return 0; }
  
    bool IsIdenticalTruthParticles(MTree::TruthParticle* particle1, 
                                   MTree::TruthParticle* particle2, 
				   bool DoStatusMatch = true);
    void TransferTruthParticles(TClonesArray* particles);
    void FlagDuplicateTruthParticles(TClonesArray* particles);
    void SetDaughterLinks(TClonesArray* particles);
    /*MTree::TruthParticle* FindParticle(TClonesArray* particles, int pdg_id, int status, 
                                       int parent_pdg_id, HepMC::FourVector& mom);*/
    MTree::TruthParticle* GetMatchingTruthParticle(MTree::TruthParticle* inputParticle);
    bool DumpD3PDLeavesToFile(const std::string& filename = "D3PDLeaves.txt");
   
    void initialize();
    void run();
    void finalize();

    ClassDef(ConvertD3PDToMTreeMC, 0);
  protected:
 
  private:
    bool mDoEMJets;
    bool mDoPrint;
    std::string mAnaType;
   
    SUSYD3PDClass_MC* mSUSYD3PD;
 
    TFile* mInputFile;
    TFile* mOutputFile;
    TTree* mInputTree;
    TTree* mOutputTree;

    std::vector< TLeaf* > mEFTriggerD3PDItems;
    //std::vector< std::string > mEFTriggerD3PDItems;
    std::vector< TLeaf* > mL1TriggerD3PDItems;
    std::vector< TLeaf* > mL2TriggerD3PDItems;

    TChain* mInputChain;
 
    TClonesArray* mElectronCollection;
    TClonesArray* mPhotonCollection;
    TClonesArray* mMuonCollection;
    TClonesArray* mCaloMuonCollection;
    TClonesArray* mStandAloneMuonCollection;
    TClonesArray* mMuTagMuonCollection;
    TClonesArray* mJetCollection;
    TClonesArray* mEMJetCollection;
    TClonesArray* mTrackCollection;
    TClonesArray* mPrimaryVertexCollection;
    TClonesArray* mTruthParticleCollection;
    TClonesArray* mL1EMTauTriggerROICollection;
    TClonesArray* mL1MuonTriggerROICollection;
    TClonesArray* mHLTTriggerROICollection;
    TClonesArray* mMissingETCollection;
    MTree::MissingET* mMissingET;
    MTree::EventInfo* mEventInfo;
    MTree::Trigger* mTrigger;
    
};

#endif //__CONVERTD3PDTOMTREE_H__
