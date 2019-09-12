
/**********************************************************************
 * $Id: ffteo_mif.h,v 1.0.0.2 2015/12/12 19:57:28 Self Exp $
 *
 * Name:     miFiles.h
 * Project:  Fixosoft SDK::MapInfo TAB Read/Write library
 * Language: C++
 * Purpose:  Header file containing public definitions for the library.
 * Author:   self, home153@mail.ru
 *
 *    В общем наш ответ OpenGIS и всякому овну типа mitab....
 *
 *
 * Copyright (c) 2015-17
 **********************************************************************/



#ifndef  MIFILES_H
#define MIFILES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fteo_base.h"
#include "cpl_conv.h"  // -  <<< #include "cpl_conv.h"



#if defined(_WIN32) && !defined(unix) && !defined(WIN32CE)
#  include <mbctype.h>  /* Multibyte chars stuff  а также _getbcmp*/
#endif



namespace fteo
{
  namespace api
{




typedef int  GBool;
/*---------------------------------------------------------------------
 * Access mode: Read or Write
 *--------------------------------------------------------------------*/
typedef enum
{   TABRead,
    TABWrite,
    TABReadWrite  /* ReadWrite not implemented yet */
} TABAccess;

/*---------------------------------------------------------------------
 * TABFieldType - Native M a p In fo attribute field types
 *--------------------------------------------------------------------*/
typedef enum
{
    TABFUnknown = 0,
    TABFChar,
    TABFInteger,
    TABFSmallInt,
    TABFDecimal,
    TABFFloat,
    TABFDate,
    TABFLogical,
    TABFTime,
    TABFDateTime
} TABFieldType;

/* Define some error codes specific to this lib.
 *--------------------------------------------------------------------*/
#define TAB_WarningFeatureTypeNotSupported     501
#define TAB_WarningInvalidFieldName            502
#define TAB_WarningBoundsOverflow              503


/*---------------------------------------------------------------------
 * Codes for the known MapInfo Geometry types
 *--------------------------------------------------------------------*/
#define TAB_GEOM_NONE           0
#define TAB_GEOM_SYMBOL_C       0x01
#define TAB_GEOM_SYMBOL         0x02
#define TAB_GEOM_LINE_C         0x04
#define TAB_GEOM_LINE           0x05
#define TAB_GEOM_PLINE_C        0x07
#define TAB_GEOM_PLINE          0x08
#define TAB_GEOM_ARC_C          0x0a
#define TAB_GEOM_ARC            0x0b
#define TAB_GEOM_REGION_C       0x0d
#define TAB_GEOM_REGION         0x0e
#define TAB_GEOM_TEXT_C         0x10
#define TAB_GEOM_TEXT           0x11
#define TAB_GEOM_RECT_C         0x13
#define TAB_GEOM_RECT           0x14
#define TAB_GEOM_ROUNDRECT_C    0x16
#define TAB_GEOM_ROUNDRECT      0x17
#define TAB_GEOM_ELLIPSE_C      0x19
#define TAB_GEOM_ELLIPSE        0x1a
#define TAB_GEOM_MULTIPLINE_C   0x25
#define TAB_GEOM_MULTIPLINE     0x26
#define TAB_GEOM_FONTSYMBOL_C   0x28 
#define TAB_GEOM_FONTSYMBOL     0x29
#define TAB_GEOM_CUSTOMSYMBOL_C 0x2b
#define TAB_GEOM_CUSTOMSYMBOL   0x2c
/* Version 450 object types: */
#define TAB_GEOM_V450_REGION_C  0x2e
#define TAB_GEOM_V450_REGION    0x2f
#define TAB_GEOM_V450_MULTIPLINE_C 0x31
#define TAB_GEOM_V450_MULTIPLINE   0x32
/* Version 650 object types: */
#define TAB_GEOM_MULTIPOINT_C   0x34
#define TAB_GEOM_MULTIPOINT     0x35
#define TAB_GEOM_COLLECTION_C   0x37
#define TAB_GEOM_COLLECTION     0x38
/* Version 800 object types: */
#define TAB_GEOM_UNKNOWN1_C     0x3a    // ???
#define TAB_GEOM_UNKNOWN1       0x3b    // ???
#define TAB_GEOM_V800_REGION_C  0x3d
#define TAB_GEOM_V800_REGION    0x3e
#define TAB_GEOM_V800_MULTIPLINE_C 0x40
#define TAB_GEOM_V800_MULTIPLINE   0x41
#define TAB_GEOM_V800_MULTIPOINT_C 0x43
#define TAB_GEOM_V800_MULTIPOINT   0x44
#define TAB_GEOM_V800_COLLECTION_C 0x46
#define TAB_GEOM_V800_COLLECTION   0x47

#define TAB_GEOM_GET_VERSION(nGeomType)                     \
    (((nGeomType) < TAB_GEOM_V450_REGION_C)  ? 300:         \
     ((nGeomType) < TAB_GEOM_MULTIPOINT_C)   ? 450:         \
     ((nGeomType) < TAB_GEOM_UNKNOWN1_C)     ? 650: 800 )


/*---------------------------------------------------------------------
 *							 TmyFile	
 *                      Базовый class Файл
 *
 *     Управляет handlом на последнюю строку. Ну и функции чтения+.
 *
 *--------------------------------------------------------------------*/

class TMyFile
	{
	protected:
      FILE *m_fp; //Указатель файла. Основа жизни Уэфа///
	public:
	  TMyFile();
    char        *m_pszFname;
	char		*pszFeatureClassName;
	TABAccess   m_eAccessMode;
	GBool       m_bEof;
	       // Set limit for the length of a line
#define MIDMAXCHAR 10000
     char m_szLastRead[MIDMAXCHAR];
     char m_szSavedLine[MIDMAXCHAR];
	 int         Open(const char *pszFname, const char *pszAccess);
     int         Close();
     const char *GetLine();
	 const char *GetLastLine();
     void        SaveLine(const char *pszLine);
     void		 SetEof_(GBool bEof);
     GBool		 GetEof_();
     int         Rewind();

	};

/*---------------------------------------------------------------------
 *
 *                      Базовый Map-In-fo class 
 *
 * 
 *
 *--------------------------------------------------------------------*/
class TMifBase  : public TMyFile
	{
	 public:
		 TMifBase();

     GBool IsValidFeature(const char *pszString);
//  Translation information
     void SetTranslation(double, double, double, double);
     double GetXTrans(double);
     double GetYTrans(double);
     double GetXMultiplier(){return m_dfXMultiplier;}
     const char *GetDelimiter(){return m_pszDelimiter;}
     void SetDelimiter(const char *pszDelimiter){m_pszDelimiter=pszDelimiter;}
     private:
       const char *m_pszDelimiter;
       double      m_dfXMultiplier;
       double      m_dfYMultiplier;
       double      m_dfXDisplacement;
       double      m_dfYDisplacement;
       
	};

/*---------------------------------------------------------------------
 *               class TMapInfoPolygon - он же Region
 *                  Наша обертка для TabREGION
 * Feature class to handle the MapInfo region types:
 *
 *     TAB_GEOM_REGION_C         0x0d
 *     TAB_GEOM_REGION           0x0e
 *     TAB_GEOM_V450_REGION_C    0x2e
 *     TAB_GEOM_V450_REGION      0x2f
 *
 * Feature geometry will be returned as OGRPolygon (with a single ring)
 * or OGRMultiPolygon (for multiple rings).
 *
 * REGIONs with multiple rings are returned as OGRMultiPolygon instead of
 * as OGRPolygons since OGRPolygons require that the first ring be the
 * outer ring, and the other all be inner rings, but this is not guaranteed
 * inside MapInfo files.  However, when writing features, OGRPolygons with
 * multiple rings will be accepted without problem.
 *--------------------------------------------------------------------*/
class TMapInfoRegion : public TMyContour
  {

    GBool       m_bSmooth;
  private:
    long        nFID;
    GBool       m_bDeletedFlag;
    int         m_nMapInfoType;
    GBool       m_bCenterIsSet;
    double      m_dCenterX, m_dCenterY;

    double      m_dXMin;
    double      m_dYMin;
    double      m_dXMax;
    double      m_dYMax;
//    OGRErr      SetGeometryDirectly( OGRGeometry * poGeomIn );
public:
  //TMapInfoRegion(OGRFeatureDefn *poDefnIn);
  TMapInfoRegion();
    void        SetMBR(double dXMin, double dYMin,double dXMax, double dYMax);
    int         ReadGeometryFromMIFFile(TMifBase *fp);
    int         ReadFteoGeometryFromMIFFile(TMifBase *fp);
    void        SetCenter(double dX, double dY);
    int		    SetFID( long nFID );
  };



/*---------------------------------------------------------------------
 *                      class IMapInfoFile
 *
 * Virtual base class for the TABFile and MIFFile classes.
 *
 * This is the definition of the public interface methods that should
 * be available for any type of MapInfo dataset.
 *--------------------------------------------------------------------*/

class IMapInfoFile_ //: public OGRLayer
  { 
    private:
    protected:
      int              m_nCurFeatureId;
      TMapInfoRegion  *m_poCurFeature;

  public:
    IMapInfoFile_();
    virtual ~IMapInfoFile_();
      //OGRFeature      *GetNextFeature();
    /////>>>!!!  virtual OGRFeature *GetNextFeature();
      //virtual int    GetNextFeatureId(int nPrevId) = 0;

  };


/*---------------------------------------------------------------------
 *                      class MIDDATAFile
 *
 * Class to handle a file pointer with a copy of the latest readed line
 *
 *--------------------------------------------------------------------*/

class MIDDATAFile
{
   public:
      MIDDATAFile();
     ~MIDDATAFile();

     int         Open(const char *pszFname, const char *pszAccess);
     int         Close();

     const char *GetLine();
     const char *GetLastLine();
     int Rewind();
     void SaveLine(const char *pszLine);
     const char *GetSavedLine();
     void WriteLine(const char*, ...);
     GBool IsValidFeature(const char *pszString);

//  Translation information
     void SetTranslation(double, double, double, double);
     double GetXTrans(double);
     double GetYTrans(double);
     double GetXMultiplier(){return m_dfXMultiplier;}
     const char *GetDelimiter(){return m_pszDelimiter;}
     void SetDelimiter(const char *pszDelimiter){m_pszDelimiter=pszDelimiter;}

     void SetEof(GBool bEof);
     GBool GetEof();

     private:
       FILE *m_fp;
       const char *m_pszDelimiter;

       // Set limit for the length of a line
#define MIDMAXCHAR 10000
       char m_szLastRead[MIDMAXCHAR];
       char m_szSavedLine[MIDMAXCHAR];

       char        *m_pszFname;
       TABAccess   m_eAccessMode;
       double      m_dfXMultiplier;
       double      m_dfYMultiplier;
       double      m_dfXDisplacement;
       double      m_dfYDisplacement;
       GBool       m_bEof;
};


	  
	  
  

/*=====================================================================
 *                      class MIFFile
 *====================================================================*/
class TMifFile //: public IMapInfoFile_
	{
	public:
	int         m_nPoints;
    int         m_nRegions;
    int         m_nLines;
    int         m_nTexts;
    TMifFile();
    int Open(const char *pszFname);
    int Close();
    int         m_nPreloadedId;  // preloaded mif line is for this feature id
    TMifBase *m_poMIDFile;   // Mid file
	//MIDDATAFile *m_poMIDFile;   // Mid file
    TMifBase    *m_poMIFFile;   // Mif File

	private:
	char        *m_pszFname;
	char        *pszFeatureClassName;
	//OGRFeatureDefn *m_poDefn;
	TABAccess   m_eAccessMode;

    int         m_nCurFeatureId;
    TMapInfoRegion  *m_poCurFeature; //for OGR
    TEntitySpatial  *m_poCurFeatureF; //for fteo

    int         m_nFeatureCount;
    int         m_nWriteFeatureId;
    int         m_nAttribut;

    int           m_nVersion;   /* Dataset version: 300, 450, 600, 900, etc. */
    char         *m_pszDelimiter;
    char         *m_pszUnique;
    char         *m_pszIndex;
    char         *m_pszCoordSys;

    TABFieldType *m_paeFieldType;
    GBool        *m_pabFieldIndexed;
    GBool        *m_pabFieldUnique;
    GBool         m_bBoundsSet;
    char         *m_pszCharset;

    GBool         m_bPreParsed;
    GBool         m_bHeaderWrote;


    double       m_dfXMultiplier;
    double       m_dfYMultiplier;
    double       m_dfXDisplacement;
    double       m_dfYDisplacement;

    /* these are the projection bounds, possibly much broader than extents */
    double      m_dXMin;
    double      m_dYMin;
    double      m_dXMax;
    double      m_dYMax;
  /* extents, as cached by MIFFile::PreParseFile() */
    int         m_bExtentsSet;
	Envelope    m_sExtents; // replace OGREnvelope 
	public:
	/*int       AddFields(const char *pszLine);
	int         AddFieldNative_(const char *pszName, 
		                        fteo::api::TABFieldType eMapInfoType,
								int nWidth = 0, 
								int nPrecision=0,
								fteo::api::GBool bIndexed= false, 
								fteo::api::GBool bUnique= false);
								*/
    TMapInfoRegion *GetFeatureRef(int nFeatureId);
    int         GetFeatureCountByType(int &numPoints, int &numLines,
                                      int &numRegions, int &numTexts,
                                      GBool bForce = TRUE);
    int         GetNextFeatureId(int nPrevId);
    int         GotoFeature(int nFeatureId);
    GBool       NextFeature();
    void        PreParseFile();
    int         ParseMIFHeader();
    void        ResetReading();
    void        UpdateExtents(double dfX,double dfY);
	};
	  
	  
  
	  
	 }//namespace api
	}//namespace fteo
#endif
