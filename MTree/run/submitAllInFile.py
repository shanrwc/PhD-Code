#!/usr/bin/env python
import sys,os,string,shutil,datetime,time

if len(sys.argv) < 2:
    fileName = "Datasamples.txt"
##    fileName = "MCsamples.txt"
##    fileName = "EgammaSamples.txt"
##    fileName = "HiggsSamples.txt"
else:
    fileName = sys.argv[1]

datasets = open(fileName).readlines()

for tempDataset in datasets:
    dataset = string.strip(tempDataset)
    if dataset == "": continue
    if dataset[0] == '#': continue
    command = "./submitGridJob.py %s" % dataset
    print ">", command
    os.system(command)
    time.sleep(1)
