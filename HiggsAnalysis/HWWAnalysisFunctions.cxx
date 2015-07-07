#include "HWWAnalysis.h"

std::vector<std::string> HWWAnalysis::findChannels(std::vector<MTree::Lepton*> &leptons)
{
  int nE = 0;
  int nM = 0;

  for (int i = 0; i != (int)leptons.size(); ++i)
  {
    if (leptons[i]->LeptonPdgID() == 11)
    {
      ++nE;
    }
    else
    {
      ++nM;
    }
  }

  std::string label;
  if (nM == 0 && nE > 0)
  {
    label = "ee";
  }
  else if (nE == 0 && nM > 0)
  {
    label = "mm";
  }
  else
  {
    label = "em";
  }

  std::vector<std::string> results;
  results.push_back(label + "_zeroj");
  results.push_back(label + "_onej");
  results.push_back(label + "_twoj");

  return(results);
}

//######################################################################################


std::vector<std::string> HWWAnalysis::findChannels(std::vector<MTree::Lepton*> &leptons, int nJets)
{
  int nE = 0;
  int nM = 0;

  for (int i = 0; i != (int)leptons.size(); ++i)
  {
    if (leptons[i]->LeptonPdgID() == 11)
    {
      ++nE;
    }
    else
    {
      ++nM;
    }
  }

  std::string label;
  if (nM == 0 && nE > 0)
  {
    label = "ee";
  }
  else if (nE == 0 && nM > 0)
  {
    label = "mm";
  }
  else
  {
    label = "em";
  }

  std::string tag;
  if (nJets == 0)
  {
    tag = "_zeroj";
  }
  else if (nJets == 1)
  {
    tag = "_onej";
  }
  else
  {
    tag = "_twoj";
  }

  std::vector<std::string> results;
  results.push_back(label + tag);

  return(results);
}

//######################################################################################

std::vector<std::string> HWWAnalysis::findChannels(std::vector<bool> triggers)
{
  std::vector<std::string> channels;

  if (triggers[0])
  {
    channels.push_back("ee_zeroj");
    channels.push_back("ee_onej");
    channels.push_back("ee_twoj");
  }
  if (triggers[1] || triggers[2])
  {
    channels.push_back("mm_zeroj");
    channels.push_back("mm_onej");
    channels.push_back("mm_twoj");
  }
  if (triggers[0] || triggers[1] || triggers[2])
  {
    channels.push_back("em_zeroj");
    channels.push_back("em_onej");
    channels.push_back("em_twoj");
  }
  return(channels);
}

//######################################################################################

void HWWAnalysis::selectGoodPVs(bool &goodPV, int &nGoodPVs, int eventNumber)
{
  goodPV = false;
  nGoodPVs = 0;
  int nPVs = _primaryVertices->GetEntriesFast();
  for (int i = 0; i != nPVs; ++i)
  {
    MTree::PrimaryVertex* vertex = (MTree::PrimaryVertex*)_primaryVertices->At(i);
    if (vertex->NTracks() < 3) continue;
    if (i == 0)
    {
      goodPV = true;
    }
    ++nGoodPVs;
  }
}

//######################################################################################

double HWWAnalysis::primVtxWeight(int nGoodPVs)
{
  const int nrWeight = 6;
//   double weight[nrWeight] = {1.66781,1.11649,0.846559,0.695447,0.678215,0.672109};
  double weight[nrWeight] = {0.270335,0.444096,0.711927,1.13953,1.99392,4.85826};

  if (nGoodPVs == 0)
  {
    return 1.;
  }
  else if (nGoodPVs <= nrWeight)
  {
    return weight[nGoodPVs-1];
  }
  else
  {
    return weight[nrWeight-1];
  }
}

//######################################################################################

bool HWWAnalysis::usePVWeight(std::string sample)
{
  bool useIt = false;

  if ((sample.find("data") == string::npos))
  {
    useIt = true;
  }

  return(useIt);
}

//######################################################################################

bool HWWAnalysis::badJetsPresent(std::vector<MTree::Lepton*> leptons, std::string& result)
{
// //   bool failCleaning = false;
  int nJets = _jets->GetEntries();
  for (int i = 0; i != nJets; ++i)
  {
    MTree::Jet* jet = (MTree::Jet*)_jets->At(i);

//     double jes = jet->GetEmJESFactor();
//     double jes = jet->JetMoment("EMJES");
    TLorentzVector jet4v;
    jet4v.SetPxPyPzE(jet->GetRawPx(), jet->GetRawPy(), jet->GetRawPz(), jet->GetRawE());
//     double pt = jet4v.Pt();
    double pt = jet->Pt();
    if (pt < 20) continue;

    bool overlaps = false;
    for (int k = 0; k != (int)leptons.size(); ++k)
    {
      if (jet->DeltaR(*leptons[k]) < 0.3)
      {
	overlaps = true;
      }
    }
    if (overlaps) continue;

    double Hecf = jet->HECFrac();
    double HecQ = jet->JetMoment("HECQuality");
    double negE = fabs(jet->JetMoment("NegativeE"))/1000;
    double eta = jet4v.Eta();
    double Emf = jet->JetEMFraction();
    double Q = jet->JetMoment("LArQuality");
    double time = jet->JetMoment("Timing");
    double chf = jet->JetMoment("sumPtTrk")/(1000*pt);
    double Fmax = jet->SamplingMaxFrac();

    //HEC spikes
    if ((Hecf > 0.5 && fabs(HecQ) > .5) ||  negE > 60)
    {
      std::stringstream sstr;
      sstr << "Failed HEC spikes: hecf = " << Hecf << " HecQ = " << HecQ << " negE = " << negE << std::endl;
      result = sstr.str(); 
      return true;
    }

    //EM coherant noise
    if (Emf > 0.95 && fabs(Q) > 0.8 && fabs(eta) < 2.8)
    {
      std::stringstream sstr;
      sstr << "Failed EM noise: emf = " << Emf << " Q = " << Q << " eta = " << eta << std::endl;
      result = sstr.str(); 
      return true;
    }

    //Non-collision background and cosmics
    if ((fabs(time) > 25 || 
       (Emf < 0.05 && chf < 0.05 && fabs(eta) < 2) ||
       (Emf < 0.05 && fabs(eta) >= 2) ||
       (Fmax > 0.99 && fabs(eta) < 2)))
    {
      std::stringstream sstr;
      sstr << "Failed non-collision: time = " << time << " Emf = " << Emf << " chf = " << chf << "  eta = " << eta << " Fmax = " << Fmax << std::endl;
      result = sstr.str(); 
      return true;
    }

  }

  return(false);
}

//######################################################################################

void HWWAnalysis::correctMET(std::vector<MTree::Lepton*> leptons, std::vector<MTree::Jet*> jets)
{

  //set met term
  MTree::MissingET* met_topo = NULL;
  MTree::MissingET* met_muon = NULL;
  MTree::MissingET* met_track = NULL;
  MTree::MissingET* met_ref = NULL;
  std::string topoTerm = "MET_LocHadTopo";
  std::string muonTerm = "MET_MuonBoy";
  std::string trackTerm = "MET_RefMuon_Track";
  std::string refTerm = "MET_RefFinal_em";

  int nrMETTerms = _missingETTerms->GetEntriesFast();
  for (int i=0; i<nrMETTerms; i++)
  {
    MTree::MissingET* term = (MTree::MissingET*)_missingETTerms->At(i);
    if (term->GetTerm() == topoTerm)
    {
      met_topo = term;
    }
    if (term->GetTerm() == muonTerm)
    {
      met_muon = term;
    }
    if (term->GetTerm() == trackTerm)
    {
      met_track = term;
    }
    if (term->GetTerm() == refTerm)
    {
      met_ref = term;
    }
  }

  if (!met_topo)
  {
    std::cout << "Couldn't find missingET term: " << topoTerm << endl;
    std::cout << "Skipping event..." << endl;
    return;
  }
  if (!met_muon)
  {
    std::cout << "Couldn't find missingET term: " << muonTerm << endl;
    std::cout << "Skipping event..." << endl;
    return;
  }
  if (!met_track)
  {
    std::cout << "Couldn't find missingET term: " << trackTerm << endl;
    std::cout << "Skipping event..." << endl;
    return;
  }
  if (!met_ref)
  {
    std::cout << "Couldn't find missingET term: " << trackTerm << endl;
    std::cout << "Skipping event..." << endl;
    return;
  }

  double topo_ex = (met_topo->GetExCentralRegion()+
		      met_topo->GetExEndcapRegion()+
		      met_topo->GetExForwardRegion())/1000.;
  double topo_ey = (met_topo->GetEyCentralRegion()+
		      met_topo->GetEyEndcapRegion()+
		      met_topo->GetEyForwardRegion())/1000.;
  double topo_sumET = (met_topo->GetSumETCentralRegion()+
			 met_topo->GetSumETEndcapRegion()+
			 met_topo->GetSumETForwardRegion())/1000.;
  met_topo->Initialize(sqrt(topo_ex*topo_ex+topo_ey*topo_ey), topo_ex, topo_ey, topo_sumET, "TopoEta4.5");

  double muon_px = met_muon->Px() - selector->getDelElecPx() - selector->getDelMuonPx() - selector->getDelJetPx();
  double muon_py = met_muon->Py() - selector->getDelElecPy() - selector->getDelMuonPy() - selector->getDelJetPy();
  double muon_sumET = met_muon->SumET(); // need to fix this
  met_muon->Initialize(sqrt(muon_px*muon_px+muon_py*muon_py), muon_px, muon_py, muon_sumET, "MET_MuonBoy_smear");

  double px = met_topo->Px()+met_muon->Px()-met_track->Px();
  double py = met_topo->Py()+met_muon->Py()-met_track->Py();

  double E = sqrt(px*px+py*py);
  double sumET = met_topo->SumET() + met_muon->SumET() - met_track->SumET();

//   double newPx = met_ref->Px() - selector->getDelMuonPx() - selector->getDelElecPx();
//   double newPy = met_ref->Py() - selector->getDelMuonPy() - selector->getDelElecPy();
//   met_ref->Initialize(sqrt(newPx*newPx+newPy*newPy),newPx,newPy,met_ref->SumET(),"MET_RefFinal_em_smear");

  TLorentzVector temp;
  temp.SetPxPyPzE(px,py,0.0,E);
  temp = selector->getMuTagCorMet(temp);
  metForWW->Initialize(temp.E(),temp.Px(),temp.Py(),sumET,"ForWW");
  metRel->Initialize(temp.E(),temp.Px(),temp.Py(),sumET,"metRel");

  double minDPhi = TMath::Pi();

  for (int i = 0; i != (int)leptons.size(); ++i)
  {
    double temp = fabs(metForWW->DeltaPhi(*leptons[i]));
    if (temp < minDPhi)
    {
      minDPhi = temp;
    }
  }
  for (int i = 0; i != (int)jets.size(); ++i)
  {
    double temp = fabs(metForWW->DeltaPhi(*jets[i]));
    if (temp < minDPhi)
    {
      minDPhi = temp;
    }
  }

  if (minDPhi < TMath::Pi()/2)
  {
    double px = metForWW->Px();
    double py = metForWW->Py();
    double pz = metForWW->Pz();
    double E = metForWW->E();
    metRel->SetPxPyPzE(px*TMath::Sin(minDPhi),py*TMath::Sin(minDPhi),pz*TMath::Sin(minDPhi),E*TMath::Sin(minDPhi));
  }

}

//######################################################################################

void HWWAnalysis::muonjetOverlap(double cut, std::vector<MTree::Muon*> &muons, std::vector<MTree::Jet*> &jets)
{
  //So, if a jet does not overlap with a muon and fails the medium loose/ugly requirements
  //boot it out.

  for (int nJet = 0; nJet != (int)jets.size(); ++nJet)
  {
    bool overlaps = false;
    for (int nM = 0; nM != (int)muons.size(); ++nM)
    {
      if (fabs(jets[nJet]->DeltaR(*muons[nM])) < cut)
      {
	overlaps = true;
      }
    }

    if (!overlaps && selector->badOrUglyJet(jets[nJet]))
    {
      jets.erase(jets.begin() + nJet);
      --nJet;
    }
  }
}

//######################################################################################

void HWWAnalysis::fillPVrefs(std::string sample)
{
  if (_debug)
  {
    std::cout << "callng fillPVrefs" << std::endl;
  }

  std::vector<std::string> samples = GetAllSamples(sample);
  std::vector<std::string>::iterator iter;
  std::vector<std::string>::iterator end = samples.end();

  for (iter = samples.begin(); iter != end; ++iter)
  {
    std::string sampleName = *iter;
    TChain* chain = GetChain(sampleName);
    chain->SetBranchStatus("*",1);

    MTree::EventInfo* eventInfo = new MTree::EventInfo();
    chain->SetBranchAddress("EventInfo",&eventInfo);

    std::string fileName = "root-files/hww/pv_maps/" + sample + "_ref.root";
    TFile* file = new TFile(TString(fileName.c_str()),"RECREATE");

    std::string plotname = "mu_" + sample;
    TH1D* muplot = new TH1D(plotname.c_str(),plotname.c_str(),20,-0.5,19.5);

    int eventNr = 0;
    while (chain->GetEntry(eventNr++))
    {
      if (eventNr % 10000 == 0)
      {
	std::cout << "fillPVrefs has analyzed " << eventNr << " events." << std::endl;
      }
      double weight = eventInfo->GeneratorEventWeight()/fabs(eventInfo->GeneratorEventWeight());
      muplot->Fill(eventInfo->LumiBlock(),weight);
    }//closes loop over events

    file->Write();
    file->Close();
  }//Closes loop over samples vector
}

//######################################################################################

bool HWWAnalysis::passDeadFEB(bool isMC, double& evWeight, int runNumber, int eventNumber)
{
  bool print = false;
//   if (eventNumber == 82399153) print = true;

  std::vector<MTree::Electron*> electrons = selector->getFEBElectrons();
  if (print)
  {
    std::cout << "Checking " << (int)electrons.size() << " FEB electrons" << std::endl;
    for (int i = 0; i != (int)electrons.size(); ++i)
    {
      std::cout << "Electron: " << electrons[i]->Pt() << " " << electrons[i]->ClusterEta() << " " << electrons[i]->ClusterPhi() << std::endl;
    }
  }
  febEs += (int)electrons.size();

  bool inDeadFEB = false;
  if (isMC)
  {
    runNumber = 999999;
  }
  inDeadFEB = !(selector->allElecsPassFEB(runNumber,electrons,print));
  if (print && inDeadFEB) 
  {
    std::cout << "One electron failed FEB" << std::endl;
//     for (int k = 0; k != (int)electrons.size(); ++k)
//     {
//       std::cout <<"("<<electrons[k]->Pt()<<","<<electrons[k]->ClusterEta()<<","<<electrons[k]->ClusterPhi()<<")"<<std::endl;
//     }
  }

  std::vector<MTree::Jet*> jets = selector->getFEBJets();
  if (print) std::cout << "Checking " << (int)jets.size() << " FEB jets" << std::endl;
  removeOverlap(0.3,electrons,jets);
  if (print) std::cout << "After overlap, " << (int)jets.size() << " FEB jets" << std::endl;
  febJs += (int)jets.size();

  if (!inDeadFEB)
  {
    for (int i = 0; i != (int)jets.size(); ++i)
    {
      if (inDeadFEB)
      {
	if (print) std::cout << "inDeadFEB is true!" << std::endl;
	continue;
      }

      TLorentzVector jet;
      double jes = jets[i]->JetMoment("EMJES");
      jet.SetPxPyPzE(jes*jets[i]->GetRawPx(),jes*jets[i]->GetRawPy(),jes*jets[i]->GetRawPz(),jes*jets[i]->GetRawE());
      double pt = jets[i]->Pt();
      double eta = jets[i]->Eta();
      double phi = jets[i]->Phi();
      double bch_corr_jet = jets[i]->JetMoment("BCH_CORR_JET");
      double bch_corr_cell = jets[i]->JetMoment("BCH_CORR_CELL");

      if (print)
      { 
	std::cout << "Jet: " << pt << " " << eta << " " << phi << std::endl;
	std::cout << "Jet: " << jets[i]->Pt() << " " << jets[i]->Eta() << " " << jets[i]->Phi() << std::endl;
	std::cout << "Jet: " << bch_corr_jet << " " << bch_corr_cell << std::endl;
      }
      inDeadFEB = IsLArHoleVeto(pt,eta,phi,bch_corr_jet,bch_corr_cell,!(isMC),25);
      if (inDeadFEB)
      {
	++badJs;
	if (print) std::cout << "jet pointing to dead FEB" << std::endl;
      }

    }
  }

  if (isMC && inDeadFEB)
  {
    evWeight *= FEBSCALE;
    return(true);
  }
  else if (!isMC && inDeadFEB && runNumber > 180614 && runNumber < 185353)
  {
    return(false);
  }
  else
  {
    return(true);
  }

}

//######################################################################################

double HWWAnalysis::getBJetWeight(std::vector<MTree::Jet*> jets)
{
//   calib = new Analysis::CalibrationDataInterfaceROOT("SV0","BTagCalibration.env");
//   ajet = new Analysis::CalibrationDataVariables();
//   ajet->jetAuthor = "AntiKt4Topo";

  double weight = 1.0;
  Analysis::Uncertainty uncertainty = Analysis::Total;

  for (int i = 0; i != (int)jets.size(); ++i)
  {
    MTree::Jet* jet = jets[i];

    ajet->jetPt = (jet->Pt())*1000;
    ajet->jetEta = jet->Eta();

    std::string flavor;
    if (jet->JetFlavor() == 5)
    {
      flavor = "B";
    }
    else if (jet->JetFlavor() == 4)
    {
      flavor = "C";
    }
    else
    {
      flavor = "Light";
    }
    std::string op = "5_85";

    Analysis::CalibResult res = calib->getScaleFactor(*ajet,flavor,op,uncertainty);
    if (res.first != -1)
    {
      weight *= res.first;
    }
    else
    {
      Analysis::CalibResult resineff = calib->getInefficiencyScaleFactor(*ajet,flavor,op,uncertainty);
      weight *= resineff.first;
    }
  }

  return(weight);
}

//######################################################################################

double HWWAnalysis::getHiggsPtWeight(std::string sample)
{
  if (sample.find("hww") == std::string::npos)
  {
    return 1.0;
  }
  else
  {
    double hpT = 0.0;
    int nTruthParticles = _truthParticles->GetEntriesFast();
    std::vector<MTree::TruthParticle*> daughters;
    for (int i = 0; i != nTruthParticles; ++i)
    {
      MTree::TruthParticle* truth = (MTree::TruthParticle*)_truthParticles->At(i);
      if (truth->PdgID() == 25)
      {
	hpT = truth->Pt();
      }
    }

    if (hpT < 0.01) std::cout << "failed to find higgs!" << std::endl;
    return (reweight->getWeight(hpT));
  }
}

//######################################################################################

// int HWWAnalysis::bunchpos(const int run, const int bcid, const int isMC, int & gap, int & len ) {

//   gap = 999999;
//   len = 999999;

//   // list of BCIDs at which bunch trains start
//   std::vector<int> trainstarts;
//   // list of lengths of sub-trains
//   std::vector<int> lengths;
//   // list of gaps between sub-trains
//   //  gap.at(n-1) is the gap preceding the train that starts at trainstarts.at(n)
//   std::vector<int> gaps;

//   if (isMC) {
//     // Set up list of bunch train start positions for MC
//     // (obtained by looking at gaps in BCID distributions in MC)
//     trainstarts.push_back(1); lengths.push_back(36); gaps.push_back(9);
//     trainstarts.push_back(80); lengths.push_back(36); gaps.push_back(9);
//     trainstarts.push_back(159); lengths.push_back(36);
//   } else {
//     // Set up list of bunch train start positions for data
//     // (obtained from atlas-runquery.cern.ch with 'show all')
//     trainstarts.reserve(39);
//     lengths.reserve(39);
//     gaps.reserve(38);
//     if (run==179710 ||
//         run==179725 ||
//         run==179739) {//D1: 214 in 8 trains in ARQ
//       trainstarts.push_back(1); lengths.push_back(1); gaps.push_back(44);
//       trainstarts.push_back(45); lengths.push_back(9); gaps.push_back(123);
//       trainstarts.push_back(183); lengths.push_back(36); gaps.push_back(824);
//       trainstarts.push_back(1077); lengths.push_back(36); gaps.push_back(639);
//       trainstarts.push_back(1786); lengths.push_back(30); gaps.push_back(127);
//       trainstarts.push_back(1971); lengths.push_back(30); gaps.push_back(639);
//       trainstarts.push_back(2668); lengths.push_back(36); gaps.push_back(115);
//       trainstarts.push_back(2853); lengths.push_back(36);
//     } else if (run==179771 ||
//                run==179804 ||
//                run==179938 ||
//                run==179939 ||
//                run==179940) {//D2,D3: 322 in 11 trains in ARQ
//       trainstarts.push_back(1); lengths.push_back(1); gaps.push_back(44);
//       trainstarts.push_back(45); lengths.push_back(9); gaps.push_back(123);
//       trainstarts.push_back(184); lengths.push_back(36); gaps.push_back(195);
//       trainstarts.push_back(449); lengths.push_back(36); gaps.push_back(559);
//       trainstarts.push_back(1078); lengths.push_back(36); gaps.push_back(195);
//       trainstarts.push_back(1343); lengths.push_back(36); gaps.push_back(559);
//       trainstarts.push_back(1972); lengths.push_back(30); gaps.push_back(207);
//       trainstarts.push_back(2237); lengths.push_back(30); gaps.push_back(373);
//       trainstarts.push_back(2668); lengths.push_back(36); gaps.push_back(116);
//       trainstarts.push_back(2854); lengths.push_back(36); gaps.push_back(195);
//       trainstarts.push_back(3119); lengths.push_back(36);
//     } else if (run==180122 ||
//                run==180144
//                ) {//D3: 322 in 7 trains
//       trainstarts.push_back(1); lengths.push_back(1); gaps.push_back(44);
//       trainstarts.push_back(45); lengths.push_back(9); gaps.push_back(123);
//       trainstarts.push_back(184); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(263); lengths.push_back(36); gaps.push_back(745);
//       trainstarts.push_back(1078); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1157); lengths.push_back(36); gaps.push_back(745);
//       trainstarts.push_back(1972); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2051); lengths.push_back(30); gaps.push_back(559);
//       trainstarts.push_back(2668); lengths.push_back(36); gaps.push_back(116);
//       trainstarts.push_back(2854); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(2933); lengths.push_back(36);
//     } else if (run==180124 ||
//                run==180139 ||
//                run==180149 ||
//                run==180153 ||
//                run==180164) {//D4: 424 in 14 trains in ARQ
//       trainstarts.push_back(1); lengths.push_back(1); gaps.push_back(44);
//       trainstarts.push_back(45); lengths.push_back(9); gaps.push_back(123);
//       trainstarts.push_back(184); lengths.push_back(36); gaps.push_back(88);
//       trainstarts.push_back(342); lengths.push_back(36); gaps.push_back(37);
//       trainstarts.push_back(449); lengths.push_back(36); gaps.push_back(559);
//       trainstarts.push_back(1078); lengths.push_back(36); gaps.push_back(88);
//       trainstarts.push_back(1236); lengths.push_back(36); gaps.push_back(37);
//       trainstarts.push_back(1343); lengths.push_back(36); gaps.push_back(559);
//       trainstarts.push_back(1972); lengths.push_back(30); gaps.push_back(100);
//       trainstarts.push_back(2130); lengths.push_back(30); gaps.push_back(49);
//       trainstarts.push_back(2237); lengths.push_back(30); gaps.push_back(559);
//       trainstarts.push_back(2854); lengths.push_back(36); gaps.push_back(88);
//       trainstarts.push_back(3012); lengths.push_back(36); gaps.push_back(37);
//       trainstarts.push_back(3119); lengths.push_back(36);
//     } else if (run==180212 ||
//                run==180225 ||
//                run==180241 ||
//                run==180242 ||
//                run==180309
//                ) {//D4,5,6,F2: 424 in 10 trains in ARQ
//       trainstarts.push_back(1); lengths.push_back(1); gaps.push_back(44);
//       trainstarts.push_back(45); lengths.push_back(9); gaps.push_back(123);
//       trainstarts.push_back(184); lengths.push_back(36); gaps.push_back(195);
//       trainstarts.push_back(449); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(528); lengths.push_back(36); gaps.push_back(480);
//       trainstarts.push_back(1078); lengths.push_back(36); gaps.push_back(195);
//       trainstarts.push_back(1343); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1422); lengths.push_back(36); gaps.push_back(480);
//       trainstarts.push_back(1972); lengths.push_back(30); gaps.push_back(207);
//       trainstarts.push_back(2237); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2316); lengths.push_back(30); gaps.push_back(480);
//       trainstarts.push_back(2854); lengths.push_back(36); gaps.push_back(195);
//       trainstarts.push_back(3119); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(3198); lengths.push_back(36);
//     } else if (run==180400 ||
//                run==180448 ||
//                run==180481 ||
//                run==180614 ||
//                run==180636) {//D6 D7 E1: 598 in 11 trains in ARQ
//       trainstarts.push_back(1); lengths.push_back(1); gaps.push_back(44);
//       trainstarts.push_back(45); lengths.push_back(9); gaps.push_back(44);
//       trainstarts.push_back(105); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(184); lengths.push_back(36); gaps.push_back(193);
//       trainstarts.push_back(447); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(526); lengths.push_back(36); gaps.push_back(403);
//       trainstarts.push_back(999); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1078); lengths.push_back(36); gaps.push_back(193);
//       trainstarts.push_back(1341); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1420); lengths.push_back(36); gaps.push_back(403);
//       trainstarts.push_back(1893); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(1972); lengths.push_back(30); gaps.push_back(205);
//       trainstarts.push_back(2235); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2314); lengths.push_back(30); gaps.push_back(296);
//       trainstarts.push_back(2668); lengths.push_back(36); gaps.push_back(37);
//       trainstarts.push_back(2775); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(2854); lengths.push_back(36); gaps.push_back(193);
//       trainstarts.push_back(3117); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(3196); lengths.push_back(36);
//     } else if (run==180664 ||
//                run==180710 ||
//                run==180776 ||
//                run==182284) {//E1 F2: 700 in 14 trains in ARQ
//       trainstarts.push_back(1); lengths.push_back(1); gaps.push_back(44);
//       trainstarts.push_back(45); lengths.push_back(9); gaps.push_back(44);
//       trainstarts.push_back(105); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(184); lengths.push_back(36); gaps.push_back(86);
//       trainstarts.push_back(340); lengths.push_back(36); gaps.push_back(37);
//       trainstarts.push_back(447); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(526); lengths.push_back(36); gaps.push_back(404);
//       trainstarts.push_back(999); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1078); lengths.push_back(36); gaps.push_back(96);
//       trainstarts.push_back(1234); lengths.push_back(36); gaps.push_back(37);
//       trainstarts.push_back(1341); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1420); lengths.push_back(36); gaps.push_back(403);
//       trainstarts.push_back(1893); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(1972); lengths.push_back(30); gaps.push_back(98);
//       trainstarts.push_back(2128); lengths.push_back(30); gaps.push_back(49);
//       trainstarts.push_back(2235); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2314); lengths.push_back(30); gaps.push_back(403);
//       trainstarts.push_back(2775); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(2854); lengths.push_back(36); gaps.push_back(86);
//       trainstarts.push_back(3010); lengths.push_back(36); gaps.push_back(37);
//       trainstarts.push_back(3117); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(3196); lengths.push_back(36);
//     } else if (run==182161) {//F2: 424 in 10 trains in ARQ
//       trainstarts.push_back(1); lengths.push_back(1); gaps.push_back(45);
//       trainstarts.push_back(46); lengths.push_back(9); gaps.push_back(122);
//       trainstarts.push_back(184); lengths.push_back(36); gaps.push_back(195);
//       trainstarts.push_back(449); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(528); lengths.push_back(36); gaps.push_back(480);
//       trainstarts.push_back(1078); lengths.push_back(36); gaps.push_back(195);
//       trainstarts.push_back(1343); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1422); lengths.push_back(36); gaps.push_back(480);
//       trainstarts.push_back(1972); lengths.push_back(30); gaps.push_back(207);
//       trainstarts.push_back(2237); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2316); lengths.push_back(30); gaps.push_back(480);
//       trainstarts.push_back(2854); lengths.push_back(36); gaps.push_back(195);
//       trainstarts.push_back(3119); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(3198); lengths.push_back(36);
//     } else if (run==182346 ||
//                run==182372) {//F2: 700 in 10 trains in ARQ
//       trainstarts.push_back(1); lengths.push_back(1); gaps.push_back(44);
//       trainstarts.push_back(45); lengths.push_back(9); gaps.push_back(44);
//       trainstarts.push_back(105); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(184); lengths.push_back(36); gaps.push_back(195);
//       trainstarts.push_back(449); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(528); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(607); lengths.push_back(36); gaps.push_back(322);
//       trainstarts.push_back(999); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1078); lengths.push_back(36); gaps.push_back(195);
//       trainstarts.push_back(1343); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1422); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1501); lengths.push_back(36); gaps.push_back(322);
//       trainstarts.push_back(1893); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(1972); lengths.push_back(30); gaps.push_back(207);
//       trainstarts.push_back(2237); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2316); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2395); lengths.push_back(30); gaps.push_back(322);
//       trainstarts.push_back(2775); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(2854); lengths.push_back(36); gaps.push_back(195);
//       trainstarts.push_back(3119); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(3198); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(3277); lengths.push_back(36);
//     } else if (run==182424 ||
//                run==182449 ||
//                run==182450 ||
//                run==182454 ||
//                run==182455 ||
//                run==182456 ||
//                run==182486 ||
//                run==182516 ||
//                run==182518 ||
//                run==182519) { //F2 F3: 874 in 11 trains in ARQ
//       trainstarts.push_back(1); lengths.push_back(1); gaps.push_back(44);
//       trainstarts.push_back(45); lengths.push_back(9); gaps.push_back(44);
//       trainstarts.push_back(105); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(184); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(263); lengths.push_back(36); gaps.push_back(116);
//       trainstarts.push_back(449); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(528); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(607); lengths.push_back(36); gaps.push_back(322);
//       trainstarts.push_back(999); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1078); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1157); lengths.push_back(36); gaps.push_back(116);
//       trainstarts.push_back(1343); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1422); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1501); lengths.push_back(36); gaps.push_back(322);
//       trainstarts.push_back(1893); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(1972); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2051); lengths.push_back(30); gaps.push_back(128);
//       trainstarts.push_back(2237); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2316); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2395); lengths.push_back(30); gaps.push_back(215);
//       trainstarts.push_back(2668); lengths.push_back(36); gaps.push_back(37);
//       trainstarts.push_back(2775); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(2854); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(2933); lengths.push_back(36); gaps.push_back(116);
//       trainstarts.push_back(3119); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(3198); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(3277); lengths.push_back(36);
//     } else if (run==182726 ||
//                run==182747 ||
//                run==182766) { // G1 G2: 874 in 11 trains in ARQ
//       trainstarts.push_back(1); lengths.push_back(1); gaps.push_back(45);
//       trainstarts.push_back(46); lengths.push_back(9); gaps.push_back(43);
//       trainstarts.push_back(105); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(184); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(263); lengths.push_back(36); gaps.push_back(116);
//       trainstarts.push_back(449); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(528); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(607); lengths.push_back(36); gaps.push_back(322);
//       trainstarts.push_back(999); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1078); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1157); lengths.push_back(36); gaps.push_back(116);
//       trainstarts.push_back(1343); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1422); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1501); lengths.push_back(36); gaps.push_back(322);
//       trainstarts.push_back(1893); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(1972); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2051); lengths.push_back(30); gaps.push_back(128);
//       trainstarts.push_back(2237); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2316); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2395); lengths.push_back(30); gaps.push_back(215);
//       trainstarts.push_back(2668); lengths.push_back(36); gaps.push_back(37);
//       trainstarts.push_back(2775); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(2854); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(2933); lengths.push_back(36); gaps.push_back(116);
//       trainstarts.push_back(3119); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(3198); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(3277); lengths.push_back(36);
//     } else if (run==182787 ||
//                run==182796 ||
//                run==182879 ||
//                run==182886 ||
//                run==182997 ||
//                run==183003 ||
//                run==183021 ||
//                run==183038 ||
//                run==183045 ||
//                run==183054 ||
//                run==183078 ||
//                run==183079 ||
//                run==183081 ||
//                run==183127 ||
//                run==183129 ||
//                run==183130 ||
//                run==183216) {// G2-: 1042 in 13 trains in ARQ
//       trainstarts.push_back(1); lengths.push_back(1); gaps.push_back(44);
//       trainstarts.push_back(45); lengths.push_back(9); gaps.push_back(44);
//       trainstarts.push_back(105); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(184); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(263); lengths.push_back(36); gaps.push_back(116);
//       trainstarts.push_back(449); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(528); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(607); lengths.push_back(36); gaps.push_back(136);
//       trainstarts.push_back(813); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(892); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(999); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1078); lengths.push_back(36); gaps.push_back(195);
//       trainstarts.push_back(1157); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1343); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1422); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1501); lengths.push_back(36); gaps.push_back(136);
//       trainstarts.push_back(1707); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(1786); lengths.push_back(30); gaps.push_back(49);
//       trainstarts.push_back(1893); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(1972); lengths.push_back(30); gaps.push_back(207);
//       trainstarts.push_back(2051); lengths.push_back(30); gaps.push_back(128);
//       trainstarts.push_back(2237); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2316); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2395); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2589); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(2668); lengths.push_back(36); gaps.push_back(37);
//       trainstarts.push_back(2775); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(2854); lengths.push_back(36); gaps.push_back(95);
//       trainstarts.push_back(2933); lengths.push_back(36); gaps.push_back(116);
//       trainstarts.push_back(3119); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(3198); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(3277); lengths.push_back(36);
//     } else if (run==183272 ||
//                run==183286 ||
//                run==183347 ||
//                run==183391 ||
//                run==183407 ||
//                run==183412 ||
//                run==183426 ||
//                run==183462) {// period G: 1042 in 13 (but different from earlier 1042in13 runs!)
//       trainstarts.push_back(1); lengths.push_back(1); gaps.push_back(44);
//       trainstarts.push_back(45); lengths.push_back(9); gaps.push_back(44);
//       trainstarts.push_back(105); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(184); lengths.push_back(36); gaps.push_back(195);
//       trainstarts.push_back(449); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(528); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(607); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(686); lengths.push_back(36); gaps.push_back(57);
//       trainstarts.push_back(813); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(892); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(999); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1078); lengths.push_back(36); gaps.push_back(195);
//       trainstarts.push_back(1343); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1422); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1501); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1580); lengths.push_back(36); gaps.push_back(57);
//       trainstarts.push_back(1707); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(1786); lengths.push_back(30); gaps.push_back(49);
//       trainstarts.push_back(1893); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(1972); lengths.push_back(30); gaps.push_back(207);
//       trainstarts.push_back(2237); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2316); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2395); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2474); lengths.push_back(30); gaps.push_back(57);
//       trainstarts.push_back(2589); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(2668); lengths.push_back(36); gaps.push_back(37);
//       trainstarts.push_back(2775); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(2854); lengths.push_back(36); gaps.push_back(95);
//       trainstarts.push_back(3119); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(3198); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(3277); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(3356); lengths.push_back(36);
//     } else if (run==183544 ||
//                run==183580 ||
//                run==183581 ||
//                run==183602
//                ) {// period H: 1042 in 13
//       trainstarts.push_back(1); lengths.push_back(1); gaps.push_back(44);
//       trainstarts.push_back(45); lengths.push_back(9); gaps.push_back(44);
//       trainstarts.push_back(105); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(184); lengths.push_back(36); gaps.push_back(195);
//       trainstarts.push_back(449); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(528); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(607); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(686); lengths.push_back(36); gaps.push_back(57);
//       trainstarts.push_back(813); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(892); lengths.push_back(36); gaps.push_back(37);
//       trainstarts.push_back(999); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1078); lengths.push_back(36); gaps.push_back(195);
//       trainstarts.push_back(1343); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1422); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1501); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1580); lengths.push_back(36); gaps.push_back(57);
//       trainstarts.push_back(1707); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(1786); lengths.push_back(30); gaps.push_back(49);
//       trainstarts.push_back(1893); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(1972); lengths.push_back(30); gaps.push_back(207);
//       trainstarts.push_back(2237); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2316); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2395); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2474); lengths.push_back(30); gaps.push_back(57);
//       trainstarts.push_back(2589); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(2668); lengths.push_back(36); gaps.push_back(37);
//       trainstarts.push_back(2775); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(2854); lengths.push_back(36); gaps.push_back(195);
//       trainstarts.push_back(3119); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(3198); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(3277); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(3356); lengths.push_back(36);
//     }
//     else if (run == 183780) {//1041 bunches in 12 trains
//       trainstarts.push_back(45); lengths.push_back(9); gaps.push_back(44);
//       trainstarts.push_back(105); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(184); lengths.push_back(36); gaps.push_back(195);
//       trainstarts.push_back(449); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(528); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(607); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(686); lengths.push_back(36); gaps.push_back(57);
//       trainstarts.push_back(813); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(892); lengths.push_back(36); gaps.push_back(37);
//       trainstarts.push_back(999); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1078); lengths.push_back(36); gaps.push_back(195);
//       trainstarts.push_back(1343); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1422); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1501); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1580); lengths.push_back(36); gaps.push_back(57);
//       trainstarts.push_back(1707); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(1786); lengths.push_back(30); gaps.push_back(49);
//       trainstarts.push_back(1893); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(1972); lengths.push_back(30); gaps.push_back(207);
//       trainstarts.push_back(2237); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2316); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2395); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2474); lengths.push_back(30); gaps.push_back(57);
//       trainstarts.push_back(2589); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(2668); lengths.push_back(36); gaps.push_back(37);
//       trainstarts.push_back(2775); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(2854); lengths.push_back(36); gaps.push_back(195);
//       trainstarts.push_back(3119); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(3198); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(3277); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(3356); lengths.push_back(36);
//     }
//      else if (run==183963 ||
//                run==184022 ||
//                run==184066 ||
//                run==184072 ||
//                run==184074 ||
//                run==184088 ||
//                run==184130
//                ) {//1180 in 13 trains
//       trainstarts.push_back(1); lengths.push_back(1); gaps.push_back(44);
//       trainstarts.push_back(45); lengths.push_back(9); gaps.push_back(44);
//       trainstarts.push_back(105); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(184); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(263); lengths.push_back(36); gaps.push_back(116);
//       trainstarts.push_back(449); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(528); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(607); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(686); lengths.push_back(36); gaps.push_back(57);
//       trainstarts.push_back(813); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(892); lengths.push_back(36); gaps.push_back(37);
//       trainstarts.push_back(999); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1078); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1157); lengths.push_back(36); gaps.push_back(116);
//       trainstarts.push_back(1343); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1422); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1501); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1580); lengths.push_back(36); gaps.push_back(57);
//       trainstarts.push_back(1707); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(1786); lengths.push_back(30); gaps.push_back(49);
//       trainstarts.push_back(1893); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(1972); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2051); lengths.push_back(30); gaps.push_back(128);
//       trainstarts.push_back(2237); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2316); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2395); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2474); lengths.push_back(30); gaps.push_back(57);
//       trainstarts.push_back(2589); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(2668); lengths.push_back(36); gaps.push_back(37);
//       trainstarts.push_back(2775); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(2854); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(2933); lengths.push_back(36); gaps.push_back(116);
//       trainstarts.push_back(3119); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(3198); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(3277); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(3356); lengths.push_back(36);
//     } else if (run==184169) {//1318 in 13 trains
//       trainstarts.push_back(1); lengths.push_back(1); gaps.push_back(44);
//       trainstarts.push_back(45); lengths.push_back(9); gaps.push_back(44);
//       trainstarts.push_back(105); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(184); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(263); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(342); lengths.push_back(36); gaps.push_back(37);
//       trainstarts.push_back(449); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(528); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(607); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(686); lengths.push_back(36); gaps.push_back(57);
//       trainstarts.push_back(813); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(892); lengths.push_back(36); gaps.push_back(37);
//       trainstarts.push_back(999); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1078); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1157); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1236); lengths.push_back(36); gaps.push_back(37);
//       trainstarts.push_back(1343); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1422); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1501); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(1580); lengths.push_back(36); gaps.push_back(57);
//       trainstarts.push_back(1707); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(1786); lengths.push_back(30); gaps.push_back(49);
//       trainstarts.push_back(1893); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(1972); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2051); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2130); lengths.push_back(30); gaps.push_back(49);
//       trainstarts.push_back(2237); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2316); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2395); lengths.push_back(30); gaps.push_back(21);
//       trainstarts.push_back(2474); lengths.push_back(30); gaps.push_back(57);
//       trainstarts.push_back(2589); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(2668); lengths.push_back(36); gaps.push_back(37);
//       trainstarts.push_back(2775); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(2854); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(2933); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(3119); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(3198); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(3277); lengths.push_back(36); gaps.push_back(9);
//       trainstarts.push_back(3356); lengths.push_back(36);
//     } else {
//       return -1;
//     }
//   }

//   int retval = -1;
//   unsigned int tsSize = trainstarts.size();
//   //std::cout << "bunchpos: " << run << " " << bcid << " " << isMC << " " << tsSize << std::endl;
//   for (int i=tsSize-1; i>=0; i--) {
//     if (bcid >= trainstarts.at(i)) {
//       retval = bcid - trainstarts.at(i);
//       if ( tsSize == lengths.size() ) {
//         len = lengths.at(i);
//       }
//       //else {std::cout << "lengths size: "<<trainstarts.size()<<" " <<lengths.size()<<std::endl;}
//       if ( tsSize-1 == gaps.size()
//            && (i>0)) {
//         gap = gaps.at(i-1);
//       }
//       //else {std::cout << "gaps size: "<<trainstarts.size()<<" "<<gaps.size()
//       //              <<" ("<<i<<")"<<std::endl;}
//       break;
//     }
//   }

//   //std::cout << "  bunchpos will return " << retval << std::endl;

//   return retval;
// }


//######################################################################################
