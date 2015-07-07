
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/IToolSvc.h"

#include "StoreGate/StoreGateSvc.h"


#include "muonEvent/MuonContainer.h"
#include "Particle/TrackParticleContainer.h"
#include "MissingETEvent/MissingET.h"
#include "MissingETEvent/MissingEtCalo.h"
#include "MissingETEvent/MissingEtRegions.h"
#include "EventInfo/EventInfo.h"
#include "EventInfo/EventID.h"
#include "EventInfo/EventType.h"
#include "TrigDecisionTool/ChainGroup.h"
#include "TrigDecisionTool/FeatureContainer.h"
#include "TrigDecisionTool/Feature.h"

#include "AnalysisTriggerEvent/LVL1_ROI.h"
#include "AnalysisTriggerEvent/EmTau_ROI.h"

//Extra includes for testing trigger access
#include "AnalysisTriggerEvent/Muon_ROI.h"
#include "TrigMuonEvent/MuonFeature.h"
#include "TrigMuonEvent/MuonFeatureContainer.h"
#include "TrigMuonEvent/TrigMuonEFContainer.h"
#include "TrigMuonEvent/TrigMuonEF.h"
#include "TrigMuonEvent/TrigMuonEFInfoContainer.h"
#include "TrigMuonEvent/TrigMuonEFInfoTrackContainer.h"
#include "TrigMuonEvent/TrigMuonEFCbTrack.h"
#include "TrigParticle/TrigElectron.h"
#include "TrigParticle/TrigElectronContainer.h"
#include "TrigMuonEvent/CombinedMuonFeature.h"
#include "TrigMuonEvent/CombinedMuonFeatureContainer.h"

#include "egammaEvent/egammaContainer.h"
#include "egammaEvent/egamma.h"

#include "JetTagInfo/TruthInfo.h"
#include "JetEvent/JetTagInfoBase.h"
#include "JetUtils/JetCaloHelper.h"
#include "JetUtils/JetCaloQualityUtils.h"
#include "JetTagEvent/TrackAssociation.h"
#include "JetEvent/JetMomentMap.h"
#include "MuonIDEvent/MuonAssociation.h"
#include "egammaEvent/ElectronAssociation.h"
#include "JetTagInfo/IPInfoBase.h"
#include "JetTagInfo/IPInfoPlus.h"
#include "JetTagInfo/SVInfoBase.h"
#include "JetTagInfo/SVInfoPlus.h"
#include "CaloEvent/CaloSampling.h"

#include "TTree.h"

#include "MTree/ConvertAODToMTree.h"

#include <algorithm>
#include <math.h>
#include <functional>
#include <iostream>
#include <sys/stat.h> 

ConvertAODToMTree::ConvertAODToMTree(const std::string& name, ISvcLocator* pSvcLocator) : 
  Algorithm(name, pSvcLocator), m_trigDec("Trig::TrigDecisionTool/TrigDecisionTool"),
  m_trackToVertex("Reco::TrackToVertex"), m_isolationTool("TrackIsolationTool"), m_classifier ("MCTruthClassifier"), m_jvfCalc("JetVertexAssociationTool"), m_EventCounter(0),
  m_IgnoreL1TriggerNames(), m_IgnoreHLTTriggerNames(),
  m_HLTTriggerROINames(), m_METTermContainerNames() {
  
  declareProperty("RootFileName", m_RootFileName = "MTree.root");
  declareProperty("ElectronPtCut", m_electronPtCut = 0.0);
  declareProperty("PhotonPtCut", m_photonPtCut = 0.0);
  declareProperty("TrackPtCut", m_trackPtCut = 0.0);
  declareProperty("ApplySkim", m_applySkim = false);
  declareProperty("NrLeptons", m_nrLeptonsCut);
  declareProperty("LeptonPtCut", m_leptonPtCut);
  declareProperty("SmearLeptonPt", m_smearLeptonPt);
  declareProperty("VtxContainerName", m_VtxContainerName = "VxPrimaryCandidate");
  declareProperty("JetContainerName", m_JetContainerName = "AntiKt4TopoJets");
  declareProperty("TrackContainerName", m_TrackContainerName = "TrackParticleCandidate");
  declareProperty("ElectronContainerName", m_ElectronContainerName = "ElectronAODCollection");
  declareProperty("PhotonContainerName", m_PhotonContainerName = "PhotonAODCollection");
  declareProperty("MuonContainerNames", m_MuonContainerNames);
  declareProperty("CaloMuonContainerName", m_CaloMuonContainerName = "CaloMuonCollection");
  declareProperty("StandaloneMuonContainerNames", m_StandaloneMuonContainerNames);
  declareProperty("MuTagContainerName", m_MuTagContainerName = "MuTagTrackParticles");
  declareProperty("METContainerName", m_METContainerName = "MET_RefFinal");
  declareProperty("MCTruthContainerName", m_MCTruthContainerName = "GEN_AOD");
  declareProperty("IgnoreL1TriggerNames", m_IgnoreL1TriggerNames);
  declareProperty("IgnoreHLTTriggerNames", m_IgnoreHLTTriggerNames);
  declareProperty("HLTTriggerROINames", m_HLTTriggerROINames);
  declareProperty("EventCounterInterval", m_EventCounterInterval = 1000);
  declareProperty("TrigDecisionTool", m_trigDec, "Tool to access the trigger decision");
  declareProperty("TrackToVertex", m_trackToVertex);
  declareProperty("TrackIsolationTool", m_isolationTool);
  declareProperty("METTermContainerNames", m_METTermContainerNames);
  declareProperty("METCaloContainerNames", m_METCaloContainerNames);
  declareProperty("JetVertexAssociationTool",m_jvfCalc);

}


ConvertAODToMTree::~ConvertAODToMTree() {
}


StatusCode ConvertAODToMTree::initialize() {
  
  MsgStream mLog( messageService(), name() );
  
  mLog << MSG::INFO << "Initializing ConvertAODToMTree" << endreq;
  
  /** get a handle of StoreGate for access to the Event Store */
  StatusCode sc = service("StoreGateSvc", m_storeGate);
  if (sc.isFailure()) {
    mLog << MSG::ERROR
	 << "Unable to retrieve pointer to StoreGateSvc"
	 << endreq;
    return sc;
  }

  mLog << MSG::INFO << "Electron pT cut: " << m_electronPtCut << endreq;
  mLog << MSG::INFO << "Photon pT cut  : " << m_photonPtCut << endreq;
  mLog << MSG::INFO << "Track pT cut   : " << m_trackPtCut << endreq;
  
  /** retrieve trigger decision tool to trigger IoV registration of underlying DSConfig service */
  sc = m_trigDec.retrieve();
  if (sc.isFailure()) {
    mLog << MSG::ERROR
	 << "Unable to get a handle on TrigDecisionTool"
	 << endreq;
    return sc;
  }

  sc = m_trackToVertex.retrieve();
  if (sc.isFailure() ) {
    mLog << MSG::ERROR 
	  << "Failed to retrieve TrackToVertex tool" 
	  << endreq;
    return sc;
  }

  // get a handle on the isolation tools
  sc = m_isolationTool.retrieve();
  if ( sc.isFailure() ) {
    mLog << MSG::ERROR << "Can't get a handle on the Isolation Tools" << endreq;
    return sc;
  } 

//   sc = m_jetVtxHelp.retrieve();
//   if ( sc.isFailure() ) {
//     mLog << MSG::ERROR << "Failed to retrieve JetVtxTrackHelper" << endreq;
//     return sc;
//   }

  sc = m_jvfCalc.retrieve();
  if ( sc.isFailure() ) {
    mLog << MSG::ERROR << "Failed to retrieve JetVertexAssociationTool" << endreq;
    return sc;
  }

  m_triggerObjects = new TClonesArray("MTree::TriggerObject", 20);
  m_electrons = new TClonesArray("MTree::Electron", 10);
  m_photons = new TClonesArray("MTree::Photon", 10);
  m_muons = new TClonesArray("MTree::Muon", 10);
  m_caloMuons = new TClonesArray("MTree::Muon", 10);
  m_jets = new TClonesArray("MTree::Jet", 20);
  m_tracks = new TClonesArray("MTree::Track", 100);
  m_standAloneMuons = new TClonesArray("MTree::Track", 10);
  m_muTagMuons = new TClonesArray("MTree::Track", 10);
  m_primaryVertices = new TClonesArray("MTree::PrimaryVertex", 5);
  m_truthParticles = new TClonesArray("MTree::TruthParticle", 10);
  m_L1EmTauTriggerROIs = new TClonesArray("MTree::L1TriggerROI", 10);
  m_L1MuonTriggerROIs = new TClonesArray("MTree::L1TriggerROI", 10);
  m_HLTTriggerROIs = new TClonesArray("MTree::HLTTriggerROI", 10);
  m_missingET = new MTree::MissingET();
  m_eventInfo = new MTree::EventInfo();
  m_trigger = new MTree::Trigger();

  m_missingETTerms = new TClonesArray("MTree::MissingET", 10);
  
  m_outFile = new TFile(m_RootFileName.c_str(), "RECREATE");   
  
  m_tree = new TTree("MTree", "An Analysis Tree");
  m_tree->Branch("TriggerObjects", &m_triggerObjects);
  m_tree->Branch("Electrons", &m_electrons);
  m_tree->Branch("Photons", &m_photons);
  m_tree->Branch("Muons", &m_muons);
  m_tree->Branch("CaloMuons", &m_caloMuons);
  m_tree->Branch("Jets", &m_jets);
  m_tree->Branch("Tracks", &m_tracks);
  m_tree->Branch("StandAloneMuons", &m_standAloneMuons);
  m_tree->Branch("MuTagMuons", &m_muTagMuons);
  m_tree->Branch("PrimaryVertices", &m_primaryVertices);
  m_tree->Branch("TruthParticles", &m_truthParticles);
  m_tree->Branch("L1EmTauTriggerROIs", &m_L1EmTauTriggerROIs);
  m_tree->Branch("L1MuonTriggerROIs", &m_L1MuonTriggerROIs);
  m_tree->Branch("HLTTriggerROIs", &m_HLTTriggerROIs);
  m_tree->Branch("MissingET", &m_missingET);
  m_tree->Branch("EventInfo", &m_eventInfo);
  m_tree->Branch("Trigger", &m_trigger);
  
  m_tree->Branch("MissingETTerms", &m_missingETTerms);



//skimming init
  if (m_applySkim)
  {
//     m_muSmear.Initialize("muid","MuonMomentumCorrections/root/data");
    m_muSmear.UseGeV();
    m_muSmear.UseScale(1);

    m_elSmear.SetRandomSeed(1);
    m_elSmear.useDefaultCalibConstants("2011");



    //m_muFile = new TFile("mu.root","recreate");
    m_muHist = new TH1D("mu","mu",20,0.5,20.5);

    m_logHist = new TH1D("logHist","log for filter efficiency",4,0,4);
    //m_outFile->cd();

    m_nPlusEvents = 0;
    m_nMinusEvents = 0;
    m_nPlusEventsPassed = 0;
    m_nMinusEventsPassed = 0;
  }

  return StatusCode::SUCCESS;
}		 

StatusCode ConvertAODToMTree::finalize() {
  MsgStream mLog( messageService(), name() );
  mLog << MSG::INFO << "ConvertAODToMTree closing output ROOT file." << endreq;
/*
  if (m_applySkim)
  {
    m_muFile->Write("mu");
    m_muFile->Close();

    ofstream logFile("log.out");
    logFile << "Initial: " << m_nPlusEvents - m_nMinusEvents << std::endl;
    logFile << "Filtered: " << m_nPlusEventsPassed - m_nMinusEventsPassed << std::endl;
    logFile << "Efficiency: " << (m_nPlusEventsPassed - m_nMinusEventsPassed)/(double)(m_nPlusEvents - m_nMinusEvents) << std::endl;
    logFile << "Nplus_init: " << m_nPlusEvents << std::endl;
    logFile << "Nminus_init: " << m_nMinusEvents << std::endl;
    logFile << "Nplus: " << m_nPlusEventsPassed << std::endl;
    logFile << "Nminus: " << m_nMinusEventsPassed << std::endl;
    logFile.close();
  }
*/

  if (m_applySkim)
  {
    m_logHist->GetXaxis()->SetBinLabel(1, "Nplus_init");
    m_logHist->GetXaxis()->SetBinLabel(2, "Nminus_init");
    m_logHist->GetXaxis()->SetBinLabel(3, "Nplus_final");
    m_logHist->GetXaxis()->SetBinLabel(4, "Nminus_final");

    m_logHist->SetBinContent(1, m_nPlusEvents);
    m_logHist->SetBinContent(2, m_nMinusEvents);
    m_logHist->SetBinContent(3, m_nPlusEventsPassed);
    m_logHist->SetBinContent(4, m_nMinusEventsPassed);
  }
  m_outFile->Write();
  m_outFile->Close();

  return StatusCode::SUCCESS;

}



StatusCode ConvertAODToMTree::execute() {
  MsgStream mLog( messageService(), name() );
  
  if (++m_EventCounter % m_EventCounterInterval == 0) {
    mLog << MSG::INFO << "Processing Event " << m_EventCounter << endreq;
  }
  mLog << MSG::DEBUG << "Event, " << m_EventCounter << ", in ConvertAODToMTree::execute()" << endreq;


  // Fill configured triggers on the first event
  // Put it here to make sure it is always executed
  if (m_EventCounter == 1) { 
    m_ConfiguredL1Names = m_trigDec->getListOfTriggers("L1_.*");
    m_ConfiguredL2Names = m_trigDec->getListOfTriggers("L2_.*");
    m_ConfiguredEFNames = m_trigDec->getListOfTriggers("EF_.*");
  }
  





  //
  // Fill Global Info for the Event
  //
  const EventInfo* eventInfo(0);
  StatusCode sc = m_storeGate->retrieve(eventInfo);
  if (sc.isFailure() || !eventInfo) {
    mLog << MSG::ERROR << "No AOD container found for Event Info" << endreq;
    return StatusCode::FAILURE;
  }
  mLog << MSG::DEBUG << "Filling the Event Info." << endreq; 
  m_eventInfo->ClearVariables();
  m_eventInfo->Initialize(eventInfo->event_ID()->run_number(),
			  eventInfo->event_ID()->event_number(),
			  eventInfo->event_ID()->bunch_crossing_id(),
			  eventInfo->event_ID()->lumi_block(),
			  eventInfo->event_type()->mc_event_weight(),
			  eventInfo->errorState(EventInfo::LAr));

//fill avg mu hist for skimming output
  if (m_applySkim)
  {
    m_muHist->Fill(m_eventInfo->LumiBlock(), m_eventInfo->EventWeight());
  
    double eventWeight = m_eventInfo->EventWeight();
    if (eventWeight != 0)
    {
      int sign = eventWeight / std::fabs(eventWeight);

      if (sign > 0)
      {
	m_nPlusEvents++;
      }
      else
      {
	m_nMinusEvents++;
      }
    }
  }

  //
  // Fill Primary Vertices
  //  
  const VxContainer* vtxContainer(0);
  sc = m_storeGate->retrieve(vtxContainer, m_VtxContainerName);
  if (sc.isFailure() || !vtxContainer) {
    mLog << MSG::ERROR << "No AOD primary vertex container found in StoreGate" << endreq;
    return StatusCode::FAILURE;
  }   

  VxContainer::const_iterator vtxItr = vtxContainer->begin(), vtxEnd = vtxContainer->end();

  m_primaryVertices->Clear();
  int primaryVertexCounter = 0;
  mLog << MSG::DEBUG << "Looping over the Primary Vertices." << endreq; 
  for (; vtxItr != vtxEnd; ++vtxItr) {
    MTree::PrimaryVertex* vertex = (MTree::PrimaryVertex*) m_primaryVertices->New(primaryVertexCounter++);    
    vertex->Initialize((*vtxItr)->recVertex().position().x(),
		       (*vtxItr)->recVertex().position().y(),
		       (*vtxItr)->recVertex().position().z(),
		       (*vtxItr)->recVertex().errorPosition().error(Trk::x),
		       (*vtxItr)->recVertex().errorPosition().error(Trk::y),
		       (*vtxItr)->recVertex().errorPosition().error(Trk::z),
		       (int) (*vtxItr)->vxTrackAtVertex()->size(),
		       (*vtxItr)->recVertex().fitQuality().chiSquared(),
		       (*vtxItr)->recVertex().fitQuality().numberDoF(),
		       (*vtxItr)->vertexType());
  }
  mLog << MSG::DEBUG << "   Finished looping over the Primary Vertices." << endreq; 

  //
  // Fill Jets
  //
  const JetCollection* jetContainer(0);
  sc = m_storeGate->retrieve( jetContainer, m_JetContainerName);
  if (sc.isFailure() || !jetContainer) {
    mLog << MSG::ERROR << "No AOD jet container found in StoreGate" << endreq;
    return StatusCode::FAILURE;
  } 

  sc = m_jvfCalc->setupEvent();

  JetCollection::const_iterator jetItr = jetContainer->begin(), jetEnd = jetContainer->end();
  VxContainer::const_iterator vtxOne = vtxContainer->begin();
  m_jets->Clear();
  int jetCounter = 0;
  mLog << MSG::DEBUG << "Looping over the Jets." << endreq; 
  for (; jetItr != jetEnd; ++jetItr) {
    MTree::Jet* jet = (MTree::Jet*) m_jets->New(jetCounter++);
    jet->Initialize((*jetItr)->pt() / 1000.0,
		    (*jetItr)->eta(),
		    (*jetItr)->phi(),
		    (*jetItr)->m() / 1000.0,
		    (*jetItr)->getFlavourTagWeight("IP3D"),
		    (*jetItr)->getFlavourTagWeight("SV0"),
		    (*jetItr)->getFlavourTagWeight("SV1"),
		    (*jetItr)->getFlavourTagWeight("SV2"),
		    (*jetItr)->getFlavourTagWeight(),
		    (*jetItr)->getFlavourTagWeight("TrackCounting2D"),
		    (*jetItr)->getFlavourTagWeight("JetProb"),
		    (*jetItr)->getFlavourTagWeight("JetFitterTag"),
		    (*jetItr)->getFlavourTagWeight("JetFitterCOMB"),
		    (*jetItr)->getFlavourTagWeight("JetFitterTagNN"),
		    (*jetItr)->getFlavourTagWeight("JetFitterCOMBNN"));
    int SamplingMax = 0;
    double fracSamplingMax = JetCaloQualityUtils::fracSamplingMax(*jetItr, SamplingMax);
    jet->SetJetQualities(JetCaloHelper::jetEMFraction(*jetItr),
// 			 (*jetItr)->getMoment("LArQuality"),
//                          (*jetItr)->getMoment("HECQuality"),
// 			 (*jetItr)->getMoment("Timing"),
// 			 (*jetItr)->getMoment("n90"),
			 JetCaloQualityUtils::nLeadingConstituents(*jetItr, 0.9),
//                          (*jetItr)->getMoment("NegativeE"),
			 SamplingMax,
                         fracSamplingMax,
			 JetCaloQualityUtils::hecF(*jetItr),
			 JetCaloQualityUtils::tileGap3F(*jetItr));
    jet->SetJetQualityFlags(JetCaloQualityUtils::isGood(*jetItr, true),
			    JetCaloQualityUtils::isBad(*jetItr, true),
			    JetCaloQualityUtils::isUgly(*jetItr, true));
    jet->SetRawEnergies((*jetItr)->getRawE() / 1000.0,
			(*jetItr)->getRawPx() / 1000.0,
			(*jetItr)->getRawPy() / 1000.0,
			(*jetItr)->getRawPz() / 1000.0);//,
// 			(*jetItr)->getCScaleE() / 1000.0,
// 			(*jetItr)->getCScalePx() / 1000.0,
// 			(*jetItr)->getCScalePy() / 1000.0,
// 			(*jetItr)->getCScalePz() / 1000.0,
// 			(*jetItr)->getCalE() / 1000.0,
// 			(*jetItr)->getCalPx() / 1000.0,
// 			(*jetItr)->getCalPy() / 1000.0,
// 			(*jetItr)->getCalPz() / 1000.0,
// 			(*jetItr)->getMoment("EMJES"),
// 			(*jetItr)->getMoment("GCWJES"));
    double newJVF = 0.0;
    int newNTrk = 0.0;
    double newSumPtTrk = 0.0;
    if (vtxOne != vtxEnd)
    {
      sc = m_jvfCalc->getJVF( (*jetItr)->eta(), (*jetItr)->phi(), *vtxOne,
			      newJVF, newNTrk, newSumPtTrk);
    }
    jet->SetJVF( // (*jetItr)->getMoment("JVF"),
//                  (*jetItr)->getMoment("nTrk"),
//                  ((*jetItr)->getMoment("sumPtTrk"))/1000,
                 newJVF,
                 newNTrk,
                 newSumPtTrk/1000 );

    const Analysis::TruthInfo* mcinfo = (*jetItr)->getTagInfo<Analysis::TruthInfo>("TruthInfo");
    if (mcinfo != 0)
    {
      int iflav = 0;
      std::string label = mcinfo->jetTruthLabel();
      if (label == "B") iflav = 5;
      else if (label == "C") iflav = 4;
      else if (label == "T") iflav = 15;

      jet->SetTrueFlavor(iflav);
    }

    std::vector< std::string > keys = (*jetItr)->getShapeKeys();
    std::vector< std::string >::const_iterator b = keys.begin(), e = keys.end();
    mLog << MSG::DEBUG << "Jet moments for this jet:" << endreq;
    for (; b != e; b++) 
    {
      jet->SetJetMoment(*b, (*jetItr)->getMoment(*b));
      mLog << MSG::DEBUG << " " << *b << endreq;
    }
  }
  mLog << MSG::DEBUG << "   Finished looping over the Jets." << endreq; 

  //
  // Fill Tracks in m_tracks
  //
  const int NrOfTrackContainers = 4;

  m_tracks->Clear();
  int trackCounter = 0;
  m_standAloneMuons->Clear();
  int standAloneMuonsCounter = 0;
  m_muTagMuons->Clear();
  int muTagMuonsCounter = 0;
  for (int i = 0; i != NrOfTrackContainers; ++i) {
  
    const Rec::TrackParticleContainer* trackContainer(0);
    std::string currentContainerName;
    if (i == 0) 
      currentContainerName = m_TrackContainerName;
    else if (i == 1)
      currentContainerName = m_StandaloneMuonContainerNames[0];
    else if (i == 2)
      currentContainerName = m_StandaloneMuonContainerNames[1];
    else 
      currentContainerName = m_MuTagContainerName;
    sc = m_storeGate->retrieve(trackContainer, currentContainerName);      
    if(sc.isFailure() || !trackContainer) {
      mLog << MSG::WARNING << "No AOD container called " << currentContainerName 
	   << " found in StoreGate" << endreq; 
      return StatusCode::FAILURE;
    }  
  
    Rec::TrackParticleContainer::const_iterator trackItr  = trackContainer->begin(), trackEnd = trackContainer->end();
 
    MTree::Track* track = 0;
    mLog << MSG::DEBUG << "Looping over the Tracks from " << currentContainerName << "." << endreq; 
    for (; trackItr != trackEnd; ++trackItr) {
      double trackPt = (*trackItr)->pt() / 1000.0;
      if (trackPt < m_trackPtCut) continue;
      if (i == 0) 
	track = (MTree::Track*) m_tracks->New(trackCounter++);
      else if (i == 1)
	track = (MTree::Track*) m_standAloneMuons->New(standAloneMuonsCounter++);
      else if (i == 2)
	track = (MTree::Track*) m_standAloneMuons->New(standAloneMuonsCounter++);
      else
	track = (MTree::Track*) m_muTagMuons->New(muTagMuonsCounter++);
      int hitMask = 0;
      if (i == 0) {
	for (int j = 0; j != Trk::numberOfDetectorTypes; ++j) {
	  hitMask |= (*trackItr)->trackSummary()->isHit(Trk::DetectorType(j)) << j;
	}
      }
      const Trk::VxCandidate* vertex = (*trackItr)->reconstructedVertex();
      //
      // Get z0 and d0 w.r.t. the vertex associated with the track
      //
      double vtx_x = 0.0, vtx_y = 0.0, vtx_z = 0.0;
      float vtx_d0 = -1.0, vtx_d0_err = -1.0, vtx_z0 = -1.0, vtx_z0_err = -1.0;
      if (vertex) {
	vtx_x = vertex->recVertex().position().x();
	vtx_y = vertex->recVertex().position().y();
	vtx_z = vertex->recVertex().position().z();
	const Trk::MeasuredPerigee* per = m_trackToVertex->perigeeAtVertex(*(*(trackItr)));
	if (per)
	{	
	  vtx_d0 = per->parameters()[Trk::d0];
	  vtx_d0_err = per->localErrorMatrix().error(Trk::d0);
	  vtx_z0 = per->parameters()[Trk::z0];
	  vtx_z0_err = per->localErrorMatrix().error(Trk::z0);
	}
      }

      //
      // Get params wrt 0th vertex in container
      //
      float prim_vtx_d0 = -1.0, prim_vtx_d0_err = -1.0, prim_vtx_z0 = -1.0, prim_vtx_z0_err = -1.0;
      const Trk::VxCandidate* vertex0 = *vtxContainer->begin();
      if (vertex0)
      {
        // create a global position from this
	const Hep3Vector& vertexPosition = vertex0->recVertex().position();
	Trk::GlobalPosition persfPosition(vertexPosition.x(), vertexPosition.y(), vertexPosition.z());
	const Trk::MeasuredPerigee* per = m_trackToVertex->perigeeAtVertex(*(*(trackItr)), persfPosition);
	if (per)
	{
	  prim_vtx_d0 = per->parameters()[Trk::d0];
	  prim_vtx_d0_err = per->localErrorMatrix().error(Trk::d0);
	  prim_vtx_z0 = per->parameters()[Trk::z0];
	  prim_vtx_z0_err = per->localErrorMatrix().error(Trk::z0);
	}
      }


      track->Initialize(trackPt,
			(*trackItr)->eta(),
			(*trackItr)->phi(),
			(int) (*trackItr)->charge(),
			(*trackItr)->fitQuality()->numberDoF(),
			(float) (*trackItr)->fitQuality()->chiSquared(),
			(float) (*trackItr)->measuredPerigee()->parameters()[Trk::d0],
			(float) (*trackItr)->measuredPerigee()->localErrorMatrix().error(Trk::d0),
			(float) (*trackItr)->measuredPerigee()->parameters()[Trk::z0],
			(float) (*trackItr)->measuredPerigee()->localErrorMatrix().error(Trk::z0),
			vtx_d0,
			vtx_d0_err,
			vtx_z0,
			vtx_z0_err,
			prim_vtx_d0,
			prim_vtx_d0_err,
			prim_vtx_z0,
			prim_vtx_z0_err,
                        (*trackItr)->trackSummary()->get(Trk::expectBLayerHit),
			(*trackItr)->trackSummary()->get(Trk::numberOfBLayerHits),
			(*trackItr)->trackSummary()->get(Trk::numberOfPixelHits),
			(*trackItr)->trackSummary()->get(Trk::numberOfPixelHoles),
			(*trackItr)->trackSummary()->get(Trk::numberOfPixelDeadSensors),
			(*trackItr)->trackSummary()->get(Trk::numberOfSCTHits),
			(*trackItr)->trackSummary()->get(Trk::numberOfSCTHoles),
			(*trackItr)->trackSummary()->get(Trk::numberOfSCTDeadSensors),
			(*trackItr)->trackSummary()->get(Trk::numberOfTRTHits),
			(*trackItr)->trackSummary()->get(Trk::numberOfTRTOutliers),
			(*trackItr)->trackSummary()->get(Trk::numberOfTRTHighThresholdHits),
			(*trackItr)->particleOriginType(),
			hitMask);   
      track->SetTrackAuthor(i);
      if (vertex) {
	int vtxCounter = -1;
	double epsilon = 0.00001; //perhaps change to pointer matching
	for (vtxItr = vtxContainer->begin(); vtxItr != vtxEnd; ++vtxItr) {
	  ++vtxCounter;
	  if (TMath::Abs((*vtxItr)->recVertex().position().x() - vtx_x) > epsilon) continue;
	  if (TMath::Abs((*vtxItr)->recVertex().position().y() - vtx_y) > epsilon) continue;
	  if (TMath::Abs((*vtxItr)->recVertex().position().z() - vtx_z) > epsilon) continue;
	  track->SetPrimaryVertex((MTree::PrimaryVertex*) m_primaryVertices->At(vtxCounter));
	}
      }
    }
    mLog << MSG::DEBUG << "   Finished looping over the Tracks from " 
	 << currentContainerName << "." << endreq; 
  }
  //
  // Fill Electrons in m_electrons
  //
  const ElectronContainer* electronContainer(0);
  sc = m_storeGate->retrieve(electronContainer, m_ElectronContainerName);
  if ( sc.isFailure() || !electronContainer ) {
    mLog << MSG::ERROR << "No AOD electron container found in StoreGate" << endreq; 
    return StatusCode::FAILURE;
  }  
  
  ElectronContainer::const_iterator elecItr  = electronContainer->begin();
  ElectronContainer::const_iterator elecEnd = electronContainer->end();

  if (m_applySkim) m_elSmear.SetRandomSeed(m_eventInfo->EventNumber());
  int nrElectronsPassed = 0;

  m_electrons->Clear();
  int electronCounter = 0;
  mLog << MSG::DEBUG << "Looping over the Electrons." << endreq; 
  for (; elecItr != elecEnd; ++elecItr) {
    double electronPt = (*elecItr)->pt() / 1000.0;
    if (electronPt < m_electronPtCut) continue;

    

    MTree::Electron* electron = (MTree::Electron*) m_electrons->New(electronCounter++);
    const Rec::TrackParticle* matched_track = (*elecItr)->trackParticle();
    
    electron->InitializeLepton(electronPt,
			       (*elecItr)->eta(),
			       (*elecItr)->phi(),
			       (*elecItr)->m() / 1000.0,
			       (*elecItr)->author(),
			       (int) (*elecItr)->charge(),
			       11);     
    unsigned long int IsEM = (*elecItr)->egammaID(egammaPID::IsEM);
    unsigned long int IsGoodOQ = (*elecItr)->egammaID(egammaPID::IsGoodOQ);
    bool PassedOQ = ((int) (*elecItr)->isgoodoq(egammaPID::BADCLUSELECTRON)==0 ? 1 : 0) ;
    electron->Initialize((*elecItr)->detailValue(egammaParameters::EoverP),
			 (*elecItr)->detailValue(egammaParameters::etcone20) / 1000.0,
			 (*elecItr)->detailValue(egammaParameters::etcone30) / 1000.0,
			 (*elecItr)->detailValue(egammaParameters::etcone40) / 1000.0,
			 (*elecItr)->detailValue(egammaParameters::etcone) / 1000.0,
			 IsEM,
			 IsGoodOQ,
			 PassedOQ,
			 (float) (*elecItr)->detailValue(egammaParameters::e233) / 1000.0,
			 (float) (*elecItr)->detailValue(egammaParameters::e237) / 1000.0,
			 (float) (*elecItr)->detailValue(egammaParameters::e277) / 1000.0,
			 (float) (*elecItr)->detailValue(egammaParameters::ethad) / 1000.0,
			 (float) (*elecItr)->detailValue(egammaParameters::f1),
			 (float) (*elecItr)->detailValue(egammaParameters::f1core),
			 (float) (*elecItr)->detailValue(egammaParameters::fracs1),
			 //(float) (*elecItr)->detailValue(egammaParameters::iso),
			 0.0,
			 (bool) (*elecItr)->detailValue(egammaParameters::trackRefit),
			 (bool) (*elecItr)->detailValue(egammaParameters::convTrackMatch),
			 (bool) (*elecItr)->detailValue(egammaParameters::convAngleMatch),
			 (float) (*elecItr)->detailValue(egammaParameters::zvertex),
			 (float) (*elecItr)->detailValue(egammaParameters::errz),
			 (*elecItr)->egammaID(egammaPID::ElectronWeight),
			 (*elecItr)->egammaID(egammaPID::BgWeight),
			 (*elecItr)->egammaID(egammaPID::NeuralNet),
			 (*elecItr)->egammaID(egammaPID::Hmatrix),
			 (*elecItr)->egammaID(egammaPID::Hmatrix5),
			 (*elecItr)->egammaID(egammaPID::IsolationLikelihood_jets),
			 (*elecItr)->egammaID(egammaPID::AdaBoost),
			 (*elecItr)->egammaID(egammaPID::PhotonWeight),
			 (*elecItr)->egammaID(egammaPID::BgPhotonWeight));
    electron->InitializeExtraVariables((float) (*elecItr)->detailValue(egammaParameters::ethad1) / 1000.0,
				       (float) (*elecItr)->detailValue(egammaParameters::weta2),
				       (float) (*elecItr)->detailValue(egammaParameters::e2tsts1) / 1000.0,
				       (float) (*elecItr)->detailValue(egammaParameters::emins1) / 1000.0,
				       (float) (*elecItr)->detailValue(egammaParameters::wtots1),
				       (float) (*elecItr)->detailValue(egammaParameters::weta1),
				       (float) (*elecItr)->detailValue(egammaParameters::deltaEta0),
				       (float) (*elecItr)->detailValue(egammaParameters::deltaEta1),
				       (float) (*elecItr)->detailValue(egammaParameters::deltaEta2),
				       (float) (*elecItr)->detailValue(egammaParameters::deltaEta3),
				       (float) (*elecItr)->detailValue(egammaParameters::deltaPhi0),
				       (float) (*elecItr)->detailValue(egammaParameters::deltaPhi1),
				       (float) (*elecItr)->detailValue(egammaParameters::deltaPhi2),
				       (float) (*elecItr)->detailValue(egammaParameters::deltaPhi3),
				       (*elecItr)->nConversions(),
				       (float) (*elecItr)->detailValue(egammaParameters::etap));
    electron->InitializeClusterVariables((*elecItr)->cluster()->eta(),
					 (*elecItr)->cluster()->phi(),
					 (*elecItr)->cluster()->e() / 1000.0,
					 (*elecItr)->cluster()->m() / 1000.0,
					 (*elecItr)->cluster()->pt() / 1000.0,
					 (*elecItr)->cluster()->etaSample(CaloSampling::EMB2),
					 (*elecItr)->cluster()->etaSample(CaloSampling::EME2),
					 (*elecItr)->cluster()->phiSample(CaloSampling::EMB2),
					 (*elecItr)->cluster()->phiSample(CaloSampling::EME2),
					 (*elecItr)->cluster()->etaBE(2),
					 (*elecItr)->cluster()->phiBE(2),
					 (*elecItr)->cluster()->energyBE(2) / 1000.0);
    electron->InitializeElectronFlags( (*elecItr)->isElectron(egammaPID::ElectronLoose),
                                       (*elecItr)->isElectron(egammaPID::ElectronMedium_WithTrackMatch),
                                       (*elecItr)->isElectron(egammaPID::ElectronMediumIso_WithTrackMatch),
                                       (*elecItr)->isElectron(egammaPID::ElectronTight_WithTrackMatch),
                                       (*elecItr)->isElectron(egammaPID::ElectronTightIso_WithTrackMatch),
				       (bool) (*elecItr)->detailValue(egammaParameters::expectHitInBLayer) );
    
    std::pair<MCTruthPartClassifier::ParticleType, MCTruthPartClassifier::ParticleOrigin> res;
    if (const Analysis::Electron* q = dynamic_cast<const Analysis::Electron*>(*elecItr)) {
      res = m_classifier->particleTruthClassifier (q);
    }
    else {
      mLog << MSG::ERROR << "Cast of egamma to Analysis::Electron failed" << endreq; 
      return StatusCode::FAILURE;
    }  
    electron->InitializeTruthClassifiers(res.first, res.second);

    //
    // If matched track is valid and above trackPtCut, loop over all tracks in tree and do matching
    //
    if (matched_track) {
      double trackPt = matched_track->pt() / 1000.0;
      electron->InitializeTrackIsolation(m_isolationTool->trackIsolationEnergy(matched_track, 0.1) / 1000.0,
					 m_isolationTool->trackIsolationEnergy(matched_track, 0.2) / 1000.0,
					 m_isolationTool->trackIsolationEnergy(matched_track, 0.3) / 1000.0,
					 m_isolationTool->trackIsolationEnergy(matched_track, 0.4) / 1000.0,
					 (*elecItr)->detailValue(egammaParameters::ptcone20) / 1000.0,
					 (*elecItr)->detailValue(egammaParameters::ptcone30) / 1000.0,
					 (*elecItr)->detailValue(egammaParameters::ptcone40) / 1000.0);
      if (trackPt > m_trackPtCut) {
	int nrTracks = m_tracks->GetEntriesFast();
	double epsilon = 0.001;
	for (int i = 0; i != nrTracks; ++i) {
	  MTree::Track* track = (MTree::Track*) m_tracks->At(i);
	  if (TMath::Abs(track->Pt() - trackPt) > epsilon) continue; 
	  if (TMath::Abs(track->Eta() - matched_track->eta()) > epsilon) continue; 
	  if (TMath::Abs(track->Phi() - matched_track->phi()) > epsilon) continue;
	  electron->SetTrack(track);
	}
      }
    }

//electron skimming selections

    if (m_applySkim)
    {
      double clE = electron->ClusterE();
      double clEta = electron->ClusterEta();
      //double absClEta = fabs(clEta);

      MTree::Track* track = electron->GetTrack();

      int nSil = !track ? 0 : track->NrOfPixelHits() + track->NrOfSCTHits();
      double cond_eta = nSil>=4?track->Eta():clEta;
      double condTheta = 2*atan(exp(-fabs(cond_eta)));
      double Et = clE*sin(condTheta);

      if (m_smearLeptonPt)
      {
	Et *= m_elSmear.getSmearingCorrection(cond_eta, clE);
      }




      if (Et < m_leptonPtCut) continue;

      //if (absClEta > 2.47 || (absClEta > 1.37 && absClEta < 1.52)) continue;

      if (!electron->IsLoose()) continue;
      nrElectronsPassed++;
    }
  }
  mLog << MSG::DEBUG << "   Finished looping over the Electrons." << endreq; 

 
  //
  // Fill Photons from the PhotonContainer
  //
  const PhotonContainer* photonContainer(0);
  sc = m_storeGate->retrieve(photonContainer, m_PhotonContainerName);
  if ( sc.isFailure() || !photonContainer ) {
    mLog << MSG::ERROR << "No AOD photon container found in StoreGate" << endreq; 
    return StatusCode::FAILURE;
  }  
  
  PhotonContainer::const_iterator photItr  = photonContainer->begin();
  PhotonContainer::const_iterator photEnd = photonContainer->end();

  m_photons->Clear();
  int photonCounter = 0;
  mLog << MSG::DEBUG << "Looping over the Photons." << endreq; 
  for (; photItr != photEnd; ++photItr) {
    double photonPt = (*photItr)->pt() / 1000.0;
    if (photonPt < m_photonPtCut) continue;
    MTree::Photon* photon = (MTree::Photon*) m_photons->New(photonCounter++);
    photon->Initialize(photonPt,
		       (*photItr)->eta(),
		       (*photItr)->phi(),
		       (*photItr)->m() / 1000.0,
		       (*photItr)->author(),		       
		       (*photItr)->detailValue(egammaParameters::EoverP),
		       (*photItr)->detailValue(egammaParameters::etcone20) / 1000.0,
		       (*photItr)->detailValue(egammaParameters::etcone30) / 1000.0,
		       (*photItr)->detailValue(egammaParameters::etcone40) / 1000.0,
		       (*photItr)->detailValue(egammaParameters::etcone) / 1000.0,
		       (*photItr)->isem(),
		       (float) (*photItr)->detailValue(egammaParameters::e233) / 1000.0,
		       (float) (*photItr)->detailValue(egammaParameters::e237) / 1000.0,
		       (float) (*photItr)->detailValue(egammaParameters::e277) / 1000.0,
		       (float) (*photItr)->detailValue(egammaParameters::ethad) / 1000.0,
		       (float) (*photItr)->detailValue(egammaParameters::f1),
		       (float) (*photItr)->detailValue(egammaParameters::f1core),
		       (float) (*photItr)->detailValue(egammaParameters::fracs1),
		       0.0,
		       //(float) (*photItr)->detailValue(egammaParameters::iso),
		       (bool) (*photItr)->detailValue(egammaParameters::trackRefit),
		       (bool) (*photItr)->detailValue(egammaParameters::convTrackMatch),
		       (bool) (*photItr)->detailValue(egammaParameters::convAngleMatch),
		       (float) (*photItr)->detailValue(egammaParameters::zvertex),
		       (float) (*photItr)->detailValue(egammaParameters::errz),
		       (*photItr)->egammaID(egammaPID::ElectronWeight),
		       (*photItr)->egammaID(egammaPID::BgWeight),
		       (*photItr)->egammaID(egammaPID::NeuralNet),
		       (*photItr)->egammaID(egammaPID::Hmatrix),
		       (*photItr)->egammaID(egammaPID::Hmatrix5),
		       (*photItr)->egammaID(egammaPID::IsolationLikelihood_jets),
		       (*photItr)->egammaID(egammaPID::AdaBoost),
		       (*photItr)->egammaID(egammaPID::PhotonWeight),
		       (*photItr)->egammaID(egammaPID::BgPhotonWeight));
    photon->InitializeClusterVariables((*photItr)->cluster()->eta(),
				       (*photItr)->cluster()->phi(),
				       (*photItr)->cluster()->e() / 1000.0,
				       (*photItr)->cluster()->m() / 1000.0,
				       (*photItr)->cluster()->pt() / 1000.0,
				       (*photItr)->cluster()->etaSample(CaloSampling::EMB2),
				       (*photItr)->cluster()->etaSample(CaloSampling::EME2),
				       (*photItr)->cluster()->phiSample(CaloSampling::EMB2),
				       (*photItr)->cluster()->phiSample(CaloSampling::EME2),
				       (*photItr)->cluster()->etaBE(2),
				       (*photItr)->cluster()->phiBE(2),
				       (*photItr)->cluster()->energyBE(2) / 1000.0);
    photon->InitializePhotonFlags( (*photItr)->isPhoton(egammaPID::PhotonLoose),
                                   (*photItr)->isPhoton(egammaPID::PhotonLooseAR),
                                   (*photItr)->isPhoton(egammaPID::PhotonTight),
                                   (*photItr)->isPhoton(egammaPID::PhotonTightIso),
                                   (*photItr)->isPhoton(egammaPID::PhotonTightAR),
                                   (*photItr)->isPhoton(egammaPID::PhotonTightARIso) );
  }
  mLog << MSG::DEBUG << "   Finished looping over the Photons." << endreq; 

  //
  // Fill Muons in m_muons
  //
  const int NrOfMuonContainers = 3;
  int nrMuonsPassed[NrOfMuonContainers];
  int muon_index[NrOfMuonContainers];
  for (int i=0;i<NrOfMuonContainers;i++) 
  {
    nrMuonsPassed[i] = 0;
    muon_index[i] = 0;
  }

  m_muons->Clear();
  int muonCounter = 0;
  m_caloMuons->Clear();
  int caloMuonCounter = 0;
  for (int i = 0; i != NrOfMuonContainers; ++i) {
    const Analysis::MuonContainer* muonContainer(0);
    std::string currentContainerName;
    if (i == 0) 
      currentContainerName = m_MuonContainerNames[0];
    else if (i == 1) 
      currentContainerName = m_MuonContainerNames[1];
    else
      currentContainerName = m_CaloMuonContainerName;
    sc = m_storeGate->retrieve(muonContainer, currentContainerName);      
    if (sc.isFailure() || !muonContainer) {
      mLog << MSG::ERROR << "No AOD Muon container called " << currentContainerName 
	   << " found in StoreGate" << endreq;
      return StatusCode::FAILURE;
    }
    
    Analysis::MuonContainer::const_iterator muonItr = muonContainer->begin(), muonEnd = muonContainer->end();
    MTree::Muon* muon = 0;
    mLog << MSG::DEBUG << "Looping over the Muons from " << currentContainerName << ". Index i = " << i << endreq; 
    for (; muonItr!=muonEnd; ++muonItr) {
      if (i == 0 || i == 1) 
	muon = (MTree::Muon*) m_muons->New(muonCounter++);      
      else 
	muon = (MTree::Muon*) m_caloMuons->New(caloMuonCounter++);
      
      muon->InitializeLepton((*muonItr)->pt() / 1000.0,
			     (*muonItr)->eta(),
			     (*muonItr)->phi(),
			     (*muonItr)->m() / 1000.0,
			     (*muonItr)->author(),		       
			     (int) (*muonItr)->charge(),
			     13);
      const CaloEnergy* caloEnergy = (*muonItr)->caloEnergyLoss();
      muon->Initialize((*muonItr)->parameter(MuonParameters::etcone10) / 1000.0,
		       (*muonItr)->parameter(MuonParameters::etcone20) / 1000.0,
		       (*muonItr)->parameter(MuonParameters::etcone30) / 1000.0,
		       (*muonItr)->parameter(MuonParameters::etcone40) / 1000.0,
		       (*muonItr)->parameter(MuonParameters::ptcone10) / 1000.0,
		       (*muonItr)->parameter(MuonParameters::ptcone20) / 1000.0,
		       (*muonItr)->parameter(MuonParameters::ptcone30) / 1000.0,
		       (*muonItr)->parameter(MuonParameters::ptcone40) / 1000.0,
		       (*muonItr)->fitChi2(),		       
		       (*muonItr)->fitChi2OverDoF(),		       
		       (*muonItr)->matchChi2(),		       
		       (*muonItr)->matchChi2OverDoF(),		       
		       caloEnergy ? caloEnergy->deltaE() / 1000.0 : -1.0,
		       caloEnergy ? caloEnergy->etCore() / 1000.0 : -1.0,
		       (*muonItr)->bestMatch(),
		       (*muonItr)->isCombinedMuon(),
		       (*muonItr)->isStandAloneMuon(),
		       (*muonItr)->isLowPtReconstructedMuon(),
		       (*muonItr)->isCaloMuonId(),
		       (*muonItr)->numberOfMDTHits(),
		       (*muonItr)->numberOfCSCEtaHits(),
		       (*muonItr)->numberOfCSCPhiHits(),
		       (*muonItr)->numberOfRPCEtaHits(),
		       (*muonItr)->numberOfRPCPhiHits(),
		       (*muonItr)->numberOfTGCEtaHits(),
		       (*muonItr)->numberOfTGCPhiHits(),
		       (int) (*muonItr)->numberOfSegments());
      muon->InitializeMuonQuality((*muonItr)->isLoose(),
				  (*muonItr)->isMedium(),
				  (*muonItr)->isTight(),
				  (*muonItr)->isMuonLikelihood());
      const Rec::TrackParticle* matched_track = (*muonItr)->inDetTrackParticle();
      //
      // If matched track is valid and above trackPtCut, loop over all tracks in tree and do matching
      //
      if (matched_track) {
	double trackPt = matched_track->pt() / 1000.0;
	muon->InitializeTrackIsolation(m_isolationTool->trackIsolationEnergy(matched_track, 0.1) / 1000.0,
				       m_isolationTool->trackIsolationEnergy(matched_track, 0.2) / 1000.0,
				       m_isolationTool->trackIsolationEnergy(matched_track, 0.3) / 1000.0,
				       m_isolationTool->trackIsolationEnergy(matched_track, 0.4) / 1000.0);
	if (trackPt > m_trackPtCut) {
	  int nrTracks = m_tracks->GetEntriesFast();
	  double epsilon = 0.001;
	  for (int j = 0; j != nrTracks; ++j) {
	    MTree::Track* track = (MTree::Track*) m_tracks->At(j);
	    if (TMath::Abs(track->Pt() - trackPt) > epsilon) continue; 
	    if (TMath::Abs(track->Eta() - matched_track->eta()) > epsilon) continue; 
	    if (TMath::Abs(track->Phi() - matched_track->phi()) > epsilon) continue;
	    muon->SetTrack(track);
	  }
	}
      }
      //Retrieve the spectrometer track, evalulated at the IP
      const Rec::TrackParticle* matched_StandAlone_track = 0;
      if (i == 0) matched_StandAlone_track = (*muonItr)->muonExtrapolatedTrackParticle();
      else if (i == 1) matched_StandAlone_track = (*muonItr)->muonSpectrometerTrackParticle();

      //
      // If matched muon spectrometer track is valid and above trackPtCut, loop over and do matching
      //
      if (matched_StandAlone_track) {
        mLog << MSG::DEBUG << " Found matched StandAlone muon track..." << endreq;
        double trackPt = matched_StandAlone_track->pt() / 1000.0;
        int nrStandAloneMuons = m_standAloneMuons->GetEntriesFast();
        double epsilon = 0.001;
        mLog << MSG::DEBUG << " Looping over " << nrStandAloneMuons << " StandAlone muons..." << endreq;
        for (int j = 0; j != nrStandAloneMuons; j++)
        {
          MTree::Track* saTrack = (MTree::Track*) m_standAloneMuons->At(j);
          if (TMath::Abs(saTrack->Pt() - trackPt) > epsilon) continue;
          if (TMath::Abs(saTrack->Eta() - matched_StandAlone_track->eta()) > epsilon) continue;
          if (TMath::Abs(saTrack->Phi() - matched_StandAlone_track->phi()) > epsilon) continue;
	  if (saTrack->TrackAuthor() != i+1) continue;
          mLog << MSG::DEBUG << " Setting StandAlone muon track..." << endreq;
          muon->SetStandAloneTrack(saTrack);
        }
      }


//do skimming selections
      if (m_applySkim)
      {
	mLog << MSG::DEBUG << "Applying pT smearing to muons" << endreq;
	MTree::Track* saTrack = muon->GetStandAloneTrack();
	MTree::Track* idTrack = muon->GetTrack();

	double sa_pt = !saTrack ? 0 : saTrack->Pt();
	double id_pt = !idTrack ? 0 : idTrack->Pt();
	double muon_pt = muon->Pt();
	double muon_eta = muon->Eta();

	if (m_smearLeptonPt)
	{
	  m_muSmear.SetSeed(m_eventInfo->EventNumber(), muon_index[i]++);
	  m_muSmear.Event(sa_pt, id_pt, muon_pt, muon_eta);
	
	  sa_pt = m_muSmear.pTMS();
	  id_pt = m_muSmear.pTID();
	  muon_pt = m_muSmear.pTCB();
	}

	if (muon_pt < m_leptonPtCut) continue;
	nrMuonsPassed[i]++;
      }


    }
    mLog << MSG::DEBUG << "   Finished looping over the Muons from " 
	 << currentContainerName << "." << endreq; 
  }
  

//done with electron and muon loop. if skimming, check if passed, don't bother otherwise
  bool passedSkim = false;
  for (int i=0;i<NrOfMuonContainers;i++)
  {
    if (nrMuonsPassed[i] + nrElectronsPassed >= m_nrLeptonsCut) passedSkim = true;
  }
  if (!passedSkim && m_applySkim) return StatusCode::SUCCESS;


  if (m_applySkim)
  {
    double eventWeight = m_eventInfo->EventWeight();
    if (eventWeight != 0)
    {
      int sign = eventWeight / std::fabs(eventWeight);

      if (sign > 0)
      {
	m_nPlusEventsPassed++;
      }
      else
      {
	m_nMinusEventsPassed++;
      }
    }
  }


  //
  // Fill missingET
  //
  const MissingET* met(0);
  sc = m_storeGate->retrieve(met, m_METContainerName);
  if (sc.isFailure() || !met) {
    mLog << MSG::ERROR << "No AOD MissingET container found in StoreGate" << endreq;
    return StatusCode::FAILURE;
  } 
  mLog << MSG::DEBUG << "Filling the MET." << endreq; 
  m_missingET->ClearVariables();
  m_missingET->Initialize(met->et() / 1000.0,
			  met->etx() / 1000.0,
			  met->ety() / 1000.0,
			  met->sumet() / 1000.0,
                          m_METContainerName.c_str());
  


  //
  // Fill MissingET Terms in m_missingETTerms
  //
  int NrOfMETTermContainers = m_METTermContainerNames.size();
    
  mLog << MSG::DEBUG << "Looping over the MET containers." << endreq; 
  m_missingETTerms->Clear();
  int mETTermCounter = 0;
  for (int i = 0; i != NrOfMETTermContainers; ++i) {
    const MissingET* metTermContainer(0);
    std::string currentContainerName;
    currentContainerName = m_METTermContainerNames.at(i);
    sc = m_storeGate->retrieve(metTermContainer, currentContainerName);
    if (sc.isFailure() || !metTermContainer) {
      mLog << MSG::ERROR << "No AOD MET Container called " << currentContainerName
           << " found in StoreGate" << endreq;
      return StatusCode::FAILURE;
    }
    MTree::MissingET* metTerm = 0;
    mLog << MSG::DEBUG << "   Adding MET Term " << currentContainerName << "." << endreq;
    metTerm = (MTree::MissingET*) m_missingETTerms->New(mETTermCounter++);
    metTerm->ClearVariables();
    metTerm->Initialize(metTermContainer->et() / 1000.0,
                        metTermContainer->etx() / 1000.0,
                        metTermContainer->ety() / 1000.0,
                        metTermContainer->sumet() / 1000.0,
                        currentContainerName.c_str());
    const MissingEtRegions* p_reg = metTermContainer->getRegions();
    if (p_reg) {
      metTerm->InitializeRegions(p_reg->exReg(MissingEtRegions::Central),
				 p_reg->exReg(MissingEtRegions::EndCap),
				 p_reg->exReg(MissingEtRegions::Forward),
				 p_reg->eyReg(MissingEtRegions::Central),
				 p_reg->eyReg(MissingEtRegions::EndCap),
				 p_reg->eyReg(MissingEtRegions::Forward),
				 p_reg->etSumReg(MissingEtRegions::Central),
				 p_reg->etSumReg(MissingEtRegions::EndCap),
				 p_reg->etSumReg(MissingEtRegions::Forward));
    }
  }
  //
  // Add METCalo terms
  //
  int NrOfMETCaloContainers = m_METCaloContainerNames.size();
  for (int i = 0; i != NrOfMETCaloContainers; ++i) {
    const MissingEtCalo* metTopo(0);
    std::string currentContainerName;
    currentContainerName = m_METCaloContainerNames.at(i);
    sc = m_storeGate->retrieve(metTopo, currentContainerName);
    if (sc.isFailure() || !metTopo) {
      mLog << MSG::ERROR << "No METCalo container called " << currentContainerName 
	   << " found in StoreGate" << endreq;
      return StatusCode::FAILURE;
    }
    MTree::MissingET* topo = 0;
    mLog << MSG::DEBUG << "Adding the METCalo term called " << currentContainerName << "." << endreq;
    topo = (MTree::MissingET*) m_missingETTerms->New(mETTermCounter++); 
    topo->ClearVariables();
    topo->Initialize(metTopo->et() / 1000.0,
		     metTopo->etx() / 1000.0,
		     metTopo->ety() / 1000.0,
		     metTopo->sumet() / 1000.0,
		     currentContainerName.c_str());
    const MissingEtRegions* p_reg = metTopo->getRegions();
    topo->InitializeRegions(p_reg->exReg(MissingEtRegions::Central),
			    p_reg->exReg(MissingEtRegions::EndCap),
			    p_reg->exReg(MissingEtRegions::Forward),
			    p_reg->eyReg(MissingEtRegions::Central),
			    p_reg->eyReg(MissingEtRegions::EndCap),
			    p_reg->eyReg(MissingEtRegions::Forward),
			    p_reg->etSumReg(MissingEtRegions::Central),
			    p_reg->etSumReg(MissingEtRegions::EndCap),
			    p_reg->etSumReg(MissingEtRegions::Forward));
  }
  mLog << MSG::DEBUG << "   Finished looping over the MET containers." << endreq; 

  //
  // Fill Trigger information for the Event
  //

  m_trigger->ClearVariables();
  std::vector<std::string>::iterator nameItr = m_ConfiguredL1Names.begin(), 
    nameItrEnd = m_ConfiguredL1Names.end();

  std::vector<std::string> passedNames;
  passedNames.reserve(200);
  mLog << MSG::DEBUG << "Filling the L1 Triggers." << endreq; 
  for (; nameItr != nameItrEnd; ++nameItr) {
    if (m_trigDec->isPassed(*nameItr, TrigDefs::Physics)) passedNames.push_back(*nameItr);
  }
  m_trigger->SetL1TriggersPassed(passedNames, m_IgnoreL1TriggerNames);

  for (int k = 0; k != (int)passedNames.size(); ++k)
  {
    const Trig::FeatureContainer feats = m_trigDec->features(passedNames[k]);

    std::vector<Trig::Feature<Muon_ROI> > L1muon = feats.get<Muon_ROI>();
  }

  passedNames.clear();

  mLog << MSG::DEBUG << "Filling the L2 Triggers." << endreq; 
  nameItr = m_ConfiguredL2Names.begin(), nameItrEnd = m_ConfiguredL2Names.end();
  for (; nameItr != nameItrEnd; ++nameItr) {
    if (m_trigDec->isPassed(*nameItr, TrigDefs::Physics)) passedNames.push_back(*nameItr);
  }
  m_trigger->SetL2TriggersPassed(passedNames, m_IgnoreHLTTriggerNames);
  passedNames.clear();

  mLog << MSG::DEBUG << "Filling the EF Triggers." << endreq; 
  nameItr = m_ConfiguredEFNames.begin(), nameItrEnd = m_ConfiguredEFNames.end();
  for (; nameItr != nameItrEnd; ++nameItr) {

    mLog << MSG::DEBUG << "Checking EF trigger: " << *nameItr << endreq; 
    if (m_trigDec->isPassed(*nameItr, TrigDefs::Physics)) 
    {
      passedNames.push_back(*nameItr);
      mLog << MSG::DEBUG << "Passed" << endreq; 
    }
    else
    {
      mLog << MSG::DEBUG << "Failed" << endreq; 
    }
      mLog << MSG::DEBUG << endreq; 
  }
  m_trigger->SetEFTriggersPassed(passedNames, m_IgnoreHLTTriggerNames);

  //
  // Start Filling the Trigger ROIs
  // 

  const LVL1_ROI* lvl1ROI;
  sc = m_storeGate->retrieve(lvl1ROI);
  if (sc.isFailure()) {  
    std::cout << "Could not find LVL1_ROI" << std::endl;
    return StatusCode::SUCCESS;
  }
  m_L1EmTauTriggerROIs->Clear();
  m_L1MuonTriggerROIs->Clear();
  int L1EmTauROICounter = 0;
  int L1MuonROICounter = 0;

  mLog << MSG::DEBUG << "Filling the L1 Triggers RoIs." << endreq; 
  // Start filling the EmTau L1 ROIs 
  LVL1_ROI::emtaus_type::const_iterator itEm   = (lvl1ROI->getEmTauROIs()).begin();
  LVL1_ROI::emtaus_type::const_iterator itEm_e = (lvl1ROI->getEmTauROIs()).end();
  for (; itEm != itEm_e; ++itEm) {
    MTree::L1TriggerROI* L1ROI = (MTree::L1TriggerROI*) m_L1EmTauTriggerROIs->New(L1EmTauROICounter++);
    // Setting the widths to 0.1! /Jonas
    L1ROI->Initialize(itEm->pt() / 1000.0, itEm->eta(), itEm->phi(), 0.1, 0.1, (int) itEm->getROIWord());
  }
  // Start filling the Muon L1 ROIs 
  LVL1_ROI::muons_type::const_iterator itMu   = (lvl1ROI->getMuonROIs()).begin();
  LVL1_ROI::muons_type::const_iterator itMu_e = (lvl1ROI->getMuonROIs()).end();
  for (; itMu != itMu_e; ++itMu) {
    MTree::L1TriggerROI* L1ROI = (MTree::L1TriggerROI*) m_L1MuonTriggerROIs->New(L1MuonROICounter++);
    // Setting the widths to 0.1! /Jonas
    L1ROI->Initialize(itMu->pt() / 1000.0, itMu->eta(), itMu->phi(), 0.1, 0.1, (int) itMu->getROIWord());
  }

  mLog << MSG::DEBUG << "Filling the HLT Triggers RoIs." << endreq; 
  m_HLTTriggerROIs->Clear();
  m_triggerObjects->Clear();
  int HLTROICounter = 0;
  int TrigObjCounter = 0;
  int NrTriggerChains = (int) m_HLTTriggerROINames.size();
  for (int i = 0; i != NrTriggerChains; ++i) {

    const Trig::FeatureContainer feats = m_trigDec->features(m_HLTTriggerROINames[i]);

    //
    //Adding electron objects associated with trigger to MTree
    //
    mLog << MSG::DEBUG << "Getting egammaContainer trigger objects" << endreq;
    std::vector<Trig::Feature<egammaContainer> > elecs = feats.get<egammaContainer>();
    std::vector<Trig::Feature<egammaContainer> >::const_iterator iter;
    std::vector<Trig::Feature<egammaContainer> >::const_iterator end = elecs.end();
    for (iter = elecs.begin(); iter != end; ++iter)
    {
      egammaContainer::const_iterator itrEg = iter->cptr()->begin();
      egammaContainer::const_iterator itrEgEnd = iter->cptr()->end();
      for (; itrEg != itrEgEnd; ++itrEg)
      {
	int NrTrigObjs = m_triggerObjects->GetEntriesFast();
	bool beenFound = false;
	for (int k = 0; k != NrTrigObjs; ++k)
	{
	  MTree::TriggerObject* trigObj = (MTree::TriggerObject*)m_triggerObjects->At(k);
	  if ((trigObj->Eta() == (*itrEg)->eta()) && (trigObj->Phi() == (*itrEg)->phi()))
	  {
	    beenFound = true;
	    trigObj->AddTriggerName(m_HLTTriggerROINames[i]);
	    trigObj->SetAuthor(1);
	  } 
	}

	if (!beenFound)
	{
	  MTree::TriggerObject* newObj = (MTree::TriggerObject*)m_triggerObjects->New(TrigObjCounter++);
	  double pT = (*itrEg)->pt()/1000;
	  newObj->Initialize(pT,(*itrEg)->eta(),(*itrEg)->phi());
	  newObj->SetAuthor(1);
	  newObj->AddTriggerName(m_HLTTriggerROINames[i]);
	}
      }
    }

    //
    //Adding muon objects associated with trigger to MTree
    //
    mLog << MSG::DEBUG << "Getting CombinedMuonFeature trigger objects" << endreq;
    std::vector<Trig::Feature<CombinedMuonFeature> > cmuons = feats.get<CombinedMuonFeature>();
    std::vector<Trig::Feature<CombinedMuonFeature> >::const_iterator muonCB;
    std::vector<Trig::Feature<CombinedMuonFeature> >::const_iterator endCB = cmuons.end();
    for (muonCB = cmuons.begin(); muonCB != endCB; ++muonCB)
    {
      const CombinedMuonFeature* cbMuonObj = (*muonCB);
      if (cbMuonObj != 0)
      {
	int NrTrigObjs = m_triggerObjects->GetEntriesFast();
	bool beenFound = false;
	for (int k = 0; k != NrTrigObjs; ++k)
	{
	  MTree::TriggerObject* trigObj = (MTree::TriggerObject*)m_triggerObjects->At(k);
	  if (trigObj->IsSame(cbMuonObj->pt()/1000,cbMuonObj->eta(),cbMuonObj->phi()))
	  {
	    beenFound = true;
	    trigObj->AddTriggerName(m_HLTTriggerROINames[i]);
	    trigObj->SetAuthor(2);
	  } 
	}

	if (!beenFound)
	{
	  MTree::TriggerObject* newObj = (MTree::TriggerObject*)m_triggerObjects->New(TrigObjCounter++);
	  double pT = cbMuonObj->pt()/1000;
	  if (pT > 100000)
	  {
	    pT = 0.01;
	  }
	  newObj->Initialize(pT,cbMuonObj->eta(),cbMuonObj->phi());
	  newObj->SetAuthor(2);
	  newObj->AddTriggerName(m_HLTTriggerROINames[i]);
	}
      }
    }

    mLog << MSG::DEBUG << "Getting TrigMuonEFInfoContainer trigger objects" << endreq;
    std::vector<Trig::Feature<TrigMuonEFInfoContainer> > efmuons = feats.get<TrigMuonEFInfoContainer>();
    std::vector<Trig::Feature<TrigMuonEFInfoContainer> >::const_iterator muonEF;
    std::vector<Trig::Feature<TrigMuonEFInfoContainer> >::const_iterator endEF = efmuons.end();
    for (muonEF = efmuons.begin(); muonEF != endEF; ++muonEF)
    {
      TrigMuonEFInfoContainer::const_iterator efItr = muonEF->cptr()->begin();
      TrigMuonEFInfoContainer::const_iterator efItrEnd = muonEF->cptr()->end();
      for (; efItr != efItrEnd; ++efItr)
      {
	int NrTrigObjs = m_triggerObjects->GetEntriesFast();

	//Behold, a loop that I spent a week trying to figure out how to write.
	//Because dealing with the AODs and Athena is that annoying.
	TrigMuonEFInfoTrackContainer* container = (*efItr)->TrackContainer();
	TrigMuonEFInfoTrackContainer::const_iterator trItr = container->begin();
	TrigMuonEFInfoTrackContainer::const_iterator trEnd = container->end();
	for (; trItr != trEnd; ++trItr)
	{
	  const TrigMuonEFCbTrack* cbtrack = (*trItr)->CombinedTrack();
	  if (cbtrack)
	  {
  	    bool beenFound = false;
	    for (int k = 0; k != NrTrigObjs; ++k)
	    {
	      MTree::TriggerObject* trigObj = (MTree::TriggerObject*)m_triggerObjects->At(k);
	      if (trigObj->IsSame(cbtrack->pt()/1000,cbtrack->eta(),cbtrack->phi()))
	      {
	        beenFound = true;
	        trigObj->AddTriggerName(m_HLTTriggerROINames[i]);
	        trigObj->SetAuthor(3);
	      }     
	    }

	    if (!beenFound)
	    {
	      MTree::TriggerObject* newObj = (MTree::TriggerObject*)m_triggerObjects->New(TrigObjCounter++);
	      double pT = cbtrack->pt()/1000;
	      if (pT > 100000)
	      {
	        pT = 0.01;
	      }
	      newObj->Initialize(pT,cbtrack->eta(),cbtrack->phi());
	      newObj->SetAuthor(3);
	      newObj->AddTriggerName(m_HLTTriggerROINames[i]);
	    }
          }
	  const TrigMuonEFTrack* mstrack = (*trItr)->SpectrometerTrack();
	  if (mstrack)
	  {
	    bool beenFound = false;
	    for (int k = 0; k != NrTrigObjs; ++k)
	    {
	      MTree::TriggerObject* trigObj = (MTree::TriggerObject*)m_triggerObjects->At(k);
	      if (trigObj->IsSame(mstrack->pt()/1000,mstrack->eta(),mstrack->phi()))
	      {
	        beenFound = true;
	        trigObj->AddTriggerName(m_HLTTriggerROINames[i]);
	        trigObj->SetAuthor(5);
	      }  
	    }

	    if (!beenFound)
	    {
	      MTree::TriggerObject* newObj = (MTree::TriggerObject*)m_triggerObjects->New(TrigObjCounter++);
	      double pT = mstrack->pt()/1000;
	      if (pT > 100000)
	      {
	        pT = 0.01;
	      }
	      newObj->Initialize(pT,mstrack->eta(),mstrack->phi());
	      newObj->SetAuthor(5);
	      newObj->AddTriggerName(m_HLTTriggerROINames[i]);
	    }
	  }
	  const TrigMuonEFTrack* extrack = (*trItr)->ExtrapolatedTrack();
	  if (extrack)
	  {
	    bool beenFound = false;
	    for (int k = 0; k != NrTrigObjs; ++k)
	    {
	      MTree::TriggerObject* trigObj = (MTree::TriggerObject*)m_triggerObjects->At(k);
	      if (trigObj->IsSame(extrack->pt()/1000,extrack->eta(),extrack->phi()))
	      {
	        beenFound = true;
	        trigObj->AddTriggerName(m_HLTTriggerROINames[i]);
	        trigObj->SetAuthor(6);
	      }  
	    }

	    if (!beenFound)
	    {
	      MTree::TriggerObject* newObj = (MTree::TriggerObject*)m_triggerObjects->New(TrigObjCounter++);
	      double pT = extrack->pt()/1000;
	      if (pT > 100000)
	      {
	        pT = 0.01;
	      }
	      newObj->Initialize(pT,extrack->eta(),extrack->phi());
	      newObj->SetAuthor(6);
	      newObj->AddTriggerName(m_HLTTriggerROINames[i]);
	    }
	  }
	}
      }
    }

    mLog << MSG::DEBUG << "Getting MuonFeature trigger objects" << endreq;
    std::vector<Trig::Feature<MuonFeature> > trigmuons = feats.get<MuonFeature>();
    std::vector<Trig::Feature<MuonFeature> >::const_iterator trmItr = trigmuons.begin();
    std::vector<Trig::Feature<MuonFeature> >::const_iterator trmEnd = trigmuons.end();
    for (; trmItr != trmEnd; ++trmItr)
    {
      const MuonFeature* muonFeatObj = (*trmItr);
      if (muonFeatObj)
      {
	int NrTrigObjs = m_triggerObjects->GetEntriesFast();
	bool beenFound = false;
	for (int k = 0; k != NrTrigObjs; ++k)
	{
	  MTree::TriggerObject* trigObj = (MTree::TriggerObject*)m_triggerObjects->At(k);
	  if (trigObj->IsSame(muonFeatObj->pt(),muonFeatObj->eta(),muonFeatObj->phi()))
	  {
	    beenFound = true;
	    trigObj->AddTriggerName(m_HLTTriggerROINames[i]);
	    trigObj->SetAuthor(4);
	  } 
	}

	if (!beenFound)
	{
	  MTree::TriggerObject* newObj = (MTree::TriggerObject*)m_triggerObjects->New(TrigObjCounter++);
	  double pT = muonFeatObj->pt();
	  if (pT > 100000)
	  {
	    pT = 0.01;
	  }
	  newObj->Initialize(pT,muonFeatObj->eta(),muonFeatObj->phi());
	  newObj->SetAuthor(4);
	  newObj->AddTriggerName(m_HLTTriggerROINames[i]);
	}
      }
    }

    //mLog << MSG::INFO << "TAG::Looking at " << endreq;
    //
    // For single object triggers, combinations are really equal to RoIs. 
    // This gets all combinations/RoIs in the event.
    //
    const std::vector<Trig::Combination> combinations = feats.getCombinations(); 
    int NrCombinations = combinations.size();
    for (int j = 0; j != NrCombinations; ++j) {
      std::vector<Trig::Feature<TrigRoiDescriptor> > rois = combinations[j].get<TrigRoiDescriptor>("initialRoI");

      //
      // Should always find exactly one RoI for single object triggers
      //
      std::vector<Trig::Feature<TrigRoiDescriptor> >::const_iterator tempRoI = rois.begin();
//       if (tempRoI != rois.end()) {
      for (; tempRoI != rois.end(); ++tempRoI)
      {
	const TrigRoiDescriptor* roi = tempRoI->cptr(); 
	int CurrentROIWord = roi->roiWord();
	//
	// We have found our RoI, check if it already exists (because of some other trigger)
	//
	bool existedBefore = false;
	int NrHLTROIs = m_HLTTriggerROIs->GetEntriesFast();
	for (int k = 0; k != NrHLTROIs; ++k) {
	  MTree::HLTTriggerROI* HLTROI = (MTree::HLTTriggerROI*) m_HLTTriggerROIs->At(k);
	  if (HLTROI->ROIWord() == CurrentROIWord) {
	    HLTROI->AddROIName(m_HLTTriggerROINames[i]);
	    existedBefore = true;
	  }
	}
	//
	// If the RoI does not exist before, add it to the MTree
	//
	if (!existedBefore) {
	  MTree::HLTTriggerROI* HLTROI = (MTree::HLTTriggerROI*) m_HLTTriggerROIs->New(HLTROICounter++);
	  HLTROI->Initialize(999.0, roi->eta0(), roi->phi0(), roi->etaHalfWidth(), 
			     roi->phiHalfWidth(), CurrentROIWord);
	  HLTROI->AddROIName(m_HLTTriggerROINames[i]);

	}
      }
    }
  }

  //
  // Fill Truth Information for MC
  //
  
  const McEventCollection* McEventColl(0);
  sc = m_storeGate->retrieve(McEventColl, m_MCTruthContainerName);
  m_truthParticles->Clear();
  mLog << MSG::DEBUG << "Start Filling the MC Truth." << endreq; 
  if (!sc.isFailure() && McEventColl) {
    McEventCollection::const_iterator itMcEvt = McEventColl->begin(), itMcEvtEnd = McEventColl->end();
    if (itMcEvt != itMcEvtEnd) {
      HepMC::GenEvent* genEvent(*itMcEvt);
      if (genEvent)
      {
	HepMC::WeightContainer weightContainer = genEvent->weights();
	int PdfID1 = 0, PdfID2 = 0;
	double PdfX1 = -1.0, PdfX2 = -1.0, PdfScale = -1.0, Pdf1 = -1.0, Pdf2 = -1.0;
	if (genEvent->pdf_info()) {
	  PdfID1 = genEvent->pdf_info()->id1();
	  PdfID2 = genEvent->pdf_info()->id2();
	  PdfX1 = genEvent->pdf_info()->x1();
	  PdfX2 = genEvent->pdf_info()->x2();
	  PdfScale = genEvent->pdf_info()->scalePDF();
	  Pdf1 = genEvent->pdf_info()->pdf1();
	  Pdf2 = genEvent->pdf_info()->pdf2();
	}
	double weight_0 = weightContainer.size() != 0 ? weightContainer[0] : 0;
	m_eventInfo->SetGeneratorInfo(weight_0,
				      genEvent->signal_process_id(),
				      genEvent->event_number(),
				      genEvent->event_scale(),
				      genEvent->alphaQCD(),
				      genEvent->alphaQED(),
				      PdfID1,
				      PdfID2,
				      PdfX1,
				      PdfX2,
				      PdfScale,
				      Pdf1,
				      Pdf2);
      }
      else
      {
	mLog << MSG::DEBUG << "WARNING: genEvent returned NULL" << endreq; 
      }
      //
      // Define a vector of interesting bosons. This vector has to be sorted!
      // All of these particles will be saved in the Tree, together with interesting
      // daughter particles. Will also store high pT photons.
      //
      std::vector<int> interestingParticles;
      interestingParticles.reserve(8);
      interestingParticles.push_back(6);  // t
      interestingParticles.push_back(23); // Z
      interestingParticles.push_back(24); // W
      interestingParticles.push_back(25); // H
      interestingParticles.push_back(32); // Z'
      interestingParticles.push_back(34); // W'
      interestingParticles.push_back(1000022); // Neutralino_1
      interestingParticles.push_back(1000039); // Gravitino
      std::vector<int>::iterator itIPBegin = interestingParticles.begin(), itIPEnd = interestingParticles.end(); 

      TClonesArray* truthParticles = new TClonesArray("MTree::TruthParticle", 20);
      int truthParticleCounter = 0;
      mLog << MSG::DEBUG << "   Start first loop over truth particles." << endreq; 
      int NrMcEvts = 0;
      //
      // Start a loop over MCEvent containers, could be just one in some (the most natural) cases.
      //
      for (; itMcEvt != itMcEvtEnd; ++itMcEvt) {
	++NrMcEvts;
	mLog << MSG::DEBUG << "      Starting with McEvt nr " << NrMcEvts << "." << endreq; 
	genEvent = (*itMcEvt);
	HepMC::GenEvent::particle_const_iterator it = genEvent->particles_begin(),
	  it_end = genEvent->particles_end();
	//
	// Start looping over all truth particles in the AOD.
	//
	for (; it!=it_end; ++it) {
	  int signed_pdg_id = (*it)->pdg_id();
	  int pdg_id = TMath::Abs(signed_pdg_id);
	  int status = (*it)->status();

	  int pdg_id2 = pdg_id;
	  if (pdg_id > 10000) pdg_id2 = pdg_id % 10000; //Excited b- or c-hadrons

	  //
	  // Find c-hadrons
	  //
	  if ( (pdg_id  >= 400 && pdg_id  < 500) || (pdg_id  >= 4000 && pdg_id  < 5000) ||
	       (pdg_id2 >= 400 && pdg_id2 < 500) ) {
	    HepMC::FourVector mom = (*it)->momentum();
	    TLorentzVector chadron;
	    chadron.SetPxPyPzE(mom.px(), mom.py(), mom.pz(), mom.e());
	    int nrJets = m_jets->GetEntriesFast();
	    for (int i = 0; i != nrJets; ++i) {
	      MTree::Jet* jet = (MTree::Jet*) m_jets->At(i);
	      if (TMath::Abs(jet->DeltaR(chadron)) < 0.4) {
		if (jet->JetFlavor() < 4) jet->SetJetFlavor(4);
	      }
	    }
	  }

	  //
	  // Find b-hadrons
	  //
	  if ( (pdg_id  >= 500 && pdg_id  < 600) || (pdg_id  >= 5000 && pdg_id  < 6000) ||
	       (pdg_id2 >= 500 && pdg_id2 < 600) ) {
	    HepMC::FourVector mom = (*it)->momentum();
	    TLorentzVector bhadron;
	    bhadron.SetPxPyPzE(mom.px(), mom.py(), mom.pz(), mom.e());
	    int nrJets = m_jets->GetEntriesFast();
	    for (int i = 0; i != nrJets; ++i) {
	      MTree::Jet* jet = (MTree::Jet*) m_jets->At(i);
	      if (TMath::Abs(jet->DeltaR(bhadron)) < 0.4) {
		if (jet->JetFlavor() < 5) jet->SetJetFlavor(5);
	      }
	    }
	  }

	  //
	  // Do leading parton matching to jets
	  //
	  if ( (pdg_id  >= 1 && pdg_id <= 5) || pdg_id == 15 || pdg_id == 21 ) {
	    HepMC::FourVector mom = (*it)->momentum();
	    if (mom.perp() > 500.0) {
	      TLorentzVector parton;
	      parton.SetPxPyPzE(mom.px(), mom.py(), mom.pz(), mom.e());
	      double partonPt = parton.Pt() / 1000.0;
	      int nrJets = m_jets->GetEntriesFast();
	      for (int i = 0; i != nrJets; ++i) {
		MTree::Jet* jet = (MTree::Jet*) m_jets->At(i);
		if (TMath::Abs(jet->DeltaR(parton)) < 0.4) {
		  if (partonPt > jet->LeadingPartonPt()) {
		    jet->SetLeadingPartonPdgID(pdg_id);
		    jet->SetLeadingPartonPt(partonPt);
		  }
		}
	      }
	    }	    
	  }

	  //
	  // Find Interesting particles and b-quarks from tops
	  // 
	  if (binary_search(itIPBegin, itIPEnd, pdg_id) || pdg_id == 5) {
	    //
	    // Store all interesting bosons
	    //
	    HepMC::FourVector mom = (*it)->momentum();
	    HepMC::GenVertex* vertex = (*it)->production_vertex();
	    if (pdg_id == 5) {
	      if (!vertex) continue; // No parent
	      if (vertex->particles_in_size() != 1) continue; // Require exactly one parent
	      int parentID = TMath::Abs((*(vertex->particles_begin(HepMC::parents)))->pdg_id());
	      if (parentID != 6) continue;
	    }
	    MTree::TruthParticle* particle = 
	      (MTree::TruthParticle*) truthParticles->New(truthParticleCounter++);
	    particle->Initialize(mom.perp() / 1000.0, 
				 mom.eta(), 
				 mom.phi(), 
				 mom.m() / 1000.0, 
				 signed_pdg_id, 
				 status);	    
	    if (!vertex) continue; // No parent
	    if (vertex->particles_in_size() != 1) continue; // Require exactly one parent
	    int signed_parentID = (*(vertex->particles_begin(HepMC::parents)))->pdg_id();
	    int parentID = TMath::Abs(signed_parentID);
	    if (binary_search(itIPBegin, itIPEnd, parentID)) {
	      particle->SetParentPdgID(signed_parentID);
	    }
	  }

	  //
	  // Find leptons and photons
	  //
	  if ((pdg_id >= 11 && pdg_id < 17) || pdg_id == 22) {
	    HepMC::GenVertex* vertex = (*it)->production_vertex();
	    if (!vertex) continue; // Do not store orphans 
	    if (vertex->particles_in_size() != 1) continue; // Should not happen
	    int signed_parentID = (*(vertex->particles_begin(HepMC::parents)))->pdg_id();
	    int parentID = TMath::Abs(signed_parentID);
	    if (pdg_id == 22) {
	      HepMC::FourVector mom = (*it)->momentum();
	      if (mom.perp() < 500.0) continue; 
	    }
	    //
	    // Store electrons and muons from interesting bosons or taus, b and c
	    // For b and c parents, store the PDG ids but not the particles or links.
	    //
	    if (binary_search(itIPBegin, itIPEnd, parentID) || parentID == 15 || parentID == 5 || parentID == 4) {
	      //
	      // If lepton from b or c, only interested in e or mu
	      //
	      if (parentID == 4 || parentID == 5) {
		if (pdg_id != 11 && pdg_id != 13) continue;
	      }
	      //
	      // Check grand parent in case of tau or c parent
	      // Only store leptons from taus with interesting parents
	      //
	      int signed_grandParentID = 0;
	      int grandParentID = 0;
	      if (parentID == 15 || parentID == 4) {
		HepMC::GenParticle* parent = *(vertex->particles_begin(HepMC::parents));
		HepMC::GenVertex* parent_vertex = parent->production_vertex();
		if (parent_vertex && parent_vertex->particles_in_size() == 1) {
		  signed_grandParentID = (*(parent_vertex->particles_begin(HepMC::parents)))->pdg_id();
		  grandParentID = TMath::Abs(signed_grandParentID);
		}
		if (parentID == 15 && (!binary_search(itIPBegin, itIPEnd, grandParentID))) continue;
	      }
	      HepMC::FourVector mom = (*it)->momentum();
	      MTree::TruthParticle* particle = (MTree::TruthParticle*) truthParticles->New(truthParticleCounter++);
	      particle->Initialize(mom.perp() / 1000.0, 
				   mom.eta(), 
				   mom.phi(), 
				   mom.m() / 1000.0, 
				   signed_pdg_id, 
				   status, 
				   signed_parentID);
	      if (parentID == 15) particle->SetGrandParentPdgID(signed_grandParentID);
	      if (parentID == 4 && grandParentID == 5) particle->SetGrandParentPdgID(signed_grandParentID);
	    }
	    else if (pdg_id == 22) {
	      //
	      // Store high pT photons even if they don't come from interesting particles.
	      // These photons will be stored as orphans in the MTree.
	      //
	      HepMC::FourVector mom = (*it)->momentum();
	      if (mom.perp() < m_photonPtCut * 1000.0) continue;
	      	      MTree::TruthParticle* particle = (MTree::TruthParticle*) truthParticles->New(truthParticleCounter++);
	      particle->Initialize(mom.perp() / 1000.0, 
				   mom.eta(), 
				   mom.phi(), 
				   mom.m() / 1000.0, 
				   signed_pdg_id, 
				   status, 
				   0);
	    }
	  }
	}
      }

      //
      // Second loop to fill in parent links
      //
      mLog << MSG::DEBUG << "   Start second loop over truth particles." << endreq; 
      for (itMcEvt = McEventColl->begin(); itMcEvt != itMcEvtEnd; ++itMcEvt) {
	genEvent = (*itMcEvt);
	HepMC::GenEvent::particle_const_iterator it = genEvent->particles_begin(),
	  it_end = genEvent->particles_end();
	//
	// Start looping over all truth particles in the AOD.
	//
	for (; it!=it_end; ++it) {
	  int signed_pdg_id = (*it)->pdg_id();
	  int pdg_id = TMath::Abs(signed_pdg_id);	  
	  //
	  // Find any added particles
	  // 
	  if (binary_search(itIPBegin, itIPEnd, pdg_id) ||
	      (pdg_id >= 11 && pdg_id < 17) ||
	      pdg_id == 5 || pdg_id == 22) {
	    if (pdg_id == 22) {
	      HepMC::FourVector mom = (*it)->momentum();
	      if (mom.perp() < 500.0) continue; 
	    }
	    HepMC::GenVertex* vertex = (*it)->production_vertex();
	    if (!vertex) continue; // No parent
	    if (vertex->particles_in_size() != 1) continue; // Require exactly one parent
	    HepMC::GenParticle* parent = *(vertex->particles_begin(HepMC::parents));
	    int signed_parentID = parent->pdg_id();
	    int parentID = TMath::Abs(signed_parentID);
	    if (pdg_id == 5 && parentID != 6) continue;  // Only stored b from t
	    if (binary_search(itIPBegin, itIPEnd, parentID) || parentID == 15) {
	      //
	      // If the parent is a tau, make sure it came from an interesting grandparent
	      //
	      int grandParentID = 0;
	      if (parentID == 15) {
		HepMC::GenParticle* tau = *(vertex->particles_begin(HepMC::parents));
		HepMC::GenVertex* parent_vertex = tau->production_vertex();
		if (parent_vertex && parent_vertex->particles_in_size() == 1) {
		  grandParentID = TMath::Abs((*(parent_vertex->particles_begin(HepMC::parents)))->pdg_id());
		}
		if (!binary_search(itIPBegin, itIPEnd, grandParentID)) continue;
	      }
	      HepMC::FourVector mom = (*it)->momentum();
	      int status = (*it)->status();     
	      HepMC::FourVector parent_mom = parent->momentum();
	      int parent_status = parent->status();
	      HepMC::GenVertex* parent_vertex = parent->production_vertex();
	      int signed_grandParentID = 0;
	      if (parent_vertex && parent_vertex->particles_in_size() == 1) signed_grandParentID = (*(parent_vertex->particles_begin(HepMC::parents)))->pdg_id();
	      MTree::TruthParticle* this_particle = FindParticle(truthParticles, signed_pdg_id, status, signed_parentID, mom);
	      if (!this_particle) {
		mLog << MSG::ERROR << "Inconsistency in truth algorithm!" << endreq;
		mLog << MSG::ERROR << "When looking for the parent, find a particle not stored in the tree. Current particle is:" << endreq;
		mLog << MSG::ERROR << "PDG Id: " << signed_pdg_id << ", status: " << status << ", parent ID: " << signed_parentID << endreq;
		mLog << MSG::ERROR << "Current particle kinematics: x = " << (mom.x() / 1000.0) << ", y = " << (mom.y() / 1000.0) 
		     << ", z = " << (mom.z() / 1000.0) << ", t = " << (mom.t() / 1000.0) << endreq;
		mLog << MSG::ERROR << "Stored truth particles are:" << endreq;	
		int nrParticles = truthParticles->GetEntriesFast();
		for (int i = 0; i != nrParticles; ++i) {
		  MTree::TruthParticle* particle = (MTree::TruthParticle*) truthParticles->At(i);
		  mLog << MSG::ERROR << "  PDG Id: " << particle->PdgID() << ", Status: " << particle->Status() 
		       << ", Parent ID: " << particle->ParentPdgID()
		       << ", x = " << particle->X() << ", y = " << particle->Y() << ", z = " << particle->Z() << ", t = " 
		       << particle->T() << endreq; 
		  
		}		
		return StatusCode::FAILURE;
	      }
	      MTree::TruthParticle* parent_particle = FindParticle(truthParticles, signed_parentID, parent_status, 
								   signed_grandParentID, parent_mom);
	      if (!parent_particle) {
		mLog << MSG::ERROR << "Inconsistency in truth algorithm, for the parent!" << endreq;
                mLog << MSG::ERROR << "Looking for the parent, but it is not stored. Current particle is:" << endreq;
                mLog << MSG::ERROR << "PDG Id: " << signed_pdg_id << ", parent ID: " << signed_parentID 
		     << ", parent Status: " << parent_status << ", grand parent ID: " << signed_grandParentID << endreq;
		mLog << MSG::ERROR << "Parent particle kinematics: x = " << (parent_mom.x() / 1000.0) << ", y = " 
		     << (parent_mom.y() / 1000.0) << ", z = " << (parent_mom.z() / 1000.0) << ", t = " << (parent_mom.t() / 1000.0) << endreq;
		mLog << MSG::ERROR << "Stored truth particles are:" << endreq;	
		int nrParticles = truthParticles->GetEntriesFast();
		for (int i = 0; i != nrParticles; ++i) {
		  MTree::TruthParticle* particle = (MTree::TruthParticle*) truthParticles->At(i);
		  mLog << MSG::ERROR << "  PDG Id: " << particle->PdgID() << ", Status: " << particle->Status() 
		       << ", Parent ID: " << particle->ParentPdgID()
		       << ", x = " << particle->X() << ", y = " << particle->Y() << ", z = " << particle->Z() << ", t = " 
		       << particle->T() << endreq; 
		  
		}		
		return StatusCode::FAILURE;
	      }
	      this_particle->SetParent(parent_particle);
	    }	    
	  }
	}
      }
      mLog << MSG::DEBUG << "   Setting the daughter links of truthParticles." << endreq; 
      SetDaughterLinks(truthParticles);
      mLog << MSG::DEBUG << "   Flagging duplicate truth particles." << endreq; 
      FlagDuplicateTruthParticles(truthParticles);
      mLog << MSG::DEBUG << "   Transferring the truth particles." << endreq; 
      TransferTruthParticles(truthParticles);
      mLog << MSG::DEBUG << "   Setting the daughter links in m_truthParticles." << endreq; 
      SetDaughterLinks(m_truthParticles);
      delete truthParticles;
    }
  }
  mLog << MSG::DEBUG << "Done with MC Truth. Now just filling the MTree." << endreq; 

  m_tree->Fill();  
    
  return StatusCode::SUCCESS;
}

MTree::TruthParticle* ConvertAODToMTree::FindParticle(TClonesArray* particles, int pdg_id, int status, 
						      int parent_pdg_id, HepMC::FourVector& mom) {
  double x = mom.x() / 1000.0, y = mom.y() / 1000.0, z = mom.z() / 1000.0, t = mom.t() / 1000.0;
  int nrParticles = particles->GetEntriesFast();
  double epsilon = 0.0001;
  for (int i = 0; i != nrParticles; ++i) {
    MTree::TruthParticle* particle = (MTree::TruthParticle*) particles->At(i);
    if ( (TMath::Abs(particle->X() - x) < epsilon) &&
	 (TMath::Abs(particle->Y() - y) < epsilon) &&
	 (TMath::Abs(particle->Z() - z) < epsilon) &&
	 (TMath::Abs(particle->T() - t) < epsilon) &&
	 ((pdg_id - particle->PdgID()) == 0)       &&
	 (((parent_pdg_id - particle->ParentPdgID()) == 0) || particle->ParentPdgID() == 0) &&
	 ((status - particle->Status()) == 0)) return particle;
    
  }
  return 0;
}

void ConvertAODToMTree::SetDaughterLinks(TClonesArray* particles) {
  int NrParticles = particles->GetEntriesFast();
  for (int i = 0; i != NrParticles; ++i) {
    MTree::TruthParticle* particle = (MTree::TruthParticle*) particles->At(i);
    MTree::TruthParticle* parent = particle->GetParent();
    if (!parent) continue;
    parent->AddDaughter(particle);
  }  
}

void ConvertAODToMTree::FlagDuplicateTruthParticles(TClonesArray* particles) {
  int NrParticles = particles->GetEntriesFast();
  for (int i = 0; i != NrParticles; ++i) {
    MTree::TruthParticle* particle = (MTree::TruthParticle*) particles->At(i);
    if (particle->GetNrOfDaughters() == 0 && particle->GetParent() == 0) {
      if (particle->PdgID() != 22) particle->SetParentPdgID(-999);
    }    
    if (particle->PdgID() == particle->ParentPdgID()) {
      MTree::TruthParticle* parent = particle->GetParent();
      //
      // Move up all the daughters of the particle to the parent of the particle.
      //
      int NrDaughters = particle->GetNrOfDaughters();
      for (int j = 0; j != NrDaughters; ++j) {
	MTree::TruthParticle* daughter = particle->GetDaughter(j);
	//
	// Transfer this daughter to the parent. First check for duplicates.
	//
	int NrParentDaughters = parent->GetNrOfDaughters();
	int DuplicateDaughterIndex = -1;
	for (int k = 0; k != NrParentDaughters; ++k) {
	  MTree::TruthParticle* parentDaughter = parent->GetDaughter(k);
	  if (parentDaughter == particle) continue;	    
	  if (IsIdenticalTruthParticles(daughter, parentDaughter, false) ) {
	    // Flag the old daughter for deletion.
	    parentDaughter->SetParentPdgID(-999);
	    parentDaughter->SetParent(0);
	    // Rely on only finding one match
	    DuplicateDaughterIndex = k;
	  }
	}
	// Transfer the parentage
	daughter->SetParent(parent);
	daughter->SetParentPdgID(parent->PdgID());
	daughter->SetGrandParentPdgID(parent->ParentPdgID());
	if (DuplicateDaughterIndex == -1)
	  parent->AddDaughter(daughter);
	else 
	  parent->AddDaughterAt(daughter, DuplicateDaughterIndex);
      }
      // Flag the particle for deletion
      particle->SetParentPdgID(-999);
      particle->SetParent(0); //saw off the tree here
    }      
  }
  //
  // Second loop to flag all particles originating from deleted particles
  //
  for (int i = 0; i != NrParticles; ++i) {
    MTree::TruthParticle* particle = (MTree::TruthParticle*) particles->At(i);    
    int parentPdgID = particle->ParentPdgID();
    if (parentPdgID == -999 || parentPdgID == 0) continue;
    MTree::TruthParticle* parent = particle->GetParent();
    MTree::TruthParticle* tempParticle = particle;
    while (parentPdgID != -999 && parentPdgID != 0) {
      tempParticle = parent;
      parentPdgID = tempParticle->ParentPdgID();
      parent = tempParticle->GetParent();
    }
    if (parentPdgID == -999) {
      particle->SetParentPdgID(-999);
      particle->SetParent(0); //saw off the tree here
    }
  }

}

bool ConvertAODToMTree::IsIdenticalTruthParticles(MTree::TruthParticle* particle1, MTree::TruthParticle* particle2, 
						  bool DoStatusMatch) {
  double epsilon = 0.0001;
  if ( (TMath::Abs(particle1->X() - particle2->X()) < epsilon) &&
       (TMath::Abs(particle1->Y() - particle2->Y()) < epsilon) &&
       (TMath::Abs(particle1->Z() - particle2->Z()) < epsilon) &&
       (TMath::Abs(particle1->T() - particle2->T()) < epsilon) &&
       (particle1->PdgID() == particle2->PdgID()) ) {
    if (!DoStatusMatch || particle1->Status() == particle2->Status())   return true;
  }
  return false;
}

void ConvertAODToMTree::TransferTruthParticles(TClonesArray* particles) {
  int particleCounter = 0;
  int NrInitialParticles = particles->GetEntriesFast();
  for (int i = 0; i != NrInitialParticles; ++i) {
    MTree::TruthParticle* initialParticle = (MTree::TruthParticle*) particles->At(i);
    if (initialParticle->ParentPdgID() == -999) continue;
    MTree::TruthParticle* particle = (MTree::TruthParticle*) m_truthParticles->New(particleCounter++);
    particle->Initialize(initialParticle->Pt(), 
			 initialParticle->Eta(), 
			 initialParticle->Phi(), 
			 initialParticle->M(), 
			 initialParticle->PdgID(), 
			 initialParticle->Status(),
			 initialParticle->ParentPdgID(), 
			 initialParticle->GrandParentPdgID());
  }
  for (int i = 0; i != NrInitialParticles; ++i) {
    MTree::TruthParticle* initialParticle = (MTree::TruthParticle*) particles->At(i);
    if (initialParticle->ParentPdgID() == -999) continue;
    MTree::TruthParticle* initialParticleParent = initialParticle->GetParent();
    if (!initialParticleParent) continue;
    MTree::TruthParticle* particle = GetMatchingTruthParticle(initialParticle);
    MTree::TruthParticle* parent = GetMatchingTruthParticle(initialParticleParent);
    particle->SetParent(parent);
  }
}

MTree::TruthParticle* ConvertAODToMTree::GetMatchingTruthParticle(MTree::TruthParticle* inputParticle) {
  int NrParticles = m_truthParticles->GetEntriesFast();
  for (int i = 0; i != NrParticles; ++i) {
    MTree::TruthParticle* particle = (MTree::TruthParticle*) m_truthParticles->At(i);
    if (IsIdenticalTruthParticles(inputParticle, particle)) return particle;
  }
  return 0;
}

