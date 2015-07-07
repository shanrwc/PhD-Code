ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs)
ROOTGLIBS     = $(shell root-config --glibs)

MTREEVER      = 16.0.2
MTREEDIR      = $(TestArea)/$(MTREEVER)/MTree
MTREEDIR      = $(MTreeDir)

CXX           = g++
CXXFLAGS      =-I$(ROOTSYS)/include -O -Wall -fPIC -I$(MTREEDIR)
LD            = g++
LDFLAGS       = -g
SOFLAGS       = -shared
ARCHFLAGS     = -m32

CXXFLAGS     += $(ROOTCFLAGS)
LIBS          = $(ROOTLIBS)
GLIBS         = $(ROOTGLIBS)


ROOTLIBS      = $(shell root-config --libs) -lMLP -lTreePlayer -lMinuit
ROOTCFLAGS    = $(shell root-config --cflags)
CXX           = g++
CXXFLAGS      = -O6 -Wall -fPIC#-g
CXXFLAGS      += $(ROOTCFLAGS)
CXXFLAGS      += -I$(MTREEDIR)
LD            = g++
LDFLAGS       = -O6 -Wall#-g
ARCHFLAGS     = -m32
SOFLAGS       = -shared



JOBOBJS    = main.o \

SHAREDOBJS = libMTree.so \
             #SUSYD3PDClass_MC_C.so \


HDRS       = ConvertD3PDToMTreeMC.h \
             SUSYD3PDClass_MC.h \

SRCS       = D3PDConvertDictMC.cpp \
             ConvertD3PDToMTreeMC.cpp \
             SUSYD3PDClass_MC.C \

OBJS       = D3PDConvertDictMC.o \
             ConvertD3PDToMTreeMC.o \
             SUSYD3PDClass_MC.o \

FINAL      = libConvertD3PDToMTree.so


mc: $(OBJS) $(JOBOBJS) $(FINAL)
#	@echo "| Done compiling! $(FINAL) ready."

$(FINAL): $(OBJS) $(JOBOBJS)
	@echo "| Linking $(FINAL)..."
	@rm -rf $(FINAL)
	$(LD) $(ARCHFLAGS) $(LDFLAGS) $(SOFLAGS) $(OBJS) $(ROOTLIBS) -I$(MTREEDIR) -o$(FINAL)
	$(CXX) -o main $(JOBOBJS) $(SHAREDOBJS) $(FINAL) $(CXXFLAGS) $(ROOTLIBS) -I $(MTREEDIR)
#	@./setup
#	@echo "| New .rootrc available."
	@chmod 775 $(FINAL)
#	@mv $(FINAL) lib
#	@cp lib/*.so .
	@echo "| Finished linking."

%.o: %.cpp %.h
	@echo "| Working on $*"
	$(CXX) -c $(CXXFLAGS)  $*.cpp

%.o: %.C %.h
	@echo "| Working on $*"
	$(CXX) -c $(CXXFLAGS)  $*.C

%.o: %.C
	@echo "| Working on $*"
	$(CXX) -c $(CXXFLAGS)  $*.C

%.so:;

D3PDConvertDictMC.cpp: $(DATAHDRS) 
	@echo "| Generating dictionary"
	rootcint -f D3PDConvertDictMC.cpp -c -I$(MTREEDIR) -p $(HDRS)

prelim: prelim.o
	$(CXX) -o $@ prelim.o $(CXXFLAGS) $(LIBS)

clean:;	
	@echo "| Cleaning up."
	@rm -rf $(JOBOBJS) $(OBJS) $(FINAL) *.o *Dict* core prelim main

cleanup: clean
	@rm -rf G*LinkDef.h
