#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "TCanvas.h"
#include "TH1D.h"
#include "TText.h"

using namespace std;

map< unsigned int, unsigned int > fNEventsPerRun;

void plot( char *ifile, string iName, double iHisMin, double iHisMax )
{
    if( !ifile ) return;

    char hname[200];
    char htitle[200];

    cout << "analysing " << iName << " numbers from " << ifile << endl;
     
    sprintf( hname, "h%s", iName.c_str() );
    TH1D *hTime = new TH1D( hname, "", 100, iHisMin, iHisMax );
    hTime->SetStats( 0 );
    hTime->SetTitle( "" );
    sprintf( hname, "log_{10} %s execution time per event [s]", iName.c_str() );
    hTime->SetXTitle( hname );
    hTime->SetYTitle( "number of runs" );
    hTime->SetFillColor( 8 );

    ifstream ftxt( ifile );
    if( !ftxt ) return;

    float iMax = 0.;
    float iMin = 1.e9;
    unsigned int irun;
    float iTime = 0.;
    float iTot = 0.;
    float iTotN = 0.;

    while( !ftxt.eof() )
    {
        ftxt >> irun;
	ftxt >> iTime;
     
	if( iTime > iMax ) iMax = iTime;
	if( iTime < iMin ) iMin = iTime;

	iTot += iTime;

	if( fNEventsPerRun.find( irun ) != fNEventsPerRun.end() )
	{
	   if( fNEventsPerRun[irun] > 0. )
	   {
	      iTotN += fNEventsPerRun[irun];
	      iTime /= fNEventsPerRun[irun];
	      if( iTime > 0. ) hTime->Fill( log10( iTime ) );
           }
        }
    }
    ftxt.close();

    cout << "maximum execution time: " << iMax << endl;
    cout << "minimum execution time: " << iMin << endl;
    cout << "total execution time: " << iTot << endl;
    cout << "total number of events: " << iTotN << endl;

    sprintf( hname, "c%s", iName.c_str() );
    sprintf( htitle, "%s execution time", iName.c_str() );
    TCanvas *cT = new TCanvas( hname, htitle, 10, 10, 600, 400 );
    cT->SetGridx( 0 );
    cT->SetGridy( 0 );
    cT->Draw();

    hTime->Draw(); 

    sprintf( hname, "total time (%s; %d events): %.0f [h]", iName.c_str(), (int)iTotN, iTot/3600. );
    TText *iT = new TText( 0.15, 0.82, hname );
    iT->SetNDC();
    iT->Draw();
}

void plotExecutiontime( char *iNeventFile, char *iCorsFile = 0, char *GrisuFile = 0, char *EVNDFile = 0 )
{
    string i_line;
    unsigned int i1;
    unsigned int i2;

// get number of events per run

    ifstream fnev( iNeventFile );
    if( !fnev ) return;

    while( !fnev.eof() )
    {
        fnev >> i1;
	fnev >> i2;

	fNEventsPerRun[i1] = i2;
    }

    cout << "total number of runs: " << fNEventsPerRun.size() << endl;

    plot( iCorsFile, "CORSIKA", 0., 3. );
    plot( GrisuFile, "grisudet-CFG2", 0., 4. );
    plot( EVNDFile, "ANALYSIS-CFG2", 0., 1.5 );
}
