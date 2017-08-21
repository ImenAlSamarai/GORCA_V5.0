/*! \file writeGRISU_pe_to_root.cpp
    \short read pe format from grisudet and write into root trees

    Revision $Id: writeGRISU_pe_to_root.cpp,v 1.1.1.1 2011/07/21 20:35:57 gmaier Exp $

    \author
    Gernot Maier
*/

#include "VPEWriter.h"

#include "TFile.h"
#include "TROOT.h"

#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

#ifdef __MAKECINT__
#pragma link C++ class vector<float>+;
#endif 


void help()
{
     cout << "writeGRISU_pe_to_root <ntel> <outputfile.root> <pix> " << endl;
     cout << endl;
     cout << "command line options: " << endl;
     cout << "\t <ntel> \t maximum number of telescope " << endl;
     cout << "\t <outputfile.root> \t ROOT output file" << endl;
     cout << "\t <pix> \t 0=x/y postion of photon [mm], 1=pixel number" << endl;
     cout << endl;
     exit( 0 );
}

void fillEvent( VPEWriter* f, unsigned int iEventNumber, unsigned int iPrimaryType, float iPrimaryEnergy, float iXcore, float iYcore, float iXcos, float iYcos, float iXsource, float iYsource, float iDelay )
{
  if( f ) f->add_event( iEventNumber, iPrimaryType, iPrimaryEnergy, iXcore, iYcore, iXcos, iYcos, iXsource, iYsource, iDelay );
}

int main( int argc, char **argv )
{
    if( argc != 4 ) help();

    gROOT->ProcessLine("#include <vector>");

    unsigned int fNTel = (unsigned int)(atoi( argv[1] ) );
    string fOutputFileName = argv[2];
    unsigned int fPixel = (unsigned int)atoi( argv[3] );

    TFile *fO = new TFile( fOutputFileName.c_str(), "RECREATE" );
    if( fO->IsZombie() )
    {
        cout << "error opening root output file: " << fOutputFileName << endl;
	cout << "...exiting" << endl;
	exit( -1 );
    }

    vector< VPEWriter* > fpe_data;
    for( unsigned int t = 0; t < fNTel; t++ ) fpe_data.push_back( new VPEWriter( t, fPixel ) );

    string iLine;
    string iT;
    unsigned int iEventNumber = 0;  
    unsigned int iPrimaryType = 0;
    float iPrimaryEnergy = 0.;
    float iXcore = 0.; 
    float iYcore = 0.;
    float iXcos = 0.;
    float iYcos = 0.;
    float iXsource = 0.;
    float iYsource = 0.;
    std::vector< float > iDelay(fNTel);

    unsigned int iPE_telID = 0;
    unsigned int iPE_pixelID = 0;
    float iPE_photonX = 0.;
    float iPE_photonY = 0.;
    float iPE_time = 0.;
    float iPE_wl = 0.;;

    unsigned int z = 0;
    int count = 0;
    int iPhCor = 0;

    while( cin )
    {
       getline( cin, iLine );
       if( iLine.substr( 0, 1 ) == "S" )
       {
	  if( z > 0 )
	  {
	      for( unsigned int i = 0; i < fpe_data.size(); i++ )
	      {
		  fillEvent( fpe_data[i], iEventNumber, iPrimaryType, iPrimaryEnergy, iXcore, iYcore, iXcos, iYcos, iXsource, iYsource, iDelay[i] );
              }
          } 

          istringstream is_stream( iLine );
	  is_stream >> iT;
	  is_stream >> iT;
	  iEventNumber = atoi( iT.c_str() );
	  is_stream >> iT;
	  iPrimaryType = atoi( iT.c_str() );
	  is_stream >> iPrimaryEnergy;
	  is_stream >> iXcore;
	  is_stream >> iYcore;
	  is_stream >> iXcos;
	  is_stream >> iYcos;
	  is_stream >> iXsource;
	  is_stream >> iYsource;

	  z++;
       }
       else if( iLine.substr(0,1) == "D")
       {
	  count++;
	  iPhCor=1;
	 
          istringstream is_stream( iLine );
	  is_stream >> iT;
	  for( unsigned int tel=0; tel<fNTel; tel++)
	    {
	      is_stream >> iDelay[tel];
	    }

       }
       else if( iLine.substr( 0, 1 ) == "P" )
       {
          istringstream is_stream( iLine );
	  is_stream >> iT;
	  is_stream >> iPE_telID;
// grisu counting of telescopes and pixels starts at 1, we at 0:
	  iPE_telID -= 1;
// add this pe
          if( iPE_telID < 0 || iPE_telID >= fpe_data.size() )
	  {
	      cout << "Error: telescope ID out of range: " << iPE_telID << "\t" << fpe_data.size() << endl;
	      continue;
          }
	  if( fpe_data[iPE_telID]->usePixelNumbers() )
	  {
	     is_stream >> iPE_pixelID;
	     iPE_pixelID -= 1;
          }
	  else
	  {
	     is_stream >> iPE_photonX;
	     is_stream >> iPE_photonY;
	  }
          is_stream >> iPE_wl;
	  is_stream >> iPE_time;

	  fpe_data[iPE_telID]->add_pe( iPE_pixelID, iPE_photonX, iPE_photonY, iPE_time, iPE_wl );
       }
    }
// fill last event
    for( unsigned int i = 0; i < fpe_data.size(); i++ )
    {
        fillEvent( fpe_data[i], iEventNumber, iPrimaryType, iPrimaryEnergy, iXcore, iYcore, iXcos, iYcos, iXsource, iYsource, iDelay[i] );
    }
    cout << endl << count << " " << iPhCor << endl;


    for( unsigned int t = 0; t < fNTel; t++ )
    {
        if( fpe_data[t] && fpe_data[t]->getDataTree() ) fpe_data[t]->getDataTree()->Write();
    }

    fO->Close();

}
