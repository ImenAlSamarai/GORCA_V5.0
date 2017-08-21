/*! \class VTelescopePositions
    \brief calculate telescope positons on the ground

    Revision $Id: VTelescopePositions.cpp,v 1.1.1.1 2011/07/21 20:35:56 gmaier Exp $

    \author Gernot Maier
*/

#include "VTelescopePositions.h"

VTelescopePositions::VTelescopePositions()
{
    fCORSIKA_Data = new VTelescopeGroundPosition();
}

/* telescopes are on a hexagon grid

    WORK IN PROGRESS

*/
void VTelescopePositions::addHexConfiguration( string iName, int ntel, double iTelDistance, double iMirrorDiameter, int iColor, int iMarker )
{
    VTelescopeGroundPosition *i_temp = new VTelescopeGroundPosition();
    i_temp->fID = fData.size();
    i_temp->fName = iName;
    i_temp->fColor = iColor;
    i_temp->fMarker = iMarker;
    i_temp->fTelescopeDistance = iTelDistance;

// suppress warnings()
    iMirrorDiameter *= 1.;

    if( ntel > 0 )
    {
        double x = 0.;
	double y = 0.;

	for( int i = 0; i < ntel; i++ )
	{
	   y = 0.;
	   for( int j = 0; j < ntel; i++ )
	   {
	       x = 0.;
	   }
        }
    }

}

/*! 

    telescopes are on the corner of squares (centred)

    - assume same telescope size for all telescopes
    - CORSIKA coordinates (!)

    ntel:  > 0 : rectangular grid
    ntel:  = 0 : VERITAS basecamp configuration
*/
void VTelescopePositions::addSquareConfiguration( string iName, int ntel, double iTelDistance, double iTelShift, double iMirrorDiameter, int iColor, int iMarker )
{
    VTelescopeGroundPosition *i_temp = new VTelescopeGroundPosition();
    i_temp->fID = fData.size();
    i_temp->fName = iName;
    i_temp->fColor = iColor;
    i_temp->fMarker = iMarker;
    i_temp->fTelescopeDistance = iTelDistance;

// rectangular grid
    if( ntel > 0 )
    {
       int iN_xy = sqrt( (double)(ntel+0.5) );
       double iD_xy = i_temp->fTelescopeDistance * (double)(iN_xy-1)/2. + iTelShift;
       double x, y;
       int z = 0;
       for( int i = 0; i < iN_xy; i++ )
       {
	  x = i_temp->fTelescopeDistance * (double)i - iD_xy;
	  for( int j = 0; j < iN_xy; j++ )
	  {
	      y = i_temp->fTelescopeDistance * (double)j - iD_xy;

	      i_temp->fTelID.push_back( z+1 );
	      i_temp->x.push_back( x );
	      i_temp->y.push_back( y );
	      i_temp->z.push_back( 0. );
	      i_temp->r.push_back( iMirrorDiameter/2. );
	      z++;
	  }
       }
    }
// VERITAS bascamp configuration (4-tel)
// TELESCOPE -23.7E2 37.6E2 0.E2 7.E2
// TELESCOPE -47.7E2 -44.1E2 4.4E2 7.E2
// TELESCOPE 60.1E2 -29.4E2 9.8E2 7.E2
// TELESCOPE 11.3E2 35.9E2 7.E2 7.E2
    else if( ntel == 0 )
    {
        i_temp->fTelID.push_back( 1 );   i_temp->x.push_back( -23.7 );    i_temp->y.push_back(  37.6 );  i_temp->z.push_back( 0.0 );  i_temp->r.push_back( 7. );
        i_temp->fTelID.push_back( 2 );   i_temp->x.push_back( -47.7 );    i_temp->y.push_back( -44.1 );  i_temp->z.push_back( 4.4 );  i_temp->r.push_back( 7. );
        i_temp->fTelID.push_back( 3 );   i_temp->x.push_back(  60.1 );    i_temp->y.push_back( -29.4 );  i_temp->z.push_back( 9.8 );  i_temp->r.push_back( 7. );
        i_temp->fTelID.push_back( 4 );   i_temp->x.push_back(  11.3 );    i_temp->y.push_back(  35.9 );  i_temp->z.push_back( 7.0 );  i_temp->r.push_back( 7. );
    }
    i_temp->fNTel = i_temp->x.size();

    fData.push_back( i_temp );

    cout << "total number of telescopes in configuration " << i_temp->fID << ": " << i_temp->fNTel << endl;

}

vector< unsigned int > VTelescopePositions::getSelectedTelescopes( string iSelection )
{
   vector< unsigned int > i_SelectedArrays;
   if( iSelection.size() > 0 )
   {
       string i_temp;
       istringstream is_stream( iSelection );
       while( !is_stream.eof() )
       {
          is_stream >> i_temp;
	  i_SelectedArrays.push_back( (unsigned int)atoi( i_temp.c_str() ) );
       }
   }
   return i_SelectedArrays;
}


void VTelescopePositions::plot( string iSelection )
{
   vector< unsigned int > i_SelectedArrays = getSelectedTelescopes( iSelection );

   TCanvas *c = new TCanvas( "cTelescopePositions", "telescope positions", 10, 10, 400, 400 );
   c->Draw();

   double x = (getMaxDistance( i_SelectedArrays ) + 50.)/sqrt( 2. );
   TH1D *hNull = new TH1D( "hnull", "", 100, -1.*x, x );
   hNull->SetStats( 0 );
   hNull->SetMaximum( x );
   hNull->SetMinimum( -x );
   hNull->Draw();
   hNull->SetXTitle( "ground position X [m]" );
   hNull->SetYTitle( "ground position Y [m]" );

   for( unsigned int i = 0; i < fData.size(); i++ )
   {
       bool bSkip = false;
       for( unsigned int t = 0; t < i_SelectedArrays.size(); t++ )
       {
          if( fData[i]->fID == i_SelectedArrays[t] ) bSkip = true;
       }
       if( bSkip ) continue;
       cout << "Draw " << fData[i]->fID << " " << fData[i]->fName << endl;

       for( unsigned int t = 0; t < fData[i]->fNTel; t++ )
       {
           TEllipse *e = new TEllipse( fData[i]->x[t], fData[i]->y[t], fData[i]->r[t], fData[i]->r[t] );
	   e->SetLineColor( fData[i]->fColor );
	   e->SetFillColor( fData[i]->fColor );
//	   e->SetFillStyle( 4001 );
	   e->Draw();
       }
   }
}
          
double VTelescopePositions::getMaxDistance( vector< unsigned int > i_SelectedArrays )
{
    double iMax = 0.;

    for( unsigned int i = 0; i < fData.size(); i++ )
    {
       bool bSkip = false;
       for( unsigned int t = 0; t < i_SelectedArrays.size(); t++ )
       {
          if( fData[i]->fID == i_SelectedArrays[t] ) bSkip = true;
       }
       if( bSkip ) continue;

       for( unsigned int t = 0; t < fData[i]->fNTel; t++ )
       {
          double r = sqrt( fData[i]->x[t] * fData[i]->x[t] + fData[i]->y[t] * fData[i]->y[t] );
	  if( r > iMax )  iMax = r;
       }
    }
    return iMax;
}    

void VTelescopePositions::print()
{
   for( unsigned int i = 0; i < fData.size(); i++ )
   {
      print( fData[i]->fID );
   }
}

void VTelescopePositions::print( unsigned int iID )
{
    if( iID < fData.size() )
    {
       cout << fData[iID]->fID << "\t" << fData[iID]->fName;
       cout << " with " << fData[iID]->fNTel << " telescopes " << endl;
       for( unsigned int t = 0; t < fData[iID]->fNTel; t++ )
       {
           cout << "\t";
	   cout << t << "\t";
	   cout << fData[iID]->x[t] << "\t" << fData[iID]->y[t] << "\t" << fData[iID]->r[t] << endl;
       }
     } 
}

void VTelescopePositions::makeTELESCOPE_Lines( string iSelection, bool bCorsika, bool bPrint )
{
   vector< unsigned int > i_SelectedArrays = getSelectedTelescopes( iSelection );

   double x1 = 0.;
   double y1 = 0.;
   double x2 = 0.;
   double y2 = 0.;

   int z = 0;
   int q = 0;
   for( unsigned int i = 0; i < fData.size(); i++ )
   {
       bool bSkip = false;
       for( unsigned int t = 0; t < i_SelectedArrays.size(); t++ )
       {
          if( fData[i]->fID == i_SelectedArrays[t] ) bSkip = true;
       }
       if( bSkip ) continue;

       for( unsigned int n = 0; n < fData[i]->fNTel; n++ )
       {
          x1 = fData[i]->x[n];
	  y1 = fData[i]->y[n];

	  bool bAdd = true;
	  for( unsigned int j = 0; j < i; j++ )
	  {
	     if( i == j ) continue;

	     bool bSkip = false;
	     for( unsigned int t = 0; t < i_SelectedArrays.size(); t++ )
	     {
		if( fData[j]->fID == i_SelectedArrays[j] ) bSkip = true;
	     }
	     if( bSkip ) continue;

// check if this telescope is on top of another one
	     for( unsigned int m = 0; m < fData[j]->fNTel; m++ )
	     {
	        x2 = fData[j]->x[m];
	        y2 = fData[j]->y[m];

// calculate distance between the two positions
		double r = sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) );
		if( TMath::Abs( r ) < 5. ) bAdd = false;
             }
          }
	  if( bAdd && bCorsika )
	  {
	      z++;
	      if( bPrint )
              {
		 cout << showpoint << "echo TELESCOPE ";
		 cout <<   x1*1.e2 << " ";
		 cout <<   y1*1.e2 << " ";
		 cout <<   fData[i]->z[n]*1.e2  << " ";
		 cout <<   fData[i]->r[n]*1.e2;
		 cout << " >> $INAM";
		 cout << endl;
              }
	      fCORSIKA_Data->x.push_back( x1 );
	      fCORSIKA_Data->y.push_back( y1 );
	      fCORSIKA_Data->z.push_back( fData[i]->z[n] );
	      fCORSIKA_Data->r.push_back( fData[i]->r[n] );
          }
	  else if( bAdd )
	  {
// reasign telecope IDs
	      fData[i]->fTelID[n] = q;
	      q++;
              char hname[600];
	      sprintf( hname, "* TLLOC %d %+.3f %+.3f %+.3f %.3f 0.0 0.0", q, -1.*y1, x1, 0., fData[i]->r[n] );
	      if( bPrint ) cout << hname << endl;
	      fCORSIKA_Data->xG.push_back( x1 );
	      fCORSIKA_Data->yG.push_back( y1 );
          }
      }
   }

// fill telescope 
}

void VTelescopePositions::checkGRISU_positions()
{
    int iGood = 0;
    for( unsigned int i = 0; i < fCORSIKA_Data->x.size(); i++ )
    {
	for( unsigned int j = 0; j < fCORSIKA_Data->xG.size(); j++ )
	{
	   double r = (sqrt( (fCORSIKA_Data->x[i]-fCORSIKA_Data->xG[j])*(fCORSIKA_Data->x[i]-fCORSIKA_Data->xG[j])+(fCORSIKA_Data->y[i]-fCORSIKA_Data->yG[j])*(fCORSIKA_Data->y[i]-fCORSIKA_Data->yG[j]) ) );
	   if( r < 1 ) iGood++;
	}
    }
    cout << "\tTotal number of grisu telescopes: " << fCORSIKA_Data->xG.size() << endl;
    cout << "\tNumber of grisu telescopes found: " << iGood << endl;
}

void VTelescopePositions::checkCORSIKA_positions()
{
    if( !fCORSIKA_Data ) return;

    cout << "Total number of corsika positions: " << fCORSIKA_Data->x.size() << endl;

    for( unsigned int i = 0; i < fCORSIKA_Data->x.size(); i++ )
    {
        for( unsigned int j = 0; j < fCORSIKA_Data->x.size(); j++ )
	{
	   if( j == i ) continue;

	   double r = (sqrt( (fCORSIKA_Data->x[i]-fCORSIKA_Data->x[j])*(fCORSIKA_Data->x[i]-fCORSIKA_Data->x[j])+(fCORSIKA_Data->y[i]-fCORSIKA_Data->y[j])*(fCORSIKA_Data->y[i]-fCORSIKA_Data->y[j]) ) );

	   if( r < 1. )
	   {
	       cout << "ERROR: duplicated telescope position: " << fCORSIKA_Data->x[i] << " " << fCORSIKA_Data->y[i] << endl;
           }
       }
   }
   cout << "(end checking telescope positions)" << endl;
}	      

vector< string > VTelescopePositions::getGrisuline_TLLOC( unsigned int iID )
{
   vector< string > iTel;

   if( iID >= fData.size() ) return iTel;

   char hname[600];
   for( unsigned int i = 0; i < fData[iID]->fNTel; i++ )
   {
       sprintf( hname, "* TLLOC %d %+.3f %+.3f %+.3f %.3f 0.0 0.0", i+1, -1.*fData[iID]->y[i], fData[iID]->x[i], 0., fData[iID]->r[i] );
       iTel.push_back( hname );
   }
   return iTel;
}

void VTelescopePositions::writeEVNDISP_positions( string iSelection, string ofile )
{
   vector< unsigned int > i_SelectedArrays = getSelectedTelescopes( iSelection );

   char hname[800];

   for( unsigned int i = 0; i < fData.size(); i++ )
   {
       bool bSkip = false;
       for( unsigned int t = 0; t < i_SelectedArrays.size(); t++ )
       {
          if( fData[i]->fID == i_SelectedArrays[t] ) bSkip = true;
       }
       if( bSkip ) continue;

// open output file
       sprintf( hname, "%s_%d.dat", ofile.c_str(), i );
       ofstream os;
       os.open( hname );
       if( !os )
       {
          cout << "ERROR opening file for EVNDISP telescope positions: " << hname << endl;
	  cout << "(skipping this telescope configuration" << endl;
	  continue;
       }
       cout << "writing telescope positions to " << hname << endl;
       cout << "\t" << fData[i]->fTelescopeDistance << "\t" << fData[i]->fTelID.size() << endl;

       int z = 0;
       for( unsigned int t = 0; t < fData[i]->fTelID.size(); t++ )
       {
          for( unsigned int j = 0; j < fCORSIKA_Data->x.size(); j++ )
	  {
// remove a row to create 36 telescope array
/*              if( fCORSIKA_Data->x[j] < -1.*2.5 * fData[i]->fTelescopeDistance ) continue;
              if( fCORSIKA_Data->y[j] < -1.*2.5 * fData[i]->fTelescopeDistance ) continue; */

	      double r = (sqrt( (fCORSIKA_Data->x[j]-fData[i]->x[t])*(fCORSIKA_Data->x[j]-fData[i]->x[t])+(fCORSIKA_Data->y[j]-fData[i]->y[t])*(fCORSIKA_Data->y[j]-fData[i]->y[t]) ) );
	      if( r < 1. )
	      {
		   os << "* TELID " << z+1 << "   " << j+1 << endl;
		   sprintf( hname, "* TLLOC %d %+.3f %+.3f %+.3f %.3f 0.0 0.0", z+1, -1.*fData[i]->y[t], fData[i]->x[t], fData[i]->z[t], fData[i]->r[t] );
		   os << hname << endl;
		   z++;
              }
           }
       }
       os.close();
       cout << "total number of telescopes: " << z << endl;
   }
}

