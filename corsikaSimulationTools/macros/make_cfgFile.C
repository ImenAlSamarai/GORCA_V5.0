/*!


#################################################################

    make CORSIKA array:

       makeCorsikaArray(36, true );

#################################################################

*/

#include "TF1.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TSystem.h"

#include "VGrIsuConfigMaker.cpp"
#include "VTelescopePositions.cpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void make_PixelFile( double iF = 3., double iPixelSize_deg = 0.15, double iPixelSize_mm = -1., bool bHex = true, bool bPlot = false )
{
     VGrIsuConfigMaker *fCamera = new VGrIsuConfigMaker();
     fCamera->setF_over_D( iF );
     fCamera->setFieldofView(8.);
     if( iPixelSize_deg > 0. ) fCamera->setPixelSize_deg( iPixelSize_deg  );
     else if( iPixelSize_mm > 0. ) fCamera->setPixelSize( iPixelSize_mm );
     fCamera->setPixelGeometricalEfficiency( 1. );
     fCamera->makeCamera( false, bPlot, bHex );

     cout << fCamera->getGrisuline_CAMRA() << endl;
     cout << fCamera->getGrisuline_PIXGB( bHex ) << endl;

     for( unsigned int t = 0; t < fCamera->getGrisuline_PMPIX().size(); t++ )
     {
	 cout << fCamera->getGrisuline_PIXLC()[t] << endl;
     }
     cout << endl;
}


void make_cfgFile( int iNtel = 4, double iDistance = 50., double iF = 3., double iPxelSize_deg = 0.15,  string fFile_QUEFF = "queff.dat", string fFile_REFL = "reflectivity.dat" )
{
    cout << "* VERSN 4.1.5" << endl;

    vector< VGrIsuConfigMaker* > fTelescope;

//////////////////////////////////////////////
// telescope positions
//////////////////////////////////////////////
    VTelescopePositions fTelPos;
    string iTelPosName = "50 m array";
    fTelPos.addSquareConfiguration( iTelPosName, iNtel, iDistance, 0., 14. );
    vector< string > fGrisu_TELPOS = fTelPos.getGrisuline_TLLOC( 0 );
    unsigned int ntel = fGrisu_TELPOS.size();

//////////////////////////////////////////////
// telescope configuration
//////////////////////////////////////////////
    for( unsigned int i = 0; i < ntel; i++ )
    {
       fTelescope.push_back( new VGrIsuConfigMaker( i+1 ) );
       fTelescope.back()->setF_over_D( iF );
       fTelescope.back()->setFieldofView(8.); 
       fTelescope.back()->setPixelSize_deg( iPxelSize_deg  );
       fTelescope.back()->setPixelGeometricalEfficiency( 1. );
       fTelescope.back()->setFacetMisalignement( 0.01 );
       fTelescope.back()->setFacetBlureRadius( 0.01 );
       fTelescope.back()->makeCamera( false, false );
       fTelescope.back()->makeMirror();
    }
// read mirror reflectivity and quantum efficiency for first telescope only
    if( fTelescope.size() > 0 )
    {
       fTelescope[0]->readMirrorReflectivitiy( fFile_REFL );
       fTelescope[0]->readQuantumEfficiency( fFile_QUEFF );
    }
        
    cout << "* NBRTL " << ntel << endl << endl;

// telescope locations
    for( unsigned int i = 0; i < ntel; i++ )
    {
       cout << fGrisu_TELPOS[i] << endl;
    }
    cout << endl;

    cout << "* SIMUL 2.0 500 1" << endl << endl;

    cout << "* FADCS 1 7.84 20.0 65535 24 7 1 6.0 250" << endl << endl;

// set VERITAS CFD to 40 mV, scale by mirror diameter
    cout << "* TRIGG " << 40. * iPxelSize_deg / 0.15 << " 10.0 5.0 1 1 100.0 1" << endl << endl;

    cout << "* ANLOG 2.0E5 2722 0.0" << endl << endl;

// number of mirrors
    for( unsigned int i = 0; i < fTelescope.size(); i++ )
    {
         cout << fTelescope[i]->getGrisuline_MIROR() << endl;
    }
// mirror reflectivity
    cout << endl;
    if( fTelescope.size() > 0 )
    {
        for( unsigned int t = 0; t < fTelescope[0]->getGrisuline_RFCRV().size(); t++ )
	{
	    cout << fTelescope[0]->getGrisuline_RFCRV()[t] << endl;
        }
    }
    cout << endl;


// mirror positions
    for( unsigned int i = 0; i < fTelescope.size(); i++ )
    {
       for( unsigned int t = 0; t < fTelescope[i]->getGrisuline_MIREL().size(); t++ )
       {
           cout << fTelescope[i]->getGrisuline_MIREL()[t] << endl;
       }
   }
   cout << endl << endl;

// quantum efficiency (assume same for all telescopes here)
    if( fTelescope.size() > 0 )
    {
        for( unsigned int t = 0; t < fTelescope[0]->getGrisuline_QUEFF().size(); t++ )
	{
	    cout << fTelescope[0]->getGrisuline_QUEFF()[t] << endl;
        }
    }
    cout << endl;

// cameras
    for( unsigned int i = 0; i < fTelescope.size(); i++ )
    {
       cout << fTelescope[i]->getGrisuline_CAMRA() << endl;    
    }
    cout << endl;

// pixel positions and neighbour lists
   for( unsigned int i = 0; i < fTelescope.size(); i++ )
   {
       for( unsigned int t = 0; t < fTelescope[i]->getGrisuline_PMPIX().size(); t++ )
       {
           cout << fTelescope[i]->getGrisuline_PMPIX()[t] << endl;
       }
       cout << endl;
       for( unsigned int t = 0; t < fTelescope[i]->getGrisuline_NGHBR().size(); t++ )
       {
           cout << fTelescope[i]->getGrisuline_NGHBR()[t] << endl;
       }
       cout << endl;
    }
    cout << endl;
    cout << endl;
      
}


/*
   input:   radius of pixel
   output:  angular diameter of pixel
*/
void plot_pixelAngularSize_vs_FocalLength( double iP = 15.498533 )
{
   TF1 *fpas = new TF1( "fpas", "atan( [0] / x / 1000. ) * TMath::RadToDeg() * 2.", 10., 40. );
   fpas->SetParameter( 0, iP );

   TCanvas *cPAS = new TCanvas( "cPAS", "angular pixel size vs focal length", 10, 10, 400, 400 );
   cPAS->Draw();

   fpas->Draw();
}

void make_EVNDISPcfgfiles()
{
    vector< string > fcfg;
    fcfg.push_back( "AGIS_singleTelescope_005deg_F30_" );
    fcfg.push_back( "AGIS_singleTelescope_010deg_F30_" );
    fcfg.push_back( "AGIS_singleTelescope_020deg_F30_" );
    fcfg.push_back( "AGIS_singleTelescope_015deg_F10_" ); 

    vector< string > fdist;
    fdist.push_back( "050m" );
    fdist.push_back( "075m" );
    fdist.push_back( "100m" );
    fdist.push_back( "125m" );
    fdist.push_back( "150m" );
    fdist.push_back( "175m" );
    fdist.push_back( "200m" );
    fdist.push_back( "VERITAS" ); 

    vector< int > flength;
    flength.push_back( 46404 );
    flength.push_back( 11760 );
    flength.push_back( 3204 );
    flength.push_back( 5400 );

    char hname[600];
    for( unsigned int i = 0; i < fcfg.size(); i++ )
    {
        string iIn = fcfg[i] + "Tel0.cfg";

	for( unsigned int j = 0; j < fdist.size(); j++ )
	{
	   string iOf = fcfg[i] + fdist[j] + "_EVNDISP.cfg";
	   cout << iIn << "\t" << iOf << endl;

	   sprintf( hname, "head -n 1 %s > %s", iIn.c_str(), iOf.c_str() );
	   gSystem->Exec( hname );
	   sprintf( hname, "echo \"* CFGTYPE 1\" >> %s", iOf.c_str() );
	   gSystem->Exec( hname );
	   sprintf( hname, "tail -n %d %s | head -n 11 >> %s", flength[i]-1, iIn.c_str(), iOf.c_str() );
	   gSystem->Exec( hname );
//	   if( fdist[j] != "VERITAS" ) sprintf( hname, "echo \"* NBRTL 36\" >> %s", iOf.c_str() );
	   if( fdist[j] != "VERITAS" ) sprintf( hname, "echo \"* NBRTL 49\" >> %s", iOf.c_str() );
	   else                        sprintf( hname, "echo \"* NBRTL 4\" >> %s", iOf.c_str() );                 
	   gSystem->Exec( hname );
	   sprintf( hname, "cat AGIS_telescope_positions_EVNDISP_%d.dat >> %s", j, iOf.c_str() );
	   gSystem->Exec( hname );
	   sprintf( hname, "tail -n %d %s >> %s", flength[i]-15, iIn.c_str(), iOf.c_str() );
	   gSystem->Exec( hname );
        }
    }

}

void makeCorsikaArray( int iNTel = 36, bool bPrint = false )
{
    double iDiameter = 14.;
    VTelescopePositions fTelPos;
    fTelPos.addSquareConfiguration("50 m", iNTel, 50., 0., iDiameter, 1, 20 );
    fTelPos.addSquareConfiguration("75 m", iNTel, 75., 0., iDiameter, 2, 20 );
    fTelPos.addSquareConfiguration("100 m", iNTel, 100., 0., iDiameter, 3, 20 );
    fTelPos.addSquareConfiguration("125 m", iNTel, 125., 0., iDiameter, 4, 20 );
    fTelPos.addSquareConfiguration("150 m", iNTel, 150., 0., iDiameter, 6, 20 );
    fTelPos.addSquareConfiguration("175 m", iNTel, 175., 0., iDiameter, 7, 20 );
    fTelPos.addSquareConfiguration("200 m", iNTel, 200., 0., iDiameter, 8, 21 );  
    fTelPos.addSquareConfiguration("VERITAS", 0, iNTel, 0., iDiameter, 5, 21. ); 

    fTelPos.plot("");

       fTelPos.makeTELESCOPE_Lines( "", true, bPrint );
       fTelPos.makeTELESCOPE_Lines( "", false, bPrint );

    if( bPrint )
    {
       fTelPos.writeEVNDISP_positions( "", "AGIS_telescope_positions_EVNDISP" );
    }

    fTelPos.checkCORSIKA_positions();
    fTelPos.checkGRISU_positions();
}    

void readCorsikaInputFile( string iFile )
{
    ifstream inFileStream( iFile.c_str() );
    if( !inFileStream )
    {
       cout << "error: input file not found" << endl;
       return;
    }
    string iline;
    string itemp;
    double x = 0.;
    double y = 0.;

    int z = 1;

    while( getline( inFileStream, iline ) )
    {
        if( iline.find( "TELESCOPE" ) >= iline.size() ) continue;

	istringstream i_stream( iline );

	i_stream >> itemp;

	cout << "* TLLOC  " << z << "\t";

	i_stream >> itemp;
	x = atof( itemp.c_str() );
	i_stream >> itemp;
	y = atof( itemp.c_str() );

	cout << -1.*y/1.e2 << "\t" << x/1.e2;
	cout << "   0.0 36.0 0.0 0.0" << endl;

	z++;
    }
    inFileStream.close();
}

void makeDistanceFiles( string iPixel = "020", string iDir = "/raid/pevray/maierg/EVNDISP-340/data/detector_geometry/AGIS/telescopePositions/" )
{
    int idist = 50;
    char hname[200];
    char htel[200];
    char hcomm[800];
    

    for( unsigned int i = 0; i < 8; i++ )
    {
       sprintf( hname, "AGIS_36tel_SQ_%sdeg_%04dm_N36_EVNDISP.cfg", iPixel.c_str(), idist );
       sprintf( htel, "AGIS_telescope_positions_EVNDISP_%d.dat", i );
       cout << hname << "\t" << htel << endl;

       sprintf( hcomm, "cat %s%s > %s%s", iDir.c_str(), "head.cfg", iDir.c_str(), hname );
       gSystem->Exec( hcomm );

       sprintf( hcomm, "cat %s%s >> %s%s", iDir.c_str(), htel, iDir.c_str(), hname );
       gSystem->Exec( hcomm );

       sprintf( hcomm, "cat %s/tail_%sdeg.cfg  >> %s%s", iDir.c_str(), iPixel.c_str(), iDir.c_str(), hname );
       gSystem->Exec( hcomm );

       idist += 25;
    }


}
