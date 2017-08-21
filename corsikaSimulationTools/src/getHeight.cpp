/*! \file getHeight.cpp
    \brief calculate height [m] from atmospheric thickness [g/cm2]

    using Bernloehrs routines and cherenkf routine

    Gernot Maier 
*/


#include <cmath>
#include <iostream>
#include <stdlib.h>

#include "atmo.h"
#include "fileopen.h"

using namespace std;

double yds( double g );

int main( int argc, char **argv )
{
   int iatm = 6;   // US standard atmosphere (CORSIKA external amosphere 6)
   double iobs = 11000.;        // not important for this calculation
   double ih;

   if( argc < 2 )
   {
      cout << "getHeight: calculate height [m] from atmospheric thickness [g/cm2]" << endl;
      cout << "usage:   getHeight <thickness> <optional: atmosphere=atmprof number, -1=kascade atmosphere>" << endl;
      exit( 0 );
   }
   ih = atof( argv[1] );

   if( argc == 3 ) iatm = atoi(argv[2]);
   if( iatm > 0 )
   {
      atmset_( &iatm, &iobs );
      cout << heighx_( &ih ) / 100. << endl;
   }
   else
   {
      cout << yds( ih ) << endl;
   }
}


/*!
    Returns altitude (in m), at depth, g (in g/cm2)
    
    kascade atmosphere

    copy from cherenkf.c
    
*/
double yds( double g )
{
  double GMS1 = 230.8269624836;
  double GMS2 = 9.4225972083;
  double CSEA = 1112.0841233223;
  double BSEA = 0.1101535373;
  double C11ATM = -99.5391872300;
  double C2ATM = 1317.3859402162;
  double B2ATM = 0.1586188238;
  double C22ATM = 1.4027419991;
  double B3ATM = 0.1350052403;
  double C3ATM = 724.2503723584;

  double z;

  if( g > 1012. ) return 0.;

  if(g > GMS1) z = -log( (g - C11ATM) / CSEA ) / BSEA;
  else if(g > GMS2) z = -log( (g - C22ATM) / C2ATM ) / B2ATM;
  else z = -log( g / C3ATM ) / B3ATM;

  return(1000. * z);
}
