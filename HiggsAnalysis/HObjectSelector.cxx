#include "HObjectSelector.h"
#include "muon_Staco_SF_EPS.h"

HObjectSelector::HObjectSelector()
{
  passE20trigger = false;
  passE22trigger = false;
  passM11trigger = false;
  passM18trigger = false;
  passM40trigger = false;
  passM18mtrigger = false;
  passM40mtrigger = false;

  eCut = new TH1D("eCutflow","eCutflow",15,-0.5,14.5);
  mCut = new TH1D("mCutflow","mCutflow",20,-0.5,19.5);
  jCut = new TH1D("jCutflow","jCutflow",15,-0.5,14.5);

  mScale = 1.0;
  dpxMuon = 0;
  dpyMuon = 0;

  eScale = 1.0;
  dpxElec = 0;
  dpyElec = 0;
 
  febElecs.clear();
  febJets.clear();

  mcp_smear = new SmearingClass("muid","/home/swalch/macro-cosmos/MuonMomentumCorrections/root/data/muid");
  mcp_smear->UseScale(1);
  mcp_smear->UseGeV();

  eRescaler = new EnergyRescaler();
  eRescaler->useDefaultCalibConstants("2011");

  eSFactor = new egammaSFclass();
  mSFactor = new StacoCBScaleEffFactors();
  eOQcheck = new egammaOQ();

  //pass c-tor name of jet container that we're actually using . . .
  myJES = new JESUncertaintyProvider("AntiKt4TopoJetsEM");
//   myJES->init();

  metCorrect = new MutagMETcorrection(true);

  filename = "event-summaries/testhww160_trigMatch";
  outfile.open(filename.c_str());

}

//####################################################################################

HObjectSelector::~HObjectSelector()
{
  outfile.close();
}

//####################################################################################

void HObjectSelector::initJESTool()
{
  myJES->init();
}

//####################################################################################

void HObjectSelector::selectElectrons(bool isMC, int runNumber, int eventNumber, std::vector<MTree::Electron*> &electrons, double &scale, std::vector<MTree::TriggerObject*> trigObjs, std::vector<MTree::Muon*> muons)
{
  //  std::string part_type = "ELECTRON";

  eScale = 1.0;
  dpxElec = 0;
  dpyElec = 0;
  passE20trigger = false;
  passE22trigger = false;

  febElecs.clear();

  int nElec = 0;
  eRescaler->SetRandomSeed(eventNumber);

  bool print = false;
//   if (eventNumber == 1720247) print = true;

  for (int nE = 0; nE != (int)electrons.size(); ++nE)
  {
    ++nElec;
    if (print) std::cout << "found electron" << std::endl;
    int cutN = 0;
    MTree::Electron* elec = electrons[nE];
    eCut->Fill(cutN,1.0);
    ++cutN;

    MTree::Track* track = elec->GetTrack();

    double clusE = elec->ClusterE();
    double corE = 0;
    double eta = 0.0;
    double phi = 0.0;
    if (track != 0 && (track->NrOfPixelHits() + track->NrOfSCTHits() >= 4))
    {
      eta = track->Eta();
      phi = track->Phi();
    }
    else
    {
      eta = elec->ClusterEta();
      phi = elec->ClusterPhi();
    }

    double oldPx = elec->ClusterE()*cos(phi)/TMath::CosH(eta);
    double oldPy = elec->ClusterE()*sin(phi)/TMath::CosH(eta);

    if (isMC)
    {
      corE = eRescaler->getSmearingCorrectionGeV(elec->ClusterEta(),elec->ClusterE(),0,true,"2011");
      clusE *= corE;
    }
    else
    {
      clusE = eRescaler->applyEnergyCorrectionGeV(elec->ClusterEta(),elec->ClusterPhi(),clusE,clusE/TMath::CosH(eta),0,"ELECTRON");
    }
    double et = clusE/TMath::CosH(eta);
    elec->SetPtEtaPhiM(et,eta,phi,elec->M());
    if (print) std::cout << "(" << elec->Pt()<<", "<<elec->Eta()<<", "<<elec->Phi()<<", "<< elec->M()<<")"<<std::endl;
    if (et > 15)
    {
      febElecs.push_back(elec);
    }

    if (elec->Author() != 1 && elec->Author() != 3) 
    {
      electrons.erase(electrons.begin() + nE);
      --nE;
      continue;
    }
    eCut->Fill(cutN,1.0);
    ++cutN;
    if (print) std::cout << "passed author" << std::endl;
    //used to use elec->ClusterPt() < 15
//     if (et < 15)
    if (et < 20)
    {
      electrons.erase(electrons.begin() + nE);
      --nE;
      continue;
    }
    eCut->Fill(cutN,1.0);
    ++cutN;
    if (print) std::cout << "passed pt" << std::endl;
    if (TMath::Abs(elec->ClusterEta()) > 2.47)
    {
      electrons.erase(electrons.begin() + nE);
      --nE;
      continue;
    }
    if ((TMath::Abs(elec->ClusterEta()) > 1.37) && (TMath::Abs(elec->ClusterEta()) < 1.52)) 
    {
      electrons.erase(electrons.begin() + nE);
      --nE;
      continue;
    }
    eCut->Fill(cutN,1.0);
    ++cutN;
    if (print) std::cout << "passed eta" << std::endl;
    if (!(elec->PassedOQ()))
    {
      electrons.erase(electrons.begin() + nE);
      --nE;
      continue;
    }
    eCut->Fill(cutN,1.0);
    ++cutN;
    if (print) std::cout << "passed OQ" << std::endl;
    if (!(elec->IsTight()))
    {
      electrons.erase(electrons.begin() + nE);
      --nE;
      continue;
    }
    eCut->Fill(cutN,1.0);
    ++cutN;
    if (print) std::cout << "passed tight" << std::endl;
    if (fabs(track->Prim_Vtx_Z0()) > 10)
    {
      electrons.erase(electrons.begin() + nE);
      --nE;
      continue;
    }
    eCut->Fill(cutN,1.0);
    ++cutN;
    if (print) std::cout << "passed Z0" << std::endl;
    if (TMath::Abs(track->Prim_Vtx_D0()/track->Prim_Vtx_D0_Err()) > 10)
    {
      electrons.erase(electrons.begin() + nE);
      --nE;
      continue;
    }
    eCut->Fill(cutN,1.0);
    ++cutN;
    if (print) std::cout << "passed D0_sig" << std::endl;
    if (elec->ETCone20()/et >= 0.15)
    {
      electrons.erase(electrons.begin() + nE);
      --nE;
      continue;
    }
    eCut->Fill(cutN,1.0);
    ++cutN;
    if (print) std::cout << "passed ETCone20" << std::endl;
    if (elec->PTCone20()/et >= 0.1)
    {
      electrons.erase(electrons.begin() + nE);
      --nE;
      continue;
    }
    eCut->Fill(cutN,1.0);
    ++cutN;
    if (print) std::cout << "passed PTCone20" << std::endl;
    elec->SetPtEtaPhiM(et,eta,phi,elec->M());

    dpxElec += (elec->Px() - oldPx);
    dpyElec += (elec->Py() - oldPy);

    if (overlapLep(0.1,muons,elec))
    {
      electrons.erase(electrons.begin() + nE);
      --nE;
      continue;
    }
    eCut->Fill(cutN,1.0);
    ++cutN;
  }
//   scale = eScale;

  removeOverlap(0.1,electrons);
  eCut->Fill(11,(int)electrons.size());

  for (int i = 0; i != (int)electrons.size(); ++i)
  {
    MTree::Electron* elec = electrons[i];

    if (isMC)
    {
      std::pair<float,float> factors = eSFactor->scaleFactorTightETcorrected(elec->ClusterEta(),1000*(elec->Pt()),4);
      std::pair<float,float> trigFacts = eSFactor->scaleFactorRecoTrkQual(elec->ClusterEta());
      eScale *= factors.first*trigFacts.first;
    }

    if (elecTrigMatch(elec,trigObjs))
    {
//       passEtrigger = true;
      eCut->Fill(12,1.0);
    }
  }
  scale = eScale;
}

//#####################################################################################

void HObjectSelector::selectMuons(bool isMC, int runNumber, int eventNumber, std::vector<MTree::Muon*> &muons, double& scale, std::vector<MTree::TriggerObject*> trigObjs, std::vector<MTree::L1TriggerROI*> L1TrigObjs)
{
  mScale = 1.0;
  dpxMuon = 0;
  dpyMuon = 0;
  passM11trigger = false;
  passM18trigger = false;
  passM40trigger = false;
  passM18mtrigger = false;
  passM40mtrigger = false;
  metCorrect->reset();

  int nSmear = 0;

  bool print = false;
//   if (eventNumber == 197666)
//   {
//     std::cout << "Found event!" << std::endl;
//     print = true;
//   }

  for (int nM = 0; nM != (int)muons.size(); ++nM)
  {
    MTree::Muon* muon = muons[nM];
    int cutN = 0;
    mCut->Fill(cutN,1.0);
    ++cutN;

    if ((muon->Author() != 6) && (muon->Author() != 5) && (muon->Author() != 7))
    {
      muons.erase(muons.begin() + nM);
      --nM;
      continue;
    }

    MTree::Track* track = muon->GetTrack();
    MTree::Track* mstrack = muon->GetStandAloneTrack();

    double ptMS = 0;
    double ptID = 0;
    double thetaID = 0;
    if (mstrack != 0) ptMS = mstrack->Pt();
    if (track != 0) ptID = track->Pt();
    if (track != 0) thetaID = track->Theta();
    double ptCB = muon->Pt();
    double etaCB = muon->Eta();

    double oldPx = muon->Px();
    double oldPy = muon->Py();

    double newptMS;
    double newptID;
    double newptCB;

    //Add muon to MutagMETcorrection object
    int isLowPtRecMuon;
    if (muon->IsLowPtReconstructedMuon())
    {
      isLowPtRecMuon = 1;
    }
    else
    {
      isLowPtRecMuon = 0;
    }
    metCorrect->addMuon(isLowPtRecMuon,muon->Pt(),muon->Eta(),muon->Phi(),muon->EtCore(),muon->EnergyLoss(),thetaID,muon->PTCone20());

    if (isMC)
    {
      mcp_smear->SetSeed(eventNumber,nSmear);
      mcp_smear->Event(ptMS,ptID,ptCB,etaCB);
      ++nSmear;

      newptMS = mcp_smear->pTMS();
      newptID = mcp_smear->pTID();
      newptCB = mcp_smear->pTCB();
      muon->SetPtEtaPhiM(newptCB,muon->Eta(),muon->Phi(),muon->M());
    }
    else
    {
      newptMS = ptMS;
      newptID = ptID;
      newptCB = ptCB;
    }

//     if (print)
//     {
//       std::cout << muon->Pt() << " " << muon->Eta() << " " << muon->Phi() << std::endl;
//     }
    if (!(muon->IsCombinedMuon()) || (muon->Author() != 6))
    {
      muons.erase(muons.begin() + nM);
      --nM;
      continue;
    }
    mCut->Fill(cutN,1.0);
    ++cutN;

    if (TMath::Abs(muon->Eta()) >2.4)
    {
      muons.erase(muons.begin() + nM);
      --nM;
      continue;
    }
    mCut->Fill(cutN,1.0);
    ++cutN;

    if (newptCB < 15)
    {
      muons.erase(muons.begin() + nM);
      --nM;
      continue;
    }
    mCut->Fill(cutN,1.0);
    ++cutN;

    if (track == 0) continue;
    if (track->ExpectBLayerHit() && (track->NrOfBLayerHits() == 0))
    {
      muons.erase(muons.begin() + nM);
      --nM;
      continue;
    }
    mCut->Fill(cutN,1.0);
    ++cutN;

    if ((track->NrOfPixelHits() + track->NrOfPixelDeadSensors()) < 2)
    {
      muons.erase(muons.begin() + nM);
      --nM;
      continue;
    }
    if ((track->NrOfSCTHits() + track->NrOfSCTDeadSensors()) < 6)
    {
      muons.erase(muons.begin() + nM);
      --nM;
      continue;
    }
    mCut->Fill(cutN,1.0);
    ++cutN;

    if (track->NrOfPixelHoles() + track->NrOfSCTHoles() > 2)
    {
      muons.erase(muons.begin() + nM);
      --nM;
      continue;
    }
    mCut->Fill(cutN,1.0);
    ++cutN;

    int n = track->NrOfTRTHits() + track->NrOfTRTOutliers();
    int nOut = track->NrOfTRTOutliers();
    if (TMath::Abs(muon->Eta()) < 1.9)
    {
      if (n < 6 || nOut >= 0.9*n) 
      {
        muons.erase(muons.begin() + nM);
        --nM;
	continue;
      }
    }
    else
    {
      if (n > 5)
      {
	if (nOut >= 0.9*n)
        {
          muons.erase(muons.begin() + nM);
          --nM;
	  continue;
        }
      }
    }
    mCut->Fill(cutN,1.0);
    ++cutN;

    TLorentzVector inner;
    TLorentzVector mspec;
    if (track == 0)
    {
      inner.SetPtEtaPhiM(newptID,0,0,0);
    }
    else if (mstrack == 0)
    {
      mspec.SetPtEtaPhiM(newptMS,0,0,0);
    }
    else
    {
      inner.SetPtEtaPhiM(newptID,track->Eta(),track->Phi(),track->M());
      mspec.SetPtEtaPhiM(newptMS,mstrack->Eta(),mstrack->Phi(),mstrack->M());
    }

    if (TMath::Abs(track->Prim_Vtx_Z0()) >= 10)
    {
      muons.erase(muons.begin() + nM);
      --nM;
      continue;
    }
    mCut->Fill(cutN,1.0);
    ++cutN;

    if (TMath::Abs(track->Prim_Vtx_D0()/track->Prim_Vtx_D0_Err()) >= 10) 
    {
      muons.erase(muons.begin() + nM);
      --nM;
      continue;
    }
    mCut->Fill(cutN,1.0);
    ++cutN;

    if (muon->ETCone20()/muon->Pt() > 0.15)
    {
      muons.erase(muons.begin() + nM);
      --nM;
      continue;
    }
    mCut->Fill(cutN,1.0);
    ++cutN;

    if (muon->PTCone20()/muon->Pt() > 0.1)
    {
      muons.erase(muons.begin() + nM);
      --nM;
      continue;
    }
    mCut->Fill(cutN,1.0);
    ++cutN;

    dpxMuon += (muon->Px() - oldPx);
    dpyMuon += (muon->Py() - oldPy);

    if (isMC)
    {
      TLorentzVector temp;
      temp.SetPtEtaPhiM(1000*muon->Pt(),muon->Eta(),muon->Phi(),muon->M());
      mScale *= mSFactor->scaleFactor(temp);
    }

    if (muonTrigMatch(muon,trigObjs,L1TrigObjs))
    {
      mCut->Fill(cutN,1.0);
      ++cutN;
    }
  }
  if (print)
  {
      for (int k = 0; k != (int)trigObjs.size(); ++k)
      {
	trigObjs[k]->PrintData(std::cout);
      }
  }
  scale = mScale;
}

//#####################################################################################

TH1D* HObjectSelector::getECutflow()
{
  eCut->GetXaxis()->SetBinLabel(1,"Total Electrons");
  eCut->GetXaxis()->SetBinLabel(2,"Author 1 or 3");
  eCut->GetXaxis()->SetBinLabel(3,"pT > 20");
  eCut->GetXaxis()->SetBinLabel(4,"eta < 2.47 & !(1.37 < eta < 1.52)");
  eCut->GetXaxis()->SetBinLabel(5,"OTX cut");
  eCut->GetXaxis()->SetBinLabel(6,"Tight");
  eCut->GetXaxis()->SetBinLabel(7,"Z0 < 10");
  eCut->GetXaxis()->SetBinLabel(8,"D0_sig < 10");
  eCut->GetXaxis()->SetBinLabel(9,"ETCone20/pT < 0.15");
  eCut->GetXaxis()->SetBinLabel(10,"PTCone20/pT < 0.1");
  eCut->GetXaxis()->SetBinLabel(11,"E-M overlap");
  eCut->GetXaxis()->SetBinLabel(12,"E-E overlap");
  eCut->GetXaxis()->SetBinLabel(13,"Trigger Match");

  return(eCut);
}

//#####################################################################################

TH1D* HObjectSelector::getMCutflow()
{
  mCut->GetXaxis()->SetBinLabel(1,"Total Muons");
  mCut->GetXaxis()->SetBinLabel(2,"STACO Comb");
  mCut->GetXaxis()->SetBinLabel(3,"|eta| < 2.4");
  mCut->GetXaxis()->SetBinLabel(4,"pT > 15 GeV");
  mCut->GetXaxis()->SetBinLabel(5,"B-layer req");
  mCut->GetXaxis()->SetBinLabel(6,"Npix>0,Nsct>5");
  mCut->GetXaxis()->SetBinLabel(7,"Si Holes < 2");
  mCut->GetXaxis()->SetBinLabel(8,"Ntrt reqs");
  mCut->GetXaxis()->SetBinLabel(9,"Z0 < 10");
  mCut->GetXaxis()->SetBinLabel(10,"D0 sig < 10");
  mCut->GetXaxis()->SetBinLabel(11,"ETCone20/pT < 0.15");
  mCut->GetXaxis()->SetBinLabel(12,"PTCone20/pT < 0.1");
  mCut->GetXaxis()->SetBinLabel(13,"Trigger Match");

  return(mCut);
}

//#####################################################################################

bool HObjectSelector::checkTriggers(int nE, int nM, std::vector<bool>& triggers, int runNum, int eventNum, bool isMC, bool& numMatch)
{
  bool pass = false;

  if (isMC)
  {
    if (triggers[0] || triggers[1] || triggers[2] || triggers[3]) pass = true;

    if (nE > 0 && (triggers[0] || triggers[3])) numMatch = true;

    if (nM > 0 && (triggers[1] || triggers[2])) numMatch = true;
  }
  else
  {
    if (runNum < 186516)
    {
      if (triggers[0] || triggers[1] || triggers[2]) pass = true;

      if (nE > 0 && triggers[0]) numMatch = true;

      if (nM > 0 && (triggers[1] || triggers[2])) numMatch = true;
    }
    else
    {
      if (triggers[3] || triggers[4] || triggers[5]) pass = true;

      if (nE > 0 && triggers[3]) numMatch = true;

      if (nM > 0 && (triggers[4] || triggers[5])) numMatch = true;
    }
  }

  return(pass);
}

//#####################################################################################

void HObjectSelector::selectJets(int rNum, int eNum, bool isMC,std::vector<MTree::Jet*>& jets, std::vector<MTree::Muon*> muons)
{
  dpxJet = 0.0;
  dpyJet = 0.0;

  febJets.clear();

  for (int i = 0; i != (int)jets.size(); ++i)
  {
    MTree::Jet* Jet = jets[i];
    int cutN = 0;
    jCut->Fill(cutN,1.0);
    ++cutN;

    TLorentzVector jet;
    double jes = jets[i]->JetMoment("EMJES");
    jet.SetPxPyPzE(jes*Jet->GetRawPx(),jes*Jet->GetRawPy(),jes*Jet->GetRawPz(),jes*Jet->GetRawE());

    if (Jet->Pt() > 15)
    {
      febJets.push_back(Jet);
    }


//Standard Higgs pT threshold
    if (Jet->Pt() < 25)
//Standard Model pT threshold
//    if (jet->Pt() < 30)
    {
      jets.erase(jets.begin() + i);
      --i; 
      continue;
    }
    jCut->Fill(cutN,1.0);
    ++cutN;

    //Standard Higgs threshold is 4.5
    if (fabs(jet.Eta()) > 4.5)
    {
      jets.erase(jets.begin() + i);
      --i; 
      continue;
    }
    jCut->Fill(cutN,1.0);
    ++cutN;

//     if ((fabs(jet->Eta()) < 2.1) && (jet->JetVertexFraction() < 0.75))
//     {
//       jets.erase(jets.begin() + i);
//       --i;
//       continue;
//     }
    jCut->Fill(cutN,1.0);
    ++cutN;

//     if (isMC)
//     {
//       double absUncer = myJES->getAbsUncert(jet->Pt()*1000,jet->Eta());
//       if (USEJESUNCERT)
//       {
//         double oldPx = jet->Px();
//         double oldPy = jet->Py();
//         if (JESUNCERTUP)
//         {
//           jet->SetPtEtaPhiE(absUncer/1000 + jet->Pt(),jet->Eta(),jet->Phi(),jet->E());
//         }
//         else
//         {
// 	  jet->SetPtEtaPhiE(absUncer/1000 - jet->Pt(),jet->Eta(),jet->Phi(),jet->E());
//         }

//         dpxJet += (jet->Px() - oldPx);
//         dpyJet += (jet->Py() - oldPy);
//       }
//     }

//     Jet->SetPtEtaPhiE(jet.Pt(),jet.Eta(),jet.Phi(),jet.E());
  }

}

//#####################################################################################

TH1D* HObjectSelector::getJCutflow()
{
  jCut->GetXaxis()->SetBinLabel(1,"Total Jets");
//   jCut->GetXaxis()->SetBinLabel(2,"pT > 20");
//   jCut->GetXaxis()->SetBinLabel(3,"HECf > 1-|HECQ|");
//   jCut->GetXaxis()->SetBinLabel(4,"EMf>0.9&&|LArQ|>0.8&&|eta|< 2.8");
//   jCut->GetXaxis()->SetBinLabel(5,"Timing");
  jCut->GetXaxis()->SetBinLabel(2,"pT > 25");
  jCut->GetXaxis()->SetBinLabel(3,"|eta| < 4.5");
  jCut->GetXaxis()->SetBinLabel(4,"JVF < 0.75");

  return(jCut);
}

//######################################################################################

double HObjectSelector::getDelMuonPx()
{
  return(dpxMuon);
}

//######################################################################################

double HObjectSelector::getDelMuonPy()
{
  return(dpyMuon);
}

//######################################################################################

double HObjectSelector::getDelElecPx()
{
  return(dpxElec);
}

//######################################################################################

double HObjectSelector::getDelElecPy()
{
  return(dpyElec);
}

//######################################################################################

double HObjectSelector::getDelJetPx()
{
  return(dpxJet);
}

//######################################################################################

double HObjectSelector::getDelJetPy()
{
  return(dpyJet);
}

//######################################################################################

bool HObjectSelector::badOrUglyJet(MTree::Jet* jet)
{
  bool badJet = false;

//     //HECf > 1-|HECQ|
//     double Hecf = jet->HECFrac();
//     double HecQ = jet->HECQuality();
//     if (Hecf > (1 - HecQ))
//     {
//       badJet = true;
//     }

//     //EMf > 0.9 && |LArQ| > 0.8 && |eta| < 2.8
//     double eta = jet->Eta();
//     double Emf = jet->JetEMFraction();
//     double Q = jet->LArQuality();
//     if (Emf > 0.9 && fabs(Q) > 0.8 && fabs(eta) < 2.8)
//     {
//       badJet = true;
//     }

//     double time = jet->JetTiming();
//     double chf = jet->SumPtTracks()/jet->Pt();
//     double Fmax = jet->SamplingMaxFrac();
//     if ((fabs(time) > 10 || 
//        (Emf < 0.05 && chf < 0.1 && fabs(eta) < 2) ||
//        (Emf < 0.05 && fabs(eta) >= 2) ||
//        (Fmax > 0.95 && chf < 0.05 && fabs(eta) < 2)))
//     {
//       badJet = true;
//     }

//     //Ugly selections go here
//     if (jet->IsUgly())
//     {
//       badJet = true;
//     }

    return(badJet);
}

//######################################################################################

std::vector<MTree::Electron*> HObjectSelector::getFEBElectrons()
{
  return(febElecs);
}

//######################################################################################

std::vector<MTree::Jet*> HObjectSelector::getFEBJets()
{
  return(febJets);
}

//######################################################################################

bool HObjectSelector::allElecsPassFEB(int runNumber, std::vector<MTree::Electron*> elecs, bool print)
{
  bool allPass = true;

  for (int i = 0; i != (int)elecs.size(); ++i)
  {
    if ((eOQcheck->checkOQClusterElectron(runNumber,elecs[i]->ClusterEta(),elecs[i]->ClusterPhi())) == 3)
    {
      allPass = false;
      if (print)
      {
	std::cout << "Bad Electron (" << elecs[i]->Pt() << ", " << elecs[i]->ClusterEta() << ", " << elecs[i]->ClusterPhi() << ")" << std::endl;
      }
    }
  }

  return(allPass);
}

//######################################################################################

bool HObjectSelector::elecTrigMatch(MTree::Electron* elec, std::vector<MTree::TriggerObject*> trigObjs)
{
  bool matchE20ROI = false;
  bool matchE22ROI = false;
  int nTrigObjs = (int)trigObjs.size();
  for (int i = 0; i != nTrigObjs; ++i)
  {
    MTree::TriggerObject* obj = trigObjs[i];

    if (obj->PassedTrigger("EF_e20_medium"))
    {
      TLorentzVector temp;
      temp.SetPtEtaPhiM(obj->Pt(),obj->Eta(),obj->Phi(),0.0);

      if (fabs(elec->DeltaR(temp)) < 0.15)
      {
        matchE20ROI = true;
      }
    }

    if (obj->PassedTrigger("EF_e22_medium"))
    {
      TLorentzVector temp;
      temp.SetPtEtaPhiM(obj->Pt(),obj->Eta(),obj->Phi(),0.0);

      if (fabs(elec->DeltaR(temp)) < 0.15)
      {
        matchE22ROI = true;
      }
    }
  }

  if (elec->Pt() > 25 && matchE20ROI)
  {
    passE20trigger = true;
  }
  if (elec->Pt() > 25 && matchE22ROI)
  {
    passE22trigger = true;
  }

  if (elec->Pt() > 25 && (matchE20ROI || matchE22ROI))
  {
    return(true);
  }
  else
  {
    return(false);
  }

}

//######################################################################################

bool HObjectSelector::getETrigPass(bool isMC, int runNumber, double& weight)
{
  bool pass = false;
  weight = 0;

  if (isMC)
  {
    if (passE20trigger)
    {
      pass = true;
      weight += LUMIBI4/LUMINOSITY;
    }
    if (passE22trigger)
    {
      pass = true;
      weight += (LUMINOSITY - LUMIBI4)/LUMINOSITY;
    }
  }
  else
  {
    if (runNumber < 186516 && passE20trigger)
    {
      pass = true;
      weight = 1;
    }
    else if (runNumber >= 186516 && passE22trigger)
    {
      pass = true;
      weight = 1;
    }
  }

  return(pass);
}

//######################################################################################

bool HObjectSelector::getMTrigPass(bool isMC, int runNumber, double& weight)
{
  bool pass = false;
  weight = 0;

  if (isMC)
  {
    if (passM18trigger || passM40trigger)
    {
      pass = true;
      weight += LUMIBI4/LUMINOSITY;
    }
    if (passM11trigger && (passM18trigger || passM40trigger))
    {
      pass = true;
      weight += (LUMINOSITY - LUMIBI4)/LUMINOSITY;
    }
  }
  else
  {
    if (runNumber < 186516 && (passM18trigger || passM40trigger))
    {
      pass = true;
      weight = 1;
    }
    else if (runNumber >= 186516 && (passM18mtrigger || passM40mtrigger))
    {
      pass = true;
      weight = 1;
    }
  }


  return(pass);
}

//######################################################################################

bool HObjectSelector::muonTrigHypo(std::string trigName, double pt, double eta)
{
  double feta = fabs(eta);

  if (trigName.find("EF_mu18") != std::string::npos)
  {
    if (feta < 1.05)
    {
      if (pt > 17.53) return(true);
    }
    else if (feta < 1.5)
    {
      if (pt > 17.39) return(true);
    }
    else if (feta < 2.0)
    {
      if (pt > 17.34) return(true);
    }
    else
    {
      if (pt > 17.28) return(true);
    }
  }

  if (trigName.find("EF_mu20") != std::string::npos)
  {
    if (feta < 1.05)
    {
      if (pt > 19.47) return(true);
    }
    else if (feta < 1.5)
    {
      if (pt > 19.33) return(true);
    }
    else if (feta < 2.0)
    {
      if (pt > 19.30) return(true);
    }
    else
    {
      if (pt > 19.22) return(true);
    }
  }

  if (trigName.find("EF_mu40_MSonly_barrel") != std::string::npos)
  {
    if (feta < 1.05)
    {
      if (pt > 31.5) return(true);
    }
    else if (pt > 1000) return(true);
  }

  return(false);
}

//######################################################################################

bool HObjectSelector::muonTrigMatch(MTree::Muon* muon, std::vector<MTree::TriggerObject*> trigObjs, std::vector<MTree::L1TriggerROI*> L1TrigObjs)
{
  bool pass_pt_MG = false;
  bool pass_pt_MS = false;
  bool pass_L2_MG = false;
  bool pass_L2_MS = false;
  bool pass_EF_MG = false;
  bool pass_EF_MS = false;
  bool pass_L1_M11 = false;
  bool pass_L2_MGm = false;
  bool pass_L2_MSm = false;
  bool pass_EF_MGm = false;
  bool pass_EF_MSm = false;

  if (muon->Pt() > 20) pass_pt_MG = true;
  if (muon->Pt() > 42) pass_pt_MS = true;

  for (int i = 0; i != (int)L1TrigObjs.size(); ++i)
  {
    MTree::L1TriggerROI* l1roi = L1TrigObjs[i];

    TLorentzVector temp;
    temp.SetPtEtaPhiM(1.0,l1roi->Eta(),l1roi->Phi(),1.0);
    if (l1roi->Pt() > 10 && temp.DeltaR(*muon) < 0.4) pass_L1_M11 = true;
  }

  for (int i = 0; i != (int)trigObjs.size(); ++i)
  {
    MTree::TriggerObject* obj = trigObjs[i];
    std::set<int> authors = obj->Authors();

//     if (obj->PassedTrigger("L1_MU11"))
//     {
//       std::cout << "found L1_MU11 trigger!" << std::endl;
//       TLorentzVector temp;
//       temp.SetPtEtaPhiM(1.0,obj->Eta(),obj->Phi(),1.0);
//       if (temp.DeltaR(*muon) < 0.4 && obj->Pt() > 10)
//       {
//         pass_L1_M11 = true;
//       }
//     }
    if (obj->PassedTrigger("L2_mu18_MG") && (authors.find(2) != authors.end()))
    {
      TLorentzVector temp;
      temp.SetPtEtaPhiM(1.0,obj->Eta(),obj->Phi(),1.0);
      if (temp.DeltaR(*muon) < 0.1)
      {
        pass_L2_MG = true;
      }
    }
    if (obj->PassedTrigger("L2_mu18_MG_medium") && (authors.find(2) != authors.end()))
    {
      TLorentzVector temp;
      temp.SetPtEtaPhiM(1.0,obj->Eta(),obj->Phi(),1.0);
      if (temp.DeltaR(*muon) < 0.1)
      {
        pass_L2_MGm = true;
      }
    }
    if (obj->PassedTrigger("L2_mu40_MSonly_barrel") && (authors.find(4) != authors.end()))
    {
      TLorentzVector temp;
      temp.SetPtEtaPhiM(1.0,obj->Eta(),obj->Phi(),1.0);
      if (temp.DeltaR(*muon) < 0.1)
      {
        pass_L2_MS = true;
      }
    }
    if (obj->PassedTrigger("L2_mu40_MSonly_barrel_medium") && (authors.find(4) != authors.end()))
    {
      TLorentzVector temp;
      temp.SetPtEtaPhiM(1.0,obj->Eta(),obj->Phi(),1.0);
      if (temp.DeltaR(*muon) < 0.1)
      {
        pass_L2_MSm = true;
      }
    }
    if (obj->PassedTrigger("EF_mu18_MG") && (authors.find(3) != authors.end()))
    {
       TLorentzVector temp;
      temp.SetPtEtaPhiM(1.0,obj->Eta(),obj->Phi(),1.0);
      if ((temp.DeltaR(*muon) < 0.1) && (muonTrigHypo("EF_mu18_MG",obj->Pt(),obj->Eta())))
      {
        pass_EF_MG = true;
      }
    }
    if (obj->PassedTrigger("EF_mu18_MG_medium") && (authors.find(3) != authors.end()))
    {
       TLorentzVector temp;
      temp.SetPtEtaPhiM(1.0,obj->Eta(),obj->Phi(),1.0);
      if ((temp.DeltaR(*muon) < 0.1) && (muonTrigHypo("EF_mu18_MG_medium",obj->Pt(),obj->Eta())))
      {
        pass_EF_MGm = true;
      }
    }
    if (obj->PassedTrigger("EF_mu40_MSonly_barrel") && ((authors.find(5) != authors.end()) || (authors.find(6) != authors.end())))
    {
      TLorentzVector temp;
      temp.SetPtEtaPhiM(1.0,obj->Eta(),obj->Phi(),1.0);
      if ((temp.DeltaR(*muon) < 0.1) && (muonTrigHypo("EF_mu40_MSonly_barrel",obj->Pt(),obj->Eta())))
      {
        pass_EF_MS = true;
      }
    }
    if (obj->PassedTrigger("EF_mu40_MSonly_barrel_medium") && ((authors.find(5) != authors.end()) || (authors.find(6) != authors.end())))
    {
      TLorentzVector temp;
      temp.SetPtEtaPhiM(1.0,obj->Eta(),obj->Phi(),1.0);
      if ((temp.DeltaR(*muon) < 0.1) && (muonTrigHypo("EF_mu40_MSonly_barrel_medium",obj->Pt(),obj->Eta())))
      {
        pass_EF_MSm = true;
      }
    }
  }

  if (pass_L1_M11) passM11trigger = true;
  if (pass_pt_MG && pass_L2_MG && pass_EF_MG) passM18trigger = true;
  if (pass_pt_MS && pass_L2_MS && pass_EF_MS) passM40trigger = true;
  if (pass_pt_MG && pass_L2_MGm && pass_EF_MGm) passM18mtrigger = true;
  if (pass_pt_MS && pass_L2_MSm && pass_EF_MSm) passM40mtrigger = true;

  if ((pass_pt_MG && pass_L2_MG && pass_EF_MG) || (pass_pt_MS && pass_L2_MS && pass_EF_MS) || (pass_pt_MG && pass_L2_MGm && pass_EF_MGm) || (pass_pt_MS && pass_L2_MSm && pass_EF_MSm))
  {
    return(true);
  }
  else
  {
    return(false);
  }
}

//######################################################################################

double HObjectSelector::getTrigSF(std::vector<MTree::Lepton*> leptons)
{
  double factor = 1;

  std::vector<double> sfacts;
  std::vector<double> effs;

  for (int i = 0; i != (int)leptons.size(); ++i)
  {
    if (leptons[i]->LeptonPdgID() == 11)
    {
      MTree::Electron* elec = (MTree::Electron*)leptons[i];
      if (elec->Pt() > 25)
      {
	std::pair<float,float> result = eSFactor->scaleFactorTrigger(elec->ClusterEta());
	sfacts.push_back(result.first);
	effs.push_back(0.9894);
      }
    }
    if (leptons[i]->LeptonPdgID() == 13)
    {
      MTree::Muon* muon = (MTree::Muon*)leptons[i];

      if (muon->Pt() > 20)
      {
	sfacts.push_back(mu_CB_staco_trigger_SF(muon->Eta(),muon->Phi(),muon->Pt()));
	effs.push_back(mu_CB_staco_trigger_Eff_MC(muon->Eta(),muon->Phi(),muon->Pt()));
      }
    }
  }

  if ((int)sfacts.size() == 1)
  {
    factor *= sfacts[0];
  }
  else if ((int)sfacts.size() == 2)
  {
    double eff = effs[0] + effs[1] - effs[0]*effs[1];
    double effSF = effs[0]*sfacts[0] + effs[1]*sfacts[1] - effs[0]*sfacts[0]*effs[1]*sfacts[1];
    double sf = effSF/eff;
    factor *= sf;
  }

  return(factor);
}

//######################################################################################

TLorentzVector HObjectSelector::getMuTagCorMet(const TLorentzVector met)
{
  return(metCorrect->getEtMissCorrected(met));
}

//######################################################################################
