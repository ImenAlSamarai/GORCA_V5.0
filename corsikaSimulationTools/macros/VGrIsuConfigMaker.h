//! VGrIsuConfigMaker make a grisu configuration file for many telescopes
// Revision $Id: VGrIsuConfigMaker.h,v 1.1.1.1 2011/07/21 20:35:56 gmaier Exp $

#ifndef VGrIsuConfigMaker_H
#define VGrIsuConfigMaker_H

#include "TBox.h"
#include "TCanvas.h"
#include "TEllipse.h"
#include "TGraph.h"
#include "TH1F.h"
#include "TMath.h"
#include "TStyle.h"
#include "TROOT.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class VGrIsuConfigMaker
{
   public:

   int fTelescopeID;   //! start at 1 (grisu style)

   double fTelescopeX;                // [m]
   double fTelescopeY;
   double fTelescopeZ;
   double fTelescopeRot;      

   double fMirrorDiameter;             // [m]
   double fMirror_f_over_D;
   double fFocalLength;                // [m]
   double fFieldofView;                // field of view [deg]

   int fNPixel;                     // number of pixel 
   double fPixelSize;                  // pmt radius [mm]
   double fPixelGeometricalEfficiency; // geometric collection efficiency
   vector< double > fPixelQueffWavelength;  // wavelength vector for QE
   vector< double > fPixelQueffValue;       // quantum efficiency

   vector< double > fMirrorReflectivityWavelength;
   vector< double > fMirrorReflectivityValue;

   int fNFacets;
   double fFacetBlureRadius;                   
   double fFacetMisalignement;
   double fFacetDiameter;
   double fFacetRotationAngle;
   int    fFacetShape;
   double fFacetSpacing;                     // [m] space between Facets

   vector< double > fFacetPositionX;
   vector< double > fFacetPositionY;
   vector< double > fFacetCurvature;
   vector< double > fFacetSize;

   vector< string > fGrisuline_PMPIX;
   vector< string > fGrisuline_NGHBR;
   vector< string > fGrisuline_PIXLC;
   vector< string > fGrisuline_QUEFF;
   vector< string > fGrisuline_RFCRV;    // mirro reflectivity
   vector< string > fGrisuline_MIREL;   // facets

/////////////////////////////////////////////////////

   VGrIsuConfigMaker( int iTelID = 1 );
  ~VGrIsuConfigMaker() {}

   string getGrisuline_CAMRA();
   string getGrisuline_MIROR();
   string getGrisuline_TLLOC();
   string           getGrisuline_PIXGB( bool bHex = true );
   vector< string > getGrisuline_PIXLC() { return fGrisuline_PIXLC; }
   vector< string > getGrisuline_PMPIX() { return fGrisuline_PMPIX; }
   vector< string > getGrisuline_NGHBR() { return fGrisuline_NGHBR; }
   vector< string > getGrisuline_QUEFF() { return fGrisuline_QUEFF; }
   vector< string > getGrisuline_RFCRV() { return fGrisuline_RFCRV; }    // mirror reflectivity
   vector< string > getGrisuline_MIREL();

   void makeCamera( bool bPrint = false, bool bPlot = false, bool bHex = true );
   void makeMirror();

   TCanvas *plotFacetPosition( TCanvas *c = 0, int iColor = 1 );

   bool readQuantumEfficiency( string iFile );
   bool readMirrorReflectivitiy( string iFile );
   bool readMirrorPositions( string iFile, int iTelID = 1 );

   void setFocalLength( double iF = 12. ) { fFocalLength = iF; }
   void setFieldofView( double iV = 3.6 ) { fFieldofView = iV; }
   void setF_over_D( double iD = 1. ) { fMirror_f_over_D = iD; }
   void setFacetBlureRadius( double iR = 0.02 )  { fFacetBlureRadius = iR; }
   void setFacetDiameter( double iF = 0.355 ) { fFacetDiameter = iF; }
   void setFacetMisalignement( double iM = 0.02 ) { fFacetMisalignement = iM; }
   void setFacetRotationAngle( double iA = 90. ) { fFacetRotationAngle = iA; }
   void setFacetShape( int iS = 2 ) { fFacetShape = iS; }
   void setFacetSpacing( double iS = 0. ) { fFacetSpacing = iS; }
   void setMirrorDiameter( double iM = 12. ) { fMirrorDiameter = iM; }   
   void setNPixel( int iN = 0 ) { fNPixel = iN; }
   void setPixelSize( double iP = 15.498533 ) { fPixelSize = iP; }
   void setPixelSize_deg( double iP = 0.1 );
   void setPixelGeometricalEfficiency( double iE = 14.8/15.498533 ) { fPixelGeometricalEfficiency = iE; }
   void setTelescopePosition( double x, double y, double z, double rot ) { fTelescopeX = x; fTelescopeY = y; fTelescopeZ = z; fTelescopeRot = rot; }
};

#endif
