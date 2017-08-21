//! VGrisu  CORSIKA to GrIsu writer

#ifndef VGRISU_H
#define VGRISU_H

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>

#include "VCORSIKARunheader.h"

#include "mc_tel.h"
#include "sim_cors.h"

using namespace std;

class VGrisu
{
   private:
     bool bSTDOUT;                        //!< write output to stdout
     ofstream of_file;                    //!< output file
     map<int,int> particles;              //!< particle ID transformation matrix: first: CORSIKA ID, second: kascade ID
     float degrad;                        //!< convertion deg->rad
     int primID;                          //!< primary particle ID
     double xoff;                         //!< offset in x-coordinate for scattered core
     double yoff;                         //!< offset in y-coordinate for scattered core
     double qeff;                         //!< global quantum efficiency
     double observation_height;           //!< observation height [m]

     string fVersion;

     void transformCoord( float&, float&, float& );     //!< transform from CORSIKA to GrIsu coordinates
     void makeParticleMap();              //!< make map with  particle ID transformation matrix
     float redang( float );             //! reduce large angle to intervall 0, 2*pi

   public:
      VGrisu( string fVersion = "" );
     ~VGrisu() {}
      void setOutputfile( string );       //!< create grisu readable output file
      void setObservationHeight( double ih ) { observation_height = ih; }  //!< set observation height
      void setQeff( double iq ) { qeff = iq; } //!< set global quantum efficiency
      void setQueff( double iq ) { qeff = iq; } //!< set global quantum efficiency
      void writeRunHeader( float*, VCORSIKARunheader* );      //!<  write some information about CORSIKA run intot the runheader
      void writeEvent( telescope_array );  //!< write MC information
      void writePhotons( bunch, int );         //!< write next photon to grisu file ("P" line)
};

#endif
