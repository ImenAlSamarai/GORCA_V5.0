/*! \file  getRunHeaderFromTelescopeFile
    \brief  get run header from CORSIKA telescope file

    see README file for documentation

    Revision $Id: getRunHeaderFromTelescopeFile.cpp,v 1.1.1.1 2011/07/21 20:35:57 gmaier Exp $

     \author
        Gernot Maier (McGill University)
        based on Konrad Bernloehrs sim_skeleton.c program (CORSIKA IACT package)
*/

#include "initial.h"      
#include "io_basic.h"    
#include "fileopen.h"
#include "mc_tel.h"
#include "atmo.h"

#include <bitset>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

const unsigned int MAX_TEL = 2000;

struct linked_string corsika_inputs;

int main(int argc, char **argv)
{
   IO_BUFFER *iobuf;
//   IO_ITEM_HEADER item_header, sub_item_header;
   IO_ITEM_HEADER block_header;
   real runh[273];
//   real rune[273];
   real evth[273];
//   real evte[273];

   FILE *data_file;

   int readNevent = 0;               // event counter
   int nevents = 1;                 // number of events to be read
   bitset<32> b1;

   cout << endl;
   cout << "getRunHeaderFromTelescopeFile (v 0.1)" << endl;
   cout << "=====================================" << endl;
   cout << endl;

   if( argc < 2 || argc > 3 )
   {
      cout << "getRunHeaderFromTelescopeFile <CORSIKA telescope file> [number of events to be printed]" << endl;
      cout << endl;
      exit( 0 );
   }
   string fInputFile = argv[1];
   if( argc == 3 ) nevents = atoi( argv[2] );

   /* I/O buffer for input needed */
   if ( (iobuf = allocate_io_buffer(0)) == NULL )
   {
      fprintf(stderr,"Input I/O buffer not allocated\n");
      exit(1);
   }
   iobuf->max_length = 20000000;

// try to open Corsika file 
   data_file = 0;
   if ( (data_file = fileopen(fInputFile.c_str(),"r")) == NULL )
   {
      perror(fInputFile.c_str());
      exit(1);
   }
   iobuf->input_file = data_file;

   cout << "Input file: " << fInputFile << endl << endl;;
   
   int run = 0;

   for(;;) /* Loop over all data in the input file */
   {
      /* Find and read the next block of data. */
      /* In case of problems with the data, just give up. */
      if ( find_io_block(iobuf,&block_header) != 0 )
         break;
      if ( read_io_block(iobuf,&block_header) != 0 )
         break;

      switch ( block_header.type )
      {
///////////////////////////////////////////////////////////////////
// CORSIKA RUN HEADER 
///////////////////////////////////////////////////////////////////
         case IO_TYPE_MC_RUNH:

            read_tel_block(iobuf,IO_TYPE_MC_RUNH,runh,273);

	    run = (int)runh[1];
	    cout << "RUN " << (int) runh[1] << "\t simulated on " << (int)runh[2] << " with CORSIKA v." << runh[3] << endl;
	    cout << endl;
	    cout << "RUN " <<  run;
	    cout << "\t observation level(s) [m]: ";
	    for( int i = 0; i < (int)runh[4]; i++ ) cout << "\t" << runh[5]*0.01 << "\t";
	    cout << endl;
	    cout << "RUN " <<  run;
	    cout << "\t energy spectrum: gamma = " << runh[15] << " energy range: [" << runh[16]*0.001 << "," << runh[17]*0.001 << "] TeV" << endl;
	    cout << "RUN " <<  run;
	    cout << "\t particle energy cutoffs: hadrons: " << runh[20] << " GeV, muons: " << runh[21] << " GeV, electrons: " << runh[22] << " GeV" << endl;
	    cout << endl;
            break;

/////////////////////////////////////////////////////
// Telescope positions (relative positions in array) */
/////////////////////////////////////////////////////
         case IO_TYPE_MC_TELPOS:

	    int ntel;
	    double xtel[MAX_TEL];
	    double ytel[MAX_TEL];
	    double ztel[MAX_TEL];
	    double rtel[MAX_TEL];
            read_tel_pos(iobuf,MAX_TEL, &ntel, xtel, ytel, ztel, rtel);
	    cout << "RUN " <<  run;
	    cout << "\t telescope positions (" << ntel << " telescopes) [m]:"  << endl;
	    for( int i = 0; i < ntel; i++ )
	    {
	        cout << "RUN " <<  run;
	        cout << "\t\t x: " << xtel[i]*0.01 << ", y: " << ytel[i]*0.01 << ", z: " << ztel[i]*0.01 << ", r: " << rtel[i]*0.01 << endl;
            }
	    cout << endl;
            break; 

/////////////////////////////////////////////////////
// CORSIKA EVENT HEADER
/////////////////////////////////////////////////////
         case IO_TYPE_MC_EVTH:

            read_tel_block(iobuf,IO_TYPE_MC_EVTH,evth,273);

	    readNevent++;

	    cout << "RUN " <<  run;
	    cout << "\t Event: " << (int)evth[1] << endl;
	    cout << "RUN " <<  run;
	    cout << "\t PARTICLE ID: " << (int)evth[2] << endl;
	    cout << "RUN " <<  run;
	    cout << "\t Energy [TeV]: " << evth[3]*0.01 << endl;
	    cout << "RUN " <<  run;
	    cout << "\t Starting altitude [g/cm2]: " << evth[4] << endl;
	    cout << "RUN " <<  run;
	    cout << "\t Fixed Targed ID: " << (int)evth[5] << endl;
	    cout << "RUN " <<  run;
	    cout << "\t Height of first interaction [m]: " << evth[6]*0.01 << endl;
	    cout << "RUN " <<  run;
	    cout << "\t Zenith angle [deg]: " << 90. - (180./M_PI)*evth[10] << endl;

            float i_az;
	    i_az = (float)(180./M_PI)*(evth[11]-evth[92]);
            i_az -= floor(i_az/360.) * 360.;
	    cout << "RUN " <<  run;
	    cout << "\t Azimuth angle [deg]: " << i_az <<  endl; 
	    cout << "RUN " <<  run;
	    cout << "\t magnetic field [muT], x-comp: " << evth[70] << ", z-comp: " << evth[71] << endl;
	    cout << "RUN " <<  run;
	    if( (int)evth[79] == 2 ) cout << "\t curved atmosphere" << endl;
	    else  cout << "\t standard atmosphere" << endl;
// get interaction model
            int i_model;
	    i_model = (int)evth[74];
	    cout << "RUN " <<  run;
	    if( i_model == 1 ) cout << "\t Low energy model: GHEISHA" << endl;
	    else if( i_model == 2 ) cout << "\t Low energy model: UrQMD" << endl;
	    else if( i_model == 3 ) cout << "\t Low energy model: FLUKA" << endl;
	    i_model = (int)evth[75];
	    cout << "RUN " <<  run;
	    if( i_model == 0 ) cout << "\t High energy model: HDPM" << endl;
	    else if( i_model == 1 ) cout << "\t High energy model: VENUS" << endl;
	    else if( i_model == 2 ) cout << "\t High energy model: Sibyll" << endl;
	    else if( i_model == 3 ) cout << "\t High energy model: QGSJet" << endl;
	    else if( i_model == 4 ) cout << "\t High energy model: DPMJet" << endl;
	    else if( i_model == 5 ) cout << "\t High energy model: Nexus" << endl;
	    cout << "RUN " <<  run;
	    cout << "\t Interaction flags: ";
	    if( (int)evth[138] > 0 )
	    {
	        cout << " Sibyll ";
		if( (int)evth[138] == 1 ) cout << " v 1.6" << endl;
		else if( (int)evth[138] == 2 ) cout << " v 2.1" << endl;
            }
	    if( (int)evth[141] > 0 )
	    {
	        cout << " QGSJet";
		if( (int)evth[141] == 1 ) cout << " OLD" << endl;
		else if( (int)evth[141] == 2 ) cout << "01c" << endl;
		else if( (int)evth[141] == 3 ) cout << "-II" << endl;
            }
	    cout << "RUN " <<  run;
	    cout << "\t Transition energy [GeV]: " << evth[154] << endl;

// cherenkov flag
	    cout << "RUN " <<  run;
	    cout << "\t CHERENKOV FLAG " << (int)evth[76] << "(";
	    b1 = (int)evth[76];
	    if( b1.test( 0 ) ) cout << " CHERENKOV ";
	    if( b1.test( 1 ) ) cout << " IACT "; 
	    if( b1.test( 2 ) ) cout << " CEFFIC "; 
	    if( b1.test( 3 ) ) cout << " ATMEXT ";
	    if( b1.test( 4 ) ) cout << " ATMEXT (with refraction) ";
	    if( b1.test( 5 ) ) cout << " VOLUMEDET ";
	    if( b1.test( 6 ) ) cout << " CURVED ";
	    if( b1.test( 7 ) ) cout << " SLANT "; 
	    cout << ")" << endl;

	    cout << endl;
	    cout << "RUN " <<  run;
	    cout << "\t Cherenkov bunchsize: " << evth[84] << endl;
	    cout << "RUN " <<  run;
	    cout << "\t Wavelength interval [nm]: [" << evth[95] << "," << evth[96] << "]" << endl;
	    cout << "RUN " <<  run;
	    cout << "\t Number of Cherenkov detectors (x,y): (" << evth[85] << "," << evth[86] << ")" << endl;
	    cout << "RUN " <<  run;
	    cout << "\t Cherenkov detector grid spacing (x,y) [m]: (" << evth[87]*0.01 << "," << evth[88]*0.01 << ")" << endl;
	    cout << "RUN " <<  run;
	    cout << "\t Cherenkov detector length (x,y) [m]: (" << evth[89]*0.01 << "," << evth[90]*0.01 << ")" << endl;

	    cout << "RUN " <<  run;
	    cout << "\t Zenith angle interval [deg]: [" << evth[80] << "," << evth[81] << "]" << endl;
	    cout << "RUN " <<  run;
	    cout << "\t Azimuth angle interval [deg]: [" << 180. - (evth[83]-evth[92]) << "," << 180. - (evth[82]-evth[92]) << "]" << endl;
	    cout << endl;
	    cout << "RUN " <<  run;
	    cout << "\t Viewcone angle [deg]: [" << evth[152] << "," << evth[153] << "]" << endl;
	    cout << "RUN " <<  run;
	    cout << "\t Shower cores for Cherenkov event: " << evth[97] << endl;
	    for( int i = 0; i < (int)evth[97]; i++ ) cout << "RUN " <<  run << "\t\t x = " << evth[97+i]*0.01 << " [m], y = " << evth[117+i]*0.01 << " [m]" << endl;
	    cout << endl;

	    cout << "RUN " << run;
	    cout << "\t Random number sequence (" << (int)evth[12] << " sequences):" << endl;
	    for( int i = 0; i < (int)evth[12]; i++ )
	    {
	       cout << "RUN " << run;
	       cout << "\t\t " << evth[10+3*(i+1)] << "\t" << evth[11+3*(i+1)] << "\t" << evth[12+3*(i+1)] << endl;
            }


	    break;

/////////////////////////////////////////////////////////////////
// Offsets of telescope array instances for the following event 
/*         case IO_TYPE_MC_TELOFF:
            read_tel_offset(iobuf,MAX_ARRAY,&array.narray, &array.toff,array.xoff,array.yoff);
            toffset = array.toff; 
      	    array.mc_run.num_arrays = array.narray;
            break; */

// Unknown / any other material
         default:
            break;
      } // End of switch over all input data types
      if( readNevent >= nevents && nevents > 0 ) break;
    } // End of loop over all data in the input file 

// close all files
    fclose(iobuf->input_file);
    iobuf->input_file = NULL;

   return 0;
}
