/*! \file getThickness.cpp
    \brief calculate atmospheric thickness [g/cm2] from given height [m]

    using Bernloehrs routines and cherenkf routine

    Gernot Maier 
*/


#include <cmath>
#include <iostream>
#include <stdlib.h>
#include "atmo.h"
#include "fileopen.h"

using namespace std;

double gms( double iHeight );

int main( int argc, char **argv )
{
   int iatm = 6; // CORSIKA external atmosphere 6 (US Standard)
   double iobs = 11000.; 
   double ih;

   if( argc < 2 )
   {
      cout << "getThickness: calculate atmospheric thickness [g/cm2] from given height [m]" << endl;
      cout << "  usage: getThickness <height> <optional: atmprof number or -1 for kascade atmosphere>" << endl;
      exit( 0 );
   }
   ih = atof( argv[1] ) * 100.;    // altitude in [cm]

   if( argc == 3 ) iatm = atoi( argv[2] );
   if( iatm > 0 )
   {
      atmset_( &iatm, &iobs );
      cout << thickx_( &ih ) << endl;
   }
   else
   {
      cout << gms( ih/100. ) << endl;
   }
}


/*!
   Returns thickness(in gm/cm2) at altitude z (meters)

   kascade atmosphere

   copy from cherenkf.c
*/
double gms( double iHeight )
{
  double ZSEA = 0.;
  double ZATM1 = 11.01907;
  double ZATM2 = 32.16191;
  double ZATM3 = 53.44562;
  double CSEA = 1112.0841233223;
  double BSEA = 0.1101535373;
  double C11ATM = -99.5391872300;
  double C2ATM = 1317.3859402162;
  double B2ATM = 0.1586188238;
  double C22ATM = 1.4027419991;
  double B3ATM = 0.1350052403;
  double C3ATM = 724.2503723584;

  double z = 0.;
  double g = 0.;

  /* Convert to km. */
  z = iHeight / 1000.0;

  if( (z > ZATM3) || (z < ZSEA) ) cout << "gms: Altitude is out of bounds: " << iHeight << endl;
  else if(z < ZATM1) g = CSEA * exp(-BSEA * z) + C11ATM;
  else if(z < ZATM2) g = C2ATM * exp(-B2ATM * z) + C22ATM;
  else               g = C3ATM * exp(-B3ATM * z);

  return g;
}
