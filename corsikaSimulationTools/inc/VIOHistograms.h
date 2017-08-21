//!  VIOHistosgrams  histogram filling class

#ifndef VHISTO_H
#define VHISTO_H

#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TList.h"
#include "TClonesArray.h"
#include "TMath.h"
#include "TTree.h"

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "mc_tel.h"
#include "sim_cors.h"

using namespace std;

class VIOHistograms
{
   private:
      TList *hisList;
   
      TH1D *hBunch;
      TH1D *hT0;
      TH1D *hZem;

      TH2D *hGXY;
      TH2D *hGZeAz;
      TH1D *hGLambda;
      TH1D *hGProb;
      TH1D *hGZem;
      TH2D *hSXY;
      TH2D *hSZeAz;
      TH1D *hSLambda;
      TH1D *hSProb;
      TH1D *hSZem;

      TClonesArray *hCXYZ;

      TFile *fout;
      TTree *fTree;

      vector< double > fXYZlevelsHeight;
      vector< double > fXYZlevelsThickness;


      int nevent;
      double degrad;

// event block

      int eventNumber;
      int particleID;
      double energy;
      double startAlt;
      double firstInt;
      double ze;
      double az;
      int date;
      int runNumber;
      double corsVersion;
      double obsLevel;
      double eslope;
      double e0min;
      double e0max;
      double cutHad;
      double cutMuon;
      double cutEM;
      double cutPhot;
      double magX;
      double magZ;
      int bCher;
      double cherBunch;
      double cherLambdaMin;
      double cherLambdaMax;
      int    cherNum;
      int    telNumber;
      int    arrayNumber;
      int    coreNumber;
      double telXpos[MAX_TEL];
      double telYpos[MAX_TEL];
      double telZpos[MAX_TEL];
      double telR[MAX_TEL];
      double xCore;
      double yCore;
      double rCore;
      double xmax;
      double emax;
      double cmax;
      double hmax;
      double NCp[MAX_TEL];

      double toff;
      bool bShort;

      bool bCORSIKA_coordinates;   // fill photons in corsika coordinates

      bool bSmallFile; // reduce output
      bool bMuon;      // adjust histograms for muon input

      double redang( double );
      void transformCoord( double&, double&, double& );

   public:
      VIOHistograms();
     ~VIOHistograms() {}
      void init( string, bool );
      void newEvent( float*, telescope_array, int  );
      void initXYZhistograms();
      void fillBunch( bunch, double );
      void fillGenerated( bunch, double );
      void fillNPhotons( int iTel, double iphotons );
      void fillSurvived( bunch, double );
      void setCORSIKAcoordinates() { bCORSIKA_coordinates = true; }
      void set_small_file() { bSmallFile = true; }
      void setMuonSettings() { bMuon = true; }
      void setXYZlevelsHeight( vector<double> iL ) { fXYZlevelsHeight = iL; }
      void setXYZlevelsThickness( double istart, double idiff );
      void terminate();
};
      
#endif
