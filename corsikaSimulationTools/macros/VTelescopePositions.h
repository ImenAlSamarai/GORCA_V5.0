//! VTelescopeGroundPosition calculate telescope positions on the ground and write CORSIKA and grisu telescope lines
// Revision $Id: VTelescopePositions.h,v 1.1.1.1 2011/07/21 20:35:56 gmaier Exp $

#ifndef VTelescopeGroundPosition_H
#define VTelescopeGroundPosition_H

#include "TCanvas.h"
#include "TEllipse.h"
#include "TH1D.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class VTelescopeGroundPosition
{
    public:

    unsigned int     fNTel;
    unsigned int     fID;
    string           fName;
    double           fTelescopeDistance;
    vector< unsigned int > fTelID;                 //! this depends on the number of different arrays used
    vector< double > x;
    vector< double > y;
    vector< double > z;
    vector< double > r;
    int              fColor;
    int              fMarker;
    vector< double > xG;
    vector< double > yG;

    VTelescopeGroundPosition() {}
   ~VTelescopeGroundPosition() {}
};

class VTelescopePositions
{
   private:

   vector< VTelescopeGroundPosition* > fData;
   VTelescopeGroundPosition *fCORSIKA_Data;

   double getMaxDistance( vector< unsigned int > iSelection );
   vector< unsigned int > getSelectedTelescopes( string iSelection );

   public:

   VTelescopePositions();
  ~VTelescopePositions() {}

   void addHexConfiguration( string iName, int ntel, double iTelDistance, double iMirrorDiameter, int iColor, int iMarker );
   void addSquareConfiguration( string iName, int iNtel, double iTelDistance, double iTelShift, double iMirrorDiameter, int iColor = 1, int iMarker = 20 );
   void checkCORSIKA_positions();
   void checkGRISU_positions();
   void make( string iSelection, bool bCorsika = false, bool bPrint = true );
   void makeTELESCOPE_Lines( string iSelection, bool bCorsika, bool bPrint );
   void plot( string iSelection );
   void print();
   void print( unsigned int iID );
   vector< string > getGrisuline_TLLOC( unsigned int iID );
   void writeEVNDISP_positions( string iSelection, string ofile );
};

#endif
