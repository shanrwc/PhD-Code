#!/usr/bin/env python

import os, sys, shutil, string
import time, re



if len(sys.argv) < 2: 
  print "Usage      : ./prun_submit.py <inputType> <inDS>"
  print "<inputType : data or mc"
  print "<inDS>     : input dataset name"
  sys.exit()
inputType = sys.argv[1]
if len(sys.argv) > 2: inDS = sys.argv[2]
else:
  if inputType == 'data': inDS = 'group10.phys-susy.SUSYD3PD.data10_7TeV.periodA.physics_MuonswBeam.PhysCont.AOD.repro04_v01.V2.2_D2PDSUSY_Muon.000614.V1'
  if inputType == 'mc'  : inDS = 'user.JESunderm.SUSYD3PD.mc09_7TeV.107650.AlpgenJimmyZeeNp0_pt20.merge.AOD.e529_s765_s767_r1430_r1429.000614.V1/'
if inputType == 'data': oldUserString = inDS.split('data10')[0]
if inputType == 'mc': oldUserString = inDS.split('mc09')[0]
#-----------------------------------
# Grab script from relevant directory
mainScript = 'main.sh'
baseDir    = os.getcwd()
workDir    = os.path.join(baseDir, 'prun_%s' % inputType)
if not os.path.isdir(workDir):
  print "| ERROR: directory %s does not exist.  Exiting without running 'prun'" % workDir
  sys.exit()
org = os.path.join(workDir, mainScript)
new = os.path.join(baseDir, mainScript)
os.chdir(workDir)
#-----------------------------------
# Output DS name
outDS = 'user.harper.%s' % inDS
outDS = outDS.replace('NTUP_SUSY', 'SUSYD3PD.MTree')
if outDS[-1] == '/': outDS = outDS.replace('/', '')
if len(sys.argv) > 3: outDS += '.%s' % sys.argv[3]
outDS = outDS.replace('L1Calo', 'Egamma')
outDS = outDS.replace('MuonswBeam', 'Muons')
#-----------------------------------
# The base prun command
command = 'prun --exec "%s %s" --athenaTag=16.0.2.7,AtlasProduction --inDS %s --outDS %s --extFile=ConvertD3PDToMTree*,lib*.so,SUSYD3PDClass*,main.C,Makefile --noBuild --outputs=MTree.root' % (mainScript, inputType, inDS, outDS)
#-----------------------------------
# Some common options
# (see https://twiki.cern.ch/twiki/bin/viewauth/Atlas/PandaRun)
# or do 'prun --help'
#
command += ' --excludeFile="*bdt*","input","share","store" '
#command += ' --nGBPerJob=MAX'
command += ' --nGBPerJob=10'
#command += ' --nFilesPerJob=4'
command += ' --writeInputToTxt IN:input.txt'
command += ' --workDir=%s' % baseDir
#command += ' --excludedSite=ANALY_IFIC,ANALY_IHEP,ANALY_FREIBURG,ANALY_DESY-HH'
#command += ' --inputFileList=failedJobs.1721.txt'
#command += ' --excludedSite=ANALY_LRZ'
#command += ' --site=ANALY_MWT2'

#-----------------------------------
# Call prun
#
print ">", command
print "|", "Calling prun"
os.system(command)
os.chdir(baseDir)
print "|", "Done!"

listFile = open('userDatasets.txt', 'a')
listFile.write('%s\n' % outDS)
