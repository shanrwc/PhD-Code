#!/usr/bin/env python
import sys,os,string,shutil,datetime

#get dataset name from command line (phrase after script name)
inputDataset = sys.argv[1]

#copy dataset name to a new location (not just make another pointer to it)
inputDatasetName = inputDataset[:]

#If it is a container dataset/a directory of stuff, remove slash from output dataset name
if inputDatasetName[-1] == "/":
    inputDatasetName = inputDataset[:-1]

#Build output dataset name.  Remember to update counter to the number of versions I've produced.
outputDataset = "user.swalch.%s.MT16-01-15.v14" % inputDatasetName

#--dbRelease ddo.000001.Atlas.Ideal.DBRelease.v100701:DBRelease-10.7.1.tar.gz
command = "pathena --nFilesPerJob 1 --excludedSite ANALY_RAL,ANALY_SARA,ANALY_VICTORIZ-WG1,ANALY_LIV,ANALY_MPPMU,ANALY_GRIF-LAL,ANALY_LAPP,ANALY_TECHNION-HEP,ANALY_FREIBURG,ANALY_CERN_XROOTD,ANALY_FZK,ANALY_wuppertalprod,ANALY_ARC --dbRelease=LATEST --extOutFile MTree.root --inDS %s --outDS %s  TopOptions.py" % (inputDataset, outputDataset)

print ">", command

os.system(command)





