//! VAtmKascade Kascade atmoshpere
// Revision $Id: VAtmKascade.h,v 1.1.1.1 2011/07/21 20:35:56 gmaier Exp $

#ifndef VAtmKascade_H
#define VAtmKascade_H

#include "TGraph.h"

#include <iostream>
#include <cmath>
#include <string>
#include <vector>

using namespace std;

class VAtmKascade
{
   private:
      string ftitle;
   
      vector<double> altitude;
      vector<double> rho;
      vector<double> thick;
      vector<double> n;

      double zsea;
      double zatm1;
      double zatm2;
      double zatm3;
      double rsea;
      double ratm1;
      double ratm2;
      double ratm3;
      double bsea;
      double b2atm;
      double b3atm;
      double asea;
      double a2atm;
      double a3atm;
      double csea;
      double c2atm;
      double c3atm;
      double c11atm;
      double c22atm;
      double lambda;

      double eta_sea( double );

   public:

      VAtmKascade(); 
     ~VAtmKascade();
      void fill( vector<double> );
      double getN( double ialt );
      double getRho( double ialt );
      double getThick( double ialt );

       TGraph *g_rho;
       TGraph *g_thick;
       TGraph *g_n;
};

#endif
