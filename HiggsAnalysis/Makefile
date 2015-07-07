# ------------------------------------------------------
# All the following comes from the Makefile 
# (which is automatically created by the root install process...)
# in $ROOTSYS/doc/root/test/Makefile .....
# It sets all the needed Variables correctly...
#---------------------------------------------- 
ObjSuf        = o
SrcSuf        = cxx
ExeSuf        =
DllSuf        = so
EVENTLIB      = $(EVENTO)
OutPutOpt     = -o

ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs) -lMLP -lTreePlayer -lMinuit
ROOTGLIBS     = $(shell root-config --glibs) -lMLP -lTreePlayer -lMinuit

# Linux with egcs
CXX           = g++
CXXFLAGS      = -O6 -Wall -fPIC -m32
LD            = g++
LDFLAGS       = -O6
SOFLAGS       = -shared -m32

#
CXXFLAGS     += $(ROOTCFLAGS) -I/home/swalch/testarea/16.6.5/MTree -I/home/swalch/testarea/16.6.5/DataQuality/GoodRunsLists -I/home/swalch/testarea/16.6.5/Reconstruction/Jet/JetUncertainties -I/home/swalch/testarea/16.6.5/PhysicsAnalysis/JetTagging/JetTagPerformanceCalibration/CalibrationDataInterface
LIBS          = $(ROOTLIBS)  
GLIBS         = $(ROOTGLIBS) 
#------------------------------------------------------------------------------
#---------------------------------------------------
HDRS          = BaseAnalysis.h HObjectSelector.h HWWAnalysis.h CutflowClass.h CutlistClass.h HHistosClass.h SmearingClass.h ZControlClass.h TPileupReweighting.h EnergyRescaler.h CutCodeClass.h egammaSFclass.h EtaPhiBinning.h MuonEfficiencyScaleFactor.h StacoCBScaleEffFactors.h WWEventClass.h egammaOQ.h MutagMETcorrection.h ggFReweighting.h

SRCS          = AnalysisDict.cxx BaseAnalysis.cxx HObjectSelector.cxx HWWAnalysis.cxx HWWAnalysisFunctions.cxx CutflowClass.cxx CutlistClass.cxx HHistosClass.cxx SmearingClass.cxx ZControlClass.cxx TPileupReweighting.cxx EnergyRescaler.cxx CutCodeClass.cxx egammaSFclass.C EtaPhiBinning.cxx StacoCBScaleEffFactors.cxx WWEventClass.cxx egammaOQ.cxx MutagMETcorrection.cxx ggFReweighting.cxx

OBJS   	      = AnalysisDict.o BaseAnalysis.o HObjectSelector.o HWWAnalysis.o HWWAnalysisFunctions.o CutflowClass.o CutlistClass.o HHistosClass.o SmearingClass.o ZControlClass.o TPileupReweighting.o EnergyRescaler.o CutCodeClass.o egammaSFclass.o EtaPhiBinning.o StacoCBScaleEffFactors.o WWEventClass.o egammaOQ.o MutagMETcorrection.o ggFReweighting.o
 
PROGRAM       = libAnalysis.so


all:            $(PROGRAM)
	@echo "Used ROOTSYS - path: $(ROOTSYS)"

%.o: %.cxx
	$(CXX) -c $(CXXFLAGS) $*.cxx

$(PROGRAM):     $(OBJS)
	@echo "Linking $(PROGRAM) ..."
	@rm -f $(PROGRAM)
	$(LD) $(LDFLAGS) $(ROOTLIBS) $(SOFLAGS) $(OBJS) -o$(PROGRAM) 
	@chmod 555 $(PROGRAM)
	@echo "done"


clean:;         @rm -f $(OBJS) core *Dict* libAnalysis.so

cleanup:;       @rm -f *~ G__*LinkDef.h

###
AnalysisDict.cxx: $(HDRS)
	@echo "Generating dictionary ..."
	rootcint -f AnalysisDict.cxx -c -I/home/swalch/testarea/16.6.5/MTree -I/home/swalch/testarea/16.6.5/DataQuality/GoodRunsLists  -I/home/swalch/testarea/16.6.5/Reconstruction/Jet/JetUncertainties -I/home/swalch/testarea/16.6.5/PhysicsAnalysis/JetTagging/JetTagPerformanceCalibration/CalibrationDataInterface $(HDRS) 
#------------------------------------------------------------------------







