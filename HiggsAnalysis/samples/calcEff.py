#! /usr/bin/env python

import string,os,sys,glob

##if len(sys.argv) != 2:
##    print "Usage: calcEff.py <dirName>"

fileName = "sampleEfficiencies.txt"
if os.path.isfile(fileName): os.remove(fileName)
outfile = open(fileName,"w")

rootDir = "/net/s3_dataa/MTree/MTree-16-01-15/AOD/mc10b/logs/mc/"
print rootDir
os.chdir(rootDir)
files = glob.glob("*")
for file in files:
    if ".txt" not in file: continue
    terms = file.split('.')
    if len(terms) == 0: continue
    dataset = terms[0]

    Nplus_init = 0
    Nminus_init = 0
    Nplus = 0
    Nminus = 0

    input = open(file,"r")
    for line in input:
        number = float(line.split()[1])
        if "Nplus_init:" in line: Nplus_init += number
        if "Nminus_init:" in line: Nminus_init += number
        if "Nplus_final:" in line: Nplus += number
        if "Nminus_final:" in line: Nminus += number

    snip = "Dataset = %s, Efficiency: %s\n" % (dataset, (Nplus - Nminus)/(Nplus_init - Nminus_init))
    outfile.write(snip)
                                              
        
outfile.close()        

##dirNames=["top","WW","AlpgenDY","PythiaDY","HWW","AlpgenW","PythiaW","WZ","Wg","ZZ","AlpgenZ","PythiaZ"]

##fileName = "sampleEfficiencies.txt"
##if os.path.isfile(fileName): os.remove(fileName)
##outfile = open(fileName,"w")

##for dirName in dirNames:
##    rootDir="/net/s3_dataa/MTree/MTree-16-01-14/AOD/mc10b/"+dirName+"/"

##    datasets={}
##    print rootDir

##    folders=glob.glob("%s*" % rootDir)
##    for folder in folders:
##        terms=folder.split('.')
##        if len(terms) == 0: continue
##        dataset=0
##        if terms[0] == rootDir+"user" or terms[0] == rootDir+"user10":
##            dataset=int(terms[3])
##        else:
##            continue
##        if not datasets.has_key(dataset):
##            datasets[dataset] = [folder]
##        else:
##            datasets[dataset] += [folder]

##    for dataset in datasets.keys():
##        dirlist = datasets[dataset]

##        Nplus_init = 0
##        Nminus_init = 0
##        Nplus = 0
##        Nminus = 0

##        for dir in dirlist:
##            fileNames = os.listdir(dir)
##            for file in fileNames:
##                if "log.out" not in file: continue
####                inputDir = dir + os.sep + file[0:file.find(".log.out")] + "/*.root"
####                if len(glob.glob(inputDir)) == 0:
####                    outfile.write("No root files in corresponding log file, skipping\n")
####                    continue
##                input = open(dir + os.sep + file,"r")
##                for line in input:
##                    number = float(line.split()[1])
##                    if "Nplus_init:" in line: Nplus_init += number
##                    if "Nminus_init:" in line: Nminus_init += number
##                    if "Nplus:" in line: Nplus += number
##                    if "Nminus:" in line: Nminus += number
                        

##        snip = "Dataset = %s, Efficiency: %s\n" % (dataset, (Nplus - Nminus)/(Nplus_init - Nminus_init))
##        outfile.write(snip)
                                              
        
##outfile.close()

##for dataset in datasets.keys():

##    Nplus_init = 0
##    Nminus_init = 0
    
##    Nplus = 0
##    Nminus = 0

##    dirs=datasets[dataset]
##    for dir in dirs:

##        fileNames = glob.glob(dir + '/*log.out')
##        for fileName in fileNames:
          
##            file = open(fileName,"r")
##            for line in file:
##                number = float(line.split()[1])
##                if "Nplus_init:" in line:
##                    Nplus_init += number
##                if "Nminus_init:" in line:
##                    Nminus_init += number
##                if "Nplus:" in line:
##                    Nplus += number
##                if "Nminus:" in line:
##                    Nminus += number
                    
##            file.close()

##    if Nplus_init - Nminus_init == 0: print "dir = %s dataset = %s fileName = %s" % (dir, dataset, fileName)

##    print "File: ", fileName, ", Efficiency: ", (Nplus - Nminus)/(Nplus_init - Nminus_init)

