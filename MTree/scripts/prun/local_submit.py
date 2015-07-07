#!/usr/bin/env python

import os, sys, shutil, string
import time, re

if len(sys.argv) < 2: 
  print "Usage        : ./local_submit.py <inputType>"
  print "<inputType>  : data or mc"
  print "<inFile>     : (optional) path to local D3PD"
  sys.exit()
inputType = sys.argv[1]
if len(sys.argv) == 3: inFile = sys.argv[2]
else: inFile = '/home/harper/path/to/D3PD.root'
#-----------------------------------
# Grab script from relevant directory
mainScript = 'main.sh'
baseDir    = os.getcwd()
workDir    = os.path.join(baseDir, 'prun_%s' % inputType)
if not os.path.isdir(workDir):
  print "| ERROR: directory %s does not exist.  Exiting without running script" % workDir
  sys.exit()
org = os.path.join(workDir, mainScript)
new = os.path.join(baseDir, mainScript)
os.chdir(workDir)
inputTxt = open('input.txt', 'w')
inputTxt.write('%s' % inFile)
inputTxt.close()
#shutil.copy2(org, new)
# Run the script
command = './%s %s ' % (mainScript, inputType)
print ">", command
print "|", "Running job"
os.system(command)
os.remove('input.txt')
os.chdir(baseDir)
print "|", "Done!"
