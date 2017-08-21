//! VAtmCorsika standard corsika atmosphere
// Revision $Id: VAtmCorsika.h,v 1.1.1.1 2011/07/21 20:35:56 gmaier Exp $

#ifndef VAtmCorsika_H
#define VAtmCorsika_H

#include "TGraph.h"

#include <iostream>
#include <cmath>
#include <string>
#include <vector>

using namespace std;

class VAtmCorsika 
{
   private:
      string ftitle;
      vector<double> altitude;
      vector<double> rho;
      vector<double> thick;
      vector<double> n;

      vector< double > l_max;
      vector< double > a;
      vector< double > b;
      vector< double > c;

   public:
       VAtmCorsika(); 
      ~VAtmCorsika();

       void fill( vector<double> );
       double getN( double altitude );           //!< altitude in [m]
       double getRho( double altitude );         //!< altitude in [m]
       double getThick( double altitude );       //!< altitude in [m]

       TGraph *g_rho;
       TGraph *g_thick;
       TGraph *g_n;
};

#endif
