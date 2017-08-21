/*! VPlotModtran plot atmosphere and cherenkov properties from CORSIKA/KASCADE atmospheres

   Revision $Id: VPlotModtran.h,v 1.1.1.1 2011/07/21 20:35:56 gmaier Exp $

   \author Gernot Maier
*/

#ifndef VPlotModtran_H
#define VPlotModtran_H

#include "TApplication.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TLine.h"
#include "TMath.h"
#include "TROOT.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class VPlotModtran
{
   private:
      vector<double> altitude;
      vector<double> rho;
      vector<double> thick;
      vector<double> n;

      double fMaxAltitude;

   public:
       TGraph *g_rho;
       TGraph *g_thick;
       TGraph *g_n;


       VPlotModtran() {}
       VPlotModtran( string );
      ~VPlotModtran();

       vector<double> getAltitude() { return altitude; }
       vector<double> getRefraction() { return n; }
       void           setMaxAltitude( double iB ) { fMaxAltitude = iB; }
       double         getMaxAltitude() { return fMaxAltitude; }
};

#endif
