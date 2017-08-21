/*! \class VAtmKascade
    \brief kascade atmosphere

    Revision $Id: VAtmKascade.cpp,v 1.1.1.1 2011/07/21 20:35:56 gmaier Exp $

    \author Gernot Maier
*/

#include "VAtmKascade.h"

VAtmKascade::VAtmKascade()
{
   ftitle = "kascade atmosphere";

// kascade atmospheric parameters
   zsea = 0.;
   zatm1 = 11.01907;
   zatm2 = 32.16191; 
   zatm3 = 53.44562;
   rsea=1.2250e-3;
   ratm1=0.36391e-3;
   ratm2=0.012721e-3;
   ratm3=0.00071881e-3;
   bsea=log(rsea/ratm1)/(zatm1-zsea);
   b2atm=log(ratm1/ratm2)/(zatm2-zatm1);
   b3atm=log(ratm2/ratm3)/(zatm3-zatm2);
   asea=rsea;
   a2atm=asea*exp(-(bsea-b2atm)*zatm1);
   a3atm=a2atm*exp(-(b2atm-b3atm)*zatm2);
   csea=100000.*asea/bsea;
   c2atm=100000.*a2atm/b2atm;
   c3atm=100000.*a3atm/b3atm;
   c11atm=c3atm*exp(-zatm2*b3atm) + c2atm*(exp(-zatm1*b2atm) -exp(-b2atm*zatm2)) - csea*exp(-zatm1*bsea);
   c22atm=c3atm*exp(-zatm2*b3atm) - c2atm*exp(-b2atm*zatm2);

// index of refraction
   lambda = 400;
}

void VAtmKascade::fill( vector<double> i_alt )
{
   altitude = i_alt;
   rho.clear();
   thick.clear();
   n.clear();
// now fill the graphs
   double alt = 0.;
   for( unsigned int i = 0; i < altitude.size(); i++ )
   {
      alt = altitude[i];

      thick.push_back( getThick( alt ) );
      rho.push_back( getRho( alt ) );
      n.push_back( getN( alt ) );
   }

  g_rho = new TGraph( altitude.size() );
  g_rho->SetTitle( ftitle.c_str() );
  g_rho->SetLineColor( 4 );
  g_rho->SetLineStyle( 3 );
  
  g_thick = new TGraph( altitude.size() );
  g_thick->SetTitle( ftitle.c_str() );
  g_thick->SetLineColor( 4 );
  g_thick->SetLineStyle( 3 );

  g_n = new TGraph( altitude.size() );
  g_n->SetTitle( ftitle.c_str() );
  g_n->SetLineColor( 4 );
  g_n->SetLineStyle( 3 );

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

double VAtmKascade::getRho( double alt )
{
   if( alt < zatm1 ) return asea*exp(-alt*bsea);
   else if( alt < zatm2 ) return a2atm*exp(-alt*b2atm);
   else return a3atm*exp(-alt*b3atm);
   return 0.;
}

double VAtmKascade::getThick( double alt )
{
  if( alt < zatm1 ) return csea*exp(-bsea*alt) + c11atm;
  else if( alt < zatm2 ) return c2atm*exp(-b2atm*alt) + c22atm;
  else return c3atm*exp(-alt*b3atm);
  return 0.;
}

double VAtmKascade::getN( double alt )
{
    return ( eta_sea( lambda ) * getRho( alt ) / getRho( 0. ) );
}

double VAtmKascade::eta_sea( double lambda )
{
   return 1.0E-07 * (2726.43 + 12.288 / (lambda * lambda * 1.0E-06) + 0.3555 / (lambda * lambda * lambda * lambda * 1.0E-12));
}

VAtmKascade::~VAtmKascade()
{
/*   delete g_rho;
   delete g_thick;
   delete g_n; */
}
