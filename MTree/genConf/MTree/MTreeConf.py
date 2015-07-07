#Fri Oct  7 15:28:29 2011
"""Automatically generated. DO NOT EDIT please"""
from GaudiKernel.GaudiHandles import *
from GaudiKernel.Proxy.Configurable import *

class ConvertAODToMTree( ConfigurableAlgorithm ) :
  __slots__ = { 
    'OutputLevel' : 0, # int
    'Enable' : True, # bool
    'ErrorMax' : 1, # int
    'ErrorCount' : 0, # int
    'AuditAlgorithms' : False, # bool
    'AuditInitialize' : False, # bool
    'AuditReinitialize' : False, # bool
    'AuditRestart' : False, # bool
    'AuditExecute' : False, # bool
    'AuditFinalize' : False, # bool
    'AuditBeginRun' : False, # bool
    'AuditEndRun' : False, # bool
    'AuditStart' : False, # bool
    'AuditStop' : False, # bool
    'MonitorService' : 'MonitorSvc', # str
    'RegisterForContextService' : False, # bool
    'RootFileName' : 'MTree.root', # str
    'ElectronPtCut' : 0.0, # float
    'PhotonPtCut' : 0.0, # float
    'TrackPtCut' : 0.0, # float
    'ApplySkim' : False, # bool
    'NrLeptons' : 0, # int
    'LeptonPtCut' : 0, # int
    'SmearLeptonPt' : False, # bool
    'VtxContainerName' : 'VxPrimaryCandidate', # str
    'JetContainerName' : 'AntiKt4TopoJets', # str
    'TrackContainerName' : 'TrackParticleCandidate', # str
    'ElectronContainerName' : 'ElectronAODCollection', # str
    'PhotonContainerName' : 'PhotonAODCollection', # str
    'MuonContainerNames' : [  ], # list
    'CaloMuonContainerName' : 'CaloMuonCollection', # str
    'StandaloneMuonContainerNames' : [  ], # list
    'MuTagContainerName' : 'MuTagTrackParticles', # str
    'METContainerName' : 'MET_RefFinal', # str
    'MCTruthContainerName' : 'GEN_AOD', # str
    'IgnoreL1TriggerNames' : [  ], # list
    'IgnoreHLTTriggerNames' : [  ], # list
    'HLTTriggerROINames' : [  ], # list
    'EventCounterInterval' : 1000, # int
    'TrigDecisionTool' : PublicToolHandle('Trig::TrigDecisionTool/TrigDecisionTool'), # GaudiHandle
    'TrackToVertex' : PublicToolHandle('Reco::TrackToVertex'), # GaudiHandle
    'TrackIsolationTool' : PublicToolHandle('TrackIsolationTool'), # GaudiHandle
    'METTermContainerNames' : [  ], # list
    'METCaloContainerNames' : [  ], # list
    'JetVertexAssociationTool' : PublicToolHandle('JetVertexAssociationTool'), # GaudiHandle
  }
  _propertyDocDct = { 
    'RegisterForContextService' : """ The flag to enforce the registration for Algorithm Context Service """,
    'TrigDecisionTool' : """ Tool to access the trigger decision """,
  }
  def __init__(self, name = Configurable.DefaultName, **kwargs):
      super(ConvertAODToMTree, self).__init__(name)
      for n,v in kwargs.items():
         setattr(self, n, v)
  def getDlls( self ):
      return 'MTree'
  def getType( self ):
      return 'ConvertAODToMTree'
  pass # class ConvertAODToMTree
