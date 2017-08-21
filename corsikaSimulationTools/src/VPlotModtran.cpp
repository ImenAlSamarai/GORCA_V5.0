/*! \class VPlotModtran
    \brief plot atmosphere and cherenkov properties from CORSIKA/KASCADE atmospheres

    Revision $Id: VPlotModtran.cpp,v 1.1.1.1 2011/07/21 20:35:56 gmaier Exp $

    \author Gernot Maier
*/

#include "VPlotModtran.h"

VPlotModtran::VPlotModtran( string atmprof )
{
   fMaxAltitude = 55.;

   if( atmprof.size() == 0 ) return;
   altitude.clear(),
   rho.clear();
   thick.clear();
   n.clear();

   ifstream inf( atmprof.c_str() );
   if( !inf )
   {
      cout << "VPlotModtran::VPlotModtran error: file not found: " << atmprof << endl;
      exit( -1 ); 
   }

   string i_title;
   string i_line;
   double i_temp = 0.;
   int z = 0;
   while( getline( inf, i_line ) )
   {
      if( z == 0 ) i_title = i_line;
      z++;
      if( i_line.substr( 0, 1 ) == "#" ) continue;
      istringstream is_stream( i_line );
      is_stream >> i_temp;
      if( i_temp > 112.8 ) continue;
      altitude.push_back( i_temp );
      is_stream >> i_temp;
      rho.push_back( i_temp );
      is_stream >> i_temp;
      thick.push_back( i_temp );
      is_stream >> i_temp;
      n.push_back( i_temp );
   }

   g_rho = new TGraph( altitude.size() );
   g_rho->SetTitle( i_title.c_str() );
   
   g_thick = new TGraph( altitude.size() );
   g_thick->SetTitle( i_title.c_str() );

   g_n = new TGraph( 1 );
   g_n->SetTitle( i_title.c_str() );

   for( unsigned int i = 0; i < altitude.size(); i++ )
   { 
      if( rho[i] > 0. ) g_rho->SetPoint( i, altitude[i], log10( rho[i] ) );
      else g_rho->Set( g_rho->GetN() - 1 );
      if( thick[i] > 0. ) g_thick->SetPoint( i, altitude[i], log10( thick[i] ) );
      else g_thick->Set( g_thick->GetN() - 1 );
      if( altitude[i] > fMaxAltitude ) continue;
      if( n[i] > 0. ) g_n->SetPoint( i, altitude[i], n[i] );
      else g_n->Set( g_n->GetN() - 1 );
   }
}

VPlotModtran::~VPlotModtran()
{
   delete g_rho;
   delete g_thick;
   delete g_n;
}

