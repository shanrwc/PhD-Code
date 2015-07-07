#include "ConvertD3PDToMTreeData.h"

ConvertD3PDToMTreeData::ConvertD3PDToMTreeData(TChain *inputChain) : mSUSYD3PD(0), 
                                                                     mOutputTree(0), 
                                                                     mInputChain(inputChain)
{ ; }

void ConvertD3PDToMTreeData::initialize()
{
  mDoEMJets = true;
  mAnaType = "data";
  std::transform(mAnaType.begin(), mAnaType.end(), mAnaType.begin(), ::tolower);
  std::cout << "| " << mInputChain->GetEntries() << std::endl;
  mSUSYD3PD = new SUSYD3PDClass_Data(mInputChain);

  mElectronCollection          = new TClonesArray("MTree::Electron", 10); 
  mPhotonCollection            = new TClonesArray("MTree::Photon", 10); 
  mMuonCollection              = new TClonesArray("MTree::Muon", 10); 
  mCaloMuonCollection          = new TClonesArray("MTree::Muon", 10); 
  mJetCollection               = new TClonesArray("MTree::Jet", 20); 
  mEMJetCollection               = new TClonesArray("MTree::Jet", 20); 
  mTrackCollection             = new TClonesArray("MTree::Track", 100); 
  mPrimaryVertexCollection     = new TClonesArray("MTree::PrimaryVertex", 10); 
  mTruthParticleCollection     = new TClonesArray("MTree::TruthParticle", 5); 
  mL1EMTauTriggerROICollection = new TClonesArray("MTree::L1TriggerROI", 10);
  mL1MuonTriggerROICollection  = new TClonesArray("MTree::L1TriggerROI", 10);
  mHLTTriggerROICollection     = new TClonesArray("MTree::HLTTriggerROI", 10);
  mMissingETCollection         = new TClonesArray("MTree::MissingET", 10);

  mMissingET = new MTree::MissingET();
  mEventInfo = new MTree::EventInfo();
  mTrigger   = new MTree::Trigger();

  mOutputFile = new TFile("MTree.root", "recreate");
  mOutputTree = new TTree("MTree", "An analysis tree");
  mOutputTree->Branch("Electrons", &mElectronCollection);
  mOutputTree->Branch("Photons", &mPhotonCollection);
  mOutputTree->Branch("Muons", &mMuonCollection);
  mOutputTree->Branch("CaloMuons", &mCaloMuonCollection);
  mOutputTree->Branch("Jets", &mJetCollection);
  if (mDoEMJets) mOutputTree->Branch("EMJets", &mEMJetCollection);
  mOutputTree->Branch("Tracks", &mTrackCollection);
  mOutputTree->Branch("PrimaryVertices", &mPrimaryVertexCollection);
  mOutputTree->Branch("TruthParticles", &mTruthParticleCollection);
  mOutputTree->Branch("L1EmTauTriggerROIs", &mL1EMTauTriggerROICollection);
  mOutputTree->Branch("L1MuonTriggerROIs", &mL1MuonTriggerROICollection);
  mOutputTree->Branch("HLTTriggerROIs", &mHLTTriggerROICollection);
  mOutputTree->Branch("MissingETTerms", &mMissingETCollection);
  mOutputTree->Branch("MissingET", &mMissingET);
  mOutputTree->Branch("EventInfo", &mEventInfo);
  mOutputTree->Branch("Trigger", &mTrigger);

  mEFTriggerD3PDItems.clear();
  mL1TriggerD3PDItems.clear();
  mL2TriggerD3PDItems.clear();

  return;
}

void ConvertD3PDToMTreeData::finalize()
{
  mOutputFile->Write();
  mOutputFile->Close();
}


void ConvertD3PDToMTreeData::run()
{
  std::cout << "| DEBUG address of mInputChain: " << mInputChain << std::endl;   
  std::cout << "Events to process: " << mInputChain->GetEntries() << std::endl;
  int entry = 0;
  int total = mInputChain->GetEntries();
  int maxEvents = total;
  int countD3PD  = 0;
  int countMTree = 0;
  std::string check = "";
  //entry = 26200;
  while (mInputChain->GetEntry(entry++))
  {
    mDoPrint = false;
    if (mDoPrint) std::cout << "DEBUG Event Nr " << entry << std::endl;
    std::string curfile = std::string(mInputChain->GetCurrentFile()->GetName());
    if (check != curfile)
    {
      std::cout << "DEBUG New file name: " << curfile << std::endl;
      mDoPrint = false;
      check = curfile;
    }
    if (entry > maxEvents) break;
    if (!(entry % int(1e3))) std::cout << "| Event " << entry << " / " << total << std::endl;
    int trackCounter = 0;
    int primaryVertexCounter = 0;
    int truthParticleCounter = 0;
    int jetCounter = 0;
    int emjetCounter = 0;
    int elCounter = 0;
    int phCounter = 0;
    int muCounter = 0;
    int caloMuCounter = 0;
    int missingETCounter = 0;
    //
    // Fill PVs
    //
    if (mDoPrint) std::cout << "DEBUG PV" << std::endl;
    mPrimaryVertexCollection->Clear();
    for (int i = 0; i < mSUSYD3PD->vx_n; i++)
    {
      if (mDoPrint) std::cout << "DEBUG Adding new PV to collection"  << std::endl;
      MTree::PrimaryVertex* vertex = (MTree::PrimaryVertex*) mPrimaryVertexCollection->New(primaryVertexCounter++);
      vertex->Initialize((double)(*mSUSYD3PD->vx_x)[i],
                         (double)(*mSUSYD3PD->vx_y)[i],
                         (double)(*mSUSYD3PD->vx_z)[i],
                         TMath::Sqrt((double)(*mSUSYD3PD->vx_cov_x)[i]),
                         TMath::Sqrt((double)(*mSUSYD3PD->vx_cov_y)[i]),
                         TMath::Sqrt((double)(*mSUSYD3PD->vx_cov_z)[i]),
                         (int)(*mSUSYD3PD->vx_nTracks)[i],
                         (double)(*mSUSYD3PD->vx_chi2)[i],
                         (double)(*mSUSYD3PD->vx_ndof)[i],
                         (double)(*mSUSYD3PD->vx_type)[i]);
      if (mDoPrint) std::cout << "DEBUG Init PV finished " << primaryVertexCounter << std::endl;
    }
    //
    // Fill Jets
    //
    mJetCollection->Clear();
    if (mDoPrint) std::cout << "DEBUG Jets" << std::endl;
    for (int i = 0; i < mSUSYD3PD->jet_AntiKt4H1Topo_n; i++)
    {
      MTree::Jet* jet = (MTree::Jet*) mJetCollection->New(jetCounter++);
      if (mDoPrint) std::cout << "DEBUG Init Jet " << jetCounter << " / " << mSUSYD3PD->jet_AntiKt4H1Topo_n << std::endl;
      jet->Initialize((double)(*mSUSYD3PD->jet_AntiKt4H1Topo_pt)[i]/1000.,
                      (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_eta)[i],
                      (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_phi)[i],
                      (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_m)[i]/1000.,
                      (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_flavor_weight_IP3D)[i],
                      (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_flavor_weight_SV0)[i],
                      (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_flavor_weight_SV1)[i],
                      (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_flavor_weight_SV2)[i],
                      (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_flavor_weight)[i],
                      (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_flavor_weight_TrackCounting2D)[i],
                      (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_flavor_weight_JetProb)[i]);
      if (mDoPrint) std::cout << "DEBUG SetJetQual Jet " << jetCounter << " / " << mSUSYD3PD->jet_AntiKt4H1Topo_n << std::endl;
      jet->SetJetQualities((double)(*mSUSYD3PD->jet_AntiKt4H1Topo_emfrac)[i],
                           (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_LArQuality)[i],
                           (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_Timing)[i],
                           (int)(*mSUSYD3PD->jet_AntiKt4H1Topo_n90)[i],
                           0, //N90Constituents,
                           (int)(*mSUSYD3PD->jet_AntiKt4H1Topo_SamplingMax)[i],
                           (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_fracSamplingMax)[i],
                           (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_hecf)[i],
                           (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_tgap3f)[i]);
      if (mDoPrint) std::cout << "DEBUG SetQualFlags Jet " << jetCounter << " / " << mSUSYD3PD->jet_AntiKt4H1Topo_n << std::endl;
      jet->SetJetQualityFlags(false, //isGood
                              false, //isBad
                              false);//isUgly
      if (mDoPrint) std::cout << "DEBUG SetSamplings Jet " << jetCounter << " / " << mSUSYD3PD->jet_AntiKt4H1Topo_n << std::endl;
      jet->SetSamplings((double)(*mSUSYD3PD->jet_AntiKt4H1Topo_e_PreSamplerB)[i]/1000.,
                        (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_e_EMB1)[i]/1000.,
                        (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_e_EMB2)[i]/1000.,
                        (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_e_EMB3)[i]/1000.,
                        (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_e_PreSamplerE)[i]/1000.,
                        (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_e_EME1)[i]/1000.,
                        (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_e_EME2)[i]/1000.,
                        (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_e_EME3)[i]/1000.,
                        (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_e_HEC0)[i]/1000.,
                        (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_e_HEC1)[i]/1000.,
                        (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_e_HEC2)[i]/1000.,
                        (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_e_HEC3)[i]/1000.,
                        (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_e_TileBar0)[i]/1000.,
                        (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_e_TileBar1)[i]/1000.,
                        (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_e_TileBar2)[i]/1000.,
                        (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_e_TileGap1)[i]/1000.,
                        (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_e_TileGap2)[i]/1000.,
                        (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_e_TileGap3)[i]/1000.,
                        (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_e_TileExt0)[i]/1000.,
                        (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_e_TileExt1)[i]/1000.,
                        (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_e_TileExt2)[i]/1000.,
                        (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_e_FCAL0)[i]/1000.,
                        (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_e_FCAL1)[i]/1000.,
                        (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_e_FCAL2)[i]/1000.);
       TLorentzVector transformEM, transformCS;
       transformEM.SetPtEtaPhiM((double)(*mSUSYD3PD->jet_AntiKt4H1Topo_emscale_pt)[i]/1000.,
                                (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_emscale_eta)[i],
                                (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_emscale_phi)[i],
                                (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_emscale_m)[i]/1000.);
       transformCS.SetPtEtaPhiM((double)(*mSUSYD3PD->jet_AntiKt4H1Topo_constscale_pt)[i]/1000.,
                                (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_constscale_eta)[i],
                                (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_constscale_phi)[i],
                                (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_constscale_m)[i]/1000.);
      if (mDoPrint) std::cout << "DEBUG SetRaw Jet " << jetCounter << " / " << mSUSYD3PD->jet_AntiKt4H1Topo_n << std::endl;
       jet->SetRawEnergies((double)(*mSUSYD3PD->jet_AntiKt4H1Topo_emscale_E)[i]/1000.,
                           transformEM.Px(),
                           transformEM.Py(),
                           transformEM.Pz(),
                           (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_constscale_E)[i]/1000.,
                           transformCS.Px(),
                           transformCS.Py(),
                           transformCS.Pz(),
                           0., //CalE
                           0., //CalPx
                           0., //CalPy
                           0., //CalPz
                           (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_EMJES)[i],
                           (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_GCWJES)[i]);
      if (mDoPrint) std::cout << "DEBUG SetJVF Jet " << jetCounter << " / " << mSUSYD3PD->jet_AntiKt4H1Topo_n << std::endl;
       jet->SetJVF((double)(*mSUSYD3PD->jet_AntiKt4H1Topo_jvtxf)[i],
                   (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_nTrk)[i],
                   (double)(*mSUSYD3PD->jet_AntiKt4H1Topo_sumPtTrk)[i]);
       jet->SetBtagBDTs(0., 0., 0.);
      if (mDoPrint) std::cout << "DEBUG SetJMETWeights Jet " << jetCounter << " / " << mSUSYD3PD->jet_AntiKt4H1Topo_n << std::endl;
       jet->SetMissingETWeights((double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_MET_Simplified20withTightPhotons_wpx)[i],
                                (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_MET_Simplified20withTightPhotons_wpy)[i], 
                                (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_MET_Simplified20withTightPhotons_wet)[i]);
    }
    if (mDoEMJets)
    {
      if (mDoPrint) std::cout << "DEBUG Clear EMJets" << std::endl;
      mEMJetCollection->Clear();
      if (mDoPrint) std::cout << "DEBUG EMJets" << std::endl;
      for (int i = 0; i < mSUSYD3PD->jet_AntiKt4TopoEMJES_n; i++)
      {
        MTree::Jet* jet = (MTree::Jet*) mEMJetCollection->New(emjetCounter++);
        jet->Initialize((double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_pt)[i]/1000.,
                        (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_eta)[i],
                        (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_phi)[i],
                        (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_m)[i]/1000.,
                        (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_flavor_weight_IP3D)[i],
                        (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_flavor_weight_SV0)[i],
                        (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_flavor_weight_SV1)[i],
                        (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_flavor_weight_SV2)[i],
                        (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_flavor_weight)[i],
                        (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_flavor_weight_TrackCounting2D)[i],
                        (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_flavor_weight_JetProb)[i]);
        jet->SetJetQualities((double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_emfrac)[i],
                             (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_LArQuality)[i],
                             (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_Timing)[i],
                             (int)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_n90)[i],
                             0, //N90Constituents,
                             (int)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_SamplingMax)[i],
                             (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_fracSamplingMax)[i],
                             (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_hecf)[i],
                             (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_tgap3f)[i]);
        jet->SetJetQualityFlags(false, //isGood
                                false, //isBad
                                false);//isUgly
        jet->SetSamplings((double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_e_PreSamplerB)[i]/1000.,
                          (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_e_EMB1)[i]/1000.,
                          (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_e_EMB2)[i]/1000.,
                          (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_e_EMB3)[i]/1000.,
                          (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_e_PreSamplerE)[i]/1000.,
                          (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_e_EME1)[i]/1000.,
                          (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_e_EME2)[i]/1000.,
                          (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_e_EME3)[i]/1000.,
                          (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_e_HEC0)[i]/1000.,
                          (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_e_HEC1)[i]/1000.,
                          (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_e_HEC2)[i]/1000.,
                          (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_e_HEC3)[i]/1000.,
                          (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_e_TileBar0)[i]/1000.,
                          (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_e_TileBar1)[i]/1000.,
                          (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_e_TileBar2)[i]/1000.,
                          (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_e_TileGap1)[i]/1000.,
                          (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_e_TileGap2)[i]/1000.,
                          (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_e_TileGap3)[i]/1000.,
                          (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_e_TileExt0)[i]/1000.,
                          (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_e_TileExt1)[i]/1000.,
                          (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_e_TileExt2)[i]/1000.,
                          (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_e_FCAL0)[i]/1000.,
                          (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_e_FCAL1)[i]/1000.,
                          (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_e_FCAL2)[i]/1000.);
         TLorentzVector transformEM, transformCS;
         transformEM.SetPtEtaPhiM((double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_emscale_pt)[i]/1000.,
                                  (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_emscale_eta)[i],
                                  (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_emscale_phi)[i],
                                  (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_emscale_m)[i]/1000.);
         transformCS.SetPtEtaPhiM((double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_constscale_pt)[i]/1000.,
                                  (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_constscale_eta)[i],
                                  (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_constscale_phi)[i],
                                  (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_constscale_m)[i]/1000.);
         jet->SetRawEnergies((double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_emscale_E)[i]/1000.,
                             transformEM.Px(),
                             transformEM.Py(),
                             transformEM.Pz(),
                             (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_constscale_E)[i]/1000.,
                             transformCS.Px(),
                             transformCS.Py(),
                             transformCS.Pz(),
                             0., //CalE
                             0., //CalPx
                             0., //CalPy
                             0., //CalPz
                             (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_EMJES)[i],
                             (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_GCWJES)[i]);
         jet->SetJVF((double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_jvtxf)[i],
                     (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_nTrk)[i],
                     (double)(*mSUSYD3PD->jet_AntiKt4TopoEMJES_sumPtTrk)[i]);
         jet->SetBtagBDTs(0., 0., 0.);
         /*jet->SetMissingETWeights((double)(*mSUSYD3PD->jet_AntiKt4TopoEM_MET_EMJES_wpx)[i],
                                  (double)(*mSUSYD3PD->jet_AntiKt4TopoEM_MET_EMJES_wpy)[i], 
                                  (double)(*mSUSYD3PD->jet_AntiKt4TopoEM_MET_EMJES_wet)[i]);*/
         jet->SetMissingETWeights(0., 0., 0.);
      }
    }
    //
    // Fill Electrons
    //
    if (mDoPrint)   std::cout << "DEBUG Electrons" << std::endl;
    mElectronCollection->Clear();
    for (int i = 0; i < mSUSYD3PD->el_n; i++)
    {
      MTree::Electron* electron = (MTree::Electron*) mElectronCollection->New(elCounter++);
      electron->InitializeLepton((double)(*mSUSYD3PD->el_pt)[i]/1000.0,
                                 (double)(*mSUSYD3PD->el_eta)[i],
                                 (double)(*mSUSYD3PD->el_phi)[i],
                                 (double)(*mSUSYD3PD->el_m)[i]/1000.0,
                                 (double)(*mSUSYD3PD->el_author)[i],
                                 (double)(*mSUSYD3PD->el_charge)[i],
                                 11);
      electron->Initialize((double)((*mSUSYD3PD->el_E)[i]/((*mSUSYD3PD->el_pt)[i]*TMath::CosH((*mSUSYD3PD->el_eta)[i]))),
                           (double)(*mSUSYD3PD->el_Etcone20)[i]/1000.0,
                           (double)(*mSUSYD3PD->el_Etcone30)[i]/1000.0,
                           (double)(*mSUSYD3PD->el_Etcone40)[i]/1000.0,
                           (double)(*mSUSYD3PD->el_Etcone45)[i]/1000.0,
                           (unsigned long int)(*mSUSYD3PD->el_isEM)[i],
                           (double)(*mSUSYD3PD->el_E233)[i]/1000.0,
                           (double)(*mSUSYD3PD->el_E237)[i]/1000.0,
                           (double)(*mSUSYD3PD->el_E277)[i]/1000.0,
                           (double)(*mSUSYD3PD->el_Ethad)[i]/1000.0,
                           (double)(*mSUSYD3PD->el_f1)[i],
                           (double)(*mSUSYD3PD->el_f1core)[i],
                           0., //Fracs1
                           0., //Iso
                           0,  //TrackRefit
                           (bool)(*mSUSYD3PD->el_convtrackmatch)[i],
                           (bool)(*mSUSYD3PD->el_convanglematch)[i],
                           (double)(*mSUSYD3PD->el_zvertex)[i],
                           (double)(*mSUSYD3PD->el_errz)[i],
                           (double)(*mSUSYD3PD->el_electronweight)[i],
                           (double)(*mSUSYD3PD->el_electronbgweight)[i],
                           (double)(*mSUSYD3PD->el_neuralnet)[i],
                           (double)(*mSUSYD3PD->el_Hmatrix)[i],
                           (double)(*mSUSYD3PD->el_Hmatrix5)[i],
                           (double)(*mSUSYD3PD->el_isolationlikelihoodjets)[i],
                           (double)(*mSUSYD3PD->el_adaboost)[i],
                           0.,  //PhotonWeight
                           0.); //PhotonBgWeight
      electron->InitializeExtraVariables((double)(*mSUSYD3PD->el_Ethad1)[i]/1000.0,
                                         (double)(*mSUSYD3PD->el_weta2)[i],
                                         0., //E2Tsts1
                                         (double)(*mSUSYD3PD->el_Emins1)[i]/1000.0,
                                         (double)(*mSUSYD3PD->el_wstot)[i],
                                         0., //WEta1
                                         0., //deltaEta0
                                         (double)(*mSUSYD3PD->el_deltaeta1)[i],
                                         (double)(*mSUSYD3PD->el_deltaeta2)[i],
                                         0., //deltaEta3
                                         0., //deltaPhi0
                                         0., //deltaPhi1
                                         (double)(*mSUSYD3PD->el_deltaphi2)[i],
                                         0., //deltaPhi3
                                         (int)(*mSUSYD3PD->el_nConv)[i]);
      //Some tricks to get cluster mass
      double px = 0., py = 0., pz = 0.;
      px = (double)(*mSUSYD3PD->el_cl_pt)[i]/1000.0*TMath::Cos((*mSUSYD3PD->el_cl_phi)[i]);
      py = (double)(*mSUSYD3PD->el_cl_pt)[i]/1000.0*TMath::Sin((*mSUSYD3PD->el_cl_phi)[i]);
      double theta = 2*TMath::ATan(TMath::Exp(-1*(*mSUSYD3PD->el_cl_eta)[i]));
      pz = (double)(*mSUSYD3PD->el_cl_pt)[i]/1000.0/TMath::Tan(theta);
      TLorentzVector transfer;
      transfer.SetPxPyPzE(px, py, pz, (*mSUSYD3PD->el_cl_E)[i]/1000.0);
      electron->InitializeClusterVariables((double)(*mSUSYD3PD->el_cl_eta)[i],
                                           (double)(*mSUSYD3PD->el_cl_phi)[i],
                                           (double)(*mSUSYD3PD->el_cl_E)[i]/1000.0,
                                           transfer.M(),
                                           (double)(*mSUSYD3PD->el_cl_pt)[i]/1000.0,
                                           0., //ClusterEtaEMB2
                                           0., //ClusterEtaEME2
                                           0., //ClusterPhiEMB2
                                           0., //ClusterPhiEME2
                                           (double)(*mSUSYD3PD->el_etas2)[i],
                                           (double)(*mSUSYD3PD->el_phis2)[i],
                                           (double)(*mSUSYD3PD->el_Es2)[i]/1000.0);
      electron->InitializeElectronFlags((int)(*mSUSYD3PD->el_loose)[i],
                                        (int)(*mSUSYD3PD->el_medium)[i],
                                        (int)(*mSUSYD3PD->el_mediumIso)[i],
                                        (int)(*mSUSYD3PD->el_tight)[i],
                                        (int)(*mSUSYD3PD->el_tightIso)[i],
                                        (bool)(*mSUSYD3PD->el_expectHitInBLayer)[i]);
      MTree::Track *electronTrack = (MTree::Track*) mTrackCollection->New(trackCounter++);
      int charge = (*mSUSYD3PD->el_trackqoverp)[i] == 0 ? 0 : (int)((*mSUSYD3PD->el_trackqoverp)[i]/TMath::Abs((*mSUSYD3PD->el_trackqoverp)[i]));
      electronTrack->Initialize((double)(*mSUSYD3PD->el_trackpt)[i]/1000.0,
                                (double)(*mSUSYD3PD->el_tracketa)[i],
                                (double)(*mSUSYD3PD->el_trackphi)[i],
                                charge,
                                (int)(*mSUSYD3PD->el_trackfitndof)[i],
                                (double)(*mSUSYD3PD->el_trackfitchi2)[i],
                                (double)(*mSUSYD3PD->el_trackd0)[i],
                                0., //d0_err
                                (double)(*mSUSYD3PD->el_trackz0)[i],
                                0., //z0_err
                                (double)(*mSUSYD3PD->el_trackd0pv)[i],
                                (double)(*mSUSYD3PD->el_tracksigd0pv)[i],
                                (double)(*mSUSYD3PD->el_trackz0pv)[i],
                                (double)(*mSUSYD3PD->el_tracksigz0pv)[i],
                                0.,//Prim_Vtx_D0
                                0.,//Prim_Vtx_D0_Err
                                0.,//Prim_Vtx_Z0
                                0.,//Prim_Vtx_Z0_Err
                                (int)(*mSUSYD3PD->el_nBLHits)[i],
                                (int)(*mSUSYD3PD->el_nPixHits)[i],
                                (int)(*mSUSYD3PD->el_nSCTHits)[i],
                                (int)(*mSUSYD3PD->el_nTRTHits)[i],
                                (int)(*mSUSYD3PD->el_nTRTOutliers)[i],
                                (int)(*mSUSYD3PD->el_nTRTHighTHits)[i],
                                0,  //ParticleOrigin
                                0); //HitMask
      electron->SetTrack(electronTrack);
      electron->SetMissingETWeights((double)(*mSUSYD3PD->el_MET_Simplified20withTightPhotons_wpx)[i],
                                    (double)(*mSUSYD3PD->el_MET_Simplified20withTightPhotons_wpy)[i], 
                                    (double)(*mSUSYD3PD->el_MET_Simplified20withTightPhotons_wet)[i]);
  
    }
    //
    // Fill Photons
    //
    if (mDoPrint)   std::cout << "DEBUG Photons" << std::endl;
    mPhotonCollection->Clear();
    for (int i = 0; i < mSUSYD3PD->ph_n; i++)
    {
       MTree::Photon* photon = (MTree::Photon*) mPhotonCollection->New(phCounter++);
       photon->Initialize((double)(*mSUSYD3PD->ph_pt)[i]/1000.0,
                          (double)(*mSUSYD3PD->ph_eta)[i],
                          (double)(*mSUSYD3PD->ph_phi)[i],
                          (double)(*mSUSYD3PD->ph_m)[i]/1000.0,
                          (double)(*mSUSYD3PD->ph_author)[i],
                          0., //EoverP
                          (double)(*mSUSYD3PD->ph_Etcone20)[i]/1000.0,
                          (double)(*mSUSYD3PD->ph_Etcone30)[i]/1000.0,
                          (double)(*mSUSYD3PD->ph_Etcone40)[i]/1000.0,
                          (double)(*mSUSYD3PD->ph_Etcone45)[i]/1000.0,
                          (unsigned long int)(*mSUSYD3PD->ph_isEM)[i],
                          (double)(*mSUSYD3PD->ph_E233)[i]/1000.0,
                          (double)(*mSUSYD3PD->ph_E237)[i]/1000.0,
                          (double)(*mSUSYD3PD->ph_E277)[i]/1000.0,
                          (double)(*mSUSYD3PD->ph_Ethad)[i]/1000.0,
                          (double)(*mSUSYD3PD->ph_f1)[i],
                          (double)(*mSUSYD3PD->ph_f1core)[i],
                          0., //Fracs1
                          0., //Iso
                          0,  //TrackRefit
                          (bool)(*mSUSYD3PD->ph_convtrackmatch)[i],
                          (bool)(*mSUSYD3PD->ph_convanglematch)[i],
                          (double)(*mSUSYD3PD->ph_zvertex)[i],
                          (double)(*mSUSYD3PD->ph_errz)[i],
                          0., //ElectronWeight,
                          0., //BgWeight
                          (double)(*mSUSYD3PD->ph_neuralnet)[i],
                          (double)(*mSUSYD3PD->ph_Hmatrix)[i],
                          (double)(*mSUSYD3PD->ph_Hmatrix5)[i],
                          (double)(*mSUSYD3PD->ph_isolationlikelihoodjets)[i],
                          (double)(*mSUSYD3PD->ph_adaboost)[i],
                          (double)(*mSUSYD3PD->ph_photonweight)[i],
                          (double)(*mSUSYD3PD->ph_photonbgweight)[i]);
      //Some tricks to get cluster mass
      double px = 0., py = 0., pz = 0.;
      px = (double)(*mSUSYD3PD->ph_cl_pt)[i]/1000.0*TMath::Cos((*mSUSYD3PD->ph_cl_phi)[i]);
      py = (double)(*mSUSYD3PD->ph_cl_pt)[i]/1000.0*TMath::Sin((*mSUSYD3PD->ph_cl_phi)[i]);
      double theta = 2*TMath::ATan(TMath::Exp(-1*(*mSUSYD3PD->ph_cl_eta)[i]));
      pz = (double)(*mSUSYD3PD->ph_cl_pt)[i]/1000.0/TMath::Tan(theta);
      TLorentzVector transfer;
      transfer.SetPxPyPzE(px, py, pz, (*mSUSYD3PD->ph_cl_E)[i]/1000.0);
      photon->InitializeClusterVariables((double)(*mSUSYD3PD->ph_cl_eta)[i],
                                         (double)(*mSUSYD3PD->ph_cl_phi)[i],
                                         (double)(*mSUSYD3PD->ph_cl_E)[i]/1000.,
                                         transfer.M(),
                                         (double)(*mSUSYD3PD->ph_cl_pt)[i]/1000.,
                                         0., //ClusterEtaEMB2
                                         0., //ClusterEtaEME2
                                         0., //ClusterPhiEMB2
                                         0., //ClusterPhiEME2
                                         (double)(*mSUSYD3PD->ph_etas2)[i],
                                         (double)(*mSUSYD3PD->ph_phis2)[i],
                                         (double)(*mSUSYD3PD->ph_Es2)[i]/1000.0);
     photon->InitializeExtraClusterVariables((double)(*mSUSYD3PD->ph_Ethad1)[i]/1000.0,
                                             (double)(*mSUSYD3PD->ph_weta2)[i],
                                             (double)(*mSUSYD3PD->ph_emaxs1)[i]/1000.0,
                                             (double)(*mSUSYD3PD->ph_Emax2)[i]/1000.0,
                                             (double)(*mSUSYD3PD->ph_Emax2)[i]/1000.0 -
                                             (double)(*mSUSYD3PD->ph_deltaEs)[i]/1000.0,
                                             (double)(*mSUSYD3PD->ph_fside)[i],
                                             (double)(*mSUSYD3PD->ph_wstot)[i],
                                             (double)(*mSUSYD3PD->ph_ws3)[i]);
      photon->InitializeConversionVariables((int)(*mSUSYD3PD->ph_convFlag)[i], 
                                            (int)(*mSUSYD3PD->ph_isConv)[i],
                                            (int)(*mSUSYD3PD->ph_nConv)[i],
                                            (int)(*mSUSYD3PD->ph_nSingleTrackConv)[i],
                                            (int)(*mSUSYD3PD->ph_nDoubleTrackConv)[i]);
             
      photon->InitializeCorrectedIsolation((double)(*mSUSYD3PD->ph_Etcone40_pt_corrected)[i]/1000.0,
                                           (double)(*mSUSYD3PD->ph_Etcone40_ED_corrected)[i]/1000.0,
                                           (double)(*mSUSYD3PD->ph_Etcone40_corrected)[i]/1000.0);
                                         
      photon->InitializePhotonFlags((bool)(*mSUSYD3PD->ph_loose)[i],
                                    0, //LooseAR
                                    (bool)(*mSUSYD3PD->ph_tight)[i],
                                    (bool)(*mSUSYD3PD->ph_tightIso)[i],
                                    0, //TightAR
                                    0); //TightARIso
                          
      photon->SetMissingETWeights((double)(*mSUSYD3PD->ph_MET_Simplified20withTightPhotons_wpx)[i],
                                  (double)(*mSUSYD3PD->ph_MET_Simplified20withTightPhotons_wpy)[i], 
                                  (double)(*mSUSYD3PD->ph_MET_Simplified20withTightPhotons_wet)[i]);
    }
    //
    // Fill Muons
    //
    //STACO loop
     if (mDoPrint)   std::cout << "DEBUG STACO Muons" << std::endl;
    mMuonCollection->Clear();
    for (int i = 0; i < mSUSYD3PD->mu_staco_n; i++)
    {
      MTree::Muon* muon = (MTree::Muon*) mMuonCollection->New(muCounter++);
      muon->InitializeLepton((double)(*mSUSYD3PD->mu_staco_pt)[i]/1000.,
                             (double)(*mSUSYD3PD->mu_staco_eta)[i],
                             (double)(*mSUSYD3PD->mu_staco_phi)[i],
                             (double)(*mSUSYD3PD->mu_staco_m)[i]/1000.,
                             (int)(*mSUSYD3PD->mu_staco_author)[i],
                             (int)(*mSUSYD3PD->mu_staco_charge)[i],
                             13);
      muon->Initialize(0., //ETcone10
                       (double)(*mSUSYD3PD->mu_staco_etcone20)[i]/1000.,
                       (double)(*mSUSYD3PD->mu_staco_etcone30)[i]/1000.,
                       (double)(*mSUSYD3PD->mu_staco_etcone40)[i]/1000.,
                       0., //PTcone10
                       (double)(*mSUSYD3PD->mu_staco_ptcone20)[i]/1000.,
                       (double)(*mSUSYD3PD->mu_staco_ptcone30)[i]/1000.,
                       (double)(*mSUSYD3PD->mu_staco_ptcone40)[i]/1000.,
                       (double)(*mSUSYD3PD->mu_staco_trackfitchi2)[i],
                       (double)(*mSUSYD3PD->mu_staco_trackfitndof)[i],
                       (double)(*mSUSYD3PD->mu_staco_matchchi2)[i],
                       (double)(*mSUSYD3PD->mu_staco_matchndof)[i],
                       (double)(*mSUSYD3PD->mu_staco_energyLossPar)[i]/1000.,
                       (bool)(*mSUSYD3PD->mu_staco_bestMatch)[i],
                       (bool)(*mSUSYD3PD->mu_staco_isCombinedMuon)[i],
                       (bool)(*mSUSYD3PD->mu_staco_isStandAloneMuon)[i],
                       (bool)(*mSUSYD3PD->mu_staco_isLowPtReconstructedMuon)[i],
                       0, //isCaloMuon
                       (int)(*mSUSYD3PD->mu_staco_nMDTHits)[i],
                       (int)(*mSUSYD3PD->mu_staco_nCSCEtaHits)[i],
                       (int)(*mSUSYD3PD->mu_staco_nCSCPhiHits)[i],
                       (int)(*mSUSYD3PD->mu_staco_nRPCEtaHits)[i],
                       (int)(*mSUSYD3PD->mu_staco_nRPCPhiHits)[i],
                       (int)(*mSUSYD3PD->mu_staco_nTGCEtaHits)[i],
                       (int)(*mSUSYD3PD->mu_staco_nTGCPhiHits)[i],
                       0); //NrOfSegments
      muon->InitializeMuonQuality((bool)(*mSUSYD3PD->mu_staco_loose)[i],
                                  (bool)(*mSUSYD3PD->mu_staco_medium)[i],
                                  (bool)(*mSUSYD3PD->mu_staco_tight)[i],
                                  0.); //IsMuonLikelihood
      MTree::Track *muonIDTrk = (MTree::Track*) mTrackCollection->New(trackCounter++);
      MTree::Track *muonMETrk = (MTree::Track*) mTrackCollection->New(trackCounter++);
      int charge = (*mSUSYD3PD->mu_staco_id_qoverp)[i] == 0 ? 0 : (int)((*mSUSYD3PD->mu_staco_id_qoverp)[i]/TMath::Abs((*mSUSYD3PD->mu_staco_id_qoverp)[i]));
      muonIDTrk->Initialize(TMath::Abs(TMath::Sin((*mSUSYD3PD->mu_staco_id_theta)[i])/((*mSUSYD3PD->mu_staco_id_qoverp)[i])) / 1000.,
                            -1*TMath::Log(((*mSUSYD3PD->mu_staco_id_theta)[i])/2),
                            (double)(*mSUSYD3PD->mu_staco_id_phi)[i],
                            charge,
                            0, //NDOF
                            0, //Chi2
                            (double)(*mSUSYD3PD->mu_staco_id_d0)[i],
                            0., //d0_err
                            (double)(*mSUSYD3PD->mu_staco_id_z0)[i],
                            0., //z0_err
                            (double)(*mSUSYD3PD->mu_staco_d0_exPV)[i],
                            TMath::Sqrt((double)(*mSUSYD3PD->mu_staco_cov_d0_exPV)[i]),
                            (double)(*mSUSYD3PD->mu_staco_z0_exPV)[i],
                            TMath::Sqrt((double)(*mSUSYD3PD->mu_staco_cov_z0_exPV)[i]),
                            0.,//Prim_Vtx_D0
                            0.,//Prim_Vtx_D0_Err
                            0.,//Prim_Vtx_Z0
                            0.,//Prim_Vtx_Z0_Err
                            (int)(*mSUSYD3PD->mu_staco_nBLHits)[i],
                            (int)(*mSUSYD3PD->mu_staco_nPixHits)[i],
                            (int)(*mSUSYD3PD->mu_staco_nSCTHits)[i],
                            (int)(*mSUSYD3PD->mu_staco_nTRTHits)[i],
                            (int)(*mSUSYD3PD->mu_staco_nTRTOutliers)[i],
                            (int)(*mSUSYD3PD->mu_staco_nTRTHighTHits)[i],
                            0, //ParticleOrigin
                            0);//HitMask
      muonIDTrk->SetTrackAuthor(0);
      charge = (*mSUSYD3PD->mu_staco_me_qoverp)[i] == 0 ? 0 : (int)((*mSUSYD3PD->mu_staco_me_qoverp)[i]/TMath::Abs((*mSUSYD3PD->mu_staco_me_qoverp)[i]));
      muonMETrk->Initialize(TMath::Abs(TMath::Sin((*mSUSYD3PD->mu_staco_me_theta)[i])/((*mSUSYD3PD->mu_staco_me_qoverp)[i])) / 1000.,
                            -1*TMath::Log(((*mSUSYD3PD->mu_staco_me_theta)[i])/2),
                            (double)(*mSUSYD3PD->mu_staco_me_phi)[i],
                            charge,
                            0, //NDOF
                            0, //Chi2
                            (double)(*mSUSYD3PD->mu_staco_me_d0)[i],
                            0., //d0_err
                            (double)(*mSUSYD3PD->mu_staco_me_z0)[i],
                            0., //z0_err
                            (double)(*mSUSYD3PD->mu_staco_me_d0_exPV)[i],
                            0., //d0_err_vtx
                            (double)(*mSUSYD3PD->mu_staco_me_z0_exPV)[i],
                            0., //z0_err_vtx
                            0.,//Prim_Vtx_D0
                            0.,//Prim_Vtx_D0_Err
                            0.,//Prim_Vtx_Z0
                            0.,//Prim_Vtx_Z0_Err
                            (int)(*mSUSYD3PD->mu_staco_nBLHits)[i],
                            (int)(*mSUSYD3PD->mu_staco_nPixHits)[i],
                            (int)(*mSUSYD3PD->mu_staco_nSCTHits)[i],
                            (int)(*mSUSYD3PD->mu_staco_nTRTHits)[i],
                            (int)(*mSUSYD3PD->mu_staco_nTRTOutliers)[i],
                            (int)(*mSUSYD3PD->mu_staco_nTRTHighTHits)[i],
                            0, //ParticleOrigin
                            0);//HitMask
      muonMETrk->SetTrackAuthor(1);
      muon->SetTrack(muonIDTrk);
      muon->SetStandAloneTrack(muonMETrk);
      muon->SetMissingETWeights((double)(*mSUSYD3PD->mu_staco_MET_Simplified20withTightPhotons_wpx)[i],
                                (double)(*mSUSYD3PD->mu_staco_MET_Simplified20withTightPhotons_wpy)[i], 
                                (double)(*mSUSYD3PD->mu_staco_MET_Simplified20withTightPhotons_wet)[i]);
    }
    //MUID loop
    if (mDoPrint)   std::cout << "DEBUG MUID Muons" << std::endl;
    for (int i = 0; i < mSUSYD3PD->mu_muid_n; i++)
    {
      MTree::Muon* muon = (MTree::Muon*) mMuonCollection->New(muCounter++);
      muon->InitializeLepton((double)(*mSUSYD3PD->mu_muid_pt)[i]/1000.,
                             (double)(*mSUSYD3PD->mu_muid_eta)[i],
                             (double)(*mSUSYD3PD->mu_muid_phi)[i],
                             (double)(*mSUSYD3PD->mu_muid_m)[i]/1000.,
                             (int)(*mSUSYD3PD->mu_muid_author)[i],
                             (int)(*mSUSYD3PD->mu_muid_charge)[i],
                             13);
      muon->Initialize(0., //ETcone10
                       (double)(*mSUSYD3PD->mu_muid_etcone20)[i]/1000.,
                       (double)(*mSUSYD3PD->mu_muid_etcone30)[i]/1000.,
                       (double)(*mSUSYD3PD->mu_muid_etcone40)[i]/1000.,
                       0., //PTcone10
                       (double)(*mSUSYD3PD->mu_muid_ptcone20)[i]/1000.,
                       (double)(*mSUSYD3PD->mu_muid_ptcone30)[i]/1000.,
                       (double)(*mSUSYD3PD->mu_muid_ptcone40)[i]/1000.,
                       (double)(*mSUSYD3PD->mu_muid_trackfitchi2)[i],
                       (double)(*mSUSYD3PD->mu_muid_trackfitndof)[i],
                       (double)(*mSUSYD3PD->mu_muid_matchchi2)[i],
                       (double)(*mSUSYD3PD->mu_muid_matchndof)[i],
                       (double)(*mSUSYD3PD->mu_muid_energyLossPar)[i]/1000.,
                       (bool)(*mSUSYD3PD->mu_muid_bestMatch)[i],
                       (bool)(*mSUSYD3PD->mu_muid_isCombinedMuon)[i],
                       (bool)(*mSUSYD3PD->mu_muid_isStandAloneMuon)[i],
                       (bool)(*mSUSYD3PD->mu_muid_isLowPtReconstructedMuon)[i],
                       0, //isCaloMuon
                       (int)(*mSUSYD3PD->mu_muid_nMDTHits)[i],
                       (int)(*mSUSYD3PD->mu_muid_nCSCEtaHits)[i],
                       (int)(*mSUSYD3PD->mu_muid_nCSCPhiHits)[i],
                       (int)(*mSUSYD3PD->mu_muid_nRPCEtaHits)[i],
                       (int)(*mSUSYD3PD->mu_muid_nRPCPhiHits)[i],
                       (int)(*mSUSYD3PD->mu_muid_nTGCEtaHits)[i],
                       (int)(*mSUSYD3PD->mu_muid_nTGCPhiHits)[i],
                       0); //NrOfSegments
      muon->InitializeMuonQuality((bool)(*mSUSYD3PD->mu_muid_loose)[i],
                                  (bool)(*mSUSYD3PD->mu_muid_medium)[i],
                                  (bool)(*mSUSYD3PD->mu_muid_tight)[i],
                                  0.); //IsMuonLikelihood
      MTree::Track *muonIDTrk = (MTree::Track*) mTrackCollection->New(trackCounter++);
      MTree::Track *muonMETrk = (MTree::Track*) mTrackCollection->New(trackCounter++);
      int charge = (*mSUSYD3PD->mu_muid_id_qoverp)[i] == 0 ? 0 : (int)((*mSUSYD3PD->mu_muid_id_qoverp)[i]/TMath::Abs((*mSUSYD3PD->mu_muid_id_qoverp)[i]));
      muonIDTrk->Initialize(TMath::Abs(TMath::Sin((*mSUSYD3PD->mu_muid_id_theta)[i])/((*mSUSYD3PD->mu_muid_id_qoverp)[i])) / 1000.,
                            -1*TMath::Log(((*mSUSYD3PD->mu_muid_id_theta)[i])/2),
                            (double)(*mSUSYD3PD->mu_muid_id_phi)[i],
                            charge,
                            0, //NDOF
                            0, //Chi2
                            (double)(*mSUSYD3PD->mu_muid_id_d0)[i],
                            0., //d0_err
                            (double)(*mSUSYD3PD->mu_muid_id_z0)[i],
                            0., //z0_err
                            (double)(*mSUSYD3PD->mu_muid_d0_exPV)[i],
                            TMath::Sqrt((double)(*mSUSYD3PD->mu_muid_cov_d0_exPV)[i]),
                            (double)(*mSUSYD3PD->mu_muid_z0_exPV)[i],
                            TMath::Sqrt((double)(*mSUSYD3PD->mu_muid_cov_z0_exPV)[i]),
                            0.,//Prim_Vtx_D0
                            0.,//Prim_Vtx_D0_Err
                            0.,//Prim_Vtx_Z0
                            0.,//Prim_Vtx_Z0_Err
                            (int)(*mSUSYD3PD->mu_muid_nBLHits)[i],
                            (int)(*mSUSYD3PD->mu_muid_nPixHits)[i],
                            (int)(*mSUSYD3PD->mu_muid_nSCTHits)[i],
                            (int)(*mSUSYD3PD->mu_muid_nTRTHits)[i],
                            (int)(*mSUSYD3PD->mu_muid_nTRTOutliers)[i],
                            (int)(*mSUSYD3PD->mu_muid_nTRTHighTHits)[i],
                            0, //ParticleOrigin
                            0);//HitMask
      muonIDTrk->SetTrackAuthor(0);
      charge = (*mSUSYD3PD->mu_muid_me_qoverp)[i] == 0 ? 0 : (int)((*mSUSYD3PD->mu_muid_me_qoverp)[i]/TMath::Abs((*mSUSYD3PD->mu_muid_me_qoverp)[i]));
      muonMETrk->Initialize(TMath::Abs(TMath::Sin((*mSUSYD3PD->mu_muid_me_theta)[i])/((*mSUSYD3PD->mu_muid_me_qoverp)[i])) / 1000.,
                            -1*TMath::Log(((*mSUSYD3PD->mu_muid_me_theta)[i])/2),
                            (double)(*mSUSYD3PD->mu_muid_me_phi)[i],
                            charge,
                            0, //NDOF
                            0, //Chi2
                            (double)(*mSUSYD3PD->mu_muid_me_d0)[i],
                            0., //d0_err
                            (double)(*mSUSYD3PD->mu_muid_me_z0)[i],
                            0., //z0_err
                            (double)(*mSUSYD3PD->mu_muid_me_d0_exPV)[i],
                            0., //d0_err_vtx
                            (double)(*mSUSYD3PD->mu_muid_me_z0_exPV)[i],
                            0., //z0_err_vtx
                            0.,//Prim_Vtx_D0
                            0.,//Prim_Vtx_D0_Err
                            0.,//Prim_Vtx_Z0
                            0.,//Prim_Vtx_Z0_Err
                            (int)(*mSUSYD3PD->mu_muid_nBLHits)[i],
                            (int)(*mSUSYD3PD->mu_muid_nPixHits)[i],
                            (int)(*mSUSYD3PD->mu_muid_nSCTHits)[i],
                            (int)(*mSUSYD3PD->mu_muid_nTRTHits)[i],
                            (int)(*mSUSYD3PD->mu_muid_nTRTOutliers)[i],
                            (int)(*mSUSYD3PD->mu_muid_nTRTHighTHits)[i],
                            0, //ParticleOrigin
                            0);//HitMask
      muonMETrk->SetTrackAuthor(2);
      muon->SetTrack(muonIDTrk);
      muon->SetStandAloneTrack(muonMETrk);
      muon->SetMissingETWeights((double)(*mSUSYD3PD->mu_muid_MET_Simplified20withTightPhotons_wpx)[i],
                                (double)(*mSUSYD3PD->mu_muid_MET_Simplified20withTightPhotons_wpy)[i], 
                                (double)(*mSUSYD3PD->mu_muid_MET_Simplified20withTightPhotons_wet)[i]);
    }
    
    //CaloMuon loop
    if (mDoPrint)   std::cout << "DEBUG Calo Muons" << std::endl;
    mCaloMuonCollection->Clear();
    for (int i = 0; i < mSUSYD3PD->mu_calo_n; i++)
    {
      MTree::Muon* muon = (MTree::Muon*) mCaloMuonCollection->New(caloMuCounter++);
      muon->InitializeLepton((double)(*mSUSYD3PD->mu_calo_pt)[i]/1000.,
                             (double)(*mSUSYD3PD->mu_calo_eta)[i],
                             (double)(*mSUSYD3PD->mu_calo_phi)[i],
                             (double)(*mSUSYD3PD->mu_calo_m)[i]/1000.,
                             (int)(*mSUSYD3PD->mu_calo_author)[i],
                             (int)(*mSUSYD3PD->mu_calo_charge)[i],
                             13);
      muon->Initialize(0., //ETcone10
                       (double)(*mSUSYD3PD->mu_calo_etcone20)[i]/1000.,
                       (double)(*mSUSYD3PD->mu_calo_etcone30)[i]/1000.,
                       (double)(*mSUSYD3PD->mu_calo_etcone40)[i]/1000.,
                       0., //PTcone10
                       (double)(*mSUSYD3PD->mu_calo_ptcone20)[i]/1000.,
                       (double)(*mSUSYD3PD->mu_calo_ptcone30)[i]/1000.,
                       (double)(*mSUSYD3PD->mu_calo_ptcone40)[i]/1000.,
                       (double)(*mSUSYD3PD->mu_calo_trackfitchi2)[i],
                       (double)(*mSUSYD3PD->mu_calo_trackfitndof)[i],
                       (double)(*mSUSYD3PD->mu_calo_matchchi2)[i],
                       (double)(*mSUSYD3PD->mu_calo_matchndof)[i],
                       (double)(*mSUSYD3PD->mu_calo_energyLossPar)[i]/1000.,
                       (bool)(*mSUSYD3PD->mu_calo_bestMatch)[i],
                       (bool)(*mSUSYD3PD->mu_calo_isCombinedMuon)[i],
                       (bool)(*mSUSYD3PD->mu_calo_isStandAloneMuon)[i],
                       (bool)(*mSUSYD3PD->mu_calo_isLowPtReconstructedMuon)[i],
                       0, //isCaloMuon
                       (int)(*mSUSYD3PD->mu_calo_nMDTHits)[i],
                       (int)(*mSUSYD3PD->mu_calo_nCSCEtaHits)[i],
                       (int)(*mSUSYD3PD->mu_calo_nCSCPhiHits)[i],
                       (int)(*mSUSYD3PD->mu_calo_nRPCEtaHits)[i],
                       (int)(*mSUSYD3PD->mu_calo_nRPCPhiHits)[i],
                       (int)(*mSUSYD3PD->mu_calo_nTGCEtaHits)[i],
                       (int)(*mSUSYD3PD->mu_calo_nTGCPhiHits)[i],
                       0); //NrOfSegments
      muon->InitializeMuonQuality((bool)(*mSUSYD3PD->mu_calo_loose)[i],
                                  (bool)(*mSUSYD3PD->mu_calo_medium)[i],
                                  (bool)(*mSUSYD3PD->mu_calo_tight)[i],
                                  0.); //IsMuonLikelihood
      MTree::Track *muonIDTrk = (MTree::Track*) mTrackCollection->New(trackCounter++);
      MTree::Track *muonMETrk = (MTree::Track*) mTrackCollection->New(trackCounter++);
      int charge = (*mSUSYD3PD->mu_calo_id_qoverp)[i] == 0 ? 0 : (int)((*mSUSYD3PD->mu_calo_id_qoverp)[i]/TMath::Abs((*mSUSYD3PD->mu_calo_id_qoverp)[i]));
      muonIDTrk->Initialize(TMath::Abs(TMath::Sin((*mSUSYD3PD->mu_calo_id_theta)[i])/((*mSUSYD3PD->mu_calo_id_qoverp)[i])) / 1000.,
                            -1*TMath::Log(((*mSUSYD3PD->mu_calo_id_theta)[i])/2),
                            (double)(*mSUSYD3PD->mu_calo_id_phi)[i],
                            charge,
                            0, //NDOF
                            0, //Chi2
                            (double)(*mSUSYD3PD->mu_calo_id_d0)[i],
                            0., //d0_err
                            (double)(*mSUSYD3PD->mu_calo_id_z0)[i],
                            0., //z0_err
                            (double)(*mSUSYD3PD->mu_calo_d0_exPV)[i],
                            TMath::Sqrt((double)(*mSUSYD3PD->mu_calo_cov_d0_exPV)[i]),
                            (double)(*mSUSYD3PD->mu_calo_z0_exPV)[i],
                            TMath::Sqrt((double)(*mSUSYD3PD->mu_calo_cov_z0_exPV)[i]),
                            0.,//Prim_Vtx_D0
                            0.,//Prim_Vtx_D0_Err
                            0.,//Prim_Vtx_Z0
                            0.,//Prim_Vtx_Z0_Err
                            (int)(*mSUSYD3PD->mu_calo_nBLHits)[i],
                            (int)(*mSUSYD3PD->mu_calo_nPixHits)[i],
                            (int)(*mSUSYD3PD->mu_calo_nSCTHits)[i],
                            (int)(*mSUSYD3PD->mu_calo_nTRTHits)[i],
                            (int)(*mSUSYD3PD->mu_calo_nTRTOutliers)[i],
                            (int)(*mSUSYD3PD->mu_calo_nTRTHighTHits)[i],
                            0, //ParticleOrigin
                            0);//HitMask
      muonIDTrk->SetTrackAuthor(0);
      charge = (*mSUSYD3PD->mu_calo_me_qoverp)[i] == 0 ? 0 : (int)((*mSUSYD3PD->mu_calo_me_qoverp)[i]/TMath::Abs((*mSUSYD3PD->mu_calo_me_qoverp)[i]));
      muonMETrk->Initialize(TMath::Abs(TMath::Sin((*mSUSYD3PD->mu_calo_me_theta)[i])/((*mSUSYD3PD->mu_calo_me_qoverp)[i])) / 1000.,
                            -1*TMath::Log(((*mSUSYD3PD->mu_calo_me_theta)[i])/2),
                            (double)(*mSUSYD3PD->mu_calo_me_phi)[i],
                            charge,
                            0, //NDOF
                            0, //Chi2
                            (double)(*mSUSYD3PD->mu_calo_me_d0)[i],
                            0., //d0_err
                            (double)(*mSUSYD3PD->mu_calo_me_z0)[i],
                            0., //z0_err
                            (double)(*mSUSYD3PD->mu_calo_me_d0_exPV)[i],
                            0., //d0_err_vtx
                            (double)(*mSUSYD3PD->mu_calo_me_z0_exPV)[i],
                            0., //z0_err_vtx
                            0.,//Prim_Vtx_D0
                            0.,//Prim_Vtx_D0_Err
                            0.,//Prim_Vtx_Z0
                            0.,//Prim_Vtx_Z0_Err
                            (int)(*mSUSYD3PD->mu_calo_nBLHits)[i],
                            (int)(*mSUSYD3PD->mu_calo_nPixHits)[i],
                            (int)(*mSUSYD3PD->mu_calo_nSCTHits)[i],
                            (int)(*mSUSYD3PD->mu_calo_nTRTHits)[i],
                            (int)(*mSUSYD3PD->mu_calo_nTRTOutliers)[i],
                            (int)(*mSUSYD3PD->mu_calo_nTRTHighTHits)[i],
                            0, //ParticleOrigin
                            0);//HitMask
      muonMETrk->SetTrackAuthor(1);
      muon->SetTrack(muonIDTrk);
      muon->SetStandAloneTrack(muonMETrk);
    }
  
    
    // 
    //Fill Global Event Info
    // 
     if (mDoPrint)   std::cout << "DEBUG Global" << std::endl;
    mEventInfo->ClearVariables();
    mEventInfo->Initialize((int)(mSUSYD3PD->RunNumber),
                           (int)(mSUSYD3PD->EventNumber),
                           (int)(mSUSYD3PD->bcid),
                           (int)(mSUSYD3PD->lbn),
                           1.); //EventWeight
    //
    //Fill Trigger Information
    //
    mTrigger->ClearVariables();
    if (mDoPrint) std::cout << "DEBUG Trigger" << std::endl;
    mEFTriggerD3PDItems.clear();
    mL2TriggerD3PDItems.clear();
    mL1TriggerD3PDItems.clear();
    std::vector< std::string > passedEFNames;
    std::vector< std::string > passedL2Names;
    std::vector< std::string > passedL1Names;
    std::vector< std::string > ignoreNames;
    passedEFNames.clear();
    passedL1Names.clear();
    passedL2Names.clear();
    ignoreNames.clear();
    TObjArray* leaves = mSUSYD3PD->fChain->GetListOfLeaves();
    for (int i = 0; i < leaves->GetSize(); i++)
    {
      TLeafObject* leaf = (TLeafObject*)(TLeaf*)leaves->UncheckedAt(i);
      std::string name = std::string(leaf->GetName());
      std::string type = std::string(leaf->GetTypeName());
      if (type != "Bool_t") continue;
      if (name.substr(0, 3) == std::string("EF_")) 
      {
        if ((bool)leaf->GetValue()) passedEFNames.push_back(name);
      }
      if (name.substr(0, 3) == std::string("L1_")) 
      {
        if ((bool)leaf->GetValue()) passedL1Names.push_back(name);
      }
      if (name.substr(0, 3) == std::string("L2_"))
      {
        if ((bool)leaf->GetValue()) passedL2Names.push_back(name);
      }
    }
    mTrigger->SetEFTriggersPassed(passedEFNames, ignoreNames);
    mTrigger->SetL2TriggersPassed(passedL2Names, ignoreNames);
    mTrigger->SetL1TriggersPassed(passedL1Names, ignoreNames);
    //
    // Fill the MissingET
    // 
    if (mDoPrint)   std::cout << "DEBUG MissingET" << std::endl;
    mMissingET->ClearVariables();
    //Default MissingET algorithm 
    mMissingET->Initialize((double)(mSUSYD3PD->MET_Simplified20_RefFinal_et)/1000.0,
                           (double)(mSUSYD3PD->MET_Simplified20_RefFinal_etx)/1000.0,
                           (double)(mSUSYD3PD->MET_Simplified20_RefFinal_ety)/1000.0,
                           (double)(mSUSYD3PD->MET_Simplified20_RefFinal_sumet)/1000.0,
                           "MET_Simplified20_RefFinal");
    
    
    
    mMissingETCollection->Clear();
    MTree::MissingET* term = new MTree::MissingET();
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_Simplified20_RefFinal_et)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20_RefFinal_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20_RefFinal_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20_RefFinal_sumet)/1000.0,
                     "MET_Simplified20_RefFinal");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_Simplified20_CellOut_et)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20_CellOut_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20_CellOut_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20_CellOut_sumet)/1000.0,
                     "MET_Simplified20_CellOut");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_Simplified20_RefGamma_et)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20_RefGamma_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20_RefGamma_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20_RefGamma_sumet)/1000.0,
                     "MET_Simplified20_RefGamma");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_Simplified20_RefEle_et)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20_RefEle_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20_RefEle_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20_RefEle_sumet)/1000.0,
                     "MET_Simplified20_RefEle");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_Simplified20_RefJet_et)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20_RefJet_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20_RefJet_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20_RefJet_sumet)/1000.0,
                     "MET_Simplified20_RefJet");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_Simplified20_Muon_Isol_Staco_et)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20_Muon_Isol_Staco_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20_Muon_Isol_Staco_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20_Muon_Isol_Staco_sumet)/1000.0,
                     "MET_Simplified20_Muon_Isol_Staco");
    
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_Simplified20_Muon_NonIsol_Staco_et)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20_Muon_NonIsol_Staco_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20_Muon_NonIsol_Staco_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20_Muon_NonIsol_Staco_sumet)/1000.0,
                     "MET_Simplified20_Muon_NonIsol_Staco");
    
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_Simplified20_Muon_Total_Staco_et)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20_Muon_Total_Staco_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20_Muon_Total_Staco_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20_Muon_Total_Staco_sumet)/1000.0,
                     "MET_Simplified20_Muon_Total_Staco");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_Simplified20withTightPhotons_RefFinal_et)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20withTightPhotons_RefFinal_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20withTightPhotons_RefFinal_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20withTightPhotons_RefFinal_sumet)/1000.0,
                     "MET_Simplified20withTightPhotons_RefFinal");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_Simplified20withTightPhotons_CellOut_et)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20withTightPhotons_CellOut_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20withTightPhotons_CellOut_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20withTightPhotons_CellOut_sumet)/1000.0,
                     "MET_Simplified20withTightPhotons_CellOut");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_Simplified20withTightPhotons_RefGamma_et)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20withTightPhotons_RefGamma_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20withTightPhotons_RefGamma_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20withTightPhotons_RefGamma_sumet)/1000.0,
                     "MET_Simplified20withTightPhotons_RefGamma");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_Simplified20withTightPhotons_RefEle_et)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20withTightPhotons_RefEle_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20withTightPhotons_RefEle_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20withTightPhotons_RefEle_sumet)/1000.0,
                     "MET_Simplified20withTightPhotons_RefEle");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_Simplified20withTightPhotons_RefJet_et)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20withTightPhotons_RefJet_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20withTightPhotons_RefJet_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20withTightPhotons_RefJet_sumet)/1000.0,
                     "MET_Simplified20withTightPhotons_RefJet");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_Simplified20withTightPhotons_Muon_Isol_Staco_et)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20withTightPhotons_Muon_Isol_Staco_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20withTightPhotons_Muon_Isol_Staco_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20withTightPhotons_Muon_Isol_Staco_sumet)/1000.0,
                     "MET_Simplified20withTightPhotons_Muon_Isol_Staco");
    
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_et)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_sumet)/1000.0,
                     "MET_Simplified20withTightPhotons_Muon_NonIsol_Staco");
    
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_Simplified20withTightPhotons_Muon_Total_Staco_et)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20withTightPhotons_Muon_Total_Staco_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20withTightPhotons_Muon_Total_Staco_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified20withTightPhotons_Muon_Total_Staco_sumet)/1000.0,
                     "MET_Simplified20withTightPhotons_Muon_Total_Staco");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_Simplified30_RefFinal_et)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified30_RefFinal_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified30_RefFinal_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified30_RefFinal_sumet)/1000.0,
                     "MET_Simplified30_RefFinal");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_Simplified30_CellOut_et)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified30_CellOut_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified30_CellOut_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified30_CellOut_sumet)/1000.0,
                     "MET_Simplified30_CellOut");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_Simplified30_RefGamma_et)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified30_RefGamma_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified30_RefGamma_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified30_RefGamma_sumet)/1000.0,
                     "MET_Simplified30_RefGamma");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_Simplified30_RefEle_et)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified30_RefEle_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified30_RefEle_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified30_RefEle_sumet)/1000.0,
                     "MET_Simplified30_RefEle");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_Simplified30_RefJet_et)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified30_RefJet_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified30_RefJet_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified30_RefJet_sumet)/1000.0,
                     "MET_Simplified30_RefJet");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_Simplified30_Muon_Isol_Staco_et)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified30_Muon_Isol_Staco_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified30_Muon_Isol_Staco_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified30_Muon_Isol_Staco_sumet)/1000.0,
                     "MET_Simplified30_Muon_Isol_Staco");
    
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_Simplified30_Muon_NonIsol_Staco_et)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified30_Muon_NonIsol_Staco_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified30_Muon_NonIsol_Staco_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified30_Muon_NonIsol_Staco_sumet)/1000.0,
                     "MET_Simplified30_Muon_NonIsol_Staco");
    
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_Simplified30_Muon_Total_Staco_et)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified30_Muon_Total_Staco_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified30_Muon_Total_Staco_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_Simplified30_Muon_Total_Staco_sumet)/1000.0,
                     "MET_Simplified30_Muon_Total_Staco");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_RefFinal_em_et)/1000.0,
                     (double)(mSUSYD3PD->MET_RefFinal_em_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_RefFinal_em_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_RefFinal_em_sumet)/1000.0,
                     "MET_RefFinal_em");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_RefEle_em_et)/1000.0,
                     (double)(mSUSYD3PD->MET_RefEle_em_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_RefEle_em_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_RefEle_em_sumet)/1000.0,
                     "MET_RefEle_em");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_RefJet_em_et)/1000.0,
                     (double)(mSUSYD3PD->MET_RefJet_em_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_RefJet_em_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_RefJet_em_sumet)/1000.0,
                     "MET_RefJet_em");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_RefMuon_em_et)/1000.0,
                     (double)(mSUSYD3PD->MET_RefMuon_em_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_RefMuon_em_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_RefMuon_em_sumet)/1000.0,
                     "MET_RefMuon_em");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_RefMuon_Track_em_et)/1000.0,
                     (double)(mSUSYD3PD->MET_RefMuon_Track_em_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_RefMuon_Track_em_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_RefMuon_Track_em_sumet)/1000.0,
                     "MET_RefMuon_Track_em");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_RefGamma_em_et)/1000.0,
                     (double)(mSUSYD3PD->MET_RefGamma_em_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_RefGamma_em_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_RefGamma_em_sumet)/1000.0,
                     "MET_RefGamma_em");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_RefTau_em_et)/1000.0,
                     (double)(mSUSYD3PD->MET_RefTau_em_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_RefTau_em_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_RefTau_em_sumet)/1000.0,
                     "MET_RefTau_em");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_CellOut_em_et)/1000.0,
                     (double)(mSUSYD3PD->MET_CellOut_em_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_CellOut_em_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_CellOut_em_sumet)/1000.0,
                     "MET_CellOut_em");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_RefFinal_et)/1000.0,
                     (double)(mSUSYD3PD->MET_RefFinal_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_RefFinal_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_RefFinal_sumet)/1000.0,
                     "MET_RefFinal");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_RefEle_et)/1000.0,
                     (double)(mSUSYD3PD->MET_RefEle_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_RefEle_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_RefEle_sumet)/1000.0,
                     "MET_RefEle");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_RefTau_et)/1000.0,
                     (double)(mSUSYD3PD->MET_RefTau_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_RefTau_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_RefTau_sumet)/1000.0,
                     "MET_RefTau");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_MuonBoy_Track_et)/1000.0,
                     (double)(mSUSYD3PD->MET_MuonBoy_Track_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_MuonBoy_Track_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_MuonBoy_Track_sumet)/1000.0,
                     "MET_MuonBoy_Track");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_MuonBoy_Spectro_et)/1000.0,
                     (double)(mSUSYD3PD->MET_MuonBoy_Spectro_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_MuonBoy_Spectro_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_MuonBoy_Spectro_sumet)/1000.0,
                     "MET_MuonBoy_Spectro");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_RefMuon_Staco_et)/1000.0,
                     (double)(mSUSYD3PD->MET_RefMuon_Staco_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_RefMuon_Staco_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_RefMuon_Staco_sumet)/1000.0,
                     "MET_RefMuon_Staco");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_RefMuon_Track_Staco_et)/1000.0,
                     (double)(mSUSYD3PD->MET_RefMuon_Track_Staco_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_RefMuon_Track_Staco_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_RefMuon_Track_Staco_sumet)/1000.0,
                     "MET_RefMuon_Track_Staco");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_RefMuon_Track_et)/1000.0,
                     (double)(mSUSYD3PD->MET_RefMuon_Track_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_RefMuon_Track_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_RefMuon_Track_sumet)/1000.0,
                     "MET_RefMuon_Track");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_MuonBoy_et)/1000.0,
                     (double)(mSUSYD3PD->MET_MuonBoy_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_MuonBoy_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_MuonBoy_sumet)/1000.0,
                     "MET_MuonBoy");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_Topo_et)/1000.0,
                     (double)(mSUSYD3PD->MET_Topo_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_Topo_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_Topo_sumet)/1000.0,
                     "MET_Topo");
    term = (MTree::MissingET*)mMissingETCollection->New(missingETCounter++);
    term->ClearVariables();
    term->Initialize((double)(mSUSYD3PD->MET_LocHadTopo_et)/1000.0,
                     (double)(mSUSYD3PD->MET_LocHadTopo_etx)/1000.0,
                     (double)(mSUSYD3PD->MET_LocHadTopo_ety)/1000.0,
                     (double)(mSUSYD3PD->MET_LocHadTopo_sumet)/1000.0,
                     "MET_LocHadTopo");
    term->InitializeRegions((double)(mSUSYD3PD->MET_LocHadTopo_etx_CentralReg)/1000.0,
                            (double)(mSUSYD3PD->MET_LocHadTopo_etx_EndcapRegion)/1000.0,
                            (double)(mSUSYD3PD->MET_LocHadTopo_etx_ForwardReg)/1000.0,
                            (double)(mSUSYD3PD->MET_LocHadTopo_ety_CentralReg)/1000.0,
                            (double)(mSUSYD3PD->MET_LocHadTopo_ety_EndcapRegion)/1000.0,
                            (double)(mSUSYD3PD->MET_LocHadTopo_ety_ForwardReg)/1000.0,
                            (double)(mSUSYD3PD->MET_LocHadTopo_sumet_CentralReg)/1000.0,
                            (double)(mSUSYD3PD->MET_LocHadTopo_sumet_EndcapRegion)/1000.0,
                            (double)(mSUSYD3PD->MET_LocHadTopo_sumet_ForwardReg)/1000.0);
    //
    //Fill the MTree
    //
    if (mDoPrint)   std::cout << "DEBUG Fill MTree" << std::endl;
    mOutputTree->Fill(); 
    mOutputFile = mOutputTree->GetCurrentFile();

    if (mDoPrint)   std::cout << "DEBUG End" << std::endl;
  }
  
  return;
}



bool ConvertD3PDToMTreeData::DumpD3PDLeavesToFile(const std::string& filename)
{
  std::cout << "| Dumping D3PD leaves to file: " << filename << std::endl;
  if (!mInputChain) return false;
  ofstream outfile;
  outfile.open(filename.c_str()); 
  if (outfile.fail())
  {
    std::cout << "| ERROR: Cannot open file " << filename 
              << " for writing, in ConvertD3PDToMTreeData::DumpD3PDLeavesToFile" << std::endl;
    outfile.clear();
    return false;
  }
  TObjArray* leaves = mInputChain->GetListOfLeaves();
  for (int i = 0; i < leaves->GetSize(); i++)
  {
    TLeafObject* leaf = (TLeafObject*)(TLeaf*)leaves->UncheckedAt(i);
    outfile << std::setw(50) << std::left << leaf->GetName() 
            << std::setw(50) << std::left << leaf->GetTypeName() << std::endl;
  }
  outfile.close();
  std::cout << "| Done with dump." << std::endl;
  return true;
}


ClassImp(ConvertD3PDToMTreeData);
