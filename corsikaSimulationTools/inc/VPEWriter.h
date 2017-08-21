//! VPEWriter write for pe data to root files
// $Revision $ID$

#ifndef VPEWRITER_H
#define VPEWRITER_H

#include <iostream>
#include <string>
#include <vector>

#include "TFile.h"
#include "TTree.h"

using namespace std;

class VPEWriter
{
   private:

   unsigned int fTelID;

   TTree *fTree;

   unsigned int fEventNumber;
   unsigned int fPrimaryType;
   float        fPrimaryEnergy;
   float        fXcore;
   float        fYcore;
   float        fXcos;
   float        fYcos;
   float        fXsource;
   float        fYsource;
   float        fDelay;
   std::vector< unsigned int > *fPE_pixelID;
   std::vector< float > *fPE_photonX;
   std::vector< float > *fPE_photonY;
   std::vector< float > *fPE_time;
   std::vector< float > *fPE_wl;

   unsigned int fUsePixelNumbers;

   public:

   VPEWriter( unsigned int iTelID = 0, unsigned int iUsePixelNumbers = 1, unsigned int iNInitEvents = 100000 );
   void         add_pe( unsigned int iPE_pixelID, float iPE_photonX, float iPE_photonY, float iPE_time, float iPE_wl = 0. );
   int          add_event( unsigned int iEventNumber, unsigned int iPrimaryType, float iPrimaryEnergy, float iXcore, float iYcore, float iXcos, float iYcos, float iXsource, float iYsource, float iDelay );
   TTree*       getDataTree() { return fTree; }
   unsigned int usePixelNumbers() { return fUsePixelNumbers; }
   
};

#endif

