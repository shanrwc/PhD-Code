# Author: Jonas Strandberg
# CERN, October 10, 2008

from AthenaCommon.AlgSequence import AlgSequence 
topSequence = AlgSequence()

import EventKernel.ParticleDataType
import AthenaPoolCnvSvc.ReadAthenaPool

# the POOL converters
include( "ParticleBuilderOptions/ESD_PoolCnv_jobOptions.py" )
include( "ParticleBuilderOptions/AOD_PoolCnv_jobOptions.py" )
include( "ParticleBuilderOptions/McAOD_PoolCnv_jobOptions.py" )
include( "EventAthenaPool/EventAthenaPool_joboptions.py" )

#
# Write out summary of time spent, if not wanted just uncomment
#
theAuditorSvc.Auditors  += [ "NameAuditor"]

if RunSite == "UM":
  include("RecJobTransforms/UseOracle.py")
    
# get a handle on the ServiceManager which holds all the services
from AthenaCommon.AppMgr import ServiceMgr
from AthenaCommon.DetFlags import DetFlags

# configure MCTruth
from MCTruthClassifier.MCTruthClassifierBase import MCTruthClassifier
MCTruthClassifier.McEventCollection = "GEN_AOD"

# Set the event selector to the same input specified before
ServiceMgr.EventSelector.InputCollections = jp.AthenaCommonFlags.FilesInput()

# Particle Properties
from PartPropSvc.PartPropSvcConf import PartPropSvc

# -- extrapolation to propagate tracks to primary vertex:
from TrkExTools.AtlasExtrapolator import AtlasExtrapolator
AtlasExtrapolator = AtlasExtrapolator()
ToolSvc += AtlasExtrapolator 


#Jet/ParticleJet backwards compatability
include( "TrackIsolationTools/TrackIsolationTool_jobOptions.py" )


# set up trigger decision tool
from TrigDecision.TrigDecisionConfig import TrigDecisionTool
tdt = TrigDecisionTool()
ToolSvc += tdt

from TriggerJobOpts.TriggerFlags import TriggerFlags
TriggerFlags.configurationSourceList = ['ds']

# set up trigger config service
from TriggerJobOpts.TriggerConfigGetter import TriggerConfigGetter
cfg =  TriggerConfigGetter()

#set up jet vtx helper thingy to help the jet vertex fraction thingy
##from JetMomentTools.SetupJetMomentTools import getDefaultJetVtxTrackHelper
##jhelp = getDefaultJetVtxTrackHelper()
##ToolSvc += jhelp

#set up jet vertex fraction calculator thingy
from JetMomentTools.SetupJetMomentTools import getJetVertexAssociationTool
jvatool = getJetVertexAssociationTool('AntiKt',0.4,'Topo')
#according to twiki page, the parameters are not critical; the tool will work for all jets.
ToolSvc += jvatool

# Re-run the MissingET
if RedoMissingET:
  include("MissingET/MissingET_jobOptions.py")

# Re-run the b-tagging
if RedoBTagging:
  BTagJetContainerName = [ JetContainer ]
  if not 'BTaggingFlags' in dir():
    from BTagging.BTaggingFlags import BTaggingFlags
  BTaggingFlags.Jets                             = BTagJetContainerName[:]
  BTaggingFlags.JetsWithInfoPlus                 = BTagJetContainerName[:]
  include( "BTagging/BTagging_jobOptions.py" )

# Import the MTree
from MTree.MTreeConf  import ConvertAODToMTree
topSequence += ConvertAODToMTree()
ConvertAODToMTree = ConvertAODToMTree()

ConvertAODToMTree.JetVertexAssociationTool = jvatool

############### The properties of the ConvertAODToMTree Algorithm

# Name of output file
ConvertAODToMTree.RootFileName = "MTree.root"
# AOD Container Names
ConvertAODToMTree.VtxContainerName = "VxPrimaryCandidate"
JetContainer += "Jets"
if RedoBTagging:
  JetContainer += "AOD"
ConvertAODToMTree.JetContainerName = JetContainer
ConvertAODToMTree.TrackContainerName = "TrackParticleCandidate"
ConvertAODToMTree.ElectronContainerName = "ElectronAODCollection"
ConvertAODToMTree.MuonContainerNames = ["StacoMuonCollection", "MuidMuonCollection"]
ConvertAODToMTree.StandaloneMuonContainerNames = ["MuonboyTrackParticles", "MooreTrackParticles"]
ConvertAODToMTree.METContainerName = "MET_RefFinal"
ConvertAODToMTree.MCTruthContainerName = "GEN_AOD"
if ApplyMTreeSelections:
  ConvertAODToMTree.IgnoreL1TriggerNames = ["TAU", "J", "XE", "TE", "RD0", "MBTS"]
  ConvertAODToMTree.IgnoreHLTTriggerNames = ["tau", "j", "J", "xe", "te", "b", "MbTrk", "MbSp", "trk"]
ConvertAODToMTree.HLTTriggerROINames = ["L2_e10_medium", "L2_e20_loose", "EF_e10_medium", "EF_e20_loose", "L2_e20_medium", "EF_e20_medium", "EF_e22_medium","L2_mu10", "L2_mu20", "EF_mu10", "EF_mu20", "EF_mu40", "L2_mu18_MG", "L2_mu18_MG_medium","L2_mu20_MG", "L2_mu40_MSonly","L2_mu40_MSonly_barrel","L2_mu40_MSonly_barrel_medium","EF_mu18_MG", "EF_mu18_MG_medium","EF_mu20_MG", "EF_mu40_MSonly","EF_mu40_MSonly_barrel", "EF_mu40_MSonly_barrel_medium"]
ConvertAODToMTree.METTermContainerNames = [
                                           "MET_TopoObj",
                                           "MET_LocHadTopoObj",
                                           "MET_Final",
                                           "MET_RefFinal_em",
                                           "MET_RefGamma", 
                                           "MET_RefGamma_em", 
                                           "MET_RefEle", 
                                           "MET_RefEle_em", 
                                           "MET_RefTau_em", 
                                           "MET_Cryo", 
                                           "MET_CellOut", 
                                           "MET_CellOut_em", 
                                           "MET_CellOut_Muid", 
                                           "MET_MuonBoy",
                                           "MET_MuonBoy_Track",
                                           "MET_MuonBoy_Spectro",
                                           "MET_Muid",
                                           "MET_Muid_Track",
                                           "MET_Muid_Spectro",
                                           "MET_RefMuon",
                                           "MET_RefMuon_Track",
                                           "MET_RefMuon_em",
                                           "MET_RefMuon_Track_em",
                                           "MET_RefMuon_Muid",
                                           "MET_RefMuon_Track_Muid",
                                           "MET_RefJet", 
                                           "MET_RefJet_em", 
                                          ]
ConvertAODToMTree.METCaloContainerNames = ["MET_Topo", "MET_CorrTopo", "MET_LocHadTopo"]
if ApplyMTreeSelections:
  ConvertAODToMTree.ElectronPtCut = 0.0
  ConvertAODToMTree.PhotonPtCut = 10.0
  ConvertAODToMTree.TrackPtCut = 1.0

#skimming options
if ApplyMTreeSkim:
  ConvertAODToMTree.ApplySkim = ApplyMTreeSkim
  ConvertAODToMTree.NrLeptons = NrLeptons
  ConvertAODToMTree.LeptonPtCut = LeptonPtCut
  ConvertAODToMTree.SmearLeptonPt = SmearLeptonPt
  
# Some configuration parameters
ConvertAODToMTree.EventCounterInterval = 1
ConvertAODToMTree.OutputLevel = INFO

##########################################


