#include <vector>

/*!
     calculate amount of showers needed in simulations per energie bin (fixed energies) assuming a certain spectral slope

     t = energy bin
     n = number of showers
*/
void plot_fixedEnergies( int t = 7, double nshower = 10., double index = -2. ) 
{
    double imax = 0.;

    vector< double > e;     //! energy bin
    vector< double > n;     //! number of showers per bin
    vector< double > f;

    e.push_back( 0.05 );    n.push_back( 1000. );
    e.push_back( 0.10 );    n.push_back(  250. );
    e.push_back( 0.20 );    n.push_back(  100. );
    e.push_back( 0.50 );    n.push_back(   20. );
    e.push_back( 1.00 );    n.push_back(   20. );
    e.push_back( 2.00 );    n.push_back(   10. );
    e.push_back( 5.00 );    n.push_back(    4. );
    e.push_back( 10.00 );   n.push_back(    2. );

    for( unsigned int i = 0; i < n.size(); i++ ) f.push_back( nshower );

    TGraph *g = new TGraph( 1 );
    g->SetLineWidth( 2 );
    g->SetMarkerStyle( 20 );

    for( unsigned int i = 0; i < e.size(); i++ )
    {
        g->SetPoint( i, log10( e[i] ), n[i] * f[i] );
	if( n[i] * f[i] > imax ) imax = n[i] * f[i];
    }

// calculate how much should be simulated

    TGraph *s = new TGraph( 1 );
    s->SetLineColor( 2 );
    s->SetLineWidth( 2 );
    s->SetMarkerColor( 2 );
    s->SetMarkerStyle( 20 );

    if( t >= e.size() )
    {
       cout << "Maximum size of array: " << e.size() << endl;
       continue;
    }

    for( unsigned int i = 0; i < e.size(); i++ )
    {
       double w = TMath::Power( e[i] / e[t], index );

       double z = w * n[t] * f[t];

       int ns = (int)((z+1)/n[i]);

       cout << "E=" << e[i] << " TeV, (bin " << i << " ): " << "\t" << z << " showers, \t";
       cout << ns << " files" << endl;

       s->SetPoint( i, log10( e[i] ),  z );
	if( z > imax ) imax = z;
    }

    TCanvas *c = new TCanvas( "c", "AGIS simulations (n)", 10, 10, 400, 400 );
    c->SetGridx( 0 );
    c->SetGridy( 0 );
    c->SetLogy( 1 );

    TH1D *hnull = new TH1D( "hnull", "", 10, -2., 1.5 );
    hnull->SetStats( 0 );
    hnull->SetMinimum( 1. );
    hnull->SetMaximum( imax * 2.  );
    hnull->SetXTitle( "log_{10} energy" );
    hnull->Draw();

//    g->Draw( "pl" );
    s->Draw( "pl" );

}
