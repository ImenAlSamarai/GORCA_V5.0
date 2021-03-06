/********************************************************************
* src/ROBASTDict.h
* CAUTION: DON'T CHANGE THIS FILE. THIS FILE IS AUTOMATICALLY GENERATED
*          FROM HEADER FILES LISTED IN G__setup_cpp_environmentXXX().
*          CHANGE THOSE HEADER FILES AND REGENERATE THIS FILE.
********************************************************************/
#ifdef __CINT__
#error src/ROBASTDict.h/C is only for compilation. Abort cint.
#endif
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define G__ANSIHEADER
#define G__DICTIONARY
#define G__PRIVATE_GVALUE
#include "G__ci.h"
#include "FastAllocString.h"
extern "C" {
extern void G__cpp_setup_tagtableROBASTDict();
extern void G__cpp_setup_inheritanceROBASTDict();
extern void G__cpp_setup_typetableROBASTDict();
extern void G__cpp_setup_memvarROBASTDict();
extern void G__cpp_setup_globalROBASTDict();
extern void G__cpp_setup_memfuncROBASTDict();
extern void G__cpp_setup_funcROBASTDict();
extern void G__set_cpp_environmentROBASTDict();
}


#include "TObject.h"
#include "TMemberInspector.h"
#include "include/ABorderSurfaceCondition.h"
#include "include/ACauchyFormula.h"
#include "include/AFocalSurface.h"
#include "include/AGeoAsphericDisk.h"
#include "include/AGeoBezierPcon.h"
#include "include/AGeoBezierPgon.h"
#include "include/AGeoWinstonCone2D.h"
#include "include/AGeoWinstonConePoly.h"
#include "include/AGlassCatalog.h"
#include "include/ALens.h"
#include "include/AMirror.h"
#include "include/AObscuration.h"
#include "include/AOpticalComponent.h"
#include "include/AOpticsManager.h"
#include "include/ARayArray.h"
#include "include/ARay.h"
#include "include/ARayShooter.h"
#include "include/ARefractiveIndex.h"
#include "include/ASchottFormula.h"
#include "include/ASellmeierFormula.h"
#include <algorithm>
namespace std { }
using namespace std;

#ifndef G__MEMFUNCBODY
#endif

extern G__linked_taginfo G__ROBASTDictLN_TClass;
extern G__linked_taginfo G__ROBASTDictLN_TBuffer;
extern G__linked_taginfo G__ROBASTDictLN_TMemberInspector;
extern G__linked_taginfo G__ROBASTDictLN_TObject;
extern G__linked_taginfo G__ROBASTDictLN_TNamed;
extern G__linked_taginfo G__ROBASTDictLN_basic_ostreamlEcharcOchar_traitslEchargRsPgR;
extern G__linked_taginfo G__ROBASTDictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR;
extern G__linked_taginfo G__ROBASTDictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__ROBASTDictLN_TObjArray;
extern G__linked_taginfo G__ROBASTDictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR;
extern G__linked_taginfo G__ROBASTDictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__ROBASTDictLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR;
extern G__linked_taginfo G__ROBASTDictLN_TGeoAtt;
extern G__linked_taginfo G__ROBASTDictLN_TAttLine;
extern G__linked_taginfo G__ROBASTDictLN_TAttFill;
extern G__linked_taginfo G__ROBASTDictLN_TAtt3D;
extern G__linked_taginfo G__ROBASTDictLN_TAttMarker;
extern G__linked_taginfo G__ROBASTDictLN_maplElongcOintcOlesslElonggRcOallocatorlEpairlEconstsPlongcOintgRsPgRsPgR;
extern G__linked_taginfo G__ROBASTDictLN_maplEintcOTGeoElementRNmUcOlesslEintgRcOallocatorlEpairlEconstsPintcOTGeoElementRNmUgRsPgRsPgR;
extern G__linked_taginfo G__ROBASTDictLN_TGeoMedium;
extern G__linked_taginfo G__ROBASTDictLN_TGeoBBox;
extern G__linked_taginfo G__ROBASTDictLN_TGeoMatrix;
extern G__linked_taginfo G__ROBASTDictLN_TGeoVolume;
extern G__linked_taginfo G__ROBASTDictLN_TBuffer3D;
extern G__linked_taginfo G__ROBASTDictLN_TGeoShape;
extern G__linked_taginfo G__ROBASTDictLN_TGeoNode;
extern G__linked_taginfo G__ROBASTDictLN_TGeoManager;
extern G__linked_taginfo G__ROBASTDictLN_vectorlETGeoVolumeAssemblycLcLThreadData_tmUcOallocatorlETGeoVolumeAssemblycLcLThreadData_tmUgRsPgR;
extern G__linked_taginfo G__ROBASTDictLN_reverse_iteratorlEvectorlETGeoVolumeAssemblycLcLThreadData_tmUcOallocatorlETGeoVolumeAssemblycLcLThreadData_tmUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__ROBASTDictLN_ABorderSurfaceCondition;
extern G__linked_taginfo G__ROBASTDictLN_ARefractiveIndex;
extern G__linked_taginfo G__ROBASTDictLN_ACauchyFormula;
extern G__linked_taginfo G__ROBASTDictLN_AOpticalComponent;
extern G__linked_taginfo G__ROBASTDictLN_AFocalSurface;
extern G__linked_taginfo G__ROBASTDictLN_AGeoAsphericDisk;
extern G__linked_taginfo G__ROBASTDictLN_TGeoPcon;
extern G__linked_taginfo G__ROBASTDictLN_TVector2;
extern G__linked_taginfo G__ROBASTDictLN_AGeoBezierPcon;
extern G__linked_taginfo G__ROBASTDictLN_TGeoPgon;
extern G__linked_taginfo G__ROBASTDictLN_AGeoBezierPgon;
extern G__linked_taginfo G__ROBASTDictLN_AGeoWinstonCone2D;
extern G__linked_taginfo G__ROBASTDictLN_AGeoWinstonConePoly;
extern G__linked_taginfo G__ROBASTDictLN_ASchottFormula;
extern G__linked_taginfo G__ROBASTDictLN_ASellmeierFormula;
extern G__linked_taginfo G__ROBASTDictLN_AGlassCatalog;
extern G__linked_taginfo G__ROBASTDictLN_AGlassCatalogcLcLdA;
extern G__linked_taginfo G__ROBASTDictLN_ALens;
extern G__linked_taginfo G__ROBASTDictLN_TVectorTlEfloatgR;
extern G__linked_taginfo G__ROBASTDictLN_TVectorTlEdoublegR;
extern G__linked_taginfo G__ROBASTDictLN_TGraph;
extern G__linked_taginfo G__ROBASTDictLN_TGraph2D;
extern G__linked_taginfo G__ROBASTDictLN_AMirror;
extern G__linked_taginfo G__ROBASTDictLN_AObscuration;
extern G__linked_taginfo G__ROBASTDictLN_TGeoTranslation;
extern G__linked_taginfo G__ROBASTDictLN_TGeoRotation;
extern G__linked_taginfo G__ROBASTDictLN_TVirtualGeoTrack;
extern G__linked_taginfo G__ROBASTDictLN_maplElongcOTGeoNavigatorArraymUcOlesslElonggRcOallocatorlEpairlEconstsPlongcOTGeoNavigatorArraymUgRsPgRsPgR;
extern G__linked_taginfo G__ROBASTDictLN_TPolyLine3D;
extern G__linked_taginfo G__ROBASTDictLN_TGeoTrack;
extern G__linked_taginfo G__ROBASTDictLN_TMatrixTBaselEfloatgR;
extern G__linked_taginfo G__ROBASTDictLN_TMatrixTBaselEdoublegR;
extern G__linked_taginfo G__ROBASTDictLN_TElementActionTlEfloatgR;
extern G__linked_taginfo G__ROBASTDictLN_TElementPosActionTlEfloatgR;
extern G__linked_taginfo G__ROBASTDictLN_TMatrixTlEfloatgR;
extern G__linked_taginfo G__ROBASTDictLN_TMatrixTRow_constlEfloatgR;
extern G__linked_taginfo G__ROBASTDictLN_TMatrixTRowlEfloatgR;
extern G__linked_taginfo G__ROBASTDictLN_TMatrixTDiag_constlEfloatgR;
extern G__linked_taginfo G__ROBASTDictLN_TMatrixTColumn_constlEfloatgR;
extern G__linked_taginfo G__ROBASTDictLN_TMatrixTFlat_constlEfloatgR;
extern G__linked_taginfo G__ROBASTDictLN_TMatrixTSub_constlEfloatgR;
extern G__linked_taginfo G__ROBASTDictLN_TMatrixTSparseRow_constlEfloatgR;
extern G__linked_taginfo G__ROBASTDictLN_TMatrixTSparseDiag_constlEfloatgR;
extern G__linked_taginfo G__ROBASTDictLN_TMatrixTColumnlEfloatgR;
extern G__linked_taginfo G__ROBASTDictLN_TMatrixTDiaglEfloatgR;
extern G__linked_taginfo G__ROBASTDictLN_TMatrixTFlatlEfloatgR;
extern G__linked_taginfo G__ROBASTDictLN_TMatrixTSublEfloatgR;
extern G__linked_taginfo G__ROBASTDictLN_TMatrixTSparseRowlEfloatgR;
extern G__linked_taginfo G__ROBASTDictLN_TMatrixTSparseDiaglEfloatgR;
extern G__linked_taginfo G__ROBASTDictLN_TVector3;
extern G__linked_taginfo G__ROBASTDictLN_ARay;
extern G__linked_taginfo G__ROBASTDictLN_ARaycLcLdA;
extern G__linked_taginfo G__ROBASTDictLN_ARayArray;
extern G__linked_taginfo G__ROBASTDictLN_AOpticsManager;
extern G__linked_taginfo G__ROBASTDictLN_AOpticsManagercLcLdA;
extern G__linked_taginfo G__ROBASTDictLN_ARayShooter;

/* STUB derived class for protected member access */
