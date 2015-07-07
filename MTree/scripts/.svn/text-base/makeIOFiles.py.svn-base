#!/usr/bin/env python

import sys, os, string, shutil

MTreeClasses = ["Lepton", "Electron", "Photon", "EventInfo", "Jet", "MissingET", "Muon", "Track", "PrimaryVertex", "Trigger", "TruthParticle", "L1TriggerROI", "HLTTriggerROI","TriggerObject"]

command = "rootcint -f MTreeIO.cxx -c "
for cls in MTreeClasses:    
    shutil.copy2("src/%s.cxx" % cls, "%s.cxx" % cls)
    command += "MTree/%s.h " % cls


command += "Linkdef.h"
os.system(command)

lines = open("MTreeIO.cxx", "r").readlines()
outFile = open("MTreeIO.cxx", "w")
for line in lines:
    index = string.find(line, "MTreeIO.h")
    if index == -1:
        outFile.write(line)
        continue
    tempLine = line[:index] + "MTree/" + line[index:]
    outFile.write(tempLine)
outFile.close()
os.rename("MTreeIO.cxx", "src/MTreeIO.cxx")
os.rename("MTreeIO.h", "MTree/MTreeIO.h")

for cls in MTreeClasses:    
    os.remove("%s.cxx" % cls)

