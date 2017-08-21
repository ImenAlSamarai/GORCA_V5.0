/*!  \file plot_psf.C
     \brief plot psf related stuff

      input from GrIsu make_photons and modified grisudet

     Revision $Id: plot_psf.C,v 1.1.1.1 2011/07/21 20:35:56 gmaier Exp $

      \author 
      Gernot Maier
*/


#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "TEllipse.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLegend.h"
#include "TLine.h"
#include "TMarker.h"
#include "TText.h"


using namespace std;

/*!
    plot point spread function

    input:  photon lists from grisu make_photons/grisudet (x/y positions of photons on camera plane)
            accept wobble in y direction only

    ywobble:    wobble offset [deg]
    F      :    F number of the telescope
    bPlot  :    plot results (if true)
    pmt    :    PMT radius in [mm]
    D      :    mirror diameter in [mm]

    returns:    PSF as max{ RMS_tangential, RMS_sagittal } x 2

*/
double psf( char *iFile = "on_camera_0040_0050.txt", int ntel = 0, double ywobble = 0., double F = 1., bool bPlot = true, double pmt = 29.60, double D = 12.E3, int nPhotons = -1, double wmax = 0.35, char *fMeasuredFile = 0  )
{
   double degrad = 45./atan(1.);
   char hname[200];
   sprintf( hname, "hXY_%f_%f", ywobble, F );
   TH2D *hXY = new TH2D( hname, "xy", 100, -2000., 2000., 100, -2000., 2000. );
   sprintf( hname, "hdeg_%f_%f", ywobble, F );
   TH2D *hdeg = new TH2D( hname, "deg", 100, -wmax, wmax, 100, ywobble - wmax, ywobble + wmax );
   sprintf( hname, "hdeg1D_%f_%f", ywobble, F );
   TH1D *hdeg1D = new TH1D( hname, "deg1D", 500, 0., 1. );
   sprintf( hname, "hdeg1DBW_%f_%f", ywobble, F );
   TH1D *hdeg1DBW = new TH1D( hname, "deg1D", 500, 0., 1. );

   double x;
   double y;
   ifstream is;
   is.open( iFile );
   if( !is )
   {
      cout << "file not found" << endl;
      return 0.;
   }
   cout << "reading " << iFile << endl;

   string itemp;
   string is_line;
   int itemp_ntel = 0;
   int z = 0;
   while( getline( is, is_line ) )
   {
      istringstream is_stream( is_line );
      for( unsigned int i = 0; i < 6; i++ ) is_stream >> itemp;
      is_stream >> itemp_ntel;
      if( itemp_ntel != ntel-1 ) continue;
      is_stream >> itemp;
      is_stream >> x;
      is_stream >> y;

      if( x > -9000. && y > -9000. ) 
      {
// x,y in cm
	 hXY->Fill( x, y );
	 hdeg->Fill( atan(x/D/F)*degrad, atan(y/D/F)*degrad );
	 hdeg1D->Fill( sqrt( atan(x/D/F)*degrad*atan(x/D/F)*degrad + (atan(y/D/F)*degrad-ywobble)*(atan(y/D/F)*degrad-ywobble) ) );
	 hdeg1DBW->Fill( sqrt( atan(x/D/F)*degrad*atan(x/D/F)*degrad + (atan(y/D/F)*degrad-ywobble)*(atan(y/D/F)*degrad-ywobble) ) ); 

// x,y in deg
/*	 hXY->Fill( D * F * tan( x / degrad ),  D * F * tan( y / degrad ) );
	 hdeg->Fill( x, y );
	 hdeg1D->Fill( sqrt( x*x + (y-ywobble)*(y-ywobble) ) );
	 hdeg1DBW->Fill( sqrt( x*x + (y-ywobble)*(y-ywobble) ) ); */
	 if( nPhotons > 0 && z > nPhotons ) break;
	 z++;
      }
   }
   is.close();

   cout << "Total number of photons: " << hdeg->GetEntries() << endl;
   cout << "PSF (FWHM) X: " << hdeg->GetRMS( 1 ) * 2.355 << endl;
   cout << "PSF (FWHM) Y: " << hdeg->GetRMS( 2 ) * 2.355 << endl;
   cout << "PSF (RMS tangential x 2) X: " << hdeg->GetRMS( 1 ) * 2. << endl;
   cout << "PSF (RMS sagittals  x 2) Y: " << hdeg->GetRMS( 2 ) * 2. << endl;
   double iPSF_FWHM = TMath::Max(  hdeg->GetRMS( 1 ) * 2.,  hdeg->GetRMS( 2 ) * 2. );

   for( int i = hdeg1D->GetNbinsX() - 1; i > 0; i-- )
   {
      hdeg1D->SetBinContent( i, hdeg1D->GetBinContent( i ) +  hdeg1D->GetBinContent( i + 1 ) );
   }
   if( hdeg1D->GetMaximum() > 0. ) hdeg1D->Scale( 1./hdeg1D->GetMaximum() );
   for( int i = 2; i <= hdeg1DBW->GetNbinsX(); i++ )
   {
      hdeg1DBW->SetBinContent( i, hdeg1DBW->GetBinContent( i ) +  hdeg1DBW->GetBinContent( i - 1 ) );
   }
   if( hdeg1DBW->GetMaximum() > 0. ) hdeg1DBW->Scale( 1./hdeg1DBW->GetMaximum() );
   double c68 = 0.;
   double c80 = 0.;
   double c95 = 0.;
   for( int i = hdeg1D->GetNbinsX() - 1; i > 0; i-- )
   {
      if( hdeg1D->GetBinContent( i ) > 1.-0.68 )
      {
          c68 = hdeg1D->GetBinCenter( i );
	  break;
      }
   }
   for( int i = hdeg1D->GetNbinsX() - 1; i > 0; i-- )
   {
      if( hdeg1D->GetBinContent( i ) > 1.-0.8 )
      {
         c80 = hdeg1D->GetBinCenter( i );
	 break;
      }
   }
   for( int i = hdeg1D->GetNbinsX() - 1; i > 0; i-- )
   {
      if( hdeg1D->GetBinContent( i ) > 1.-0.95 )
      {
         c95 = hdeg1D->GetBinCenter( i );
	 break;
      }
   }
   cout << "containment radius: " << c68 << " " << c80 << " " << c95 << endl;

// draw everything
   if( bPlot )
   {
      TCanvas *cxy = new TCanvas( "cxy", "xy (deg)", 10, 10, 450, 400 );
      cxy->Draw();
      gPad->SetRightMargin( 0.15 );
      gPad->SetGridx( 0 );
      gPad->SetGridy( 0 );

      hXY->SetTitle( "" );
      hXY->Draw( "colz" );


      TCanvas *cxyDeg = new TCanvas( "cxyDeg", "xy (deg)", 10, 10, 450, 400 );
      cxyDeg->Draw();
      gPad->SetRightMargin( 0.15 );
      gPad->SetGridx( 0 );
      gPad->SetGridy( 0 );

      hdeg->SetTitle( "" );
      hdeg->SetStats( 0 );
      hdeg->SetXTitle( "x [deg]" );
      hdeg->SetYTitle( "y [deg]" );
      hdeg->Draw( "colz" );

// plot marker at mean values
      TMarker *iC = new TMarker( hdeg->GetMean( 1 ), hdeg->GetMean( 2 ), 5 );
      cout << "MEAN XY [deg]: " << hdeg->GetMean( 1 ) << " \t" << hdeg->GetMean( 2 ) << endl;
      iC->SetMarkerSize( 2 );
      iC->Draw();


      TEllipse *e = new TEllipse( 0., ywobble, atan(pmt/D/F)*degrad, atan(pmt/D/F)*degrad );
      e->SetFillStyle( 0 );
      e->Draw();

      char hname[200];
      sprintf( hname, "FWHM %.3f deg", 0.5*(hdeg->GetRMS( 1 )+hdeg->GetRMS( 2 ))*2.355 );
      TText *it = new TText( -0.15, ywobble + 0.16, hname );
      it->Draw();

// containment radius
      TCanvas *c1D = new TCanvas( "c1D", "containment radius", 450, 10, 450, 400 );
      c1D->SetGridx( 0 );
      c1D->SetGridy( 0 );

      hdeg1D->SetAxisRange( 0., 0.2 );
      hdeg1D->SetStats( 0 );
      hdeg1D->SetTitle( "" );
      hdeg1D->SetXTitle( "distance to star" );
      hdeg1D->SetMinimum( 0. );

      hdeg1D->Draw();
      TLine *iL68 = new TLine( c68, 0., c68, 0.6 );
      iL68->Draw();
      TLine *iL80 = new TLine( c80, 0., c80, 0.6 );
      iL80->Draw();
      TLine *iL95 = new TLine( c95, 0., c95, 0.6 );
      iL95->Draw();

// PMT
      TLine *iLPMT = new TLine( atan(pmt/D/F)*degrad, 0., atan(pmt/D/F)*degrad, 0.8 );
      iLPMT->SetLineColor( 4 );
      iLPMT->SetLineStyle( 4 );
      iLPMT->Draw();

      sprintf( hname, "0.68/0.80/0.95 containment diameter:" ); 
      TText *ict = new TText( 0.03, 0.90, hname );
      ict->Draw();
      sprintf( hname, "%.3f / %.3f / %.3f deg", c68*2., c80*2., c95*2. );
      TText *ict2 = new TText( 0.03, 0.82, hname );
      ict2->Draw();

      TCanvas *c1DBC = new TCanvas( "c1DBC", "containment radius (integral)", 850, 10, 450, 400 );
      c1DBC->SetGridx( 0 );
      c1DBC->SetGridy( 0 );

      hdeg1DBW->SetAxisRange( 0., 0.1 );
      hdeg1DBW->SetStats( 0 );
      hdeg1DBW->SetTitle( "" );
      hdeg1DBW->SetXTitle( "distance to star" );
      hdeg1DBW->SetYTitle( "fraction confined" );
      hdeg1DBW->SetMinimum( 0. );
      hdeg1DBW->SetLineWidth( 2 );
      hdeg1DBW->Draw();

      if( fMeasuredFile )
      {
         TFile *f = new TFile( fMeasuredFile );
	 if( !f->IsZombie() ) 
	 {
	    TCanvas *iC = (TCanvas*)f->Get( "c1" );
	    if( iC )
	    {
	       TH1F *h = (TH1F*)iC->GetListOfPrimitives()->FindObject("hr2");
	       if( h )
	       {
		  c1DBC->cd();
		  h->SetLineColor( 2 );
		  h->SetLineWidth( 2 );
		  h->SetLineStyle( 1 );
		  h->Draw( "same" );

		  TLegend *iL = new TLegend( 0.55, 0.2, 0.82, 0.4 );
		  iL->AddEntry( h, "PSF (T4)", "l" );
		  iL->AddEntry( hdeg1DBW, "PSF (MC)", "l" );
		  iL->Draw();
	       }
	       else cout << "Error finding histogram with measured PSF" << endl;
            }
         }
	 else cout << "Problems opening file with measured PSF: " << fMeasuredFile << endl;
      }
   }

   return iPSF_FWHM;
}

/*!
    plot PSF vs distance to camera center

    input:  photon lists from grisu make_photons/grisudet (x/y positions of photons on camera plane)
            expected filename:  F15_WOFF70.txt (15 -> F=1.5, 70 -> 7 deg distance to camera center

    (hardwired list of wobble offsets)

*/
TCanvas* plot_PSF_vs_offset( TCanvas *cPSFOffset = 0, int F_10 = 30, int iColor = 1, string iDataDir = "/Users/uleeds/veritas/Documents/Dokuments/simulation_detector/optics/psf/code/" )
{
    bool bPlot = false;
    TH1D *hnull = 0;
    if( !cPSFOffset )
    {
       bPlot = true;
       cPSFOffset = new TCanvas( "cPSFOffset", "psf vs offset", 10, 10, 400, 400 );
       cPSFOffset->SetGridx( 0 );
       cPSFOffset->SetGridy( 0 );
       cPSFOffset->Draw();

       hnull = new TH1D( "hnPSFW", "", 100, 0., 8. );
       hnull->SetStats( 0 );
       hnull->SetXTitle( "distance to camera centre [deg]" );
       hnull->SetYTitle( "PSF [arcmin]" );
       hnull->GetYaxis()->SetTitleOffset( 1.2 );
       hnull->SetMinimum( 0. );
       hnull->SetMaximum( 15. );
       hnull->Draw();
    }
    else cPSFOffset->cd();

    TGraph *gPSF = new TGraph(1);
    gPSF->SetTitle( "" );
    gPSF->SetLineWidth( 2. );
    gPSF->SetMarkerStyle( 20 );
    gPSF->SetMarkerColor( iColor );
    gPSF->SetLineColor( iColor );

    char hname[200];
    string iFileName = iDataDir;
    vector< string> woff;
    woff.push_back( "00" );
    woff.push_back( "10" );
    woff.push_back( "25" );
    woff.push_back( "50" );
    woff.push_back( "70" );

    for( unsigned int i = 0; i < woff.size(); i++ )
    {
       sprintf( hname, "%sF%d_WOFF%s.grisu", iFileName.c_str(), F_10, woff[i].c_str() );

       double id = atof( woff[i].c_str() ) / 10.;

       double w = psf( hname, id, (double)(F_10)/10., false, 29.60 ) * 60.;

       gPSF->SetPoint( i, id, w );
     }
     gPSF->Draw( "pl" );

     return cPSFOffset;
}


   
