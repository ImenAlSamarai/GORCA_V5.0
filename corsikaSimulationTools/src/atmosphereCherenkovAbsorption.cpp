/*! \file  atmosphereCherenkovAbsorption
    \brief plot atmospheric absorption of Cherenkov photons

    Revision $Id: atmosphereCherenkovAbsorption.cpp,v 1.1.1.1 2011/07/21 20:35:56 gmaier Exp $
    
    \author 
      Gernot Maier
*/

#include "TApplication.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TRandom.h"
#include "TROOT.h"
#include "TStyle.h"

#include "VAtmosAbsorption.h"

/*
   the following section is for visualisation of the different extinction values
*/
void plotAbs( int nevents = 10000, double height = 10. )
{
   double imin = 180.;
   double imax = 700.;

   if( gDirectory->Get( "hWLall" ) ) delete gDirectory->Get( "hWLall" );
   if( gDirectory->Get( "hWLCorsika" ) ) delete gDirectory->Get( "hWLCorsika" );
   if( gDirectory->Get( "hWLkascade" ) ) delete gDirectory->Get( "hWLkascade" );

   TH1D *hWL = new TH1D("hWLall","no absorption", 500, imin, imax );
   hWL->SetXTitle( "wavelength [nm]" );
   TH1D *hWL2 = new TH1D("hWLCorsika","Corsika absorption", 500, imin, imax );
   hWL2->SetXTitle( "wavelength [nm]" );
   hWL2->SetLineStyle( 2 );
   hWL2->SetLineColor( 4 );
   hWL2->SetLineWidth( 2 );
   TH1D *hWL3 = new TH1D("hWLkascade","kascade absorption", 500, imin, imax );
   hWL3->SetXTitle( "wavelength [nm]" );
   hWL3->SetLineStyle( 3 );
   hWL3->SetLineColor( 2 );
   hWL3->SetLineWidth( 2 );

   VAtmosAbsorption a( "CORSIKA", 0 );
   a.setObservationlevel( 1275. );
   VAtmosAbsorption b( "kascade", 0 );
   b.setObservationlevel( 1275. );

   double wnorm = imin * imax / (imax - imin);

   double iwl;
   double iz;
   iz  = height * 1000.;
   for( int i = 0; i < nevents; i++ )
   {
      iwl = wnorm * imin / ( wnorm - gRandom->Uniform() * imin );
      hWL->Fill( iwl );
      if( a.probAtmAbsorbed( iwl, iz, 1.0 ) > gRandom->Uniform() ) hWL2->Fill( iwl );
      if( b.probAtmAbsorbed( iwl, iz, 1.0 ) > gRandom->Uniform() ) hWL3->Fill( iwl );
   }

// draw everything

   TCanvas *ca = new TCanvas( "ca", "", 10, 10, 600, 400 );
   ca->Draw();
   ca->cd();
   gStyle->SetOptStat( 0 );
   TLegend *l13 = new TLegend( 0.56,0.72,0.89,0.89);

   hWL->SetTitle( "atmospheric absorption CORSIKA/kascade" );
   hWL->Draw();
   l13->AddEntry( hWL, "no absorption", "l" );
   hWL2->Draw( "same" );
   l13->AddEntry( hWL2, "CORSIKA absorption", "l" );
   hWL3->Draw( "same" );
   l13->AddEntry( hWL3, "kascade absorption", "l" );

   l13->Draw();

}

TGraph* getTrans( double height, string model = "CORSIKA", string iSourceFile = "", double obs = 1275., bool bOpticalDepth = false )
{
   const int n = 5000;
   const double wlmin = 200.;
   const double wlmax = 700.; 
   double step = (wlmax - wlmin) / n;
   double wl[n];
   double prop[n];

   VAtmosAbsorption a( model, 0, iSourceFile );
   a.setObservationlevel( obs );

   for( int i = 0; i < n; i++ )
   {
      wl[i] = wlmin + i * step + step / 2.;
      if( !bOpticalDepth ) prop[i] = a.probAtmAbsorbed( wl[i], height, 1.0 );
      else
      {
	  double o = 0.;
          a.probAtmAbsorbed( wl[i], height, 1.0, o );  
	  prop[i] = o;
      }
      if( !isnormal( prop[i] ) )
      {
         cout << wl[i] << "\t" << height << endl;
      }
   }

   return new TGraph( n, wl, prop );
}

void plotTransCurves( double obs = 1275., double emissionHeight = 1000., string s_print = "", int iPrint = 2 )
{
    char ititle[600];

    TCanvas *cT = new TCanvas( "cT", "transmission probability", 10, 10, 600, 400 );
    cT->Draw();
    cT->cd();

    TCanvas *cX = new TCanvas( "cX", "transmission probability (comparision)", 410, 10, 600, 400 );
    cX->Draw();

    TCanvas *cO = new TCanvas( "cO", "optical depth", 610, 10, 600, 400 );
    cO->Draw();
    cO->SetLogy( 1 );

    TLegend* vLegends = new TLegend( 0.5,0.15,0.85,0.35);
    TLegend* vLegendsO = new TLegend( 0.5,0.65,0.85,0.85);

    vector< string > iName;
    vector< string > iFName;
    vector< string > iType;
    if( s_print.find( "US76" ) < s_print.size() )
    {
       iType.push_back( "modtran4" );     iFName.push_back( "data/us76.50km.ext" );    iName.push_back( "us76.50km" );
       iType.push_back( "modtran4" );     iFName.push_back( "data/us76.23km.ext" );    iName.push_back( "us76.23km" );
       iType.push_back( "modtran4" );     iFName.push_back( "data/us76.noAerosol.ext" );    iName.push_back( "us76.noAerosol" );
       iType.push_back( "modtran4" );     iFName.push_back( "data/us76.50km.2xO3.ext" );    iName.push_back( "us76.50km (2x03)" );
       iType.push_back( "modtran4" );     iFName.push_back( "data/us76.Desert_WS03.5ms.ext" );   iName.push_back( "us76.Desert (wind speed 3.5m/s)" ); 
    }
    else if( s_print.find( "KASCADE" ) < s_print.size() )
    {
       iType.push_back( "modtran4" );     iFName.push_back( "data/us76.50km.ext" );    iName.push_back( "us76.50km" );
       iType.push_back( "kascade" );      iFName.push_back( "data/kextint.dat" );         iName.push_back( "kascade" ); 
       iType.push_back( "artemis" );      iFName.push_back( "data/extinction_uv.dat" );         iName.push_back( "artemis" ); 
    }
    else if( s_print.find( "ATMO" ) < s_print.size() )
    {
       iType.push_back( "modtran4" );     iFName.push_back( "data/us76.50km.ext" );    iName.push_back( "us76.50km" );
       iType.push_back( "modtran4" );     iFName.push_back( "data/us76.23km.ext" );    iName.push_back( "us76.23km" );
       iType.push_back( "modtran4" );     iFName.push_back( "data/tropical.50km.ext" );   iName.push_back( "tropical.50km" );
       iType.push_back( "modtran4" );     iFName.push_back( "data/tropical.23km.ext" );   iName.push_back( "tropical.23km" );  
       iType.push_back( "modtran4" );     iFName.push_back( "data/midlatitudeSummer.50km.ext" );   iName.push_back( "midlatitude summer.50km" );
       iType.push_back( "modtran4" );     iFName.push_back( "data/midlatitudeSummer.23km.ext" );   iName.push_back( "midlatitude summer.23km" );
    }
    else if( s_print.find( "GND09" ) < s_print.size() )
    {
       iType.push_back( "modtran4" );     iFName.push_back( "data/us76.50km.ext" );    iName.push_back( "us76.50km" );
       iType.push_back( "modtran4" );     iFName.push_back( "data/us76.50km-GNDALT09km.ext" );    iName.push_back( "us76.50km (ground 0.9 km)" );
       iType.push_back( "modtran4" );     iFName.push_back( "data/us76.23km-GNDALT09km.ext" );    iName.push_back( "us76.23km (ground 0.9 km)" );
       iType.push_back( "modtran4" );     iFName.push_back( "data/us76.Desert_WS03.5ms-GNDALT09km.ext" );   iName.push_back( "us76.Desert (wind speed 3.5m/s, ground 0.9 km)" );
       iType.push_back( "modtran4" );     iFName.push_back( "data/us76.Desert_WS09.0ms-GNDALT09km.ext" );   iName.push_back( "us76.Desert (wind speed 9.0m/s, ground 0.9 km)" );
    }
//    iType.push_back( "corsika" );      iFName.push_back( "data/atmabs.dat" );          iName.push_back( "CORSIKA" );
    else if( s_print.find( "userprofile" ) < s_print.size() )
    {
       iType.push_back( "modtran4" );     iFName.push_back( "data/us76.50km.ext" );    iName.push_back( "us76.50km (MODTRAN)" );
       iType.push_back( "modtran4" );     iFName.push_back( "data/V.Crab.US76.50km.profile.ext" ); iName.push_back( "V.Crab" );
       iType.push_back( "modtran4" );     iFName.push_back( "data/V.Winter.US76.50km.profile.ext" ); iName.push_back( "V.Winter" );
       iType.push_back( "modtran4" );     iFName.push_back( "data/V.Summer.US76.50km.profile.ext" ); iName.push_back( "V.Summer" );
    }
    else if( s_print.find( "userdesert" ) < s_print.size() )
    {
       iType.push_back( "modtran4" );     iFName.push_back( "data/us76.50km.ext" );    iName.push_back( "us76.Tropo50km (MODTRAN)" );
       iType.push_back( "modtran4" );     iFName.push_back( "data/V.Crab.US76.50km.profile.ext" ); iName.push_back( "V.Crab.Tropo50km" );
       iType.push_back( "modtran4" );     iFName.push_back( "data/V.Winter.US76.50km.profile.ext" ); iName.push_back( "V.Winter.Tropo50km" );
       iType.push_back( "modtran4" );     iFName.push_back( "data/V.Summer.US76.50km.profile.ext" ); iName.push_back( "V.Summer.Tropo50km" );
       iType.push_back( "modtran4" );     iFName.push_back( "data/V.Summer.US76.Desert030ms.profile.ext" ); iName.push_back( "V.Summer.Desert.3.0m/s" );
       iType.push_back( "modtran4" );     iFName.push_back( "data/V.Summer.US76.Desert069ms.profile.ext" ); iName.push_back( "V.Summer.Desert.6.9m/s" );
    }
    else if( s_print.find( "ozone" ) < s_print.size() )
    {
       iType.push_back( "modtran4" );     iFName.push_back( "data/ozone/us76.50km.ext" );   iName.push_back( "us76.Tropo50km " );
       iType.push_back( "modtran4" );     iFName.push_back( "data/ozone/US76.50km.O3x100.ext" );   iName.push_back( "us76.Tropo50km (O3x100)" );
       iType.push_back( "modtran4" );     iFName.push_back( "data/ozone/US76.50km.O3x110.ext" );   iName.push_back( "us76.Tropo50km (O3x110)" );
       iType.push_back( "modtran4" );     iFName.push_back( "data/ozone/US76.50km.O3x120.ext" );   iName.push_back( "us76.Tropo50km (O3x120)" );
       iType.push_back( "modtran4" );     iFName.push_back( "data/ozone/US76.50km.O3x130.ext" );   iName.push_back( "us76.Tropo50km (O3x130)" );
       iType.push_back( "modtran4" );     iFName.push_back( "data/ozone/US76.50km.O3x200.ext" );   iName.push_back( "us76.Tropo50km (O3x200)" );
       iType.push_back( "modtran4" );     iFName.push_back( "data/ozone/US76.50km.O3x090.ext" );   iName.push_back( "us76.Tropo50km (O3x090)" );
       iType.push_back( "modtran4" );     iFName.push_back( "data/ozone/US76.50km.O3x080.ext" );   iName.push_back( "us76.Tropo50km (O3x080)" );
       iType.push_back( "modtran4" );     iFName.push_back( "data/ozone/US76.50km.O3x070.ext" );   iName.push_back( "us76.Tropo50km (O3x070)" );
       iType.push_back( "modtran4" );     iFName.push_back( "data/ozone/US76.50km.O3x050.ext" );   iName.push_back( "us76.Tropo50km (O3x050)" );
    }

    if( iName.size() == 0 )
    {
       cout << "no files" << endl;
       cout << s_print << endl;
       return;
    }

    vector< TGraph* > iC;
    vector< TGraph* > iO;
    vector< TGraph* > iCR;
    int c = 1;  // color
    for( unsigned int j = 0; j < iName.size(); j++ )
    {
       cout << "reading " << iFName[j] << endl;
       iC.push_back( getTrans( emissionHeight, iType[j], iFName[j], obs ) );
       iO.push_back( getTrans( emissionHeight, iType[j], iFName[j], obs, true ) );
       if( iC.back() )
       {
	  iC.back()->GetXaxis()->SetTitle( "wavelength [nm]" );
	  sprintf( ititle, "transmission (observation level=%.0f m)", obs );
	  iC.back()->GetYaxis()->SetTitle( ititle );
	  sprintf( ititle, "emission height above ground = %.0f m", emissionHeight );
	  iC.back()->SetTitle( ititle );
	  iC.back()->SetLineWidth( 2 );
	  iC.back()->SetMaximum( 1. );
	  if( c == 10 ) c++;
	  iC.back()->SetLineColor( c );
	  iC.back()->SetMarkerColor( c );
	  if( iFName[j] == "kascade" || iFName[j] == "artemis" ) iC.back()->SetLineStyle( 2 );
	  cT->cd();
	  if( j == 0 ) iC.back()->Draw( "ac" );
	  else         iC.back()->Draw( "c" );
	  
	  vLegends->AddEntry( iC.back(), iName[j].c_str(), "pl" );
	  iCR.push_back( new TGraph( 1 ) );
	  iCR.back()->GetXaxis()->SetTitle( "wavelength [nm]" );
	  sprintf( ititle, "transmission relative to %s", iName[0].c_str() );
	  iCR.back()->GetYaxis()->SetTitle( ititle );
	  sprintf( ititle, "emission height (asl) = %.0f m", emissionHeight );
	  iCR.back()->SetTitle( ititle );
	  iCR.back()->SetLineWidth( 2 );
	  iCR.back()->SetMaximum( 1. );
	  iCR.back()->SetLineColor( c );
	  iCR.back()->SetMarkerColor( c );
	  iCR.back()->SetLineStyle( iC.back()->GetLineStyle() );
	  double x1, x2, y1, y2;
	  for( int k = 0; k < iC.back()->GetN(); k++ )
	  {
	      iC[0]->GetPoint( k, x1, y1 );
	      iC.back()->GetPoint( k, x2, y2 );

	      if( y1 > 0. ) iCR.back()->SetPoint( k, x1, y2/y1 );
	      else          iCR.back()->SetPoint( k, x1, 1.5 );
          }
	  cX->cd();
	  if( j == 0 )
	  {
	     iCR.back()->Draw( "ac" );
	     iCR.back()->GetHistogram()->SetMaximum( 1.6 );
	     iCR.back()->GetHistogram()->SetMinimum( 0.3 );
          }
	  else         iCR.back()->Draw( "c" );

	  if( iO.back() )
	  {
	     cO->cd();
	     iO.back()->GetXaxis()->SetTitle( "wavelength [nm]" );
	     sprintf( ititle, "optical depth (observation level=%.0f m)", obs );
	     iO.back()->GetYaxis()->SetTitle( ititle );
	     sprintf( ititle, "emission height above ground = %.0f m", emissionHeight );
	     iO.back()->SetTitle( ititle );
	     iO.back()->SetLineWidth( 2 );
	     iO.back()->SetMaximum( 300. );
	     iO.back()->SetMinimum( 0.001 );
	     iO.back()->SetLineStyle( iC.back()->GetLineStyle() );
	     iO.back()->SetLineColor( c );
	     iO.back()->SetMarkerColor( c );
	     if( j == 0 ) iO.back()->Draw( "ac" );
	     else         iO.back()->Draw( "c" );
	     vLegendsO->AddEntry( iO.back(), iName[j].c_str(), "pl" );
          }
	     

          c++;
       }
          
    }

    cT->cd();
    vLegends->Draw();
    cX->cd();
    vLegends->Draw();
    cO->cd();
    vLegendsO->Draw();

    if( s_print.size() > 0 && s_print != "noprint" && iPrint < 2 )
    {
       sprintf( ititle, "%sTransmission_%f_%f.png", s_print.c_str(), obs, emissionHeight );
       cT->Print( ititle );
       sprintf( ititle, "%sTransmissionRel_%f_%f.png", s_print.c_str(), obs, emissionHeight );
       cX->Print( ititle );
       sprintf( ititle, "%sOpticalDepth_%f_%f.png", s_print.c_str(), obs, emissionHeight );
       cO->Print( ititle );
     } 
}

int main( int argc, char *argv[] )
{
   gROOT->SetStyle("Plain");
   TApplication theApp( "App", &argc, argv );

   if( argc != 5 )
   {
      cout << "atmosphereCherenkovAbsorption <observation height (above sea level) [m]> <emission height (above sea level) [m]> <print name> <0/1/2>" << endl;
      cout << endl;
      cout << "<print name>   0: exit after execution and printing, 1 don't exit and print, 2 don't exit and don't print" << endl;
      cout << "(" << argc << ")" << endl;
      for( int i = 0; i < argc; i++ )
      {
         cout << argv[i] << endl;
      }
      exit( 0 );
   }

   float obslevel = atof( argv[1] );
   float emilevel = atof( argv[2] );
   string s_print = argv[3];
   int   iExit = atoi( argv[4] );

   plotTransCurves( obslevel, emilevel, s_print, iExit );

   if( iExit == 0 ) exit( 0 );

   theApp.Run();
}
