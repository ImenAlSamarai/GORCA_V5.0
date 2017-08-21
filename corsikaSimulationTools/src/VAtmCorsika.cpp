/*! \class VAtmCorsika
     \brief standard corsika atmosphere

     Revision $Id: VAtmCorsika.cpp,v 1.1.1.1 2011/07/21 20:35:56 gmaier Exp $

     \author Gernot Maier
*/

#include "VAtmCorsika.h"

VAtmCorsika::VAtmCorsika()
{
// layers US-Standard
    ftitle = "CORSIKA US-Standard";
    l_max.push_back( 4.E3 );    a.push_back( -186.5562     );  b.push_back( 1222.6562 );  c.push_back( 994186.38 );
    l_max.push_back( 10.E3 );   a.push_back(  -94.919      );  b.push_back( 1144.9069 );  c.push_back( 878153.55 );
    l_max.push_back( 40.E3 );   a.push_back(    0.61289    );  b.push_back( 1305.5948 );  c.push_back( 636143.04 );
    l_max.push_back( 100.E3 );  a.push_back(    0.0        );  b.push_back(  540.1178 );  c.push_back( 772170.16 );
    l_max.push_back( 112.E3 );  a.push_back(    0.01128292 );  b.push_back(    1.     );  c.push_back(      1.e9 );
}

void VAtmCorsika::fill( vector<double> i_alt )
{
   altitude = i_alt;
   rho.clear();
   thick.clear();
   n.clear();

   double alt = 0.;
   for( unsigned int i = 0; i < altitude.size(); i++ )
   {
      alt = altitude[i] * 1000.;
      rho.push_back( getRho( alt ) );
      thick.push_back( getThick( alt ) );
      n.push_back( getN( alt ) - 1. );
   }


   g_rho = new TGraph( altitude.size() );
   g_rho->SetTitle( ftitle.c_str() );
   g_rho->SetLineColor( 2 );
   g_rho->SetLineStyle( 2 );
  
   g_thick = new TGraph( altitude.size() );
   g_thick->SetTitle( ftitle.c_str() );
   g_thick->SetLineColor( 2 );
   g_thick->SetLineStyle( 2 );

   g_n = new TGraph( altitude.size() );
   g_n->SetTitle( ftitle.c_str() );
   g_n->SetLineColor( 2 );
   g_n->SetLineStyle( 2 );

   for( unsigned int i = 0; i < altitude.size(); i++ )
   { 
      if( rho[i] > 0. ) g_rho->SetPoint( i, altitude[i], log10( rho[i] ) );
      else g_rho->Set( g_rho->GetN() - 1 );
      if( thick[i] > 0. ) g_thick->SetPoint( i, altitude[i], log10( thick[i] ) );
      else g_thick->Set( g_thick->GetN() - 1 );
      if( n[i] > 0. ) g_n->SetPoint( i, altitude[i], n[i] );
      else g_n->Set( g_n->GetN() - 1 );
   }
}

VAtmCorsika::~VAtmCorsika()
{
/*   delete g_rho;
   delete g_thick;
   delete g_n; */
}

double VAtmCorsika::getRho( double alt )
{
   for( unsigned int j = 0; j < l_max.size(); j++ )
   {
     if( alt <= l_max[j] )
     {
        if( j < 4 ) return b[j] / c[j] * exp( -1. * alt * 1.e2 / c[j] );
	else return 1. / c[j];
     }
     else continue;
   }
   return 0.;
}

/*!
   \par alt altitude in [m]
*/
double VAtmCorsika::getThick( double alt )
{
   for( unsigned int j = 0; j < l_max.size(); j++ )
   {
     if( alt <= l_max[j] )
     {
        if( j < 4 ) return a[j] + b[j] * exp( -1. * alt * 1.e2 / c[j] );
        else return a[j] - b[j] * alt * 1.e2 / c[j];
     }
     else continue;
   }
   return 0.;
}

/*!
   \par alt altitude in [m]
*/
double VAtmCorsika::getN( double alt )
{
   return 1 + 0.000283 * getRho( alt ) / getRho( 0. );
}
