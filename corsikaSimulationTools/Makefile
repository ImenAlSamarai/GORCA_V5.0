# Makefile for corsikaIOreader
#
#
# Revision $Id: Makefile,v 1.1.1.1 2011/07/21 20:35:50 gmaier Exp $
# 
#
ifeq ($(origin VBFSYS), undefined)
VBFSYS=/usr/local/veritas/
endif

ARCH := $(shell uname)

# linux flags
ifeq ($(ARCH),Linux)
CXX           = g++ 
CXXFLAGS      = -g -O3 -Wall -fPIC -fno-strict-aliasing  -D_FILE_OFFSET_BITS=64 -D_LARGE_FILE_SOURCE -D_LARGEFILE64_SOURCE
LD            = g++
LDFLAGS       = -O
# LDFLAGS       =  -pg -O
SOFLAGS       = -shared
endif

# Apple OS X flags
ifeq ($(ARCH),Darwin)
CXX           = g++ 
CXXFLAGS      = -g -O3 -Wall -fPIC  -fno-strict-aliasing
LD            = g++
LDFLAGS       = -O
SOFLAGS       = -shared
endif

OutPutOpt     = -o

# Root

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)

VBFCFLAGS    := $(shell vbfConfig --cppflags)
VBFLIBS      := $(shell vbfConfig --ldflags --libs)

CXXFLAGS     += $(ROOTCFLAGS)
LIBS          = $(ROOTLIBS)
LIBS         += -lMinuit
LIBS         += $(VBFLIBS)

GLIBS         = $(ROOTGLIBS)
GLIBS        += -lMinuit
GLIBS        += $(VBFLIBS)

INCLUDEFLAGS  = -I. -I./inc/  -I$(VBFSYS)/include/VBF
CXXFLAGS     += $(INCLUDEFLAGS)

vpath %.h ./src/ ./inc
vpath %.cpp ./src/
vpath %.c ./src/

.cpp.o:
	$(CXX) $(CXXFLAGS)  -c $<

all:	atmosphereAndCherenkovLight corsikaIOreader getThickness getHeight getRunHeaderFromTelescopeFile writeGRISU_pe_to_root read_pe_from_root


corsikaIOreader:	straux.o eventio.o warning.o io_simtel.o VIOHistograms.o atmo.o fileopen.o sim_cors.o corsikaIOreader.o VAtmosAbsorption.o VGrisu.o VCORSIKARunheader.o VCORSIKARunheader_Dict.o
		$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt) $@
		@echo "$@ done"

writeGRISU_pe_to_root:	VPEWriter.o writeGRISU_pe_to_root.o
		$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt) $@
		@echo "$@ done"

read_pe_from_root:	read_pe_from_root.o
		$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt) $@
		@echo "$@ done"


getThickness:	atmo.o straux.o fileopen.o getThickness.o
		$(LD) $(LDFLAGS) $^ $(OutPutOpt) $@
		@echo "$@ done"

getHeight:	atmo.o straux.o fileopen.o getHeight.o
		$(LD) $(LDFLAGS) $^ $(OutPutOpt) $@
		@echo "$@ done"

atmosphereAndCherenkovLight:	VAtmCorsika.o VAtmKascade.o VPlotModtran.o atmosphereAndCherenkovLight.o
		$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt) $@
		@echo "$@ done"

atmosphereCherenkovAbsorption:	VAtmosAbsorption.o atmosphereCherenkovAbsorption.o
		$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt) $@
		@echo "$@ done"

getRunHeaderFromTelescopeFile:  straux.o eventio.o warning.o io_simtel.o atmo.o fileopen.o getRunHeaderFromTelescopeFile.o
	$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt) $@


mergeVBF:	mergeVBF.o
	$(LD) $(LDFLAGS) $^ $(LIBS) $(VBFLIBS) $(OutPutOpt) $@
	@echo "$@ done"

mergePE:	mergePE.o
	$(LD) $(LDFLAGS) $^ $(LIBS) $(VBFLIBS) $(OutPutOpt) $@
	@echo "$@ done"




clean:	
	rm -f *.o

.SUFFIXES: .o

atmo.o: atmo.h fileopen.h
eventio.o: initial.h io_basic.h 
fileopen.o: initial.h straux.h fileopen.h
iact.o: initial.h io_basic.h mc_tel.h
io_simtel.o: initial.h io_basic.h mc_tel.h
corsikaIOreader.o: initial.h io_basic.h mc_tel.h atmo.h sim_cors.h
warning.o:	warning.h initial.h
straux.o: initial.h straux.h
VIOHistograms.o:	mc_tel.h sim_cors.h
VAtmosAbsorption.o:	VAtmosAbsorption.h
VCORSIKARunheader.o:	VCORSIKARunheader.h
VGrisu.o:	mc_tel.h sim_cors.h VCORSIKARunheader.h
sim_cors.o:	sim_cors.h	


VCORSIKARunheader_Dict.cpp:	VCORSIKARunheader.h VCORSIKARunheaderLinkDef.h
	@echo "Generating dictionary $@..."
	@echo rootcint -f $@  -c -p inc/VCORSIKARunheader.h $^
	@rootcint -f $@  -c -p inc/VCORSIKARunheader.h $^ 

