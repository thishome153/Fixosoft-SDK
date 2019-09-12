
#include "stdafx.h"
#include "fteo_mif.h"
#include "fteo_cpl.h"
//#include "cpl_string.h"  // -  <<< #include "cpl_conv.h"
#include <stdarg.h>

namespace fteo
{
  namespace api
{
/************************************************************************/
/*                   Routines from mitab, OGR, CPL                      */

/**********************************************************************
 *                       TABCleanFieldName()
 *
 * Return a copy of pszSrcName that contains only valid characters for a
 * TAB field name.  All invalid characters are replaced by '_'.
 *
 * The returned string should be freed by the caller.
 **********************************************************************/
char *TABCleanFieldName(const char *pszSrcName)
{
    char *pszNewName;
    int numInvalidChars = 0;

    pszNewName = fteo::cpl::CPLStrdup(pszSrcName);

    if (strlen(pszNewName) > 31)
    {
        pszNewName[31] = '\0';
		fteo::cpl::CPLError(fteo::cpl::CE_Warning, TAB_WarningInvalidFieldName,
                 "Field name '%s' is longer than the max of 31 characters. "
                 "'%s' will be used instead.", pszSrcName, pszNewName);
    }

#if defined(_WIN32) && !defined(unix) && !defined(WIN32CE)
    /*-----------------------------------------------------------------
     * On Windows, check if we're using a double-byte codepage, and
     * if so then just keep the field name as is... 
     *----------------------------------------------------------------*/
    if (_getmbcp() != 0)
        return pszNewName;
#endif

    /*-----------------------------------------------------------------
     * According to the MapInfo User's Guide (p. 240, v5.5)
     * New Table Command:
     *  Name:
     * Displays the field name in the name box. You can also enter new field
     * names here. Defaults are Field1, Field2, etc. A field name can contain
     * up to 31 alphanumeric characters. Use letters, numbers, and the 
     * underscore. Do not use spaces; instead, use the underscore character
     * (_) to separate words in a field name. Use upper and lower case for
     * legibility, but MapInfo is not case-sensitive.
     *
     * It was also verified that extended chars with accents are also 
     * accepted.
     *----------------------------------------------------------------*/
    for(int i=0; pszSrcName && pszSrcName[i] != '\0'; i++)
    {
        if ( !( pszSrcName[i] == '_' ||
                (pszSrcName[i]>='0' && pszSrcName[i]<='9') || 
                (pszSrcName[i]>='a' && pszSrcName[i]<='z') || 
                (pszSrcName[i]>='A' && pszSrcName[i]<='Z') ||
                (GByte)pszSrcName[i]>=192 ) )
        {
            pszNewName[i] = '_';
            numInvalidChars++;
        }
    }

    if (numInvalidChars > 0)
    {
		fteo::cpl::CPLError(fteo::cpl::CE_Warning, TAB_WarningInvalidFieldName,
                 "Field name '%s' contains invalid characters. "
                 "'%s' will be used instead.", pszSrcName, pszNewName);
    }

    return pszNewName;
}




/**********************************************************************
 *                       GetBasename()
 *
 * Extract the basename part of a complete file path.
 *
 * Returns a newly allocated string without the leading path (dirs) and
 * the extenstion.  The returned string should be freed using CPLFree().
 **********************************************************************/
char *GetBasename(const char *pszFname)
{
    const char *pszTmp = NULL;

    /*-----------------------------------------------------------------
     * Skip leading path or use whole name if no path dividers are
     * encountered.
     *----------------------------------------------------------------*/
    pszTmp = pszFname + strlen(pszFname) - 1;
    while ( pszTmp != pszFname 
            && *pszTmp != '/' && *pszTmp != '\\' ) 
        pszTmp--;

    if( pszTmp != pszFname )
        pszTmp++;

    /*-----------------------------------------------------------------
     * Now allocate our own copy and remove extension
     *----------------------------------------------------------------*/
	char *pszBasename = fteo::cpl::CPLStrdup(pszTmp);
    int i;
    for(i=0; pszBasename[i] != '\0'; i++)
    {
        if (pszBasename[i] == '.')
        {
            pszBasename[i] = '\0';
            break;
        }
    }

    return pszBasename;
}











/************************************************************************/
/*                             Classes implementation:                  */
/************************************************************************/

/**********************************************************************/

TMyFile::TMyFile() {
	this->m_fp = NULL;
    m_szLastRead[0] = '\0';
    m_szSavedLine[0] = '\0';

	};


const char *TMyFile::GetLine()
{
    const char *pszLine;
    
    if (m_eAccessMode == TABRead)
    {
        
        pszLine = fteo::cpl::CPLReadLine(m_fp);

        SetEof_(fteo::cpl::VSIFEof(m_fp));

        if (pszLine == NULL)
        {
            m_szLastRead[0] = '\0';
        }
        else
        {
            strncpy(m_szLastRead,pszLine,MIDMAXCHAR);
        }
        //if (pszLine)
        //  printf("%s\n",pszLine);
        return pszLine;
    }
    else
		// ASSERT !!!!! TODO ????	fteo::cpl::CPLAssert(FALSE);
    
    return NULL;
}

const char *TMyFile::GetLastLine()
{
    // Return NULL if EOF
    if(GetEof_())
    {
        return NULL;
    }
    else if (m_eAccessMode == TABRead)
    {
        // printf("%s\n",m_szLastRead);
        return m_szLastRead;
    }

    // We should never get here (Read/Write mode not implemented)
	// ASSERT !!!!! TODO ????fteo::cpl::CPLAssert(FALSE);
    return NULL;
}

void TMyFile::SaveLine(const char *pszLine)
{
    if (pszLine == NULL)
    {
        m_szSavedLine[0] = '\0';
    }
    else
    {
        strncpy(m_szSavedLine,pszLine,MIDMAXCHAR);
    }
}

void TMyFile::SetEof_(GBool bEof) {
	 this->m_bEof = bEof;
	};

GBool TMyFile::GetEof_()
{
    return m_bEof;
}

int  TMyFile::Rewind()
{
    if (m_fp == NULL || m_eAccessMode == TABWrite) 
    // if (m_fp == NULL ) 
        return -1;

    else
    {
		fteo::cpl::VSIRewind(m_fp);
        SetEof_(fteo::cpl::VSIFEof(m_fp));
    }
    return 0;
}


int  TMyFile::Open(const char *pszFname, const char *pszAccess) {
   if (m_fp)    {        return -1;    }
/*-----------------------------------------------------------------
     * Validate access mode and make sure we use Text access.
     *----------------------------------------------------------------*/
    if (EQUALN(pszAccess, "r", 1))
    {
        m_eAccessMode = TABRead;
        pszAccess = "rt";
    }
    else if (EQUALN(pszAccess, "w", 1))
    {
        m_eAccessMode = TABWrite;
        pszAccess = "wt";
    }
    else
    {
        return -1;
    }
	 m_pszFname = fteo::cpl::CPLStrdup(pszFname);
	 pszFeatureClassName = GetBasename(m_pszFname);
     m_fp = fteo::cpl::VSIFOpen(m_pszFname, pszAccess); // указатель файла
	

 if (m_fp == NULL)
    {
	 fteo::cpl::CPLFree(m_pszFname);
        m_pszFname = NULL;
        return -1;
    }

      SetEof_(fteo::cpl::VSIFEof(m_fp));
    return 0;
	};
	
int  TMyFile::Close()
{
    if (m_fp == NULL)
        return 0;
   
    // Close file
	fteo::cpl::VSIFClose(m_fp);
    m_fp = NULL;

    // clear readline buffer.
	fteo::cpl::CPLReadLine( NULL );

	fteo::cpl::CPLFree(m_pszFname);
    m_pszFname = NULL;

    return 0;

}




/************************************************************************/
/*                             Classes definition (implemenattion: )    */
/************************************************************************/
TMifBase::TMifBase() {

    m_pszDelimiter = "\t"; // Encom 2003 (was NULL)
    
    m_dfXMultiplier = 1.0;
    m_dfYMultiplier = 1.0;
    m_dfXDisplacement = 0.0;
    m_dfYDisplacement = 0.0;
	};


GBool TMifBase::IsValidFeature(const char *pszString)
{
    char **papszToken ;

    papszToken = fteo::cpl::CSLTokenizeString(pszString);
    
    //   printf("%s\n",pszString);

    if ( fteo::cpl::CSLCount(papszToken) == 0)
    {
		fteo::cpl::CSLDestroy(papszToken);
        return FALSE;
    }

    if (EQUAL(papszToken[0],"NONE")      || EQUAL(papszToken[0],"POINT") ||
        EQUAL(papszToken[0],"LINE")      || EQUAL(papszToken[0],"PLINE") ||
        EQUAL(papszToken[0],"REGION")    || EQUAL(papszToken[0],"ARC") ||
        EQUAL(papszToken[0],"TEXT")      || EQUAL(papszToken[0],"RECT") ||
        EQUAL(papszToken[0],"ROUNDRECT") || EQUAL(papszToken[0],"ELLIPSE") ||
        EQUAL(papszToken[0],"MULTIPOINT")|| EQUAL(papszToken[0],"COLLECTION") )
    {
		fteo::cpl::CSLDestroy(papszToken);
        return TRUE;
    }

	fteo::cpl::CSLDestroy(papszToken);
    return FALSE;
}




















double TMifBase::GetXTrans(double dfX)
{
    return (dfX * m_dfXMultiplier) + m_dfXDisplacement;
}
double TMifBase::GetYTrans(double dfY)
{
    return (dfY * m_dfYMultiplier) + m_dfYDisplacement;
}


int TMapInfoRegion::SetFID(long nFID) 
  {
     this->nFID = nFID;
    return 0;
  };

/**********************************************************************
 *                    TABRegion::SetMBR()
 *
 * Set the values for the MBR corners for this feature.
 **********************************************************************/
void TMapInfoRegion::SetMBR(double dXMin, double dYMin, 
                        double dXMax, double dYMax)
{
    m_dXMin = MIN(dXMin, dXMax);
    m_dYMin = MIN(dYMin, dYMax);
    m_dXMax = MAX(dXMin, dXMax);
    m_dYMax = MAX(dYMin, dYMax);
}


//TMapInfoRegion::TMapInfoRegion(OGRFeatureDefn *poDefnIn):OGRFeature(poDefnIn)
/*
TMapInfoRegion::TMapInfoRegion(OGRFeatureDefn *poDefnIn)
  {
     m_nMapInfoType = TAB_GEOM_NONE;
     m_bDeletedFlag = FALSE;
     SetMBR(0.0, 0.0, 0.0, 0.0);
  };
  */
TMapInfoRegion::TMapInfoRegion()
  {
     m_nMapInfoType = TAB_GEOM_NONE;
     m_bDeletedFlag = FALSE;
     SetMBR(0.0, 0.0, 0.0, 0.0);
  };
/************************************************************************/
/*                        SetGeometryDirectly()                         */
/************************************************************************/

/**
 * Set feature geometry.
 *
 * This method updates the features geometry, and operate exactly as
 * SetGeometry(), except that this method assumes ownership of the
 * passed geometry.
 *
 * This method is the same as the C function OGR_F_SetGeometryDirectly().
 *
 * @param poGeomIn new geometry to apply to feature. Passing NULL value here
 * is correct and it will result in deallocation of currently assigned geometry
 * without assigning new one.
 *
 * @return OGRERR_NONE if successful, or OGR_UNSUPPORTED_GEOMETRY_TYPE if
 * the geometry type is illegal for the OGRFeatureDefn (checking not yet
 * implemented). 
 */ 

/*
OGRErr TMapInfoRegion::SetGeometryDirectly( OGRGeometry * poGeomIn )

{
   // delete poGeometry;
   // this->poGeometry  = poGeomIn;

    // I should be verifying that the geometry matches the defn's type.
    
    return OGRERR_NONE;
}
*/
/**********************************************************************
 *                   TABRegion::ReadGeometryFromMIFFile()
 *
 * Fill the geometry and representation (color, etc...) part of the
 * feature from the contents of the .MIF file
 *
 * Returns 0 on success, -1 on error, in which case CPLError() will have
 * been called.
 **********************************************************************/
int TMapInfoRegion::ReadGeometryFromMIFFile(TMifBase *fp)
  {
  /*
    double               dX, dY;
    OGRLinearRing       *poRing;
    OGRGeometry         *poGeometry = NULL;
    OGRPolygon          *poPolygon = NULL;
    OGRMultiPolygon     *poMultiPolygon = NULL;
    int                  i,iSection, numLineSections=0;
    char               **papszToken;
    const char          *pszLine;
    OGREnvelope          sEnvelope;

    m_bSmooth = FALSE;
    /*=============================================================
     * REGION (Similar to PLINE MULTIPLE)
     *============================================================
    papszToken = CSLTokenizeString2(fp->GetLastLine(), 
                                    " \t", CSLT_HONOURSTRINGS);
    
    if (fteo::cpl::CSLCount(papszToken) ==2)
      numLineSections = atoi(papszToken[1]);
    CSLDestroy(papszToken);
    papszToken = NULL;

    /*-------------------------------------------------------------
     * For 1-ring regions, we return an OGRPolygon with one single
     * OGRLinearRing geometry. 
     *
     * REGIONs with multiple rings are returned as OGRMultiPolygon
     * instead of as OGRPolygons since OGRPolygons require that the
     * first ring be the outer ring, and the other all be inner 
     * rings, but this is not guaranteed inside MapInfo files.  
     *------------------------------------------------------------
    //Определим количество контуров
	
    if (numLineSections > 1)
        poGeometry = poMultiPolygon = new OGRMultiPolygon;
    else
        poGeometry = NULL;  // Will be set later

    for(iSection=0; iSection<numLineSections; iSection++)
    {
        int     numSectionVertices = 0;

        poPolygon = new OGRPolygon();
    //Определим количество вершин 
        if ((pszLine = fp->GetLine()) != NULL)
        {
            numSectionVertices = atoi(pszLine);
        }

        poRing = new OGRLinearRing();
        poRing->setNumPoints(numSectionVertices);


        for(i=0; i<numSectionVertices; i++)
        {
            pszLine = fp->GetLine();
            if (pszLine)
            {
                papszToken = CSLTokenizeStringComplex(pszLine," ,\t",
                                                      true,false);
                if (fteo::cpl::CSLCount(papszToken) == 2)
                {              
                    dX = fp->GetXTrans(atof(papszToken[0]));
                    dY = fp->GetYTrans(atof(papszToken[1]));
                    poRing->setPoint(i, dX, dY);
                }
                CSLDestroy(papszToken);
                papszToken = NULL;
            }   
        }
        poPolygon->addRingDirectly(poRing); //Вот примерчик: вставил и освободил! 
        poRing = NULL;                      //Запиши на будуще е.

        if (numLineSections > 1)
            poMultiPolygon->addGeometryDirectly(poPolygon);
        else
            poGeometry = poPolygon;

        poPolygon = NULL;
    }
  
  // Что он делает, эта SetGeometryDirectly?
    SetGeometryDirectly(poGeometry);
    poGeometry->getEnvelope(&sEnvelope);
    
    SetMBR(sEnvelope.MinX, sEnvelope.MinY, sEnvelope.MaxX, sEnvelope.MaxY);

   while (((pszLine = fp->GetLine()) != NULL) && 
           fp->IsValidFeature(pszLine) == false)

    {
        papszToken = CSLTokenizeStringComplex(pszLine,"() ,",
                                              true,false);
        
        if (fteo::cpl::CSLCount(papszToken) > 1)
        {
            //Пенисы нам похуй...пока. Прикроем:
            if (EQUALN(papszToken[0],"PEN",3))
            {
                /*
                if (fteo::cpl::CSLCount(papszToken) == 4)
                {           
                    SetPenWidthMIF(atoi(papszToken[1]));
                    SetPenPattern(atoi(papszToken[2]));
                    SetPenColor(atoi(papszToken[3]));
                }
                */
           // ;
      /*
            }
            //Кисточки нам похуй...пока. Прикроем:
            else if (EQUALN(papszToken[0],"BRUSH", 5))
            {
                if (fteo::cpl::CSLCount(papszToken) >= 3)
                {
                   // SetBrushFGColor(atoi(papszToken[2]));
                   // SetBrushPattern(atoi(papszToken[1]));
                    
                   // if (fteo::cpl::CSLCount(papszToken) == 4)
                      // SetBrushBGColor(atoi(papszToken[3]));
                   // else
                     // SetBrushTransparent(TRUE);
                 ;
                }
                
            }
            else if (EQUALN(papszToken[0],"CENTER",6))
            {
                if (fteo::cpl::CSLCount(papszToken) == 3)
                {
                    SetCenter(fp->GetXTrans(atof(papszToken[1])),
                              fp->GetYTrans(atof(papszToken[2])) );
                }
            }
        }
        CSLDestroy(papszToken);
        papszToken = NULL;
    }
    
    
    return 0; 
	*/
		return -1;
     }

     int TMapInfoRegion::ReadFteoGeometryFromMIFFile(fteo::api::TMifBase *fp)
  {
    double               dX, dY;
    //OGRLinearRing       *poRing;
    //OGRGeometry         *poGeometry = NULL;
    //OGRPolygon          *poPolygon = NULL;
    //OGRMultiPolygon     *poMultiPolygon = NULL;
    int                  i,iSection, numLineSections=0;
    char               **papszToken;
    const char          *pszLine;
    //OGREnvelope          sEnvelope;

    m_bSmooth = FALSE;
    /*=============================================================
     * REGION (Similar to PLINE MULTIPLE)
     *============================================================*/
    papszToken = fteo::cpl::CSLTokenizeString2(fp->GetLastLine(),
                                    " \t", CSLT_HONOURSTRINGS);
    
    if (fteo::cpl::CSLCount(papszToken) ==2)
      numLineSections = atoi(papszToken[1]);
	fteo::cpl::CSLDestroy(papszToken);
    papszToken = NULL;

    /*-------------------------------------------------------------
     * For 1-ring regions, we return an OGRPolygon with one single
     * OGRLinearRing geometry. 
     *
     * REGIONs with multiple rings are returned as OGRMultiPolygon
     * instead of as OGRPolygons since OGRPolygons require that the
     * first ring be the outer ring, and the other all be inner 
     * rings, but this is not guaranteed inside MapInfo files.  
     *------------------------------------------------------------*/
    //Определим количество контуров
    
    
    for(iSection=0; iSection<numLineSections; iSection++)
    {
        int     numSectionVertices = 0;
        TSpatialElement* poRing = new TSpatialElement();
       
    //Определим количество вершин 
        if ((pszLine = fp->GetLine()) != NULL)
        {
            numSectionVertices = atoi(pszLine);
        }
      
    for(i=0; i<numSectionVertices; i++)
        {
            pszLine = fp->GetLine();
            if (pszLine)
            {
                papszToken = fteo::cpl::CSLTokenizeStringComplex(pszLine," ,\t",
                                                      true,false);
                if (fteo::cpl::CSLCount(papszToken) == 2)
                {  
                TMyPoint* p = new TMyPoint;
                p->NewOrd->y = fp->GetXTrans(atof(papszToken[0]));
                p->NewOrd->x = fp->GetYTrans(atof(papszToken[1]));
			    // itoa(i+1, p->Name, 10);//"Point";
                poRing->AppendPoint(p);
                }
				fteo::cpl::CSLDestroy(papszToken);
                papszToken = NULL;
            }   
        }

        if (iSection ==0) this->EntitySpatial->SpatialElement(0)->AppendPoints(poRing);

        else
			{
			poRing->LayerName = (char*) malloc (sizeof(char)*16);
			itoa(iSection,	poRing->LayerName, 10);
          this->EntitySpatial->AddElement(poRing); //нет defintion !!! для childs
			};
        //poRing = NULL;
        /*
        poPolygon->addRingDirectly(poRing); //Вот примерчик: вставил и освободил! На будущее засолить, как он там с указателями 
        poRing = NULL;                      //Запиши на будуще е.
*/
     /*   if (numLineSections > 1)
            poMultiPolygon->addGeometryDirectly(poPolygon);
        else
            poGeometry = poPolygon;

        poPolygon = NULL;*/
    }
  
  // Что он делает, эта SetGeometryDirectly?
    /*
    SetGeometryDirectly(poGeometry);
    poGeometry->getEnvelope(&sEnvelope);
    
    SetMBR(sEnvelope.MinX, sEnvelope.MinY, sEnvelope.MaxX, sEnvelope.MaxY);
*/
   while (((pszLine = fp->GetLine()) != NULL) && 
           fp->IsValidFeature(pszLine) == false)

    {
        papszToken = fteo::cpl::CSLTokenizeStringComplex(pszLine,"() ,",
                                              true,false);
        
        if (fteo::cpl::CSLCount(papszToken) > 1)
        {
            //Пенисы нам похуй...пока. Прикроем:
            if (EQUALN(papszToken[0],"PEN",3))
            {
                /*
                if (fteo::cpl::CSLCount(papszToken) == 4)
                {           
                    SetPenWidthMIF(atoi(papszToken[1]));
                    SetPenPattern(atoi(papszToken[2]));
                    SetPenColor(atoi(papszToken[3]));
                }
                */
            ;
            }
            //Кисточки нам похуй...пока. Прикроем:
            else if (EQUALN(papszToken[0],"BRUSH", 5))
            {
                if (fteo::cpl::CSLCount(papszToken) >= 3)
                {
                   // SetBrushFGColor(atoi(papszToken[2]));
                   // SetBrushPattern(atoi(papszToken[1]));
                    
                   // if (fteo::cpl::CSLCount(papszToken) == 4)
                      // SetBrushBGColor(atoi(papszToken[3]));
                   // else
                     // SetBrushTransparent(TRUE);
                 ;
                }
                
            }
            else if (EQUALN(papszToken[0],"CENTER",6))
            {
                if (fteo::cpl::CSLCount(papszToken) == 3)
                {
                    SetCenter(fp->GetXTrans(atof(papszToken[1])),
                              fp->GetYTrans(atof(papszToken[2])) );
                }
            }
        }
		fteo::cpl::CSLDestroy(papszToken);
        papszToken = NULL;
    }
    
    
    return 0; 
     }




/**********************************************************************
 *                   TABRegion::SetCenter()
 *
 * Set the X,Y coordinates to use as center/label point for the region.
 **********************************************************************/
     void TMapInfoRegion::SetCenter(double dX, double dY)
{
    m_dCenterX = dX;
    m_dCenterY = dY;
    m_bCenterIsSet = TRUE;
}


/**********************************************************************
 *                   MIFFile::MIFFile()
 *
 * Constructor.
 **********************************************************************/
TMifFile::TMifFile()
{
    m_pszFname = NULL;
    m_nVersion = 300;

    // Tab is default delimiter in MIF spec if not explicitly specified.  Use
    // that by default for read mode. In write mode, we will use "," as 
    // delimiter since it's more common than tab (we do this in Open())
    m_pszDelimiter = fteo::cpl::CPLStrdup("\t");

    m_pszUnique = NULL;
    m_pszIndex = NULL;
    m_pszCoordSys = NULL;

    m_paeFieldType = NULL;
    m_pabFieldIndexed = NULL;
    m_pabFieldUnique = NULL;

    m_dfXMultiplier = 1.0;
    m_dfYMultiplier = 1.0;
    m_dfXDisplacement = 0.0;
    m_dfYDisplacement = 0.0;

    m_poMIDFile = NULL;
    m_poMIFFile = NULL;
    m_nPreloadedId = 0;

    //m_poDefn = NULL;
    //m_poSpatialRef = NULL;

    m_nCurFeatureId = 0;
    m_nFeatureCount = 0;
    m_nWriteFeatureId = -1;
    m_poCurFeature = NULL;
   
    m_bPreParsed = FALSE;
    m_nAttribut = 0;
    m_bHeaderWrote = FALSE;
    m_nPoints = m_nLines = m_nRegions = m_nTexts = 0;

    m_bExtentsSet = FALSE;
}

/**********************************************************************
 *                   MIFFile::Open()
 *
 * Returns 0 on success, -1 on error.
 **********************************************************************/
int TMifFile::Open(const char *pszFname){
    int nFnameLen = 0;
   //распишем по дефолту
    m_eAccessMode = TABRead;
     //   pszAccess = "rt";

	 char *pszTmpFname = NULL;
	this->m_poMIFFile = new TMifBase();
	
	
	  if (m_poMIDFile)
    {
		  fteo::cpl::CPLError(fteo::cpl::CE_Failure, CPLE_FileIO,
                     "Open() failed: object already contains an open file");

        return -1;
    }


  
     /*-----------------------------------------------------------------
     * Проверим, что оба файла есть и они имеют  .MIF or .MID расширение... 
     *----------------------------------------------------------------*/
    m_pszFname = fteo::cpl::CPLStrdup(pszFname);
    nFnameLen = strlen(m_pszFname);
    
    if (nFnameLen > 4 && (strcmp(m_pszFname+nFnameLen-4, ".MID")==0 ||
                          strcmp(m_pszFname+nFnameLen-4, ".MIF")==0 ) )
        strcpy(m_pszFname+nFnameLen-4, ".MIF");
    else if (nFnameLen > 4 && (EQUAL(m_pszFname+nFnameLen-4, ".mid") ||
                               EQUAL(m_pszFname+nFnameLen-4, ".mif") ) )
        strcpy(m_pszFname+nFnameLen-4, ".mif");
    else
    {
    /*    if (!bTestOpenNoError)
            CPLError(CE_Failure, CPLE_FileIO,
                     "Open() failed for %s: invalid filename extension",
                     m_pszFname);
        else
            CPLErrorReset();
    */
		fteo::cpl::CPLFree(m_pszFname);
        return -1; // вываливаемся с ошибкой
    }

    pszTmpFname = fteo::cpl::CPLStrdup(m_pszFname);
	pszFeatureClassName = GetBasename(m_pszFname);
	
	

	  
     /*-----------------------------------------------------------------
     * Open .MIF file
     *----------------------------------------------------------------*/	
     //Теперь для файла вызываем его родный метод Open, который далее обычный системный
    // fopen юзает
    if (m_poMIFFile->Open(pszTmpFname, "r") != 0)
      {
        this->Close();
        return -1;
      };
      
      
    /*-----------------------------------------------------------------
     * Open .MID file 	//Откроем mid, если он есть	:
     *----------------------------------------------------------------*/
    if (nFnameLen > 4 && strcmp(pszTmpFname+nFnameLen-4, ".MIF")==0)
        strcpy(pszTmpFname+nFnameLen-4, ".MID");
     else 
        strcpy(pszTmpFname+nFnameLen-4, ".mid");
    
    /*  this->m_poMIDFile = new MIDDATAFile;
     
    if (m_poMIDFile->Open(pszTmpFname, "r") != 0)
      {
        this->Close();
        return -1;
      };
    */
    // Освобождаем временное имя
	fteo::cpl::CPLFree(pszTmpFname);
    pszTmpFname = NULL;      


      
     /*-----------------------------------------------------------------
     * Read MIF File Header
     *----------------------------------------------------------------*/
   int res =	ParseMIFHeader();
    m_poMIFFile->SetDelimiter(m_pszDelimiter);
    //m_poMIDFile->SetDelimiter(m_pszDelimiter);
   /*-------------------------------------------------------------
     * Set geometry type if the geometry objects are uniform.
     *------------------------------------------------------------*/
    int numPoints=0, numRegions=0, numTexts=0, numLines=0;

    if( GetFeatureCountByType( numPoints, numLines, numRegions, numTexts, 
                               true) == 0 )
                               //что же делать дальше?:
      {

      }

    return 0;
	};
	
	

/**********************************************************************
 *                   MIFFile::GetFeatureCountByType()
 *
 * Return number of features of each type.
 *
 * NOTE: The current implementation always returns -1 for MIF files
 *       since this would require scanning the whole file.
 *
 * When properly implemented, the bForce flag will force scanning the
 * whole file by default.
 *
 * Returns 0 on success, or silently returns -1 (with no error) if this
 * information is not available.
 **********************************************************************/

int TMifFile::GetFeatureCountByType(int &numPoints, int &numLines, int &numRegions, int &numTexts, fteo::api::GBool bForce) {
  if( m_bPreParsed || bForce )
    {
        PreParseFile();

        numPoints = m_nPoints;
        numLines = m_nLines;
        numRegions = m_nRegions;
        numTexts = m_nTexts;
        return 0;
    }
    else
    {
        numPoints = numLines = numRegions = numTexts = 0;
        return -1;
    }

  };


//все что закомментировал, нужно разобрать... позже
int TMifFile::Close()
{
    /* flush .mif header if not already written */
  //  if ( m_poDefn != NULL && m_bHeaderWrote == FALSE 
  //       && m_eAccessMode != TABRead )
  //  {
  //      WriteMIFHeader();     
  //  }

    if (m_poMIDFile)
    {
        m_poMIDFile->Close();
        delete m_poMIDFile;
        m_poMIDFile = NULL;
    }

    if (m_poMIFFile)
    {
        m_poMIFFile->Close();
        delete m_poMIFFile;
        m_poMIFFile = NULL;
    }

    //if (m_poCurFeature)
    //{
        // delete m_poCurFeature;
        //m_poCurFeature = NULL;
    //}

    /*-----------------------------------------------------------------
     * Note: we have to check the reference count before deleting 
     * m_poSpatialRef and m_poDefn
     *----------------------------------------------------------------*
    if (m_poDefn && m_poDefn->Dereference() == 0)
        delete m_poDefn;
    m_poDefn = NULL;
    
   */
    //if (m_poSpatialRef && m_poSpatialRef->Dereference() == 0)
     //   delete m_poSpatialRef;

    //m_poSpatialRef = NULL;

	fteo::cpl::CPLFree(m_pszCoordSys);
    m_pszCoordSys = NULL;

	fteo::cpl::CPLFree(m_pszDelimiter);
    m_pszDelimiter = NULL;

	fteo::cpl::CPLFree(m_pszFname);
    m_pszFname = NULL;

    m_nVersion = 0;

	fteo::cpl::CPLFree(m_pszCharset);
    m_pszCharset = NULL;

	fteo::cpl::CPLFree(m_pabFieldIndexed);
    m_pabFieldIndexed = NULL;
	fteo::cpl::CPLFree(m_pabFieldUnique);
    m_pabFieldUnique = NULL;

	fteo::cpl::CPLFree( m_pszIndex );
    m_pszIndex = NULL;

	fteo::cpl::CPLFree(m_paeFieldType);
    m_paeFieldType = NULL;

    //m_nCurFeatureId = 0;
    //m_nPreloadedId = 0;
    m_nFeatureCount =0;
   
    m_bBoundsSet = FALSE;

    return 0;
}
/************************************************************************/
/*                             AddFields()                              */
/************************************************************************/
/*
int  TMifFile::AddFields(const char *pszLine)
{
    char **papszToken;
    int nStatus = 0,numTok;

    CPLAssert(m_bHeaderWrote == FALSE);
    papszToken = CSLTokenizeStringComplex(pszLine," (,)\t",true,false); 
    numTok = fteo::cpl::CSLCount(papszToken);

    if (numTok >= 3 && EQUAL(papszToken[1], "char"))
    {
        /*-------------------------------------------------
         * CHAR type
         *------------------------------------------------
        nStatus = AddFieldNative_(papszToken[0], TABFChar, atoi(papszToken[2]));
    }
    else if (numTok >= 2 && EQUAL(papszToken[1], "integer"))
    {
        /*-------------------------------------------------
         * INTEGER type
         *------------------------------------------------
        nStatus = AddFieldNative_(papszToken[0], TABFInteger);
    }
    else if (numTok >= 2 && EQUAL(papszToken[1], "smallint"))
    {
        /*-------------------------------------------------
         * SMALLINT type
         *------------------------------------------------*
        nStatus = AddFieldNative_(papszToken[0], TABFSmallInt);
    }
    else if (numTok >= 4 && EQUAL(papszToken[1], "decimal"))
    {
        /*-------------------------------------------------
         * DECIMAL type
         *------------------------------------------------*
        nStatus = AddFieldNative_(papszToken[0], TABFDecimal,  atoi(papszToken[2]), atoi(papszToken[3]));
    }
    else if (numTok >= 2 && EQUAL(papszToken[1], "float"))
    {
        /*-------------------------------------------------
         * FLOAT type
         *------------------------------------------------*
        nStatus = AddFieldNative_(papszToken[0], TABFFloat);
    }
    else if (numTok >= 2 && EQUAL(papszToken[1], "date"))
    {
        /*-------------------------------------------------
         * DATE type (returned as a string: "DD/MM/YYYY" or "YYYYMMDD")
         *------------------------------------------------*
        nStatus = AddFieldNative_(papszToken[0], TABFDate);
    }
    else if (numTok >= 2 && EQUAL(papszToken[1], "time"))
    {
        /*-------------------------------------------------
         *  TIME type (v900, returned as a string: "HH:MM:SS" or "HHMMSSmmm")
         *------------------------------------------------*
        nStatus = AddFieldNative_(papszToken[0], TABFTime);
    }
    else if (numTok >= 2 && EQUAL(papszToken[1], "datetime"))
    {
        /*-------------------------------------------------
         * DATETIME type (v900, returned as a string: "DD/MM/YYYY HH:MM:SS",
         * "YYYY/MM/DD HH:MM:SS" or "YYYYMMDDHHMMSSmmm")
         *------------------------------------------------*
        nStatus = AddFieldNative_(papszToken[0], TABFDateTime);
    }
    else if (numTok >= 2 && EQUAL(papszToken[1], "logical"))
    {
        /*-------------------------------------------------
         * LOGICAL type (value "T" or "F")
         *------------------------------------------------*
        nStatus = AddFieldNative_(papszToken[0], TABFLogical);
    }
    else 
      nStatus = -1; // Unrecognized field type or line corrupt
    
    CSLDestroy(papszToken);
    papszToken = NULL;

    if (nStatus != 0)
    {
        CPLError(CE_Failure, CPLE_FileIO,
                 "Failed to parse field definition in file %s", m_pszFname);
        return -1;
    }
    
    return 0;
}
*/


/**********************************************************************
 *                   TMifFile::AddFieldNative()
 *
 * Create a new field using a native mapinfo data type... this is an 
 * alternative to defining fields through the OGR interface.
 * This function should be called after creating a new dataset, but before 
 * writing the first feature.
 *
 * This function will build/update the OGRFeatureDefn that will have to be
 * used when writing features to this dataset.
 *
 * A reference to the OGRFeatureDefn can be obtained using GetLayerDefn().
 *
 * Returns 0 on success, -1 on error.
 **********************************************************************
int  TMifFile::AddFieldNative_(const char *pszName, TABFieldType eMapInfoType,
                            int nWidth /*=0*, int nPrecision /*=0*,
                            GBool bIndexed /*=FALSE*, GBool bUnique/*=FALSE*)


	{
  //мои defaults, они в определения функции не прошли.
  //    int   nWidth  = 0; int nPrecision = 0; GBool bIndexed = FALSE;   GBool bUnique = FALSE;

    OGRFieldDefn *poFieldDefn;
    char *pszCleanName = NULL;
    int nStatus = 0;

    /*-----------------------------------------------------------------
     * Check that call happens at the right time in dataset's life.
     *----------------------------------------------------------------*
    if ( m_eAccessMode == TABWrite && m_bHeaderWrote )
    {
        CPLError(CE_Failure, CPLE_AssertionFailed,
                 "AddFieldNative() must be called after opening a new "
                 "dataset, but before writing the first feature to it.");
        return -1;
    }

    /*-----------------------------------------------------------------
     * Validate field width... must be <= 254
     *----------------------------------------------------------------*
    if (nWidth > 254)
    {
        CPLError(CE_Warning, CPLE_IllegalArg,
                 "Invalid size (%d) for field '%s'.  "
                 "Size must be 254 or less.", nWidth, pszName);
        nWidth = 254;
    }

    /*-----------------------------------------------------------------
     * Map fields with width=0 (variable length in OGR) to a valid default
     *----------------------------------------------------------------*
    if (eMapInfoType == TABFDecimal && nWidth == 0)
        nWidth=20;
    else if (nWidth == 0)
        nWidth=254; /* char fields */

    /*-----------------------------------------------------------------
     * Create new OGRFeatureDefn if not done yet...
     *----------------------------------------------------------------*
    if (m_poDefn == NULL)
    {
        char *pszFeatureClassName = TABGetBasename(m_pszFname);
        //m_poDefn = new OGRFeatureDefn(pszFeatureClassName);
        CPLFree(pszFeatureClassName);
        // Ref count defaults to 0... set it to 1
        m_poDefn->Reference();
    }

    /*-----------------------------------------------------------------
     * Make sure field name is valid... check for special chars, etc.
     * (pszCleanName will have to be freed.)
     *----------------------------------------------------------------*
    pszCleanName = TABCleanFieldName(pszName);

    /*-----------------------------------------------------------------
     * Map MapInfo native types to OGR types
     *----------------------------------------------------------------*
    poFieldDefn = NULL;

    switch(eMapInfoType)
    {
      case TABFChar:
        /*-------------------------------------------------
         * CHAR type
         *------------------------------------------------*
        poFieldDefn = new OGRFieldDefn(pszCleanName, OFTString);
        poFieldDefn->SetWidth(nWidth);
        break;
      case TABFInteger:
        /*-------------------------------------------------
         * INTEGER type
         *------------------------------------------------*
        poFieldDefn = new OGRFieldDefn(pszCleanName, OFTInteger);
        break;
      case TABFSmallInt:
        /*-------------------------------------------------
         * SMALLINT type
         *------------------------------------------------*
        poFieldDefn = new OGRFieldDefn(pszCleanName, OFTInteger);
        break;
      case TABFDecimal:
        /*-------------------------------------------------
         * DECIMAL type
         *------------------------------------------------*
        poFieldDefn = new OGRFieldDefn(pszCleanName, OFTReal);
        poFieldDefn->SetWidth(nWidth);
        poFieldDefn->SetPrecision(nPrecision);
        break;
      case TABFFloat:
        /*-------------------------------------------------
         * FLOAT type
         *------------------------------------------------*
        poFieldDefn = new OGRFieldDefn(pszCleanName, OFTReal);
        break;
      case TABFDate:
        /*-------------------------------------------------
         * DATE type (V450, returned as a string: "DD/MM/YYYY" or "YYYYMMDD")
         *------------------------------------------------*
        poFieldDefn = new OGRFieldDefn(pszCleanName, OFTString);
        poFieldDefn->SetWidth(10);
        m_nVersion = MAX(m_nVersion, 450);
        break;
      case TABFTime:
        /*-------------------------------------------------
         * TIME type (v900, returned as a string: "HH:MM:SS" or "HHMMSSmmm")
         *------------------------------------------------*
        //poFieldDefn = new OGRFieldDefn(pszCleanName, OFTString);
        poFieldDefn->SetWidth(9);
        m_nVersion = MAX(m_nVersion, 900);
        break;
      case TABFDateTime:
        /*-------------------------------------------------
         * DATETIME type (v900, returned as a string: "DD/MM/YYYY HH:MM:SS",
         * "YYYY/MM/DD HH:MM:SS" or "YYYYMMDDHHMMSSmmm")
         *------------------------------------------------*
        //poFieldDefn = new OGRFieldDefn(pszCleanName, OFTString);
        poFieldDefn->SetWidth(19);
        break;
        m_nVersion = MAX(m_nVersion, 900);
      case TABFLogical:
        /*-------------------------------------------------
         * LOGICAL type (value "T" or "F")
         *------------------------------------------------*
        //poFieldDefn = new OGRFieldDefn(pszCleanName, OFTString);
        poFieldDefn->SetWidth(1);
        break;
      default:
        CPLError(CE_Failure, CPLE_NotSupported,
                 "Unsupported type for field %s", pszName);
        return -1;
    }

    /*-----------------------------------------------------
     * Add the FieldDefn to the FeatureDefn 
     *----------------------------------------------------*
    m_poDefn->AddFieldDefn(poFieldDefn);
    delete poFieldDefn;

    /*-----------------------------------------------------------------
     * Keep track of native field type
     *----------------------------------------------------------------*
    m_paeFieldType = (TABFieldType *)CPLRealloc(m_paeFieldType,
                                                m_poDefn->GetFieldCount()*
                                                sizeof(TABFieldType));
    m_paeFieldType[m_poDefn->GetFieldCount()-1] = eMapInfoType;

    /*-----------------------------------------------------------------
     * Extend array of Indexed/Unique flags
     *----------------------------------------------------------------*
    m_pabFieldIndexed = (GBool *)CPLRealloc(m_pabFieldIndexed,
                                            m_poDefn->GetFieldCount()*
                                            sizeof(GBool));
    m_pabFieldUnique  = (GBool *)CPLRealloc(m_pabFieldUnique,
                                            m_poDefn->GetFieldCount()*
                                            sizeof(GBool));
    m_pabFieldIndexed[m_poDefn->GetFieldCount()-1] = bIndexed;
    m_pabFieldUnique[m_poDefn->GetFieldCount()-1] = bUnique;

    CPLFree(pszCleanName);
    return nStatus;
}
*/

/**********************************************************************
 *                   MIFFile::GetNextFeatureId()
 *
 * Returns feature id that follows nPrevId, or -1 if it is the
 * last feature id.  Pass nPrevId=-1 to fetch the first valid feature id.
 **********************************************************************/
int TMifFile::GetNextFeatureId(int nPrevId)
{
    if (m_eAccessMode != TABRead)
    {
		fteo::cpl::CPLError(fteo::cpl::CE_Failure, CPLE_NotSupported,
                 "GetNextFeatureId() can be used only with Read access.");
        return -1;
    }

    if (nPrevId <= 0 && m_poMIFFile->GetLastLine() != NULL)
        return 1;       // Feature Ids start at 1
    else if (nPrevId > 0 && m_poMIFFile->GetLastLine() != NULL)
        return nPrevId + 1;
    else
        return -1;

    return 0;
}


/************************************************************************/
/*                            ResetReading()                            */
/************************************************************************/

void TMifFile::ResetReading()

{   
    const char *pszLine;

    m_poMIFFile->Rewind();

    while ((pszLine = m_poMIFFile->GetLine()) != NULL)
      if (EQUALN(pszLine,"DATA",4))
        break;

    while ((pszLine = m_poMIFFile->GetLine()) != NULL)
    {
        if (m_poMIFFile->IsValidFeature(pszLine))
          break;
    }

    //MID файлы отложим пока
    //m_poMIDFile->Rewind();
    //m_poMIDFile->GetLine();
    
    // We're positioned on first feature.  Feature Ids start at 1.
    if (m_poCurFeature)
    {
        delete m_poCurFeature;
        m_poCurFeature = NULL;
    }

    m_nCurFeatureId = 0;
    m_nPreloadedId = 1;
}


/**********************************************************************
 *                   MIFFile::GotoFeature()
 *
 * Private method to move MIF and MID pointers ready to read specified 
 * feature.  
 * Вот она где!   Note that Feature Ids start at 1.
 *
 * Returns 0 on success, -1 on error (likely request for invalid feature id)
 **********************************************************************/
int TMifFile::GotoFeature(int nFeatureId)
{

    if (nFeatureId < 1)
      return -1;

    if (nFeatureId == m_nPreloadedId) // CorrectPosition
    {
        return 0;
    }
    else
    {
        if (nFeatureId < m_nCurFeatureId || m_nCurFeatureId == 0)
            ResetReading();

        while(m_nPreloadedId < nFeatureId)
        {
            if (NextFeature() == false)
              return -1;
        }

        CPLAssert(m_nPreloadedId == nFeatureId);

        return 0;
    }
}

/**********************************************************************
 *                   MIFFile::NextFeature()
 **********************************************************************/

GBool TMifFile::NextFeature()
{
    const char *pszLine;
    while ((pszLine = m_poMIFFile->GetLine()) != NULL)
    {
        if (m_poMIFFile->IsValidFeature(pszLine))
        {
            m_poMIDFile->GetLine();
            m_nPreloadedId++;
            return TRUE;
        }
    }
    return FALSE;
}


/**********************************************************************
 *                   MIFFile::GetFeatureRef()
 *
 * Fill and return a TABFeature object for the specified feature id.
 *
 * The retruned pointer is a reference to an object owned and maintained
 * by this MIFFile object.  It should not be altered or freed by the 
 * caller and its contents is guaranteed to be valid only until the next
 * call to GetFeatureRef() or Close().
 *
 * Returns NULL if the specified feature id does not exist of if an
 * error happened.  In any case, CPLError() will have been called to
 * report the reason of the failure.
 **********************************************************************/
TMapInfoRegion *TMifFile::GetFeatureRef(int nFeatureId)
{
    const char *pszLine;
 /*
    if (m_eAccessMode != TABRead)
    {
        fteo::cpl::CPLError(fteo::cpl::CE_Failure, CPLE_NotSupported,
                 "GetFeatureRef() can be used only with Read access.");
        return NULL;
    }
   */ 
    /*-----------------------------------------------------------------
     * Make sure file is opened and Validate feature id by positioning
     * the read pointers for the .MAP and .DAT files to this feature id.
     *----------------------------------------------------------------*
    if (m_poMIDFile == NULL)
    {
        fteo::cpl::CPLError(fteo::cpl::CE_Failure, CPLE_IllegalArg,
                 "GetFeatureRef() failed: file is not opened!");
        return NULL;
    }
   */
    if (GotoFeature(nFeatureId)!= 0 )
    {
		fteo::cpl::CPLError(fteo::cpl::CE_Failure, CPLE_IllegalArg,
                 "GetFeatureRef() failed: invalid feature id %d", 
                 nFeatureId);
        return NULL;
    }


    /*-----------------------------------------------------------------
     * Create new feature object of the right type
     *----------------------------------------------------------------*/
    if ((pszLine = m_poMIFFile->GetLastLine()) != NULL)
    {
        // Delete previous feature... we'll start we a clean one.
        if (m_poCurFeature)
            delete m_poCurFeature;
        m_poCurFeature = NULL;

        m_nCurFeatureId = m_nPreloadedId;

        if (EQUALN(pszLine,"NONE",4)) //интересный вариант
        {
          //  m_poCurFeature = new TABFeature(m_poDefn);
        }
        else if (EQUALN(pszLine,"POINT",5))
        {
            // Special case, we need to know two lines to decide the type
            char **papszToken;
            papszToken = fteo::cpl::CSLTokenizeString(pszLine);
            
            if (fteo::cpl::CSLCount(papszToken) !=3)
            {
				fteo::cpl::CSLDestroy(papszToken);
				fteo::cpl::CPLError(fteo::cpl::CE_Failure, CPLE_NotSupported,
                         "GetFeatureRef() failed: invalid point line: '%s'",
                         pszLine);
                return NULL;
            }
            
            m_poMIFFile->SaveLine(pszLine);

            if ((pszLine = m_poMIFFile->GetLine()) != NULL)
            {
				fteo::cpl::CSLDestroy(papszToken);
                papszToken = fteo::cpl::CSLTokenizeStringComplex(pszLine," ,()\t",
                                                      true,false);
                if (fteo::cpl::CSLCount(papszToken)> 0 &&EQUALN(papszToken[0],"SYMBOL",6))
                {
                    switch (fteo::cpl::CSLCount(papszToken))
                    {
                      case 4:
                        //m_poCurFeature = new TABPoint(m_poDefn);
                        break;
                      case 7:
                       // m_poCurFeature = new TABFontPoint(m_poDefn);
                        break;
                      case 5:
                       // m_poCurFeature = new TABCustomPoint(m_poDefn);
                        break;
                      default:
						  fteo::cpl::CSLDestroy(papszToken);
						  fteo::cpl::CPLError(fteo::cpl::CE_Failure, CPLE_NotSupported,
                                 "GetFeatureRef() failed: invalid symbol "
                                 "line: '%s'", pszLine);
                        return NULL;
                        break;
                    }

                }
            }
			fteo::cpl::CSLDestroy(papszToken);

            if (m_poCurFeature == NULL)
            {
                // No symbol clause... default to TABPoint
              //  m_poCurFeature = new TABPoint(m_poDefn);
            }
        }
        else if (EQUALN(pszLine,"LINE",4) ||
                 EQUALN(pszLine,"PLINE",5))
        {
           // m_poCurFeature = new TABPolyline(m_poDefn);
        }
        else if (EQUALN(pszLine,"REGION",6))
        {
        m_poCurFeature = new TMapInfoRegion(); //(m_poDefn); //Только полигоны мне нужны
        }  
        else if (EQUALN(pszLine,"ARC",3))
        { 
           // m_poCurFeature = new TABArc(m_poDefn);
        }
        else if (EQUALN(pszLine,"TEXT",4))
        {
          //  m_poCurFeature = new TABText(m_poDefn);
        }
        else if (EQUALN(pszLine,"RECT",4) ||
                 EQUALN(pszLine,"ROUNDRECT",9))
        {
          //  m_poCurFeature = new TABRectangle(m_poDefn);
        }
        else if (EQUALN(pszLine,"ELLIPSE",7))
        {
          //  m_poCurFeature = new TABEllipse(m_poDefn);       
        }
        else if (EQUALN(pszLine,"MULTIPOINT",10))
        {
          //  m_poCurFeature = new TABMultiPoint(m_poDefn);       
        }
        else if (EQUALN(pszLine,"COLLECTION",10))
        {
           // m_poCurFeature = new TABCollection(m_poDefn);       
        }
        else
        {
            if (!EQUAL(pszLine,""))
				fteo::cpl::CPLError(fteo::cpl::CE_Failure, CPLE_NotSupported,
                   "Error during reading, unknown type %s.",
                     pszLine);
        
            //m_poCurFeature = new TABDebugFeature(m_poDefn);
            return NULL;
        }
    }

	/// ****** fteo::cpl::CPLAssert(m_poCurFeature);
    if (m_poCurFeature == NULL)
        return NULL;

     //Read fields from the .DAT file
     //GetRecordBlock() has already been called above...
     
    /*
    if (m_poCurFeature->ReadRecordFromMIDFile(m_poMIDFile) != 0)
    {
        CPLError(CE_Failure, CPLE_NotSupported,
                 "Error during reading Record.");
        
        delete m_poCurFeature;
        m_poCurFeature = NULL;
        return NULL;
    }
    */

     // Read geometry from the .MAP file
     // MoveToObjId() has already been called above...
  /*  OGR:   
    if (m_poCurFeature->ReadGeometryFromMIFFile(m_poMIFFile) != 0)
    {
        CPLError(CE_Failure, CPLE_NotSupported,
                 "Error during reading Geometry.");
        
        delete m_poCurFeature;
        m_poCurFeature = NULL;
        return NULL;
    }
*/
    // fteo:
    if (m_poCurFeature->ReadFteoGeometryFromMIFFile(m_poMIFFile) != 0)
    {
		fteo::cpl::CPLError(fteo::cpl::CE_Failure, CPLE_NotSupported,
                 "Error during reading Geometry.");
        
        delete m_poCurFeatureF;
        m_poCurFeatureF = NULL;
        return NULL;
    }
	m_poCurFeature->Definition = this->pszFeatureClassName;
    /*---------------------------------------------------------------------
     * The act of reading the geometry causes the first line of the    
     * next object to be preloaded.  Set the preloaded id appropriately.
     *--------------------------------------------------------------------- */
    if( m_poMIFFile->GetLastLine() != NULL )
        m_nPreloadedId++;
    else
        m_nPreloadedId = 0;
   
    /* Update the Current Feature ID */
    m_poCurFeature->SetFID(m_nCurFeatureId);

    return m_poCurFeature;
}

/************************************************************************/
/*                            PreParseFile()                            */
/************************************************************************/

void TMifFile::PreParseFile()
{
    char **papszToken = NULL;
    const char *pszLine;
    
    GBool bPLine = FALSE;
    GBool bText = FALSE;

    if (m_bPreParsed == TRUE)
      return;

    m_poMIFFile->Rewind();

    while ((pszLine = m_poMIFFile->GetLine()) != NULL)
      if (EQUALN(pszLine,"DATA",4))
        break;

    m_nPoints = m_nLines = m_nRegions = m_nTexts = 0;

    while ((pszLine = m_poMIFFile->GetLine()) != NULL)
    {
        if (m_poMIFFile->IsValidFeature(pszLine))
        {
            bPLine = FALSE;
            bText = FALSE;
            m_nFeatureCount++;
        }

		fteo::cpl::CSLDestroy(papszToken);
        papszToken = fteo::cpl::CSLTokenizeString(pszLine);

        if (EQUALN(pszLine,"POINT",5))
        {
            m_nPoints++;
            if (fteo::cpl::CSLCount(papszToken) == 3)
            {
                UpdateExtents(m_poMIFFile->GetXTrans(atof(papszToken[1])),
                             m_poMIFFile->GetYTrans(atof(papszToken[2])));
            }
              
        }
        else if (EQUALN(pszLine,"LINE",4) ||
                 EQUALN(pszLine,"RECT",4) ||
                 EQUALN(pszLine,"ROUNDRECT",9) ||
                 EQUALN(pszLine,"ARC",3) ||
                 EQUALN(pszLine,"ELLIPSE",7))
        {
            if (fteo::cpl::CSLCount(papszToken) == 5)
            {
                m_nLines++;
                UpdateExtents(m_poMIFFile->GetXTrans(atof(papszToken[1])), 
                             m_poMIFFile->GetYTrans(atof(papszToken[2])));
                UpdateExtents(m_poMIFFile->GetXTrans(atof(papszToken[3])), 
                             m_poMIFFile->GetYTrans(atof(papszToken[4])));
            }
        }
        else if (EQUALN(pszLine,"REGION",6) )
        {
            m_nRegions++;
            bPLine = TRUE;
        }
        else if( EQUALN(pszLine,"PLINE",5))
        {
            m_nLines++;
            bPLine = TRUE;
        }
        else if (EQUALN(pszLine,"TEXT",4)) 
        {
            m_nTexts++;
            bText = TRUE;
        }
        else if (bPLine == TRUE)
        {
            if (fteo::cpl::CSLCount(papszToken) == 2 &&
                strchr("-.0123456789", papszToken[0][0]) != NULL)
            {
                UpdateExtents( m_poMIFFile->GetXTrans(atof(papszToken[0])),
                              m_poMIFFile->GetYTrans(atof(papszToken[1])));
            }
        }
        else if (bText == TRUE)
        {
           if (fteo::cpl::CSLCount(papszToken) == 4 &&
                strchr("-.0123456789", papszToken[0][0]) != NULL)
            {
                UpdateExtents(m_poMIFFile->GetXTrans(atof(papszToken[0])),
                             m_poMIFFile->GetYTrans(atof(papszToken[1])));
                UpdateExtents(m_poMIFFile->GetXTrans(atof(papszToken[2])),
                             m_poMIFFile->GetYTrans(atof(papszToken[3])));
            } 
        }
        
      }

	fteo::cpl::CSLDestroy(papszToken);
    
    m_poMIFFile->Rewind();

    while ((pszLine = m_poMIFFile->GetLine()) != NULL)
      if (EQUALN(pszLine,"DATA",4))
        break;

    while ((pszLine = m_poMIFFile->GetLine()) != NULL)
    {
        if (m_poMIFFile->IsValidFeature(pszLine))
          break;
    }

    // пока с mid ом подождем
    //m_poMIDFile->Rewind();
    //m_poMIDFile->GetLine();
 
    m_bPreParsed = TRUE;

}



/**********************************************************************
 *                   ParseMIFHeader()
 *
 * Scan the header of a MIF file, and store any useful information into
 * class members.  The main piece of information being the fields 
 * definition that we use to build the OGRFeatureDefn for this file.
 *
 * This private method should be used only during the Open() call.
 *
 * Returns 0 on success, -1 on error.
 **********************************************************************/
int TMifFile::ParseMIFHeader() {
 GBool  bColumns = FALSE;
    int    nColumns = 0;
    GBool  bCoordSys = FALSE;
    char  *pszTmp;
            
    
    const char *pszLine;
    char **papszToken;

    char *pszFeatureClassName = GetBasename(m_pszFname);
    //m_poDefn = new OGRFeatureDefn(pszFeatureClassName);
	fteo::cpl::CPLFree(pszFeatureClassName);
    // Ref count defaults to 0... set it to 1
   // m_poDefn->Reference();

    
    if (m_eAccessMode != TABRead)
    {
		fteo::cpl::CPLError(fteo::cpl::CE_Failure, CPLE_NotSupported,
                 "ParseMIDFile() can be used only with Read access.");
        return -1;
    }
    

    /*-----------------------------------------------------------------
     * Parse header until we find the "Data" line
     *----------------------------------------------------------------*/
    while (((pszLine = m_poMIFFile->GetLine()) != NULL) && 
           !(EQUALN(pszLine,"Data",4)))
    {
        while(pszLine && (*pszLine == ' ' || *pszLine == '\t') )
            pszLine++;  // skip leading spaces

        if (bColumns == TRUE && nColumns >0)
        {
            if (nColumns == 0)
            {
                // Permit to 0 columns
                bColumns = FALSE;
            }
            else 
				if (true) //AddFields(pszLine) == 0)
            {
                nColumns--;
                if (nColumns == 0)
                  bColumns = FALSE;
            }
            else
            {
                bColumns = FALSE;
            }
        }
        else if (EQUALN(pszLine,"VERSION",7))
        {
            papszToken = fteo::cpl::CSLTokenizeStringComplex(pszLine," ()\t",true,false);
            bColumns = FALSE; bCoordSys = FALSE;
            if (fteo::cpl::CSLCount(papszToken)  == 2)
              m_nVersion = atoi(papszToken[1]);
			fteo::cpl::CSLDestroy(papszToken);
        }
        
        else if (EQUALN(pszLine,"CHARSET",7))
        {
            papszToken = fteo::cpl::CSLTokenizeStringComplex(pszLine," ()\t",true,false);
            bColumns = FALSE; bCoordSys = FALSE;
          
            if (fteo::cpl::CSLCount(papszToken)  == 2)
            {
               //VSIFree(m_pszCharset);  //CPLFree(m_pszCharset);
               fteo::cpl::VSIFree(m_pszCharset);
               m_pszCharset = fteo::cpl::CPLStrdup(papszToken[1]);
            }
			fteo::cpl::CSLDestroy(papszToken);
        
        }
        else if (EQUALN(pszLine,"DELIMITER",9))
        {
            papszToken = fteo::cpl::CSLTokenizeStringComplex(pszLine," ()\t",true,false);
             bColumns = FALSE; bCoordSys = false;
          
           if (fteo::cpl::CSLCount(papszToken)  == 2)
           {
			   fteo::cpl::CPLFree(m_pszDelimiter);
               m_pszDelimiter = fteo::cpl::CPLStrdup(papszToken[1]);
           }
		   fteo::cpl::CSLDestroy(papszToken);
        
        }
        else if (EQUALN(pszLine,"UNIQUE",6))
        {
            bColumns = FALSE; bCoordSys = FALSE;
          
            m_pszUnique = fteo::cpl::CPLStrdup(pszLine + 6);
        }
        else if (EQUALN(pszLine,"INDEX",5))
        {
            bColumns = FALSE; bCoordSys = FALSE;
          
            m_pszIndex = fteo::cpl::CPLStrdup(pszLine + 5);
        }
        else if (EQUALN(pszLine,"COORDSYS",8) )
        {
            bCoordSys = TRUE;
            m_pszCoordSys = fteo::cpl::CPLStrdup(pszLine + 9);

            // Extract bounds if present
            char  **papszFields;
            papszFields = fteo::cpl::CSLTokenizeStringComplex(m_pszCoordSys, " ,()\t",
                                                   true, false );
            int iBounds = fteo::cpl::CSLFindString( papszFields, "Bounds" );
            if (iBounds >= 0 && iBounds + 4 < fteo::cpl::CSLCount(papszFields))
            {
                m_dXMin = atof(papszFields[++iBounds]);
                m_dYMin = atof(papszFields[++iBounds]);
                m_dXMax = atof(papszFields[++iBounds]);
                m_dYMax = atof(papszFields[++iBounds]);
                m_bBoundsSet = TRUE;
            }
			fteo::cpl::CSLDestroy( papszFields );
        }
        else if (EQUALN(pszLine,"TRANSFORM",9))
        {
            papszToken = fteo::cpl::CSLTokenizeStringComplex(pszLine," ,\t",true,false);
            bColumns = FALSE; bCoordSys = FALSE;
          
            if (fteo::cpl::CSLCount(papszToken) == 5)
            {
                m_dfXMultiplier   = atof(papszToken[1]);
                m_dfYMultiplier   = atof(papszToken[2]);
                m_dfXDisplacement = atof(papszToken[3]);
                m_dfYDisplacement = atof(papszToken[4]);
                
                if (m_dfXMultiplier == 0.0)
                  m_dfXMultiplier = 1.0;
                if (m_dfYMultiplier == 0.0)
                  m_dfYMultiplier = 1.0;
            }
			fteo::cpl::CSLDestroy(papszToken);
        }
        else if (EQUALN(pszLine,"COLUMNS",7))
        {
            papszToken = fteo::cpl::CSLTokenizeStringComplex(pszLine," ()\t",true,false);
            bCoordSys = FALSE;
            bColumns = TRUE;
            if (fteo::cpl::CSLCount(papszToken) == 2)
            {
                nColumns = atoi(papszToken[1]);
                m_nAttribut = nColumns;
            }
            else
            {
                bColumns = FALSE;
                m_nAttribut = 0;
            }
			fteo::cpl::CSLDestroy(papszToken);
        }
        else if (bCoordSys == TRUE)
        {
            pszTmp = m_pszCoordSys;
            m_pszCoordSys = fteo::cpl::CPLStrdup(fteo::cpl::CPLSPrintf("%s %s",m_pszCoordSys,
                                                 pszLine));
			fteo::cpl::CPLFree(pszTmp);
            //printf("Reading CoordSys\n");
            // Reading CoordSys
        }

    }
    
    if ((pszLine = m_poMIFFile->GetLastLine()) == NULL || 
        EQUALN(m_poMIFFile->GetLastLine(),"DATA",4) == false)
    {
		fteo::cpl::CPLError(fteo::cpl::CE_Failure, CPLE_NotSupported,
                 "DATA keyword not found in %s.  File may be corrupt.",
                 m_pszFname);
        return -1;
    }
    
    /*-----------------------------------------------------------------
     * Move pointer to first line of first object
     *----------------------------------------------------------------*/
    while (((pszLine = m_poMIFFile->GetLine()) != NULL) && 
           m_poMIFFile->IsValidFeature(pszLine) == false)
        ;

    /*-----------------------------------------------------------------
     * Check for Unique and Indexed flags
     *----------------------------------------------------------------*/
    if (m_pszIndex)
    {
        papszToken = fteo::cpl::CSLTokenizeStringComplex(m_pszIndex," ,\t",true,false);
        for(int i=0; papszToken && papszToken[i]; i++)
        {
            int nVal = atoi(papszToken[i]);
          //  if (nVal > 0 && nVal <= m_poDefn->GetFieldCount())     m_pabFieldIndexed[nVal-1] = TRUE;
        }
		fteo::cpl::CSLDestroy(papszToken);
    }

    if (m_pszUnique)
    {
        papszToken = fteo::cpl::CSLTokenizeStringComplex(m_pszUnique," ,\t",true,false);
        for(int i=0; papszToken && papszToken[i]; i++)
        {
            int nVal = atoi(papszToken[i]);
           // if (nVal > 0 && nVal <= m_poDefn->GetFieldCount())                 m_pabFieldUnique[nVal-1] = TRUE;
        }
		fteo::cpl::CSLDestroy(papszToken);
    }

    return 0;
	};

 /**********************************************************************
 *                   MIFFile::UpdateExtents()
 *
 * Private Methode used to update the dataset extents
 **********************************************************************/
void TMifFile::UpdateExtents(double dfX, double dfY)
{
    if (m_bExtentsSet == false)
    {
        m_bExtentsSet = true;
        m_sExtents.MinX = m_sExtents.MaxX = dfX;
        m_sExtents.MinY = m_sExtents.MaxY = dfY;
    }
    else
    {
        if (dfX < m_sExtents.MinX)
            m_sExtents.MinX = dfX;
        if (dfX > m_sExtents.MaxX)
          m_sExtents.MaxX = dfX;
        if (dfY < m_sExtents.MinY)
          m_sExtents.MinY = dfY;
        if (dfY > m_sExtents.MaxY)
          m_sExtents.MaxY = dfY;
    }
}



/************************************************************************/
/*                Classes definition (implemenattion: )                 */
/************************************************************************/
MIDDATAFile::MIDDATAFile()
{
    m_fp = NULL;
    m_szLastRead[0] = '\0';
    m_szSavedLine[0] = '\0';
    m_pszDelimiter = "\t"; // Encom 2003 (was NULL)
    
    m_dfXMultiplier = 1.0;
    m_dfYMultiplier = 1.0;
    m_dfXDisplacement = 0.0;
    m_dfYDisplacement = 0.0;

}

MIDDATAFile::~MIDDATAFile()
{
    Close();
}

void MIDDATAFile::SaveLine(const char *pszLine)
{
    if (pszLine == NULL)
    {
        m_szSavedLine[0] = '\0';
    }
    else
    {
        strncpy(m_szSavedLine,pszLine,MIDMAXCHAR);
    }
}

const char *MIDDATAFile::GetSavedLine()
{
    return m_szSavedLine;
}

int MIDDATAFile::Open(const char *pszFname, const char *pszAccess)
{
   if (m_fp)
   {
       return -1;
   }

    /*-----------------------------------------------------------------
     * Validate access mode and make sure we use Text access.
     *----------------------------------------------------------------*/
    if (EQUALN(pszAccess, "r", 1))
    {
        m_eAccessMode = TABRead;
        pszAccess = "rt";
    }
    else if (EQUALN(pszAccess, "w", 1))
    {
        m_eAccessMode = TABWrite;
        pszAccess = "wt";
    }
    else
    {
        return -1;
    }

    /*-----------------------------------------------------------------
     * Open file for reading
     *----------------------------------------------------------------*/
    m_pszFname = fteo::cpl::CPLStrdup(pszFname);
    m_fp = fteo::cpl::VSIFOpen(m_pszFname, pszAccess);

    if (m_fp == NULL)
    {
		fteo::cpl::CPLFree(m_pszFname);
        m_pszFname = NULL;
        return -1;
    }

    SetEof(fteo::cpl::VSIFEof(m_fp));
    return 0;
}

int MIDDATAFile::Rewind()
{
    if (m_fp == NULL || m_eAccessMode == TABWrite) 
        return -1;

    else
    {
		fteo::cpl::VSIRewind(m_fp);
     SetEof(fteo::cpl::VSIFEof(m_fp));
    }
    return 0;
}

int MIDDATAFile::Close()
{
    if (m_fp == NULL)
        return 0;
   
    // Close file
	fteo::cpl::VSIFClose(m_fp);
    m_fp = NULL;

    // clear readline buffer.
    fteo::cpl::CPLReadLine( NULL );

	fteo::cpl::CPLFree(m_pszFname);
    m_pszFname = NULL;

    return 0;

}

const char *MIDDATAFile::GetLine()
{
    const char *pszLine;
    
    if (m_eAccessMode == TABRead)
    {
        
        pszLine = fteo::cpl::CPLReadLine(m_fp);

        SetEof(fteo::cpl::VSIFEof(m_fp));

        if (pszLine == NULL)
        {
            m_szLastRead[0] = '\0';
        }
        else
        {
            strncpy(m_szLastRead,pszLine,MIDMAXCHAR);
        }
        //if (pszLine)
        //  printf("%s\n",pszLine);
        return pszLine;
    }
    else
      CPLAssert(FALSE);
    
    return NULL;
}

const char *MIDDATAFile::GetLastLine()
{
    // Return NULL if EOF
    if(GetEof())
    {
        return NULL;
    }
    else if (m_eAccessMode == TABRead)
    {
        // printf("%s\n",m_szLastRead);
        return m_szLastRead;
    }

    // We should never get here (Read/Write mode not implemented)
    CPLAssert(FALSE);
    return NULL;
}

void MIDDATAFile::WriteLine(const char *pszFormat,...)
{
    va_list args;

    if (m_eAccessMode == TABWrite  && m_fp)
    {
        va_start(args, pszFormat);
         vfprintf( m_fp, pszFormat, args );
        va_end(args);
    } 
    else
    {
        CPLAssert(FALSE);
    }
}


void MIDDATAFile::SetTranslation(double dfXMul,double dfYMul, 
                                 double dfXTran,
                                 double dfYTran)
{
    m_dfXMultiplier = dfXMul;
    m_dfYMultiplier = dfYMul;
    m_dfXDisplacement = dfXTran;
    m_dfYDisplacement = dfYTran;
}

double MIDDATAFile::GetXTrans(double dfX)
{
    return (dfX * m_dfXMultiplier) + m_dfXDisplacement;
}

double MIDDATAFile::GetYTrans(double dfY)
{
    return (dfY * m_dfYMultiplier) + m_dfYDisplacement;
}


GBool MIDDATAFile::IsValidFeature(const char *pszString)
{
    char **papszToken ;

    papszToken = fteo::cpl::CSLTokenizeString(pszString);
    
    //   printf("%s\n",pszString);

    if (fteo::cpl::CSLCount(papszToken) == 0)
    {
		fteo::cpl::CSLDestroy(papszToken);
        return FALSE;
    }

    if (EQUAL(papszToken[0],"NONE")      || EQUAL(papszToken[0],"POINT") ||
        EQUAL(papszToken[0],"LINE")      || EQUAL(papszToken[0],"PLINE") ||
        EQUAL(papszToken[0],"REGION")    || EQUAL(papszToken[0],"ARC") ||
        EQUAL(papszToken[0],"TEXT")      || EQUAL(papszToken[0],"RECT") ||
        EQUAL(papszToken[0],"ROUNDRECT") || EQUAL(papszToken[0],"ELLIPSE") ||
        EQUAL(papszToken[0],"MULTIPOINT")|| EQUAL(papszToken[0],"COLLECTION") )
    {
       fteo::cpl::CSLDestroy(papszToken);
        return TRUE;
    }

	fteo::cpl::CSLDestroy(papszToken);
    return FALSE;

}


GBool MIDDATAFile::GetEof()
{
    return m_bEof;
}


void MIDDATAFile::SetEof(GBool bEof)
{
    m_bEof = bEof;
}

	  }
	};