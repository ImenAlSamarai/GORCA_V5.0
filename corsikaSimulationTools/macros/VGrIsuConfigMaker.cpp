/*! \class VGrIsuConfigMaker
    \brief make configuration for a single telescope

    - calculate PMT positions for camera of a certain size with a certain number of PMTs


    \author Gernot Maier
*/

#include "VGrIsuConfigMaker.h"

VGrIsuConfigMaker::VGrIsuConfigMaker( int iTelID )
{
    fTelescopeID = iTelID;

    setFocalLength();
    setFieldofView();
    setNPixel();
    setPixelSize();
    setPixelGeometricalEfficiency();

    setF_over_D();
    setMirrorDiameter();
    setFacetBlureRadius();
    setFacetDiameter();
    setFacetMisalignement();
    setFacetRotationAngle();
    setFacetShape();
    setFacetSpacing();

    setFocalLength();
}

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
void VGrIsuConfigMaker::makeCamera( bool bPrint, bool bPlot, bool bHex )
{
   fGrisuline_PMPIX.clear();
   fGrisuline_NGHBR.clear();
   fGrisuline_PIXLC.clear();

   double fFocalLength = fMirrorDiameter * fMirror_f_over_D;   // [m]
   if( !bPrint ) cout << "focal length: " << fFocalLength << endl;
   double kathoderadius = fPixelSize * fPixelGeometricalEfficiency;
   double cameraradius = fFocalLength * 1000. * tan( fFieldofView/TMath::RadToDeg()/2. );
   if( !bPrint ) cout << "cameraradius [mm]: " << cameraradius << endl;

   vector< double > PMTx;
   vector< double > PMTy;
   vector< double > PMTr;
   vector< unsigned int > PMTNN;

   double nTubes = cameraradius/fPixelSize + 4;

   double x = 0;
   double y = 0;
// first make a rectangular map with distance 2*pmtradius in x and 2 * pmtradius * cos(60.) = pmtradius
//    shift every odd y row by pmtradius
   for( unsigned int i = 0; i < (unsigned int)(nTubes); i++ )   // xrow
   {
      y = 0.;
      for( unsigned int j = 0; j < (unsigned int)(nTubes); j++ ) // yrow
      {
	  if( sqrt( x*x + y*y ) + fPixelSize < cameraradius )
	  {

// right top
	     PMTx.push_back( x );
	     PMTy.push_back( y );
// shift every second row by half a pixel size
   	     if( bHex && !(j%2==0) ) PMTx.back() = PMTx.back()+fPixelSize;

// left top
	     if( ( bHex && ( i != 0 || !(j%2 == 0) ) ) || (!bHex && i != 0 ) ) 
	     { 
		PMTx.push_back( -1.*x );
		PMTy.push_back(     y );
		if( bHex && !(j%2==0) ) PMTx.back() = PMTx.back()-fPixelSize;
	     }
// right bottom
             if( j != 0 )
	     {
		PMTx.push_back(     x );
		PMTy.push_back( -1.*y );
		if( bHex && !(j%2==0) ) PMTx.back() = PMTx.back()+fPixelSize;
	     }
// left bottom
	     if( ( bHex && j != 0 && ( i != 0 || !(j%2==0) ) ) || (!bHex && j != 0 && i != 0 ) )
	     { 
		PMTx.push_back( -1.*x );
		PMTy.push_back( -1.*y );
		if( bHex && !(j%2==0) ) PMTx.back() = PMTx.back()-fPixelSize;
	     }
          }
	  if( bHex ) y += fPixelSize*2.*sin(60./TMath::RadToDeg());
	  else       y += fPixelSize * 2.;
      }
      x += fPixelSize * 2.;
   }
// calculate distance to array center
   for( unsigned int i = 0; i <  PMTx.size(); i++ )
   {
       if( PMTx[i] > -99990. && PMTy[i]  > -99990. ) PMTr.push_back( sqrt(PMTx[i]*PMTx[i]+PMTy[i]*PMTy[i] ) );
       else                                          PMTr.push_back( -99999. );
   }
// now remove all elements which are further away than pmtradius
   int iElements = (int)PMTx.size();
   if( !bPrint ) cout << "remove pmts outside FOV" << endl;
   int nR = 0;
   for( unsigned int i = 0; i < PMTr.size(); i++ )
   {
      if(  PMTr[i] < -99990. || PMTr[i]+fPixelSize > cameraradius )
      {
          PMTx[i] = -99999.;
	  PMTy[i] = -99999.;
	  nR++;
	  iElements--;
      }
      if( fabs(PMTx[i])<1.e-5) PMTx[i] = 0.;
      if( fabs(PMTy[i])<1.e-5) PMTy[i] = 0.;
   } 
   if( !bPrint ) cout << "\t number of PMTs outside FOV and removed: " << nR << endl;
   fNPixel = iElements;
   vector< int > iID;
   int iZ = 0;
   for( unsigned int i = 0; i < PMTx.size(); i++ )
   {
     iID.push_back( -1 );
     if( PMTx[i] != -99999. )
     {
        iID.back() = iZ+1;
	iZ++;
      }
   }
// plot camera
   if( !bPrint ) cout << "number of PMTs: " << iElements << endl;


   iZ = 1;
   char hline_1[600];
   char hline_2[600];
   for( unsigned int i = 0; i < PMTx.size(); i++ )
   {
      if( PMTx[i] != -99999. )
      {
	 sprintf( hline_1, "* PMPIX %d \t %d    1 0.0   %.6f \t %.6f \t %.2f \t 0.81 1 0.35 3.4 30.0 0.000 1  1   0.0   1.0", fTelescopeID, iZ, PMTx[i], PMTy[i], kathoderadius );
	 if( bPrint ) cout << hline_1 << endl;
	 fGrisuline_PMPIX.push_back( hline_1 );
	 sprintf( hline_1, "* PIXLC %d \t %.6f \t %.6f", iZ, PMTx[i], PMTy[i] );
	 fGrisuline_PIXLC.push_back( hline_1 );
	 iZ++;
      }
   }
// neighbour list
   PMTNN.assign( PMTx.size(), 0 );
   for( unsigned int i = 0; i < PMTx.size(); i++ )
   {
      vector< int > iNeigh;
      if( PMTx[i] == -99999. ) continue;
      for( unsigned int j = 0; j < PMTx.size(); j++ )
      {

	 if( i == j ) continue;
// check if next pmt is in pmtradius*1.1
	 if( bHex )
	 {
	    if( sqrt((PMTx[i]-PMTx[j])*(PMTx[i]-PMTx[j])+(PMTy[i]-PMTy[j])*(PMTy[i]-PMTy[j]))<fPixelSize*2.*1.1 ) iNeigh.push_back( iID[j] );
	 }
	 else
	 {
	    if( sqrt((PMTx[i]-PMTx[j])*(PMTx[i]-PMTx[j])+(PMTy[i]-PMTy[j])*(PMTy[i]-PMTy[j]))<fPixelSize*2.*1.1*sqrt(2.) ) iNeigh.push_back( iID[j] );
	 }

      }
      PMTNN[i] = iNeigh.size();
      sprintf( hline_1, " " );
      for( unsigned j = 0; j < iNeigh.size(); j++ ) sprintf( hline_1, "%s \t %d", hline_1, iNeigh[j] );
      sprintf( hline_2, "* NGHBR\t %d \t %d \t %d %s", fTelescopeID, iID[i], (int)iNeigh.size(), hline_1 );
      if( bPrint ) cout << hline_2 << endl;
      fGrisuline_NGHBR.push_back( hline_2 );
      sprintf( hline_2, "%s \t %d %s", fGrisuline_PIXLC[fGrisuline_NGHBR.size()-1].c_str(), (int)iNeigh.size(), hline_1 );
      fGrisuline_PIXLC[fGrisuline_NGHBR.size()-1] = hline_2;
   }

   if( bPlot )
   {
      TCanvas *cX = new TCanvas( "cX", "", 10, 10, 800, 800 );
      cX->Draw();
      gPad->SetGridx( 0 );
      gPad->SetGridy( 0 );
      gPad->SetLeftMargin( 0.12 );
      gPad->SetRightMargin( 0.06 );
      TGraph *iG = new TGraph( iElements );
      vector< TEllipse * > iPMT_circ;
      vector< TBox * > iPMT_box;
      iZ = 0;
      for( unsigned int i = 0; i < PMTx.size(); i++ )
      {
	 if( PMTx[i] != -99999. )
	 {
	   iG->SetPoint( iZ, PMTx[i], PMTy[i] );
	   if( bHex ) 
	   {
	      iPMT_circ.push_back( new TEllipse( PMTx[i], PMTy[i], kathoderadius ) );
	      if( i < PMTNN.size() ) iPMT_circ.back()->SetFillColor( PMTNN[i] );
           }
	   else
	   {
	      iPMT_box.push_back( new TBox( PMTx[i] - kathoderadius*0.95, PMTy[i] - kathoderadius*0.95, PMTx[i] + kathoderadius*0.95, PMTy[i] + kathoderadius*0.95 ) );
	      iPMT_box.back()->SetLineStyle( 1 );
	      if( i < PMTNN.size() ) iPMT_box.back()->SetFillColor( PMTNN[i] );
           }
	   iZ++;
	 }
      }
      iG->SetMarkerStyle( 7 );
      iG->SetTitle( "" );
      iG->Draw( "ap" );
      iG->GetHistogram()->SetXTitle( "x [mm]" );
      iG->GetHistogram()->SetYTitle( "y [mm]" );
      iG->GetHistogram()->GetYaxis()->SetTitleOffset( 1.4 );
      for( unsigned int i = 0; i < iPMT_circ.size(); i++ ) iPMT_circ[i]->Draw();
      for( unsigned int i = 0; i < iPMT_box.size(); i++ ) iPMT_box[i]->Draw();
      iG->Draw( "p" );
// draw camera circumfence
      TEllipse *iE = new TEllipse( 0., 0., cameraradius );
      iE->SetFillStyle( 4000 );
      iE->SetLineWidth( 2 );
      iE->Draw();
   }

// output camera

}

string VGrIsuConfigMaker::getGrisuline_CAMRA()
{
    char hname[600];
    sprintf( hname, "%d  %d", fTelescopeID, fNPixel );

    string iT = "* CAMRA ";
    iT += hname;
    iT += " 0.0 1.0";

    return iT;
}

string VGrIsuConfigMaker::getGrisuline_TLLOC()
{
   char hname[600];

   sprintf( hname, "%d %.3f %.3f %.3f 0.0 %.3f 0.0 0.0", fTelescopeID, fTelescopeX, fTelescopeY, fTelescopeZ, fTelescopeRot );
   string iT = "* TLLOC ";
   iT += hname;

   return iT;
}

bool VGrIsuConfigMaker::readQuantumEfficiency( string ifile )
{
   if( ifile.size() == 0 ) return false;

   fPixelQueffWavelength.clear();
   fPixelQueffValue.clear();
   fGrisuline_QUEFF.clear();

   ifstream is( ifile.c_str() );
   if( !is )
   {
       cout << "error reading quantum efficiency from " << ifile << endl;
       return false;
   }

   double ix, iy;
   while( !is.eof() )
   {
      is >> ix;
      if( is.eof() ) break;
      is >> iy;
      fPixelQueffWavelength.push_back( ix );
      fPixelQueffValue.push_back( iy );
   }
   is.close();

   char hline[1000];
   sprintf( hline, "* QUEFF %d %d", fTelescopeID, (int)fPixelQueffWavelength.size() );
   fGrisuline_QUEFF.push_back( hline );
   for( unsigned int i = 0; i < fPixelQueffWavelength.size(); i++ )
   {
      sprintf( hline, "%f   %f", fPixelQueffWavelength[i], fPixelQueffValue[i] );
      fGrisuline_QUEFF.push_back( hline );
   }
   return true;
}

bool VGrIsuConfigMaker::readMirrorReflectivitiy( string ifile )
{
   if( ifile.size() == 0 ) return false;

   fMirrorReflectivityWavelength.clear();
   fMirrorReflectivityValue.clear();
   fGrisuline_RFCRV.clear();

   ifstream is( ifile.c_str() );
   if( !is )
   {
       cout << "error reading mirror reflectivity from " << ifile << endl;
       return false;
   }
   cout << "reading mirror reflectivity from " << ifile << endl;

   double ix, iy;
   while( !is.eof() )
   {
      is >> ix;
      if( is.eof() ) break;
      is >> iy;
      fMirrorReflectivityWavelength.push_back( ix );
      fMirrorReflectivityValue.push_back( iy );
   }
   is.close();

   char hline[1000];
   sprintf( hline, "* RFCRV %d %d", fTelescopeID, (int)fMirrorReflectivityWavelength.size() );
   fGrisuline_RFCRV.push_back( hline );
   for( unsigned int i = 0; i < fMirrorReflectivityWavelength.size(); i++ )
   {
      sprintf( hline, "%f   %f", fMirrorReflectivityWavelength[i], fMirrorReflectivityValue[i] );
      fGrisuline_RFCRV.push_back( hline );
   }
   return true;
}

bool VGrIsuConfigMaker::readMirrorPositions( string ifile, int iTel )
{
   if( ifile.size() == 0 ) return false;

   fFacetPositionX.clear();
   fFacetPositionY.clear();
   fFacetCurvature.clear();
   fFacetSize.clear();

   ifstream is( ifile.c_str() );
   if( !is )
   {
      cout << "error reading mirror position file " << ifile << endl;
      return false;
   }

   string iline;
   string itemp;

   cout << "reading mirror positions for telescope " << iTel << " from " << ifile << endl;

   while( getline( is, iline ) )
   {
      if( iline.substr( 0, 1 ) != "*" ) continue;

      if( iline.find( "MIREL" ) >= iline.size() ) continue;

      std::istringstream i_stream( iline );

      i_stream >> itemp;
      i_stream >> itemp;
      i_stream >> itemp;

      if( atoi( itemp.c_str() ) != iTel ) continue;

      i_stream >> itemp;
      i_stream >> itemp;
      i_stream >> itemp;
      i_stream >> itemp;
      fFacetSize.push_back( atof( itemp.c_str() ) );
      i_stream >> itemp;
      fFacetCurvature.push_back( atof( itemp.c_str() ) );

      i_stream >> itemp;
      fFacetPositionX.push_back( atof( itemp.c_str() ) );
      i_stream >> itemp;
      fFacetPositionY.push_back( atof( itemp.c_str() ) );
  }
  is.close();

  cout << "total number of mirrors: " << fFacetPositionX.size() << endl;
  fNFacets = (int)fFacetPositionX.size();

  return true;
}

TCanvas* VGrIsuConfigMaker::plotFacetPosition( TCanvas *c, int iColor )
{
   char hname[600];
   char hline[600];
   bool bPlot = true;
   if( !c )
   {
      sprintf( hname, "cMirrorPositions_%d", fTelescopeID );
      sprintf( hline, "mirror positions (telescope %d)", fTelescopeID );
      c = new TCanvas( hname, hline, 500, 10, 400, 400 );
      c->Draw();
   }
   else
   {
      c->cd();
      bPlot = false;
   }

   TGraph *g = new TGraph( 1 );
   g->SetTitle( "" );
   g->SetMarkerStyle( 1 );
   g->SetMarkerColor( iColor );

   for( unsigned int i = 0; i < fFacetPositionX.size(); i++ )
   {
      g->SetPoint( i, fFacetPositionX[i], fFacetPositionY[i] );
   }

   if( !bPlot ) g->Draw( "p" );
   else
   {
      g->Draw( "ap" ); 
      g->GetHistogram()->SetXTitle( "mirror x [m]" );
      g->GetHistogram()->SetYTitle( "mirror y [m]" );
   }

   for( unsigned int i = 0; i < fFacetPositionX.size(); i++ )
   {
      TEllipse *e = new TEllipse( fFacetPositionX[i], fFacetPositionY[i], fFacetSize[i], fFacetSize[i] );
      e->SetLineColor( iColor );
      e->SetFillStyle( 4001 );
      e->Draw();
   }

   return c;
}

void VGrIsuConfigMaker::makeMirror()
{
  double distance = 2.*( fFacetDiameter + fFacetSpacing );
  fFocalLength = fMirrorDiameter * fMirror_f_over_D;

  double dy = sqrt((distance*distance)-(distance*distance/4.));

  int nr = (int) (1.2*fMirrorDiameter/dy/2.+2);

  double x,y;
  double r;

  fFacetPositionX.clear();
  fFacetPositionY.clear();
  fFacetSize.clear();
  fFacetCurvature.clear();

  double curve = 0.;

  for( int i =-nr; i < nr; i++ )
  {
     for( int j = -nr;j < nr; j++ )
     {
	 x = (double)i*distance+(abs(j)%2==1)*distance/2.;
	 y = (double)j*dy;
	 r = sqrt(x*x+y*y);
	 if( r < fMirrorDiameter/2. ) 
	 {
	    curve = 2.*fFocalLength; // Davies-Cotton

	    fFacetPositionX.push_back( x );
	    fFacetPositionY.push_back( y );
	    fFacetSize.push_back( fFacetDiameter );
	    fFacetCurvature.push_back( curve );

         }
      }
  }
  fNFacets = (int)fFacetPositionX.size();

  double area = fNFacets*TMath::Pi()*fFacetDiameter*fFacetDiameter*1.1026578;
  printf("Mirrors         : %d\n",fNFacets);
  printf("Area            : %f\n",area);
  printf("Mirror Aperture : %f\n",TMath::Pi()*fMirrorDiameter*fMirrorDiameter/4. );
  printf("Ratio           : %f\n",area/(TMath::Pi()*fMirrorDiameter*fMirrorDiameter/4.) );
}

vector< string > VGrIsuConfigMaker::getGrisuline_MIREL()
{

  fGrisuline_MIREL.clear();
  char hline[600];

  for( int i = 0; i < fNFacets; i++ )
  {
      sprintf( hline, "* MIREL %d %d  %d  %4.1f %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f 1.00 1", fTelescopeID, i+1, fFacetShape, fFacetRotationAngle, fFacetSize[i], fFacetCurvature[i], fFacetPositionX[i], fFacetPositionY[i], fFacetMisalignement, fFacetBlureRadius );
      fGrisuline_MIREL.push_back( hline );
  }

  return fGrisuline_MIREL;
}

string VGrIsuConfigMaker::getGrisuline_MIROR()
{
   char hline[600];

   sprintf( hline, "* MIROR %d %5.1f %5.1f %5.1f %3d %4d", fTelescopeID, fMirrorDiameter/2.*1.1, fFocalLength, 0., 1, fNFacets);

   string iT = hline;

   return iT;
}

/*!
    angular size is diamter, pixel size is radius in mm
*/
void VGrIsuConfigMaker::setPixelSize_deg( double iPixelAngularSize )
{
   iPixelAngularSize /= 2.;

   fPixelSize = tan( iPixelAngularSize / TMath::RadToDeg() ) * fMirrorDiameter * fMirror_f_over_D;

// m to mm
   fPixelSize *= 1.e3;
}


string VGrIsuConfigMaker::getGrisuline_PIXGB( bool bHex )
{
    char hline_1[600];
    double kathoderadius = fPixelSize * fPixelGeometricalEfficiency;
    if( bHex ) sprintf( hline_1, "* PIXGB 1 0.0 %.2f \t 0.81 1 0.35 3.4 30.0 0.000 1  1  0.0   1.0", kathoderadius );
    else       sprintf( hline_1, "* PIXGB 3 0.0 %.2f \t 0.89 1 0.35 3.4 30.0 0.000 1  1  0.0   1.0", kathoderadius );
    string a = hline_1;

    return a;
}

/*! 
     grisu_write_Camera_with_new_pixel_size

     write new PMPIX lines for a camera with a different pixel size (but same number of pixel)

     use an existing .cfg files to read pixel positions (assume PMT distance is 15.498533 mm)

     iFile    :    input .cfg file
     iPMTD    :    new PMT distance in [mm], e.g. 31.4 mm

*/
/*void grisu_write_Camera_with_new_pixel_size( string iFile, double iPMTD )
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
} */
