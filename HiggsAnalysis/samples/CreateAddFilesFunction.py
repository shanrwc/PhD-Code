#!/usr/bin/env python

import os, string, sys, time, shutil, re

def GetLine(sampleTag, dirName, MTreeName):
    return '%s %s %s\n' % (sampleTag, dirName, MTreeName)

def GetRootFileNames(directory):
    fileNames = os.listdir(directory)
    files = []
    for fileName in fileNames:
        if ".C" in fileName: continue
        if ".txt" in fileName: continue
        if ".root" not in fileName: continue
        if "histo" in fileName: continue
        if ".tgz" in fileName: continue
        if "mu.root" in fileName: continue
        if "GLOBAL" not in fileName: files.append(fileName)
    return files

def AddLines(lines):
    outFile = open(AddFileName, "a")
    outFile.writelines(lines)
    outFile.close()

    
##################
#
# Start of Program
#
##################

AddFileName = os.getcwd() + os.sep + "AddFiles.txt"
print AddFileName

if os.path.isfile(AddFileName):
    os.remove(AddFileName)

StartDir = os.getcwd()

##currentDir = "/net/s3_datad/MTree/MTree2l/16-01-15/data/Merged"
##os.chdir(currentDir)
##lines = []
##directories = os.listdir(os.curdir)
##for directory in directories:
##    rootFiles = GetRootFileNames(directory)
##    for rootFile in rootFiles:
##        tag = "data" + directory[4:]
##        lines.append(GetLine(tag, currentDir + os.sep + directory, rootFile))
##lines.sort()
##AddLines(lines)
##os.chdir(StartDir)

##currentDir = "/net/s3_dataa/MTree/MTree-16-01-14/AOD/data/Egamma"
##os.chdir(currentDir)
##lines = []
##directories = os.listdir(os.curdir)
##for directory in directories:
##    if "dont_use" in directory:continue
##    if ".txt" in directory: continue
##    if ".out" in directory: continue
##    if ".job" in directory: continue
##    if ".py" in directory: continue
##    parts = string.split(directory,".")
##    number = parts[3]
##    if float(number) != 183602: continue
####    directs2 = os.listdir(os.curdir + os.sep + directory)
####    for direct2 in directs2:
####        if ".txt" in direct2: continue
####        if ".out" in direct2: continue
####        if ".tgz" in direct2: continue
####        if ".root" in direct2: continue
##    rootFiles = GetRootFileNames(directory)
##    for rootFile in rootFiles:
##        lines.append(GetLine("dataEg" + number[2:], currentDir + os.sep + directory, rootFile))
##lines.sort()
##AddLines(lines)
##os.chdir(StartDir)

##currentDir = "/net/s3_dataa/MTree/MTree-16-01-11/AOD/data/Muons"
##os.chdir(currentDir)
##lines = []
##directories = os.listdir(os.curdir)
##for directory in directories:
##    if ".txt" in directory: continue
##    if ".out" in directory: continue
##    if ".job" in directory: continue
##    if ".py" in directory: continue
##    parts = string.split(directory,".")
##    number = parts[3]
##    if float(number) != 180122: continue
##    directs2 = os.listdir(os.curdir + os.sep + directory)
##    for direct2 in directs2:
##        if ".txt" in direct2: continue
##        if ".out" in direct2: continue
##        if ".tgz" in direct2: continue
##        if ".root" in direct2: continue
##        rootFiles = GetRootFileNames(directory + os.sep + direct2)
##        for rootFile in rootFiles:
##            lines.append(GetLine("dataMu" + number[2:], currentDir + os.sep + directory + os.sep + direct2, rootFile))
##lines.sort()
##AddLines(lines)
##os.chdir(StartDir)

currentDir = "/net/s3_dataa/MTree/MTree-16-01-15/AOD/unskimmed/user.swalch.mc10_7TeV.116707.PowHegPythia_ggH150_WW2lep.merge.AOD.e773_s933_s946_r2302_r2300.MT16-01-15.unsk.v3.110817202319"
os.chdir(currentDir)
lines = []
rootFiles = GetRootFileNames(currentDir)
for rootFile in rootFiles:
    lines.append(GetLine("testhww150",currentDir,rootFile))
lines.sort()
AddLines(lines)
os.chdir(StartDir)

##currentDir = "/net/s3_dataa/MTree/MTree-16-01-15/AOD/unskimmed/user.swalch.mc10_7TeV.116707.PowHegPythia_ggH150_WW2lep.merge.AOD.e773_s933_s946_r2302_r2300.MT16-01-15.unsk.v3.110817202251"
##os.chdir(currentDir)
##lines = []
##rootFiles = GetRootFileNames(currentDir)
##for rootFile in rootFiles:
##    lines.append(GetLine("testhggf150",currentDir,rootFile))
##lines.sort()
##AddLines(lines)
##os.chdir(StartDir)

##currentDir = "/net/s3_dataa/MTree/MTree-16-01-15/AOD/unskimmed/user.swalch.mc10_7TeV.125399.PythiaZH150_WW2lep.merge.AOD.e781_s933_s946_r2302_r2300.MT16-01-15.unsk.110817141445"
##os.chdir(currentDir)
##lines = []
##rootFiles = GetRootFileNames(currentDir)
##for rootFile in rootFiles:
##    lines.append(GetLine("testzh150",currentDir,rootFile))
##lines.sort()
##AddLines(lines)
##os.chdir(StartDir)

##currentDir = "/net/s3_dataa/MTree/MTree-16-01-14/AOD/unskimmed/user.swalch.mc10_7TeV.109074.McAtNlo_JIMMY_H160_WpWm_lnulnu.merge.AOD.e598_s933_s946_r2302_r2300.MT16-01-14.unsk.v1.110715185354"
##os.chdir(currentDir)
##lines = []
##rootFiles = GetRootFileNames(currentDir)
##for rootFile in rootFiles:
##    lines.append(GetLine("testhww160",currentDir,rootFile))
##lines.sort()
##AddLines(lines)
##os.chdir(StartDir)

##currentDir = "/net/s3_datad/MTree/MTree2l/16-01-14/unskimmed/Merged/Run-183602"
##os.chdir(currentDir)
##lines = []
##rootFiles = GetRootFileNames(currentDir)
##for rootFile in rootFiles:
##    lines.append(GetLine("tdata183602",currentDir,rootFile))
##lines.sort()
##AddLines(lines)
##os.chdir(StartDir)

##currentDir = "/net/s3_dataa/MTree/MTree-16-01-15/AOD/mc10b/HWW"
##os.chdir(currentDir)
##lines = []
##directories = os.listdir(os.curdir)
##for directory in directories:
##    if ".txt" in directory: continue
##    if ".out" in directory: continue
##    if "PowHeg" not in directory: continue
##    rootFiles = GetRootFileNames(directory)
##    for rootFile in rootFiles:
##        name = "hww"
##        if "VBF" in directory: name = "vbf" + name
##        else: name = "ggf" + name
##        if "tau" in directory:
##            if "OR" in directory: name = name + "t"
##            else: name = name + "tt"
##        temp = (string.split(directory,"_"))[2]
##        name = name + temp[string.find(temp,"H") + 1:string.find(temp,"H") + 4]
##        lines.append(GetLine(name,currentDir + os.sep + directory,rootFile))
##lines.sort()
##AddLines(lines)
##os.chdir(StartDir)

##currentDir = "/net/s3_dataa/MTree/MTree-16-01-15/AOD/mc10b/WW"
##os.chdir(currentDir)
##lines = []
##directories = os.listdir(os.curdir)
##for directory in directories:
##    if ".txt" in directory: continue
##    if ".job" in directory: continue
##    if ".sh" in directory: continue
##    rootFiles = GetRootFileNames(directory)
##    for rootFile in rootFiles:
##        name = "WW"
##        if "gg2WW" in directory: name = "gg" + name
##        final = directory[48:]
##        final = (string.split(final,"."))[0]
##        final = final.replace("n","")
##        final = final.replace("u","")
##        final = final.replace("a","")
##        name = name + final
##        lines.append(GetLine(name,currentDir + os.sep + directory, rootFile))
##lines.sort()
##AddLines(lines)
##os.chdir(StartDir)


##Unskimmed datasets
##currentDir = "/net/s3_dataa/MTree/MTree-16-01-15/AOD/mc10b/AlpgenW"
##os.chdir(currentDir)
##lines = []
##directories = os.listdir(os.curdir)
##for directory in directories:
##    if ".txt" in directory: continue
##    if ".py" in directory: continue
##    rootFiles = GetRootFileNames(directory)
##    for rootFile in rootFiles:
##        name = ""
##        if "107680" in directory: name = "WevNp0"
##        elif "107681" in directory: name = "WevNp1"
##        elif "107682" in directory: name = "WevNp2"
##        elif "107683" in directory: name = "WevNp3"
##        elif "107684" in directory: name = "WevNp4"
##        elif "107685" in directory: name = "WevNp5"
##        elif "107690" in directory: name = "WmvNp0"
##        elif "107691" in directory: name = "WmvNp1"
##        elif "107692" in directory: name = "WmvNp2"
##        elif "107693" in directory: name = "WmvNp3"
##        elif "107694" in directory: name = "WmvNp4"
##        elif "107695" in directory: name = "WmvNp5"
####        if "107700" in directory: name = "WtvNp0"
####        if "107701" in directory: name = "WtvNp1"
####        if "107702" in directory: name = "WtvNp2"
####        if "107703" in directory: name = "WtvNp3"
####        if "107704" in directory: name = "WtvNp4"
####        if "107705" in directory: name = "WtvNp5"
####        if "106280" in directory: name = "WbbNp0"
####        if "106281" in directory: name = "WbbNp1"
####        if "106282" in directory: name = "WbbNp2"
####        if "106283" in directory: name = "WbbNp3"
##        else: continue
##        lines.append(GetLine(name, currentDir + os.sep + directory, rootFile))
##lines.sort()
##AddLines(lines)
##os.chdir(StartDir)

##currentDir = "/net/s3_dataa/MTree/MTree-16-01-15/AOD/mc10b/PythiaW"
##os.chdir(currentDir)
##lines = []
##directories = os.listdir(os.curdir)
##for directory in directories:
##    if ".tgz" in directory: continue
##    if ".txt" in directory: continue
##    if ".out" in directory: continue
##    if ".root" in directory: continue
##    rootFiles = GetRootFileNames(directory)
##    for rootFile in rootFiles:
##        lines.append(GetLine("PythiaWt", currentDir + os.sep + directory, rootFile))
##lines.sort()
##AddLines(lines)
##os.chdir(StartDir)

##currentDir = "/net/s3_dataa/MTree/MTree-16-01-15/AOD/mc10b/AlpgenZ"
##os.chdir(currentDir)
##lines = []
##directories = os.listdir(os.curdir)
##for directory in directories:
##    if ".txt" in directory: continue
##    if ".out" in directory: continue
##    if ".job" in directory: continue
##    if ".C" in directory: continue
##    rootFiles = GetRootFileNames(directory)    
##    for rootFile in rootFiles:
##        if "swalch" in directory: name = directory[40:]
##        else: name = directory[40:]
##        name = (string.split(name,"_"))[0]
##        name = name.replace("n","v")
##        name = name.replace("u","");
##        name = name.replace("a","");
##        parts2 = string.split(rootFile,".")
##        if "Np0" in name:
##            temp = parts2[4]
##            count = float(temp[1:])
##            if count < 400: name = name + "-0"
##            elif count < 800: name = name + "-1"
##            elif count < 1200: name = name + "-2"
##            else: name = name + "-3"
##        lines.append(GetLine(name, currentDir + os.sep + directory, rootFile))
##lines.sort()
##AddLines(lines)
##os.chdir(StartDir)


##currentDir = "/net/s3_dataa/MTree/MTree-16-01-15/AOD/mc10b/PythiaZ"
##os.chdir(currentDir)
##lines = []
##directories = os.listdir(os.curdir)
##for directory in directories:
##    rootFiles = GetRootFileNames(directory)
##    for rootFile in rootFiles:
##        name = (string.split(directory,"."))[4]
##        name = (string.split(name,"_"))[0]
##        name = name.replace("au","")
##        name = name.replace("u","")
##        if "ee" in name or "mm" in name:
##            parts = string.split(rootFile,".")
##            temp = parts[4]
##            count = float(temp[1:])
##            if count < 334: name = name + "-0"
##            elif count < 667: name = name + "-1"
##            else: name = name + "-2"
##        lines.append(GetLine(name, currentDir + os.sep + directory, rootFile))
##lines.sort()
##AddLines(lines)
##os.chdir(StartDir)

##currentDir = "/net/s3_dataa/MTree/MTree-16-01-15/AOD/mc10b/AlpgenDY"
##os.chdir(currentDir)
##lines = []
##directories = os.listdir(os.curdir)
##for directory in directories:
##    if ".job" in directory: continue
##    if ".txt" in directory: continue
##    if "116946" in directory: continue
##    rootFiles = GetRootFileNames(directory)
##    for rootFile in rootFiles:
##        if "swalch" in directory: name = directory[40:]
##        else: name = directory[40:]
##        name = (string.split(name,"_"))[0]
##        name = "dy" + name
##        name = name.replace("n","v")
##        name = name.replace("u","")
##        name = name.replace("a","")
##        lines.append(GetLine(name,currentDir + os.sep + directory, rootFile))
##lines.sort()
##AddLines(lines)
##os.chdir(StartDir)

##currentDir = "/net/s3_dataa/MTree/MTree-16-01-15/AOD/mc10b/PythiaDY"
##lines = []
##os.chdir(currentDir)
##directories = os.listdir(os.curdir)
##for directory in directories:
##    if ".tgz" in directory: continue
##    if ".txt" in directory: continue
##    if ".out" in directory: continue
##    rootFiles = GetRootFileNames(currentDir + os.sep + directory)
##    for rootFile in rootFiles:
##        name
##        if "107055" in directory: name = "PythiaDYtt"
##        if "105148" in directory: name = "PythiaDYLee"
##        if "108320" in directory: name = "PythiaDYee"
##        if "113700" in directory: name = "PythiaDYLmm"
##        if "113701" in directory: name = "PythiaDYmm"
##        lines.append(GetLine(name,currentDir + os.sep + directory, rootFile))
##lines.sort()
##AddLines(lines)
##os.chdir(StartDir)

##currentDir = "/net/s3_dataa/MTree/MTree-16-01-15/AOD/mc10b/WZ"
##os.chdir(currentDir)
##lines = []
##directories = os.listdir(os.curdir)
##for directory in directories:
##    if ".txt" in directory: continue
##    if ".py" in directory: continue
##    rootFiles = GetRootFileNames(directory)
##    for rootFile in rootFiles:
##        name = directory[47:]
##        name = (string.split(name,"."))[0]
##        name = name.replace("n","v")
##        name = name.replace("u","");
##        name = name.replace("a","");
##        name = "W" + directory[44] + "Z" + name
##        lines.append(GetLine(name, currentDir + os.sep + directory, rootFile))
##lines.sort()
##AddLines(lines)
##os.chdir(StartDir)

##currentDir = "/net/s3_dataa/MTree/MTree-16-01-15/AOD/mc10b/Wg"
##os.chdir(currentDir)
##lines = []
##directories = os.listdir(os.curdir)
##for directory in directories:
##    if ".txt" in directory: continue
##    if ".py" in directory: continue
##    rootFiles = GetRootFileNames(directory)
##    for rootFile in rootFiles:
##        name = (string.split(directory,"_"))[3]
##        name = (string.split(name,"."))[0]
##        name = name.replace("lus","")
##        name = name.replace("inus","")
##        name = name.replace("amma","")
##        name = name.replace("n","v")
##        name = name.replace("u","")
##        name = name.replace("a","")
##        lines.append(GetLine(name, currentDir + os.sep + directory, rootFile))
##lines.sort()
##AddLines(lines)
##os.chdir(StartDir)

##currentDir = "/net/s3_dataa/MTree/MTree-16-01-15/AOD/mc10b/ZZ"
##os.chdir(currentDir)
##lines = []
##directories = os.listdir(os.curdir)
##for directory in directories:
##    if ".txt" in directory: continue
##    if ".py" in directory: continue
##    rootFiles = GetRootFileNames(directory)
##    for rootFile in rootFiles:
##        name = (string.split(directory,"_"))[4]
##        if "gg2ZZ" in directory: name = (string.split(directory,"_"))[3]
##        name = (string.split(name,"."))[0]
##        name = name.replace("n","v")
##        name = name.replace("u","")
##        name = name.replace("a","")
##        name = "ZZ" + name
##        lines.append(GetLine(name, currentDir + os.sep + directory, rootFile))
##lines.sort()
##AddLines(lines)
##os.chdir(StartDir)

##currentDir = "/net/s3_dataa/MTree/MTree-16-01-15/AOD/mc10b/top"
##os.chdir(currentDir)
##lines = []
##directories = os.listdir(os.curdir)
##for directory in directories:
##    if ".job" in directory: continue
##    if ".txt" in directory: continue
##    if ".out" in directory: continue
##    rootFiles = GetRootFileNames(directory)
##    for rootFile in rootFiles:
##        name = []
##        if "105200" in directory and "swalch" in directory:
##            name = "ttbar"
##            bit = (string.split(rootFile,"."))[-3]
##            if "MTree" in bit: bit = (string.split(rootFile,"."))[-4]
##            num = string.atoi(bit[-4:])
##            if num < 500: name = name + "-0"
##            elif num < 1000: name = name + "-1"
##            else: name = name + "-2"
######        elif "117200" in directory: name = "ttbar2"
##        elif "117360" in directory: name = "tb-e"
##        elif "117361" in directory: name = "tb-m"
##        elif "117362" in directory: name = "tb-t"
##        elif "108343" in directory: name = "tqb-e"
##        elif "108344" in directory: name = "tqb-m"
##        elif "108345" in directory: name = "tqb-t"
##        elif "105500" in directory: name = "tW"
##        else: continue
##        lines.append(GetLine(name,currentDir + os.sep + directory, rootFile))
##lines.sort()
##AddLines(lines)
##os.chdir(StartDir)

os.system("cat %s" % AddFileName)
