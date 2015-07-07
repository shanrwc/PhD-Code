#!/usr/bin/env python

import sys, os, string, shutil

MTreeClasses = ["Lepton",
                "Electron",
                "Photon",
                "EventInfo",
                "Jet",
                "MissingET",
                "Muon",
                "Track",
                "PrimaryVertex",
                "Trigger",
                "TruthParticle",
                "L1TriggerROI",
                "HLTTriggerROI"]

os.chdir("i686-slc5-gcc43-opt")
os.rename("libMTree.so", "bkp_libMTree.so")
command = "g++ -O6 -m32 -shared MTreeIO.o "
for cls in MTreeClasses:
    command += "%s.o " % cls
command += "-o libMTree.so"
os.system(command)
os.rename("libMTree.so", "../run/libMTree.so")
os.rename("bkp_libMTree.so", "libMTree.so")

