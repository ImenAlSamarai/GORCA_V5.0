/*! \class VPEWriter
    \brief write for pe data to root files

    Revision $Id: VPEWriter.cpp,v 1.1.1.1 2011/07/21 20:35:56 gmaier Exp $

    \author Gernot Maier
*/

#include "VPEWriter.h"

VPEWriter::VPEWriter( unsigned int iTelID, unsigned int iUsePixelNumbers, unsigned int iNInitEvents )
{
    fTelID = iTelID;
    fUsePixelNumbers = iUsePixelNumbers;

// data vectors
    fPE_pixelID = new std::vector< unsigned int >();
    if( iNInitEvents < fPE_pixelID->max_size() ) fPE_pixelID->reserve( iNInitEvents );
    else                                         fPE_pixelID->reserve( fPE_pixelID->max_size() - 1 );
    fPE_photonX = new std::vector< float >();
    if( iNInitEvents < fPE_photonX->max_size() ) fPE_photonX->reserve( iNInitEvents );
    else                                         fPE_photonX->reserve( fPE_photonX->max_size() - 1 );
    fPE_photonY = new std::vector< float >();
    if( iNInitEvents < fPE_photonY->max_size() ) fPE_photonY->reserve( iNInitEvents );
    else                                         fPE_photonY->reserve( fPE_photonY->max_size() - 1 );
    fPE_time = new std::vector< float >();
    if( iNInitEvents < fPE_time->max_size() ) fPE_time->reserve( iNInitEvents );
    else                                      fPE_time->reserve( fPE_time->max_size() - 1 );
    fPE_wl = new std::vector< float >();
    if( iNInitEvents < fPE_wl->max_size() ) fPE_wl->reserve( iNInitEvents );
    else                                    fPE_wl->reserve( fPE_wl->max_size() - 1 );

// define trees with pe data
    char hname[400];
    char htitle[400];

    sprintf( hname, "tPE_T%d", fTelID );
    sprintf( htitle, "pe data for telescope %d", fTelID );
    
    fTree = new TTree( hname, htitle );

    fTree->Branch( "eventNumber", &fEventNumber, "eventNumber/i" );
    fTree->Branch( "primaryType", &fPrimaryType, "primaryType/i" );
    fTree->Branch( "primaryEnergy", &fPrimaryEnergy, "primaryEnergy/F"  );
    fTree->Branch( "Xcore", &fXcore, "Xcore/F" );
    fTree->Branch( "Ycore", &fYcore, "Ycore/F" );
    fTree->Branch( "Xcos", &fXcos, "Xcos/F" );
    fTree->Branch( "Ycos", &fYcos, "Ycos/F" );
    fTree->Branch( "Xsource", &fXsource, "Xsource/F" );
    fTree->Branch( "Ysource", &fYsource, "Ysource/F" );
    fTree->Branch( "delay", &fDelay, "delay/F" );
    fTree->Branch( "pixelID", &fPE_pixelID );
    fTree->Branch( "photonX", &fPE_photonX );
    fTree->Branch( "photonY", &fPE_photonY );
    fTree->Branch( "time", &fPE_time );
    fTree->Branch( "wavelength", &fPE_wl );

}


int VPEWriter::add_event( unsigned int iEventNumber, unsigned int iPrimaryType, float iPrimaryEnergy, float iXcore, float iYcore, float iXcos, float iYcos, float iXsource, float iYsource, float iDelay )
{
    if( !fTree ) return 0;

    fEventNumber = iEventNumber;
    fPrimaryType = iPrimaryType;
    fPrimaryEnergy = iPrimaryEnergy;
    fXcore = iXcore;
    fYcore = iYcore;
    fXcos = iXcos;
    fYcos = iYcos;
    fXsource = iXsource;
    fYsource = iYsource;
    fDelay = iDelay;

    int r = fTree->Fill();

    fPE_pixelID->clear();
    fPE_photonX->clear();
    fPE_photonY->clear();
    fPE_time->clear();
    fPE_wl->clear();

    return r;
}


void VPEWriter::add_pe( unsigned int iPE_pixelID, float iPE_photonX, float iPE_photonY, float iPE_time, float iPE_wl )
{
   fPE_pixelID->push_back( iPE_pixelID );
   fPE_photonX->push_back( iPE_photonX );
   fPE_photonY->push_back( iPE_photonY );
   fPE_time->push_back( iPE_time );
   fPE_wl->push_back( iPE_wl );
}
