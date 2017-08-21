/*! \file atmosphereAndCherenkovLight
    \brief plot atmosphere and cherenkov properties from CORSIKA/KASCADE atmospheres

    \date 30/09/04
*/

#include "TApplication.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TLine.h"
#include "TMath.h"
#include "TROOT.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "VPlotModtran.h"
#include "VAtmKascade.h"
#include "VAtmCorsika.h"

using namespace std;

bool bPoster = true;

void drawAll()
{
   VPlotModtran *modtran6 = new VPlotModtran( "data/atmprof6.dat" );
   VPlotModtran *modtran2 = new VPlotModtran( "data/atmprof2.dat" );
   VPlotModtran *modtran3 = new VPlotModtran( "data/atmprof3.dat" );
   VPlotModtran *corskas = new VPlotModtran( "data/atmprof11.dat" );
   VAtmCorsika *cors = new VAtmCorsika();
   cors->fill( modtran6->getAltitude() );
   VAtmKascade *kascade = new VAtmKascade();
   kascade->fill( modtran6->getAltitude() );

   TCanvas *c11 = new TCanvas( "c11", "density", 10, 10, 600, 400 );
   TLegend *l11 = new TLegend( 0.52,0.62,0.89,0.89); 
   modtran6->g_rho->SetLineWidth( 2 );
   modtran6->g_rho->Draw( "al" );
   modtran6->g_rho->SetTitle( "atmospheric density" );
   modtran6->g_rho->GetXaxis()->SetTitle( "altitude [km]" );
   modtran6->g_rho->GetYaxis()->SetTitle( "log_{10} (rho [g/cm^{3}])" );
   l11->AddEntry( modtran6->g_rho, "MODTRAN US Standard (6)", "L" );
   modtran2->g_rho->Draw( "l" );
   l11->AddEntry( modtran2->g_rho, "MODTRAN midlatitude summer (2)", "L" );
   modtran3->g_rho->SetLineStyle( 4 );
   modtran3->g_rho->Draw( "l" );
   l11->AddEntry( modtran3->g_rho, "MODTRAN midlatitude winter (3)", "L" );
   cors->g_rho->SetLineWidth( 2 );
   cors->g_rho->Draw( "l" );
   l11->AddEntry( cors->g_rho, "CORSIKA", "L" );
   kascade->g_rho->SetLineWidth( 2 );
   kascade->g_rho->Draw( "l" );
   l11->AddEntry( kascade->g_rho, "kascade", "L" );
   l11->Draw();
   c11->Update();

   TCanvas *c12 = new TCanvas( "c12", "thickness", 50, 50, 600, 400 );
   TLegend *l12 = new TLegend( 0.52,0.62,0.89,0.89); 
   modtran6->g_thick->SetLineWidth( 2 );
   modtran6->g_thick->Draw( "al" );
   modtran6->g_thick->SetTitle( "atmospheric thickness" );
   modtran6->g_thick->GetXaxis()->SetTitle( "altitude [km]" );
   modtran6->g_thick->GetYaxis()->SetTitle( "log_{10} (thickness [g/cm^{2}])" );
   l12->AddEntry( modtran6->g_thick, "MODTRAN US Standard (6)", "L" );
   modtran2->g_thick->Draw( "l" );
   l12->AddEntry( modtran2->g_thick, "MODTRAN midlatitude summer (2)", "L" );
   modtran3->g_thick->SetLineStyle( 4 );
   modtran3->g_thick->Draw( "l" );
   l12->AddEntry( modtran3->g_thick, "MODTRAN midlatitude winter (3)", "L" );
   cors->g_thick->SetLineWidth( 2 );
   cors->g_thick->Draw( "l" );
   l12->AddEntry( cors->g_thick, "CORSIKA", "L" );
   kascade->g_thick->SetLineWidth( 2 );
   kascade->g_thick->Draw( "l" );
   l12->AddEntry( kascade->g_thick, "kascade", "L" );
   corskas->g_thick->SetLineWidth( 2 );
   corskas->g_thick->SetLineColor( 9 );
   corskas->g_thick->Draw( "l" );
   l12->AddEntry( corskas->g_thick, "atmprof11 (kascade)", "L" );
   l12->Draw();
   c12->Update();

// draw standard corsika - standard kascade
   double x;
   double y1;
   double y2;
   TGraph *g_thickR = new TGraph( 36 );
   for( unsigned int i = 0; i < 37; i++ )
   {
      kascade->g_thick->GetPoint( i, x, y2 );
      modtran6->g_thick->GetPoint( i, x, y1 );
      if( y2 > 0. ) g_thickR->SetPoint( i+1, x, pow(10, y1 ) - pow(10, y2 ) );
   }
   TCanvas *c12b = new TCanvas( "c12b", "thickness difference", 50, 50, 600, 400 );
   c12b->Draw();
   g_thickR->SetTitle( "difference in thickness (Corsika-kascade)" );
   g_thickR->SetMarkerStyle( 20 );
   g_thickR->SetLineWidth( 2 );
   g_thickR->Draw("al" );
   g_thickR->GetXaxis()->SetTitle( "altitude [km]" ); 
   g_thickR->GetYaxis()->SetTitle( "#lambda_{Cors}-#lambda_{kas} [g/cm^{2}]" );

   c12b->Update(); 

   TCanvas *c13 = new TCanvas( "c13", "refractive index", 50, 50, 600, 600 );
   c13->SetLeftMargin( 0.12 );
   TLegend *l13 = new TLegend( 0.52,0.62,0.89,0.89); 
//   gPad->SetLogx( 1 );
   modtran6->g_n->SetLineWidth( 2 );
   if( bPoster ) modtran6->g_n->SetLineWidth( 5 );
   modtran6->g_n->Draw( "al" );
   modtran6->g_n->SetTitle( "" );
   modtran6->g_n->GetXaxis()->SetTitle( "altitude [km]" );
   modtran6->g_n->GetYaxis()->SetTitle( "refractive index - 1" );
   modtran6->g_n->GetYaxis()->SetTitleOffset( 1.5 );
   l13->AddEntry( modtran6->g_n, "MODTRAN US Standard (6)", "L" );
   if( !bPoster )
   {
      modtran2->g_n->Draw( "l" );
      l13->AddEntry( modtran2->g_n, "MODTRAN midlatitude summer (2)", "L" );
      modtran3->g_n->SetLineStyle( 4 );
      modtran3->g_n->Draw( "l" );
      l13->AddEntry( modtran3->g_n, "MODTRAN midlatitude winter (3)", "L" );
      cors->g_n->SetLineWidth( 2 );
      cors->g_n->Draw( "l" );
      l13->AddEntry( cors->g_n, "CORSIKA", "L" );
      kascade->g_n->SetLineWidth( 2 );
      kascade->g_n->Draw( "l" );
      l13->AddEntry( kascade->g_n, "kascade", "L" );
      corskas->g_n->SetLineWidth( 2 );
      corskas->g_n->SetLineColor( 9 );
      corskas->g_n->Draw( "l" );
      l13->AddEntry( corskas->g_n, "atmprof11 (kascade)", "L" );
      l13->Draw();
   }
   c13->Update();
}   

/*
    plot cherenkov angle vs. production height

    plot photon impact points vs. production height

    plot cherenkov emission threshold

    energy in GeV
*/
void plotMaxLight( int iD = 5, double energy = 10, double obslevel = 0. )
{
// wavelength intervall for number of emitted photons
   double lambdamin = 200.;
   double lambdamax = 685.;
// particle mass
   char cTemp[200];
   double muonmass =  0.1056;
   double electronmass = 0.511/1000.;
   double mass = 0.;
   if( iD == 2 || iD == 3 ) mass = electronmass;
   else if( iD == 5 || iD == 6 ) mass = muonmass;
   else 
   {
      cout << "unknown particle: " << iD << endl;
      return;
   }
   double gamma = energy / mass;
   double beta = sqrt( 1. -  1. / gamma / gamma );
   double betaN = 0.;
// atmospheres
   VPlotModtran *modtran6 = new VPlotModtran( "data/atmprof6.dat" );
   vector<double> altitude = modtran6->getAltitude();
   VAtmCorsika *cors = new VAtmCorsika();
   cors->fill( modtran6->getAltitude() );
   
// cherenkov emission angle
   TGraph *gTheta = new TGraph(1 );
   sprintf( cTemp, "Cherenkov emission angle #Theta, particle energy: %.1f GeV", energy );
   gTheta->SetTitle( cTemp );
// distance to shower core at observation level
   TGraph *gDist = new TGraph(1 );
   sprintf( cTemp, "Cherenkov photon impact points, particle energy: %.1f GeV, observation level: %.1f m", energy, obslevel );
   gDist->SetTitle( cTemp );
// number of emitted photons per path length
   TGraph *gPho = new TGraph(1 );
   sprintf( cTemp, "Number of Cherenkov photons per path length, particle energy: %.1f GeV", energy );
   gPho->SetTitle( cTemp );
// cherenkov emission threshold
   TGraph *gThresh = new TGraph(1 );
   if( iD < 4 ) sprintf( cTemp, "Cherenkov emission threshold for electrons" );
   else sprintf( cTemp, "Cherenkov emission threshold for muons" );
   gThresh->SetTitle( cTemp );
// fill the graphs
   double iT = 0.;
   for( unsigned int i = 0; i < altitude.size(); i++ )
   {
      if( altitude[i] > modtran6->getMaxAltitude() ) continue;
      iT = 1. / beta / cors->getN( altitude[i] * 1.E3 );
      if( iT > 1. ) iT = 1.;
      iT = acos( iT );
      gTheta->SetPoint( i, altitude[i], iT * 45. / atan( 1. ) );
      if( altitude[i]*1.E3 > obslevel ) gDist->SetPoint( i, altitude[i], ( altitude[i] * 1.E3 - obslevel ) * tan( iT ) );
      else gDist->SetPoint( i, altitude[i], 0. );
      gPho->SetPoint( i, altitude[i], 2 * TMath::Pi()/137. * (1./lambdamin-1./lambdamax)*sin(iT)*sin(iT) );
      betaN = 1./cors->getN( altitude[i] * 1.E3 );
      if( energy > 0. ) gThresh->SetPoint( i, altitude[i], (mass * sqrt( 1./(1.-betaN*betaN)))  );
//      if( energy > 0. ) gThresh->SetPoint( i, altitude[i], log10(mass * sqrt( 1./(1.-betaN*betaN)))  );
   }
   betaN = 1./cors->getN( 1. );
   cout << "threshold energy: " << mass * sqrt( 1./(1.-betaN*betaN)) << " GeV" << endl; 

// plotting
   TCanvas *cT = new TCanvas( "cT", "cherenkov angle", 50, 50, 600, 600 );
   cT->Draw();
   cT->SetLeftMargin( 0.12 );
   cT->SetLogy( 1 );
   gTheta->SetLineWidth( 5 );
   gTheta->Draw( "al" ); 
   gTheta->GetXaxis()->SetTitle( "altitude [km]" );
   gTheta->GetYaxis()->SetTitle( "Cherenkov angle #Theta [deg]" );
   gTheta->GetYaxis()->SetTitleOffset( 1.4 );

   TCanvas *cG = new TCanvas( "cG", "Cherenkov photon impact point", 150, 50, 600, 600 );
   cG->Draw();
   cG->SetLeftMargin( 0.12 );
   gDist->SetLineWidth( 5 );
   gDist->Draw( "al" ); 
   gDist->GetXaxis()->SetTitle( "altitude [km]" );
   gDist->GetYaxis()->SetTitle( "distance to shower core [m]" );
   gDist->GetYaxis()->SetTitleOffset( 1.4 );

   TCanvas *cH= new TCanvas( "cH", "Cherenkov emission threshold energy", 250, 50, 600, 600 );
   cH->Draw();
   cH->SetLeftMargin( 0.12 );
   gThresh->SetLineWidth( 5 );
   gThresh->Draw( "al" ); 
   gThresh->GetXaxis()->SetTitle( "altitude [km]" );
//   gThresh->GetYaxis()->SetTitle( "log_{10} Cherenkov emission threshold energy [GeV]" );
   gThresh->GetYaxis()->SetTitleOffset( 1.4 );
   gThresh->GetYaxis()->SetTitle( "Cherenkov emission threshold energy [GeV]" );
//   TLine *iL = new TLine( 0., log10( energy ), 120., log10( energy ) );
   TLine *iL = new TLine( 0., ( energy ), 120., ( energy ) );
   iL->Draw();

   TCanvas *cP= new TCanvas( "cP", "Cherenkov yield", 350, 50, 600, 600 );
   cP->Draw();
   cP->SetLeftMargin( 0.12 );
   cP->SetLogy( 1 );
   gPho->SetLineWidth( 5 );
   gPho->Draw( "al" ); 
   gPho->GetYaxis()->SetTitleOffset( 1.4 );
   gPho->GetXaxis()->SetTitle( "altitude [km]" );
   gPho->GetYaxis()->SetTitle( "dN_{C}/dN_{s}" );
}

int main( int argc, char *argv[] )
{
   gROOT->SetStyle("Plain");
   TApplication theApp( "App", &argc, argv );
   if( argc != 4 ) 
   {
      cout << endl;
      cout << "atmosphereAndCherenkovLight:  draw different atmosphere and Cherenkov light production properties" << endl;
      cout << " atmosphereAndCherenkovLight <particle ID> <energy [GeV]> <observation level [m]>" << endl;
      cout << "\t allowed particle IDs: 3=electron, 5=muon" << endl;
      cout << endl;
      exit( 0 );
   }
   else
   {
      int iID;
      double iE0;
      double iObs;

      iID = atoi( argv[1] );
      iE0 = atof( argv[2] );
      iObs = atof( argv[3] );
      drawAll();
      plotMaxLight( iID, iE0, iObs );
   }
   theApp.Run();
}
