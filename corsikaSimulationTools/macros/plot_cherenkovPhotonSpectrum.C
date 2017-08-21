#include <string>


TH1D* getCherenkovSpectrum( string ifile )
{
    TFile *f = new TFile( ifile.c_str() );
    if( f->IsZombie() )
    {
        cout << "error while reading file " << ifile << endl;
	return 0;
    }

    TTree *t = (TTree*)f->Get( "tcors" );
    if( !t ) return 0;

    TH1D *hI = 0;
    TH1D *h = 0;

    t->SetBranchAddress( "hSLambda", &hI );

    cout << "total number of showers " << t->GetEntries() << endl;
    for( int i = 0; i < t->GetEntries(); i++ )
//    for( int i = 0; i < 200; i++ )
    {
        t->GetEntry( i );

	if( hI )
	{
	    if( i == 0 )
	    {
	       h = (TH1D*)hI->Clone( "hOnMirror" );
            }
	    h->Add( hI );
        }
     }
     return h;
}
	        

void plot_spectra( string ifile, string ireflectivity = "../data/reflectivity.root", string iOut = "" )
{
    TH1D *hOnMirror = getCherenkovSpectrum( ifile );
    if( !hOnMirror ) return;

    hOnMirror->Rebin( 2 );

// get reflectivities
    TH1D *hOnPMT = 0;
    TFile *fM = new TFile( ireflectivity.c_str() );
    if( !fM->IsZombie() )
    {
       fM->ls();

       TGraph *gM = (TGraph*)fM->Get( "gMirrorReflectivity" );
       if( !gM )
       {
	   cout << "Error: could not find reflectivity graph" << endl;
	   return;
       }

// apply mirror reflectivity
       hOnPMT = (TH1D*)hOnMirror->Clone( "hOnPMT" );

       for( int i = 1; i <= hOnMirror->GetNbinsX(); i++ )
       {
	  if( hOnMirror->GetBinContent( i ) > 0 )
	  {
	     hOnPMT->SetBinContent( i, hOnMirror->GetBinContent( i ) * gM->Eval( hOnMirror->GetBinCenter( i ) ) );
	  }
       }
    }

// plot everything

    TCanvas *cOnMirror = new TCanvas( "cOnMirror", "on mirror", 10, 10, 400, 400 );
    cOnMirror->SetGridx( 0 );
    cOnMirror->SetGridy( 0 );

    hOnMirror->SetTitle( "" );
    hOnMirror->SetStats( 0 );
    hOnMirror->SetLineColor( 1 );
    hOnMirror->SetLineWidth( 3 );
    hOnMirror->SetAxisRange( 200., 680. );
    hOnMirror->Draw();

    if( hOnPMT )
    {
       hOnPMT->SetTitle( "" );
       hOnPMT->SetLineWidth( 3 );
       hOnPMT->Draw( "same" );
    }

// write everything to disk
    if( iOut.size() > 0 )
    {
        TFile *fO = new TFile( iOut.c_str(), "RECREATE" );
	if( fO->IsZombie() ) return;

	hOnMirror->Write();
	if( hOnPMT ) hOnPMT->Write();
	if( gM ) gM->Write();
    }
}
