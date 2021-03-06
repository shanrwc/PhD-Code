###############################
#
# Set some flags which controls the configuration to be the right one
#
EvtMax=-1                      # Number of events to process (-1 means all)
InputDataType = "MC"         # Can be "Data" or "MC"
InputDataFormat = "AOD"        # Can be "AOD" or "ESD"
RunSite = "CERN"               # Can be "CERN" or "UM"
ApplyMTreeSelections = True   # Apply or not the filtering of low pT tracks, electrons and photons and some triggers
RedoBTagging = True            # Can be True of False (should be True generally)
RedoMissingET = False          # Re-run MissingET algorithm
JetContainer = "AntiKt4TopoEM"   # Specified here to make the configuration consistent (no need to change it)

#skimming options. InputDataFormat is important here!
#When skimming, adding mu.root,log.out to your pathena --extOutFile option will also include the avg mu distribution and skimming efficiency
ApplyMTreeSkim = True #if this option is false, other arguments are irrelevant
NrLeptons = 2 #nr leptons to skim on
LeptonPtCut = 10 #skim pt cut in GeV
SmearLeptonPt = True and InputDataType == "MC" #Apply lepton smearing in MC, for consistency with unskimmed samples

###############################

from AthenaCommon.GlobalFlags import GlobalFlags
if InputDataType == "Data":
    GlobalFlags.DataSource.set_data()

#
# Check for the existance of the BDT input files
#
import shutil
        
# include your algorithm job options here
UserAlgs=[ "ConvertAODToMTree_jobOption.py" ]

# Output log setting; this is for the framework in general
OutputLevel = WARNING

# Configuring RecExCommon flags
from RecExConfig.RecFlags import rec
rec.AutoConfiguration=['everything']
rec.doPerfMon.set_Value_and_Lock(False)
rec.readRDO.set_Value_and_Lock(False)
if InputDataFormat == "AOD":
    rec.readAOD.set_Value_and_Lock(True)
    rec.readESD.set_Value_and_Lock(False)
elif InputDataFormat == "ESD":
    rec.readAOD.set_Value_and_Lock(False)
    rec.readESD.set_Value_and_Lock(True)
rec.doCBNT.set_Value_and_Lock(False)
rec.doWriteESD.set_Value_and_Lock(False)
rec.doWriteAOD.set_Value_and_Lock(False)
rec.doWriteTAG.set_Value_and_Lock(False)
rec.oldFlagTopSteering.set_Value_and_Lock(True)

# The input file
from AthenaCommon.AthenaCommonFlags import jobproperties as jp
InputFileName = "%s.pool.root" % InputDataFormat
if not os.path.isfile("filelist.py"):
    jp.AthenaCommonFlags.FilesInput = [ InputFileName ]
else:  
    include("filelist.py")

# main jobOption - must always be included
include ("RecExCommon/RecExCommon_topOptions.py")

