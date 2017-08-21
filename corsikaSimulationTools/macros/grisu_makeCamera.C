/*! \file  grisu_makeCamera
    \brief calculate PMT positions for camera of a certain size with a certain number of PMTs


    \author Gernot Maier
*/

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

/*!
     grisu_makeCamera( double fov = 3.5, double pmtradius = 15., double eff = 1., int gTel = 0 )

     make a camera (PMPIX lines) of a given size and PMT sizes.

     fov      :    field of view in degrees
     pmtradius:    pmt radius in mm
     eff      :    geometric collection efficiency
     gTel     :    0: plot only, 1: print results for T1 to screen (2 for T2, etc.)

     hardwired values:

     focal length = 12 m

     typical values for FOV = 3.5 deg:

     15mm   :  499 pixels
     10.75mm: 1003 pixels
     8.8mm  : 1500 pixels
*/
void grisu_makeCamera( double fov = 3.6, double pmtradius = 15.498533, double eff = 14.8/15.498533, int gTel = 0 )
{
   double degrad = 45./atan(1.);

   double focallength = 12.;   // [m]
   double kathoderadius = pmtradius * eff;
   double cameraradius = focallength * 1000. * tan( fov/degrad/2. );
   if( gTel == 0 ) cout << "cameraradius [mm]: " << cameraradius << endl;

   vector< double > PMTx;
   vector< double > PMTy;
   vector< double > PMTr;

   double nTubes = cameraradius/pmtradius + 4;
//   nTubes = (double)((int)nTubes*(int)nTubes);

   double x = 0;
   double y = 0;
// first make a rectangular map with distance 2*pmtradius in x and 2 * pmtradius * cos(60.) = pmtradius
//    shift every odd y row by pmtradius
   for( unsigned int i = 0; i < (unsigned int)(nTubes); i++ )   // xrow
   {
      y = 0.;
      for( unsigned int j = 0; j < (unsigned int)(nTubes); j++ ) // yrow
      {
          PMTx.push_back( x );
	  PMTy.push_back( y );
	  if( !(j%2==0) ) PMTx.back() = PMTx.back()+pmtradius;

	  if( i !=0 || j != 0 )
	  {
	      PMTx.push_back( -1.*x );
	      if( !(j%2==0) ) PMTx.back() = PMTx.back()+pmtradius;
	      PMTy.push_back( -1.*y );

	      PMTx.push_back( -1.*x );
	      if( !(j%2==0) ) PMTx.back() = PMTx.back()+pmtradius;
	      PMTy.push_back( y );
	      PMTx.push_back( x );
	      if( !(j%2==0) ) PMTx.back() = PMTx.back()+pmtradius;
	      PMTy.push_back( -1.*y );
          }
	  y += pmtradius*2.*sin(60./degrad);
      }
      x += pmtradius * 2.;
   }
// remove double elements
   if( gTel == 0 ) cout << "remove double values " << PMTx.size() << " " << PMTy.size() << endl;
   for( unsigned int i = 0; i < PMTx.size(); i++ )
   {
       for( unsigned int j = 0; j < PMTx.size(); j++ )
       {
           if( i == j ) continue;
	   if( PMTx[j] < -99990. ) continue;
	   if( PMTy[j] < -99990. ) continue;

	   if( sqrt((PMTx[i]-PMTx[j])*(PMTx[i]-PMTx[j])+(PMTy[i]-PMTy[j])*(PMTy[i]-PMTy[j])) < 1.e-2 )
	   {
	       PMTx[j] = -99999.;
	       PMTy[j] = -99999.;
           }
       }
   }




// calculate distance to array center
   for( unsigned int i = 0; i <  PMTx.size(); i++ )
   {
       if( PMTx[i] > -99990. && PMTy[i]  > -99990. ) PMTr.push_back( sqrt(PMTx[i]*PMTx[i]+PMTy[i]*PMTy[i] ) );
       else                                          PMTr.push_back( -99999. );
   }
// now remove all elements which are further away than pmtradius
   int iElements = (int)PMTx.size();
   if( gTel == 0 ) cout << "remove pmts outside FOV" << endl;
   for( unsigned int i = 0; i < PMTr.size(); i++ )
   {
      if(  PMTr[i] < -99990. || PMTr[i]+pmtradius > cameraradius )
      {
          PMTx[i] = -99999.;
	  PMTy[i] = -99999.;
	  iElements--;
      }
      if( fabs(PMTx[i])<1.e-5) PMTx[i] = 0.;
      if( fabs(PMTy[i])<1.e-5) PMTy[i] = 0.;
   } 
// plot camera
   if( gTel == 0 ) cout << "number of PMTs: " << iElements << endl;
   TCanvas *cX = new TCanvas( "cX", "", 10, 10, 800, 800 );
   cX->Draw();
   gPad->SetGridx( 1 );
   gPad->SetGridy( 1 );
   gPad->SetLeftMargin( 0.12 );
   gPad->SetRightMargin( 0.06 );
   TGraph *iG = new TGraph( iElements );
   vector< TEllipse * > iPMT;
   vector< int > iID;
   int iZ = 0;
   for( unsigned int i = 0; i < PMTx.size(); i++ )
   {
      iID.push_back( -1 );
      if( PMTx[i] != -99999. )
      {
        iG->SetPoint( iZ, PMTx[i], PMTy[i] );
	iPMT.push_back( new TEllipse( PMTx[i], PMTy[i], kathoderadius ) );
	iPMT.back()->SetFillStyle( 4000 );
	iID.back() = iZ+1;
	iZ++;
      }
   }
   iG->SetMarkerStyle( 7 );
   iG->SetTitle( "" );
   iG->Draw( "ap" );
   iG->GetHistogram()->SetXTitle( "x [mm]" );
   iG->GetHistogram()->SetYTitle( "y [mm]" );
   iG->GetHistogram()->GetYaxis()->SetTitleOffset( 1.4 );
   for( unsigned int i = 0; i < iPMT.size(); i++ ) iPMT[i]->Draw();
// draw camera circumfence
   TEllipse *iE = new TEllipse( 0., 0., cameraradius );
   iE->SetFillStyle( 4000 );
   iE->SetLineWidth( 2 );
   iE->Draw();

// output camera

   if( abs(gTel) > 0 )
   {
      iZ = 1;
      for( unsigned int i = 0; i < PMTx.size(); i++ )
      {
         if( PMTx[i] != -99999. )
	 {
	    cout << "* PMPIX " << abs(gTel) << "\t";
	    cout << iZ++ << "   1 0.0   ";
	    cout << fixed << setprecision(6) << showpos << PMTx[i] << "\t" << PMTy[i] << "\t";
	    cout << setprecision(2) << noshowpos << kathoderadius;
	    cout << "\t0.81 1 0.35 3.4 30.0 0.000 1  1   0.0   1.0" << endl;
         }
      }
   }

// neighbour list
   if( gTel < 0 )
   {
      for( unsigned int i = 0; i < PMTx.size(); i++ )
      {
         if( PMTx[i] == -99999. ) continue;
	 vector< int > iNeigh;
         for(  unsigned int j = 0; j < PMTx.size(); j++ )
	 {
	    if( PMTx[j] == -99999. ) continue;
	    if( i == j ) continue;
// check if next pmt is in pmtradius*1.1
	    if( sqrt((PMTx[i]-PMTx[j])*(PMTx[i]-PMTx[j])+(PMTy[i]-PMTy[j])*(PMTy[i]-PMTy[j]))<pmtradius*2.*1.1 )
	    {
	       iNeigh.push_back( iID[j] );
            }
         }
	 cout << "* NGHBR\t" << abs(gTel) << "\t" << iID[i] << "\t" << iNeigh.size();
	 for( unsigned j = 0; j < iNeigh.size(); j++ ) cout << "\t" << iNeigh[j];
	 cout << endl;
      }
   }

}
      


/*! 
     grisu_write_Camera_with_new_pixel_size

     write new PMPIX lines for a camera with a different pixel size (but same number of pixel)

     use an existing .cfg files to read pixel positions (assume PMT distance is 15.498533 mm)

     iFile    :    input .cfg file
     iPMTD    :    new PMT distance in [mm], e.g. 31.4 mm

*/
void grisu_write_Camera_with_new_pixel_size( string iFile, double iPMTD )
{
   std::ifstream inFileStream( iFile.c_str() );
   if( !inFileStream )
   {
       cout << "error: input file not found" << endl;
       exit( 0 );
   }

   string iline;
   string itemp;
   double iX = 0.;

   while( getline( inFileStream, iline ) )
   {
      if( iline.substr( 0, 1 ) != "*" ) continue;

      if( iline.find( "PMPIX" ) >= iline.size() ) continue;

      std::istringstream i_stream( iline );

      i_stream >> itemp;   cout << itemp << " ";
      i_stream >> itemp;   cout << itemp << " ";
      i_stream >> itemp; cout << itemp << "  ";
      i_stream >> itemp;   cout << setw( 6 ) << itemp << "  ";
      i_stream >> itemp;   cout << itemp << "  ";
      i_stream >> itemp;   cout << itemp << "  ";

      i_stream >> itemp;
      iX = (double)TMath::Nint( atof( itemp.c_str() )/15.498533 );
      cout << setw(8)  << iX*iPMTD/2 << "  ";

      i_stream >> itemp;   
      iX = (double)TMath::Nint( atof( itemp.c_str() )/26.808302 );
      cout << setw(8) << iX*iPMTD*sqrt(3.)/2 << "  ";

      for( int i = 0; i < 11; i++ )
      {
         i_stream >> itemp;   cout << itemp << "  ";
      }


      cout << endl;
   }
   inFileStream.close();
}
