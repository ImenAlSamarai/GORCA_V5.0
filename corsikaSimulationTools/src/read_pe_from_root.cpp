/*! \file read_pe_from_root.cpp
    \short read pe format from root tree and dump to screen

    Revision $Id: read_pe_from_root.cpp,v 1.1.1.1 2011/07/21 20:35:57 gmaier Exp $

    \author
    Gernot Maier
*/

#include "TFile.h"
#include "TROOT.h"
#include "TTree.h"

#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

void help()
{
     cout << "read_pe_from_root<telID> <inputfile.root> " << endl;
     cout << endl;
     cout << "command line options: " << endl;
     cout << "\t <telID> \t telescope ID" << endl;
     cout << "\t <inputfile.root> \t ROOT input file" << endl;
     cout << endl;
     exit( 0 );
}


int main( int argc, char **argv )
{
    if( argc != 3 ) help();

    gROOT->ProcessLine("#include <vector>");

    unsigned int fTelID = (unsigned int)(atoi( argv[1] ) );
    string fInputFileName = argv[2];

    TFile *fO = new TFile( fInputFileName.c_str() );
    if( fO->IsZombie() )
    {
        cout << "error opening root input file: " << fInputFileName << endl;
	cout << "...exiting" << endl;
	exit( -1 );
    }


// read tree from file
    char hname[400];
    sprintf( hname, "tPE_T%d", fTelID );
    TTree *t = (TTree*)fO->Get( hname );
    if( !t )
    {
        cout << "error: tree " << hname << " not found in " << fInputFileName << endl;
	cout << "...exiting" << endl;
	exit( -1 );
    }
    unsigned int fEventNumber = 0;
    float fPrimaryEnergy = 0.;
    float fXcore = 0.;
    float fYcore = 0.;
    float fXcos = 0.;
    float fYcos = 0.;
    float fXsource = 0.;
    float fYsource = 0.;
    std::vector< unsigned > *v_f_ID = 0;
    std::vector< float > *v_f_x = 0;
    std::vector< float > *v_f_y = 0;
    std::vector< float > *v_f_time = 0;
    std::vector< float > *v_f_wl = 0;
    TBranch *b_v_f_ID;
    TBranch *b_v_f_time = 0;
    TBranch *b_v_f_x = 0;
    TBranch *b_v_f_y = 0;
    TBranch *b_v_f_wl = 0;
    t->SetBranchAddress("eventNumber", &fEventNumber );
    t->SetBranchAddress("primaryEnergy", &fPrimaryEnergy );
    t->SetBranchAddress("Xcore", &fXcore );
    t->SetBranchAddress("Ycore", &fYcore );
    t->SetBranchAddress("Xcos", &fXcos );
    t->SetBranchAddress("Ycos", &fYcos );
    t->SetBranchAddress("Xsource", &fXsource );
    t->SetBranchAddress("Ysource", &fYsource );
    t->SetBranchAddress("pixelID", &v_f_ID, &b_v_f_ID );
    t->SetBranchAddress("photonX", &v_f_x, &b_v_f_x );
    t->SetBranchAddress("photonY", &v_f_y, &b_v_f_y );
    t->SetBranchAddress("time", &v_f_time, &b_v_f_time );
    t->SetBranchAddress("wavelength", &v_f_wl, &b_v_f_wl );

    cout << "total number of entries: " << t->GetEntries() << endl;
    for( int i = 0; i < t->GetEntries(); i++ )
    {
        t->GetEntry( i );

	cout << "entry " << i << "\t, event number: " << fEventNumber << ", primary energy [TeV]: " << fPrimaryEnergy << endl;
	cout << "\t core position [m]: " << fXcore << ", " << fYcore << endl;
	cout << "\t source direction and offsets: " << fXcos << "\t" << fYcos << "\t" << fXsource << "\t" << fYsource << endl;
	if( v_f_time && v_f_ID )
	{
	   cout << "\t vector sizes: " << v_f_time->size() << "\t" << v_f_ID->size() << "\t" << v_f_x->size();
	   cout << "\t" << v_f_y->size() << "\t" << v_f_wl->size() << endl;
	   if( v_f_time->size() == v_f_ID->size() &&  v_f_time->size() == v_f_x->size() && v_f_time->size() == v_f_y->size() && v_f_time->size() == v_f_wl->size() )
	   {
	      for( unsigned int v = 0; v < v_f_time->size(); v++ )
	      {
		  cout << "\t\t" << v_f_ID->at(v) << "\t" << v_f_time->at( v );
		  cout << "\t" << v_f_x->at( v ) << "\t" << v_f_x->at( v ) << "\t" << v_f_wl->at( v ); 
		  cout << endl;
	      }
	      cout << endl;
           }
        }
	cout << endl;
    }
    fO->Close();


}
