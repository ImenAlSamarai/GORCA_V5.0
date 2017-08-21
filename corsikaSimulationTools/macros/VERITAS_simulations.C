void zenith_bins( int nBins = 6, double ze_min = 0., double ze_max = 45. )
{
    double sec_min = 1./cos((ze_min)*TMath::Pi()/180.);
    double sec_max = 1./cos((ze_max)*TMath::Pi()/180.);

    cout << sec_min << "\t" << sec_max << endl;

    TCanvas *c = new TCanvas( "c", "zenith angle bins", 10, 10, 400, 400 );
    c->SetGridx( 0 );
    c->SetGridy( 0 );
    c->Draw();

    TF1 *f = new TF1("f1", "1./cos((90.-x)*TMath::Pi()/180.)", 90.-ze_max, 90.-ze_min );
    f->Draw();
    TH1D *h = (TH1D*)f->GetHistogram();
    h->SetTitle();
    h->SetXTitle( "elevation" );
    h->SetYTitle( "atmospheric depth [a.u.]" );
    h->SetMaximum( sec_max * 1.05 );
    double fh_min = 0.97;
    h->SetMinimum( fh_min );

    TLine *iL = new TLine( h->GetXaxis()->GetXmin(), 1., h->GetXaxis()->GetXmax(), 1. );
    iL->SetLineStyle( 2 );
    iL->Draw();

    for( int i = 0; i < nBins; i++ )
    {
       cout << "bin " << i << "\t";
       double a = sec_min + i * (sec_max-sec_min)/(double)(nBins-1);
       cout << "\t sec " << a;
       double ze = acos( 1./a ) * 180./TMath::Pi();
       cout << "\t ze " << ze << endl;
       TLine *iL = new TLine( 90.-ze, fh_min, 90.-ze, sec_max * 1.05 );
       iL->SetLineStyle( 3 );
       iL->SetLineColor( 2 );
       iL->Draw();
    }


}
