/*! \file mergePE.cpp
    \short merge several pe files into one

    Revision $Id: mergePE.cpp,v 1.1.1.1 2011/07/21 20:35:57 gmaier Exp $

    \author Gernot Maier

*/

#include <iostream>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"

using namespace std;

void help()
{
   cout << "mergePE <number of telescopes> <input files> <file with merged tree>" << endl;
   cout << endl;
   cout << "\t use UNIX wildcards to specify a number of files" << endl;
   cout << endl;
   exit( 0 );
}
int main( int argc, char **argv )
{
   if( argc != 4 ) help();

   TFile *fOut = new TFile( argv[3], "RECREATE" );
   if( fOut->IsZombie() )
   {
       cout << "error creating new file: " << argv[3] << endl;
       exit( -1 );
   }
   fOut->Close();
   unsigned int ntel = (unsigned int)atoi( argv[1] );

// loop over all telescopes
   char hname[200];
   char htitle[200];
   unsigned int eventNumber = 0;

   TChain *c = 0;
   for( unsigned int t = 0; t < ntel; t++ )
   {
      sprintf( hname, "tPE_T%d", t );
      cout << "merging tree " << hname << " for telescope " << t << endl;

      fOut = new TFile( argv[3], "UPDATE" );
      if( fOut->IsZombie() )
      {
          cout << "error updating file: " << argv[3] << endl;
          exit( -1 );
      }

      c = new TChain( hname );
      c->Add( argv[2] );
      c->SetBranchAddress( "eventNumber", &eventNumber );

      if( fOut->cd() ) 
      {
	  sprintf( htitle, "pe data for telescope %d", t );
          TTree *f = c->CloneTree( 0 );
          f->SetMaxTreeSize(1000*Long64_t(2000000000));


	  for( int i = 0; i < c->GetEntries(); i++ )
	  {
	     c->GetEntry( i );

	     eventNumber = i;

	     f->Fill();
          }

	  cout << "\t writing merged tree with " << f->GetEntries() << " entries" << endl;
	  f->Write();
          f->Delete();
      }
      c->Delete();
      fOut->Close();
   }
   cout << endl;
   cout << "closing file " << argv[3] << endl;
}

