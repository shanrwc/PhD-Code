#!/usr/bin/env python

import os, sys, shutil, string
import time, re

allDatasets = []

#allDatasets.append('group10.phys-susy.SUSYD3PD.data10_7TeV.periodE.physics_Muons.AOD.t0pro04_v01.DAOD_SUSY.000614/')
#allDatasets.append('group10.phys-susy.SUSYD3PD.data10_7TeV.periodF.physics_Muons.AOD.t0pro04_v01.DAOD_SUSY.000614/')
#allDatasets.append('group10.phys-susy.SUSYD3PD.data10_7TeV.periodG.physics_Muons.AOD.t0pro04_v01.DAOD_SUSY.000614/')
#allDatasets.append('group10.phys-susy.SUSYD3PD.data10_7TeV.periodH.physics_Muons.AOD.t0pro04_v01.DAOD_SUSY.000614/')
#allDatasets.append('group10.phys-susy.SUSYD3PD.data10_7TeV.periodE.physics_Egamma.AOD.t0pro04_v01.DAOD_SUSY.000614/')
#allDatasets.append('group10.phys-susy.SUSYD3PD.data10_7TeV.periodF.physics_Egamma.AOD.t0pro04_v01.DAOD_SUSY.000614/')
#allDatasets.append('group10.phys-susy.SUSYD3PD.data10_7TeV.periodG.physics_Egamma.AOD.t0pro04_v01.DAOD_SUSY.000614/')
#allDatasets.append('group10.phys-susy.SUSYD3PD.data10_7TeV.periodH.physics_Egamma.AOD.t0pro04_v01.DAOD_SUSY.000614/')

allDatasets.append('group10.phys-susy.SUSYD3PD.data10_7TeV.periodA.physics_L1Calo.PhysCont.AOD.repro04_v01.V2.1_D2PDSUSY_L1Calo.000614.V1')
allDatasets.append('group10.phys-susy.SUSYD3PD.data10_7TeV.periodB.physics_L1Calo.PhysCont.AOD.repro04_v01.V2.1_D2PDSUSY_L1Calo.000614.V8')
allDatasets.append('group10.phys-susy.SUSYD3PD.data10_7TeV.periodC.physics_L1Calo.PhysCont.AOD.t0pro04_v01.V2.2_D2PDSUSY_L1Calo.000614.V1')
allDatasets.append('group10.phys-susy.SUSYD3PD.data10_7TeV.periodD.physics_L1Calo.PhysCont.AOD.t0pro04_v01.V2.2_D2PDSUSY_L1Calo.000614.V1')
allDatasets.append('group10.phys-susy.SUSYD3PD.data10_7TeV.periodA.physics_MuonswBeam.PhysCont.AOD.repro04_v01.V2.2_D2PDSUSY_Muon.000614.V1')
allDatasets.append('group10.phys-susy.SUSYD3PD.data10_7TeV.periodB.physics_MuonswBeam.PhysCont.AOD.repro04_v01.V2.1_D2PDSUSY_Muon.000614.V1')
allDatasets.append('group10.phys-susy.SUSYD3PD.data10_7TeV.periodC.physics_MuonswBeam.PhysCont.AOD.t0pro04_v01.V2.1_D2PDSUSY_Muon.000614.V8')
allDatasets.append('group10.phys-susy.SUSYD3PD.data10_7TeV.periodD.physics_MuonswBeam.PhysCont.AOD.t0pro04_v01.V2.1_D2PDSUSY_Muon.000614.V2')


for dataset in allDatasets:
  command = './prun_submit.py data %s ver2.2' % dataset
  if len(sys.argv) > 1: command = command + sys.argv[1]
  print ">", command
  os.system(command)
