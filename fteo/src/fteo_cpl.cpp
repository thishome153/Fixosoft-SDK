#include "stdafx.h"
#include "fteo_cpl.h"


namespace fteo
{
  namespace cpl
{

/************************************************************************/

/************************************************************************/
/*                              VSIFEof()                               */
/************************************************************************/

int VSIFEof( FILE * fp )

{
    return( feof( fp ) );
}



/************************************************************************/
/*                             VSICalloc()                              */
/************************************************************************/

void *VSICalloc( size_t nCount, size_t nSize )

{
    return( calloc( nCount, nSize ) );
}

/************************************************************************/
/*                             VSIMalloc()                              */
/************************************************************************/

void *VSIMalloc( size_t nSize )

{
    return( malloc( nSize ) );
}

/************************************************************************/
/*                             VSIRealloc()                             */
/************************************************************************/

void * VSIRealloc( void * pData, size_t nNewSize )

{
    return( realloc( pData, nNewSize ) );
}

/************************************************************************/
/*                             CPLCalloc()                              */
/************************************************************************/

/**
 * Safe version of calloc().
 *
 * This function is like the C library calloc(), but raises a CE_Fatal
 * error with CPLError() if it fails to allocate the desired memory.  It
 * should be used for small memory allocations that are unlikely to fail
 * and for which the application is unwilling to test for out of memory
 * conditions.  It uses VSICalloc() to get the memory, so any hooking of
 * VSICalloc() will apply to CPLCalloc() as well.  CPLFree() or VSIFree()
 * can be used free memory allocated by CPLCalloc().
 *
 * @param nCount number of objects to allocate.
 * @param nSize size (in bytes) of object to allocate.
 * @return pointer to newly allocated memory, only NULL if nSize * nCount is
 * NULL.
 */

void *CPLCalloc( size_t nCount, size_t nSize )

{
    void        *pReturn;

    if( nSize * nCount == 0 )
        return NULL;
    
    pReturn = VSICalloc( nCount, nSize );
    if( pReturn == NULL )
    {
        CPLError( CE_Fatal, CPLE_OutOfMemory,
                  "CPLCalloc(): Out of memory allocating %d bytes.\n",
                  nSize * nCount );
    }

    return pReturn;
}

/************************************************************************/
/*                             CPLRealloc()                             */
/************************************************************************/

/**
 * Safe version of realloc().
 *
 * This function is like the C library realloc(), but raises a CE_Fatal
 * error with CPLError() if it fails to allocate the desired memory.  It
 * should be used for small memory allocations that are unlikely to fail
 * and for which the application is unwilling to test for out of memory
 * conditions.  It uses VSIRealloc() to get the memory, so any hooking of
 * VSIRealloc() will apply to CPLRealloc() as well.  CPLFree() or VSIFree()
 * can be used free memory allocated by CPLRealloc().
 *
 * It is also safe to pass NULL in as the existing memory block for
 * CPLRealloc(), in which case it uses VSIMalloc() to allocate a new block.
 *
 * @param pData existing memory block which should be copied to the new block.
 * @param nNewSize new size (in bytes) of memory block to allocate.
 * @return pointer to allocated memory, only NULL if nNewSize is zero.
 */


void * CPLRealloc( void * pData, size_t nNewSize )

{
    void        *pReturn;

    if ( nNewSize == 0 )
    {
        VSIFree(pData);
        return NULL;
    }

    if( nNewSize < 0 )
    {
        CPLError( CE_Failure, CPLE_AppDefined,
                  "CPLRealloc(%d): Silly size requested.\n",
                  nNewSize );
        return NULL;
    }
    
    if( pData == NULL )
        pReturn = VSIMalloc( nNewSize );
    else
        pReturn = VSIRealloc( pData, nNewSize );
    
    if( pReturn == NULL )
    {
        CPLError( CE_Fatal, CPLE_OutOfMemory,
                  "CPLRealloc(): Out of memory allocating %d bytes.\n",
                  nNewSize );
    }

    return pReturn;
}


/************************************************************************/
/*                             CPLMalloc()                              */
/************************************************************************/

/**
 * Safe version of malloc().
 *
 * This function is like the C library malloc(), but raises a CE_Fatal
 * error with CPLError() if it fails to allocate the desired memory.  It
 * should be used for small memory allocations that are unlikely to fail
 * and for which the application is unwilling to test for out of memory
 * conditions.  It uses VSIMalloc() to get the memory, so any hooking of
 * VSIMalloc() will apply to CPLMalloc() as well.  CPLFree() or VSIFree()
 * can be used free memory allocated by CPLMalloc().
 *
 * @param nSize size (in bytes) of memory block to allocate.
 * @return pointer to newly allocated memory, only NULL if nSize is zero.
 */

void *CPLMalloc( size_t nSize )

{
    void        *pReturn;

    CPLVerifyConfiguration();

    if( nSize == 0 )
        return NULL;

    if( nSize < 0 )
    {
        CPLError( CE_Failure, CPLE_AppDefined,
                  "CPLMalloc(%d): Silly size requested.\n",
                  nSize );
        return NULL;
    }
    
    pReturn = VSIMalloc( nSize );
    if( pReturn == NULL )
    {
        CPLError( CE_Fatal, CPLE_OutOfMemory,
                  "CPLMalloc(): Out of memory allocating %d bytes.\n",
                  nSize );
    }

    return pReturn;
}


/************************************************************************/
/*                              VSIFree()                               */
/************************************************************************/

void VSIFree( void * pData )

{
    if  ( pData != NULL ) 
     if ( (int) pData == 0xCDCDCDCD)    // 0xCDCDCDCD	Allocated in heap, but not initialize
        return;
    
    if  ( pData != NULL ) 
        free( pData );

}




/************************************************************************/
/*                              VSIFOpen()                              */
/************************************************************************/

FILE *VSIFOpen( const char * pszFilename, const char * pszAccess )

{
    FILE * fp;

    fp = fopen( (char *) pszFilename, (char *) pszAccess );

   // VSIDebug3( "VSIFOpen(%s,%s) = %p", pszFilename, pszAccess, fp );

    return( fp );
}

/************************************************************************/
/*                             VSIFClose()                              */
/************************************************************************/

int VSIFClose( FILE * fp )

{
    VSIDebug1( "VSIClose(%p)", fp );
    return( fclose(fp) );
}


/************************************************************************/
/*                              VSIFTell()                              */
/************************************************************************/

long VSIFTell( FILE * fp )

{
    VSIDebug2( "VSIFTell(%p) = %ld", fp, ftell(fp) );

    return( ftell( fp ) );
}

/************************************************************************/
/*                              VSIFGets()                              */
/************************************************************************/

char *VSIFGets( char *pszBuffer, int nBufferSize, FILE * fp )

{
    return( fgets( pszBuffer, nBufferSize, fp ) );
}

/************************************************************************/
/*                              VSIFSeek()                              */
/************************************************************************/

int VSIFSeek( FILE * fp, long nOffset, int nWhence )

{
#ifdef VSI_DEBUG
    if( nWhence == SEEK_SET )
    {
        VSIDebug2( "VSIFSeek(%p,%d,SEEK_SET)", fp, nOffset );
    }
    else if( nWhence == SEEK_END )
    {
        VSIDebug2( "VSIFSeek(%p,%d,SEEK_END)", fp, nOffset );
    }
    else if( nWhence == SEEK_CUR )
    {
        VSIDebug2( "VSIFSeek(%p,%d,SEEK_CUR)", fp, nOffset );
    }
    else
    {
        VSIDebug3( "VSIFSeek(%p,%d,%d-Unknown)", fp, nOffset, nWhence );
    }
#endif 

    return( fseek( fp, nOffset, nWhence ) );
}

/************************************************************************/
/*                             VSIRewind()                              */
/************************************************************************/

void VSIRewind( FILE * fp )

{
    VSIDebug1("VSIRewind(%p)", fp );
    rewind( fp );
}


/************************************************************************/
/*                             VSIStrdup()                              */
/************************************************************************/

char *VSIStrdup( const char * pszString )

{
    return( strdup( pszString ) );
}

/************************************************************************/
/*                         CPLGetConfigOption()                         */
/************************************************************************/

const char * CPL_STDCALL CPLGetConfigOption( const char *pszKey, const char *pszDefault )

{
    const char *pszResult = NULL;

    {
      //да закрою тебя на х!!!ер   
       CPLMutexHolderD( &hConfigMutex );

        pszResult = CSLFetchNameValue( (char **) papszConfigOptions, pszKey );
    }

#if !defined(WIN32CE) 
    if( pszResult == NULL )
        pszResult = getenv( pszKey );
#endif
    
    if( pszResult == NULL )
        return pszDefault;
    else
        return pszResult;
}


/************************************************************************/
/*                       CPLDefaultErrorHandler()                       */
/************************************************************************/

void CPL_STDCALL CPLDefaultErrorHandler( CPLErr eErrClass, int nError, 
                             const char * pszErrorMsg )

{
    static int       bLogInit = FALSE;
    static FILE *    fpLog = stderr;

    if( !bLogInit )
    {
        bLogInit = TRUE;

        fpLog = stderr;
        if( CPLGetConfigOption( "CPL_LOG", NULL ) != NULL )
        {
            fpLog = fopen( CPLGetConfigOption("CPL_LOG",""), "wt" );
            if( fpLog == NULL )
                fpLog = stderr;
        }
    }

    if( eErrClass == CE_Debug )
        fprintf( fpLog, "%s\n", pszErrorMsg );
    else if( eErrClass == CE_Warning )
        fprintf( fpLog, "Warning %d: %s\n", nError, pszErrorMsg );
    else
        fprintf( fpLog, "ERROR %d: %s\n", nError, pszErrorMsg );

    fflush( fpLog );
}



/**********************************************************************
 *                       CPLSPrintf()
 *
 * My own version of CPLSPrintf() that works with 10 static buffer.
 *
 * It returns a ref. to a static buffer that should not be freed and
 * is valid only until the next call to CPLSPrintf().
 *
 * NOTE: This function should move to cpl_conv.cpp. 
 **********************************************************************/
/* For now, assume that a 8000 chars buffer will be enough.
 */
#define CPLSPrintf_BUF_SIZE 8000
#define CPLSPrintf_BUF_Count 10
static CPL_THREADLOCAL char gszCPLSPrintfBuffer[CPLSPrintf_BUF_Count][CPLSPrintf_BUF_SIZE];
static CPL_THREADLOCAL int gnCPLSPrintfBuffer = 0;

const char *CPLSPrintf(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
#if defined(HAVE_VSNPRINTF)
    vsnprintf(gszCPLSPrintfBuffer[gnCPLSPrintfBuffer], CPLSPrintf_BUF_SIZE-1,
              fmt, args);
#else
    vsprintf(gszCPLSPrintfBuffer[gnCPLSPrintfBuffer], fmt, args);
#endif
    va_end(args);
    
   int nCurrent = gnCPLSPrintfBuffer;

    if (++gnCPLSPrintfBuffer == CPLSPrintf_BUF_Count)
      gnCPLSPrintfBuffer = 0;

    return gszCPLSPrintfBuffer[nCurrent];
}

/************************************************************************/
/*                             CPLStrdup()                              */
/************************************************************************/

/**
 * Safe version of strdup() function.
 *
 * This function is similar to the C library strdup() function, but if
 * the memory allocation fails it will issue a CE_Fatal error with
 * CPLError() instead of returning NULL.  It uses VSIStrdup(), so any
 * hooking of that function will apply to CPLStrdup() as well.  Memory
 * allocated with CPLStrdup() can be freed with CPLFree() or VSIFree().
 *
 * It is also safe to pass a NULL string into CPLStrdup().  CPLStrdup()
 * will allocate and return a zero length string (as opposed to a NULL
 * string).
 *
 * @param pszString input string to be duplicated.  May be NULL.
 * @return pointer to a newly allocated copy of the string.  Free with
 * CPLFree() or VSIFree().
 */

char *CPLStrdup( const char * pszString )

{
    char        *pszReturn;

    if( pszString == NULL )
        pszString = "";

    pszReturn = VSIStrdup( pszString );
        
    if( pszReturn == NULL )
    {
       /* CPLError( CE_Fatal, CPLE_OutOfMemory,
                  "CPLStrdup(): Out of memory allocating %d bytes.\n",
                  strlen(pszString) );
        */
    }
    
    return( pszReturn );
}




/************************************************************************/
/*                           CSLFindString()                            */
/*                                                                      */
/*      Find a string within a string list.  The string must match      */
/*      the full length, but the comparison is case insensitive.        */
/*      Return -1 on failure.                                           */
/************************************************************************/

int CSLFindString( char ** papszList, const char * pszTarget )

{
    int         i;

    if( papszList == NULL )
        return -1;

    for( i = 0; papszList[i] != NULL; i++ )
    {
        if( EQUAL(papszList[i],pszTarget) )
            return i;
    }

    return -1;
}

void    CPLError(CPLErr eErrClass, int err_no, const char *fmt, ...)
{
    va_list args;

    /* Expand the error message 
     */
    va_start(args, fmt);
    CPLErrorV( eErrClass, err_no, fmt, args );
    va_end(args);
}

/**********************************************************************
 *                       CSLFetchNameValue()
 *
 * In a StringList of "Name=Value" pairs, look for the
 * first value associated with the specified name.  The search is not
 * case sensitive.
 * ("Name:Value" pairs are also supported for backward compatibility
 * with older stuff.)
 * 
 * Returns a reference to the value in the StringList that the caller
 * should not attempt to free.
 *
 * Returns NULL if the name is not found.
 **********************************************************************/
const char *CSLFetchNameValue(char **papszStrList, const char *pszName)
{
    size_t nLen;

    if (papszStrList == NULL || pszName == NULL)
        return NULL;

    nLen = strlen(pszName);
    while(*papszStrList != NULL)
    {
        if (EQUALN(*papszStrList, pszName, nLen)
            && ( (*papszStrList)[nLen] == '=' || 
                 (*papszStrList)[nLen] == ':' ) )
        {
            return (*papszStrList)+nLen+1;
        }
        papszStrList++;
    }
    return NULL;
}




/************************************************************************/
/*                           CPLGetTLSList()                            */
/************************************************************************/

static void **papTLSList = NULL;

static void **CPLGetTLSList()

{
    if( papTLSList == NULL )
        papTLSList = (void **) CPLCalloc(sizeof(void*),CTLS_MAX*2);

    return papTLSList;
}


/************************************************************************/
/*                             CPLGetTLS()                              */
/************************************************************************/

void *CPLGetTLS( int nIndex )

{
    void** papTLSList = CPLGetTLSList();

/// ////    CPLAssert( nIndex >= 0 && nIndex < CTLS_MAX );

    return papTLSList[nIndex];
}


/************************************************************************/
/*                             CPLSetTLS()                              */
/************************************************************************/

void CPLSetTLS( int nIndex, void *pData, int bFreeOnExit )

{
    void **papTLSList = CPLGetTLSList();

  /// ///  CPLAssert( nIndex >= 0 && nIndex < CTLS_MAX );

    papTLSList[nIndex] = pData;
    papTLSList[CTLS_MAX + nIndex] = (void *) bFreeOnExit;
}

/************************************************************************/
/*                         CPLGetErrorContext()                         */
/************************************************************************/

static CPLErrorContext *CPLGetErrorContext()

{
    CPLErrorContext *psCtx = 
        (CPLErrorContext *) CPLGetTLS( CTLS_ERRORCONTEXT );

    if( psCtx == NULL )
    {
        psCtx = (CPLErrorContext *) CPLCalloc(sizeof(CPLErrorContext),1);
        psCtx->eLastErrType = CE_None;
        psCtx->nLastErrMsgMax = sizeof(psCtx->szLastErrMsg);
        CPLSetTLS( CTLS_ERRORCONTEXT, psCtx, TRUE );
    }

    return psCtx;
}


/************************************************************************/
/*                              CPLDebug()                              */
/************************************************************************/

/**
 * Display a debugging message.
 *
 * The category argument is used in conjunction with the CPL_DEBUG
 * environment variable to establish if the message should be displayed.
 * If the CPL_DEBUG environment variable is not set, no debug messages
 * are emitted (use CPLError(CE_Warning,...) to ensure messages are displayed).
 * If CPL_DEBUG is set, but is an empty string or the word "ON" then all
 * debug messages are shown.  Otherwise only messages whose category appears
 * somewhere within the CPL_DEBUG value are displayed (as determinted by
 * strstr()).
 *
 * Categories are usually an identifier for the subsystem producing the
 * error.  For instance "GDAL" might be used for the GDAL core, and "TIFF"
 * for messages from the TIFF translator.  
 *
 * @param pszCategory name of the debugging message category.
 * @param pszFormat printf() style format string for message to display.
 *        Remaining arguments are assumed to be for format.
 */ 

void CPLDebug( const char * pszCategory, const char * pszFormat, ... )

{
    CPLErrorContext *psCtx = CPLGetErrorContext();
    char        *pszMessage;
    va_list     args;
    const char  *pszDebug = CPLGetConfigOption("CPL_DEBUG",NULL);

#define ERROR_MAX 25000

/* -------------------------------------------------------------------- */
/*      Does this message pass our current criteria?                    */
/* -------------------------------------------------------------------- */
    if( pszDebug == NULL )
        return;

    if( !EQUAL(pszDebug,"ON") && !EQUAL(pszDebug,"") )
    {
        size_t  i, nLen = strlen(pszCategory);

        for( i = 0; pszDebug[i] != '\0'; i++ )
        {
            if( EQUALN(pszCategory,pszDebug+i,nLen) )
                break;
        }

        if( pszDebug[i] == '\0' )
            return;
    }

/* -------------------------------------------------------------------- */
/*    Allocate a block for the error.                                   */
/* -------------------------------------------------------------------- */
    pszMessage = (char *) VSIMalloc( ERROR_MAX );
    if( pszMessage == NULL )
        return;
        
/* -------------------------------------------------------------------- */
/*      Dal -- always log a timestamp as the first part of the line     */
/*      to ensure one is looking at what one should be looking at!      */
/* -------------------------------------------------------------------- */

    pszMessage[0] = '\0';
#ifdef TIMESTAMP_DEBUG
    if( CPLGetConfigOption( "CPL_TIMESTAMP", NULL ) != NULL )
    {
        strcpy( pszMessage, VSICTime( VSITime(NULL) ) );
        
        // On windows anyway, ctime puts a \n at the end, but I'm not 
        // convinced this is standard behaviour, so we'll get rid of it
        // carefully

        if (pszMessage[strlen(pszMessage) -1 ] == '\n')
        {
            pszMessage[strlen(pszMessage) - 1] = 0; // blow it out
        }
        strcat( pszMessage, ": " );
    }
#endif

/* -------------------------------------------------------------------- */
/*      Add the category.                                               */
/* -------------------------------------------------------------------- */
    strcat( pszMessage, pszCategory );
    strcat( pszMessage, ": " );
    
/* -------------------------------------------------------------------- */
/*      Format the application provided portion of the debug message.   */
/* -------------------------------------------------------------------- */
    va_start(args, pszFormat);
#if defined(HAVE_VSNPRINTF)
    vsnprintf(pszMessage+strlen(pszMessage), ERROR_MAX - strlen(pszMessage), 
              pszFormat, args);
#else
    vsprintf(pszMessage+strlen(pszMessage), pszFormat, args);
#endif
    va_end(args);

/* -------------------------------------------------------------------- */
/*      Invoke the current error handler.                               */
/* -------------------------------------------------------------------- */
    if( psCtx->psHandlerStack != NULL )
    {
        psCtx->psHandlerStack->pfnHandler( CE_Debug, CPLE_None, pszMessage );
    }
    else
    {
       //да закрою тебя на х!!!ер 
       CPLMutexHolderD( &hErrorMutex );
        if( pfnErrorHandler != NULL )
            pfnErrorHandler( CE_Debug, CPLE_None, pszMessage );
    }

    VSIFree( pszMessage );
}

/************************************************************************/
/*                             CPLErrorV()                              */
/************************************************************************/

void    CPLErrorV(CPLErr eErrClass, int err_no, const char *fmt, va_list args )
{
    CPLErrorContext *psCtx = CPLGetErrorContext();

/* -------------------------------------------------------------------- */
/*      Expand the error message                                        */
/* -------------------------------------------------------------------- */
#if defined(HAVE_VSNPRINTF)
    {
        int nPR;
        va_list wrk_args;

#ifdef va_copy
        va_copy( wrk_args, args );
#else
        wrk_args = args;
#endif

        while( ((nPR = vsnprintf( psCtx->szLastErrMsg, 
                                 psCtx->nLastErrMsgMax, fmt, wrk_args )) == -1
                || nPR >= psCtx->nLastErrMsgMax-1)
               && psCtx->nLastErrMsgMax < 1000000 )
        {
#ifdef va_copy
            va_end( wrk_args );
            va_copy( wrk_args, args );
#else
            wrk_args = args;
#endif
            psCtx->nLastErrMsgMax *= 3;
            psCtx = (CPLErrorContext *) 
                CPLRealloc(psCtx, sizeof(CPLErrorContext) - DEFAULT_LAST_ERR_MSG_SIZE + psCtx->nLastErrMsgMax + 1);
            CPLSetTLS( CTLS_ERRORCONTEXT, psCtx, TRUE );
        }

        va_end( wrk_args );
    }
#else
    vsprintf( psCtx->szLastErrMsg, fmt, args);
#endif

/* -------------------------------------------------------------------- */
/*      If the user provided his own error handling function, then      */
/*      call it, otherwise print the error to stderr and return.        */
/* -------------------------------------------------------------------- */
    psCtx->nLastErrNo = err_no;
    psCtx->eLastErrType = eErrClass;

    if( CPLGetConfigOption("CPL_LOG_ERRORS",NULL) != NULL )
        CPLDebug( "CPLError", "%s", psCtx->szLastErrMsg );

/* -------------------------------------------------------------------- */
/*      Invoke the current error handler.                               */
/* -------------------------------------------------------------------- */
    if( psCtx->psHandlerStack != NULL )
    {
        psCtx->psHandlerStack->pfnHandler(eErrClass, err_no, 
                                          psCtx->szLastErrMsg);
    }
    else
    {
       //да закрою тебя на х!!!ер 
       CPLMutexHolderD( &hErrorMutex );
        if( pfnErrorHandler != NULL )
            pfnErrorHandler(eErrClass, err_no, psCtx->szLastErrMsg);
    }

    if( eErrClass == CE_Fatal )
        abort();
}


/************************************************************************/
/*                         CPLReadLineBuffer()                          */
/*                                                                      */
/*      Fetch readline buffer, and ensure it is the desired size,       */
/*      reallocating if needed.  Manages TLS (thread local storage)     */
/*      issues for the buffer.                                          */
/************************************************************************/
static char *CPLReadLineBuffer( int nRequiredSize )

{
    
/* -------------------------------------------------------------------- */
/*      A required size of -1 means the buffer should be freed.         */
/* -------------------------------------------------------------------- */
    if( nRequiredSize == -1 )
    {
        if( CPLGetTLS( CTLS_RLBUFFERINFO ) != NULL )
        {
            CPLFree( CPLGetTLS( CTLS_RLBUFFERINFO ) );
            CPLSetTLS( CTLS_RLBUFFERINFO, NULL, FALSE );
        }
        return NULL;
    }

/* -------------------------------------------------------------------- */
/*      If the buffer doesn't exist yet, create it.                     */
/* -------------------------------------------------------------------- */
    GUInt32 *pnAlloc = (GUInt32 *) CPLGetTLS( CTLS_RLBUFFERINFO );

    if( pnAlloc == NULL )
    {
        pnAlloc = (GUInt32 *) CPLMalloc(200);
        *pnAlloc = 196;
        CPLSetTLS( CTLS_RLBUFFERINFO, pnAlloc, TRUE );
    }

/* -------------------------------------------------------------------- */
/*      If it is too small, grow it bigger.                             */
/* -------------------------------------------------------------------- */
    if( (int) *pnAlloc < nRequiredSize+1 )
    {
        int nNewSize = nRequiredSize + 4 + 500;

        pnAlloc = (GUInt32 *) CPLRealloc(pnAlloc,nNewSize);
        if( pnAlloc == NULL )
        {
            CPLSetTLS( CTLS_RLBUFFERINFO, NULL, FALSE );
            return NULL;
        }
            
        *pnAlloc = nNewSize - 4;
        CPLSetTLS( CTLS_RLBUFFERINFO, pnAlloc, TRUE );
    }

    return (char *) (pnAlloc+1);
}

/************************************************************************/
/*                            CPLReadLine()                             */
/************************************************************************/

/**
 * Simplified line reading from text file.
 * 
 * Read a line of text from the given file handle, taking care
 * to capture CR and/or LF and strip off ... equivelent of
 * DKReadLine().  Pointer to an internal buffer is returned.
 * The application shouldn't free it, or depend on it's value
 * past the next call to CPLReadLine().
 * 
 * Note that CPLReadLine() uses VSIFGets(), so any hooking of VSI file
 * services should apply to CPLReadLine() as well.
 *
 * CPLReadLine() maintains an internal buffer, which will appear as a 
 * single block memory leak in some circumstances.  CPLReadLine() may 
 * be called with a NULL FILE * at any time to free this working buffer.
 *
 * @param fp file pointer opened with VSIFOpen().
 *
 * @return pointer to an internal buffer containing a line of text read
 * from the file or NULL if the end of file was encountered.
 */

const char *CPLReadLine( FILE * fp )

{
    char *pszRLBuffer = CPLReadLineBuffer(1);
    int         nReadSoFar = 0;

/* -------------------------------------------------------------------- */
/*      Cleanup case.                                                   */
/* -------------------------------------------------------------------- */
    if( fp == NULL )
    {
        CPLReadLineBuffer( -1 );
        return NULL;
    }

/* -------------------------------------------------------------------- */
/*      Loop reading chunks of the line till we get to the end of       */
/*      the line.                                                       */
/* -------------------------------------------------------------------- */
    int nBytesReadThisTime;

    do {
/* -------------------------------------------------------------------- */
/*      Grow the working buffer if we have it nearly full.  Fail out    */
/*      of read line if we can't reallocate it big enough (for          */
/*      instance for a _very large_ file with no newlines).             */
/* -------------------------------------------------------------------- */
        pszRLBuffer = CPLReadLineBuffer( nReadSoFar + 129 );
        if( pszRLBuffer == NULL )
            return NULL;

/* -------------------------------------------------------------------- */
/*      Do the actual read.                                             */
/* -------------------------------------------------------------------- */
        if( CPLFGets( pszRLBuffer+nReadSoFar, 128, fp ) == NULL 
            && nReadSoFar == 0 )
            return NULL;

        nBytesReadThisTime = strlen(pszRLBuffer+nReadSoFar);
        nReadSoFar += nBytesReadThisTime;

    } while( nBytesReadThisTime >= 127
             && pszRLBuffer[nReadSoFar-1] != 13
             && pszRLBuffer[nReadSoFar-1] != 10 );

    return( pszRLBuffer ); 
    
}

/************************************************************************/
/*                              CPLFGets()                              */
/*                                                                      */
/*      Note: CR = \r = ASCII 13                                        */
/*            LF = \n = ASCII 10                                        */
/************************************************************************/

/**
 * Reads in at most one less than nBufferSize characters from the fp
 * stream and stores them into the buffer pointed to by pszBuffer.
 * Reading stops after an EOF or a newline. If a newline is read, it
 * is _not_ stored into the buffer. A '\0' is stored after the last
 * character in the buffer. All three types of newline terminators
 * recognized by the CPLFGets(): single '\r' and '\n' and '\r\n'
 * combination.
 *
 * @param pszBuffer pointer to the targeting character buffer.
 * @param nBufferSize maximum size of the string to read (not including
 * termonating '\0').
 * @param fp file pointer to read from.
 * @return pointer to the pszBuffer containing a string read
 * from the file or NULL if the error or end of file was encountered.
 */

char *CPLFGets( char *pszBuffer, int nBufferSize, FILE * fp )

{
    int nActuallyRead, nOriginalOffset;

    if ( nBufferSize == 0 || pszBuffer == NULL || fp == NULL )
        return NULL;

/* -------------------------------------------------------------------- */
/*      Let the OS level call read what it things is one line.  This    */
/*      will include the newline.  On windows, if the file happens      */
/*      to be in text mode, the CRLF will have been converted to        */
/*      just the newline (LF).  If it is in binary mode it may well     */
/*      have both.                                                      */
/* -------------------------------------------------------------------- */
    nOriginalOffset = VSIFTell( fp );
    if( VSIFGets( pszBuffer, nBufferSize, fp ) == NULL )
        return NULL;
    
    nActuallyRead = strlen(pszBuffer);
    if ( nActuallyRead == 0 )
        return NULL;

/* -------------------------------------------------------------------- */
/*      If we found \r and out buffer is full, it is possible there     */
/*      is also a pending \n.  Check for it.                            */
/* -------------------------------------------------------------------- */
    if( nBufferSize == nActuallyRead+1
        && pszBuffer[nActuallyRead-1] == 13 )
    {
        int chCheck;
        chCheck = fgetc( fp );
        if( chCheck != 10 )
        {
            // unget the character.
            VSIFSeek( fp, nOriginalOffset+nActuallyRead, SEEK_SET );
        }
    }

/* -------------------------------------------------------------------- */
/*      Trim off \n, \r or \r\n if it appears at the end.  We don't     */
/*      need to do any "seeking" since we want the newline eaten.       */
/* -------------------------------------------------------------------- */
    if( nActuallyRead > 1 
        && pszBuffer[nActuallyRead-1] == 10 
        && pszBuffer[nActuallyRead-2] == 13 )
    {
        pszBuffer[nActuallyRead-2] = '\0';
    }
    else if( pszBuffer[nActuallyRead-1] == 10 
             || pszBuffer[nActuallyRead-1] == 13 )
    {
        pszBuffer[nActuallyRead-1] = '\0';
    }

/* -------------------------------------------------------------------- */
/*      Search within the string for a \r (MacOS convention             */
/*      apparently), and if we find it we need to trim the string,      */
/*      and seek back.                                                  */
/* -------------------------------------------------------------------- */
    char *pszExtraNewline = strchr( pszBuffer, 13 );
    
    if( pszExtraNewline != NULL )
    {
        int chCheck;

        nActuallyRead = pszExtraNewline - pszBuffer + 1;
        
        *pszExtraNewline = '\0';
        VSIFSeek( fp, nOriginalOffset + nActuallyRead - 1, SEEK_SET );

        /* 
         * This hackery is necessary to try and find our correct
         * spot on win32 systems with text mode line translation going 
         * on.  Sometimes the fseek back overshoots, but it doesn't
         * "realize it" till a character has been read. Try to read till
         * we get to the right spot and get our CR. 
         */ 
        chCheck = fgetc( fp );
        while( (chCheck != 13 && chCheck != EOF)
               || VSIFTell(fp) < nOriginalOffset + nActuallyRead )
        {
            static volatile int bWarned = FALSE;

            if( !bWarned )
            {
                bWarned = TRUE;
                CPLDebug( "CPL", "CPLFGets() correcting for DOS text mode translation seek problem." );
            }
            chCheck = fgetc( fp );
        }
    }

    return pszBuffer;
}


/************************************************************************/
/*                       CPLVerifyConfiguration()                       */
/************************************************************************/

void CPLVerifyConfiguration()

{
/* -------------------------------------------------------------------- */
/*      Verify data types.                                              */
/* -------------------------------------------------------------------- */
 // //   CPLAssert( sizeof(GInt32) == 4 );
 // //   CPLAssert( sizeof(GInt16) == 2 );
// //    CPLAssert( sizeof(GByte) == 1 );

    if( sizeof(GInt32) != 4 )
        CPLError( CE_Fatal, CPLE_AppDefined, 
                  "sizeof(GInt32) == %d ... yow!\n", 
                  sizeof(GInt32) );

/* -------------------------------------------------------------------- */
/*      Verify byte order                                               */
/* -------------------------------------------------------------------- */
    GInt32   nTest;

    nTest = 1;

#ifdef CPL_LSB
    if( ((GByte *) &nTest)[0] != 1 )
#endif
#ifdef CPL_MSB
    if( ((GByte *) &nTest)[3] != 1 )
#endif    
        CPLError( CE_Fatal, CPLE_AppDefined, 
                  "CPLVerifyConfiguration(): byte order set wrong.\n" );
}


/************************************************************************/
/*                         CSLTokenizeString2()                         */
/*                                                                      */
/*      The ultimate tokenizer?                                         */
/************************************************************************/

char ** CSLTokenizeString2( const char * pszString,
                            const char * pszDelimiters,
                            int nCSLTFlags )

{
    char        **papszRetList = NULL;
    int         nRetMax = 0, nRetLen = 0;
    char        *pszToken;
    int         nTokenMax, nTokenLen;
    int         bHonourStrings = (nCSLTFlags & CSLT_HONOURSTRINGS);
    int         bAllowEmptyTokens = (nCSLTFlags & CSLT_ALLOWEMPTYTOKENS);

    pszToken = (char *) CPLCalloc(10,1);
    nTokenMax = 10;
    
    while( pszString != NULL && *pszString != '\0' )
    {
        int     bInString = FALSE;

        nTokenLen = 0;
        
        /* Try to find the next delimeter, marking end of token */
        for( ; *pszString != '\0'; pszString++ )
        {

            /* End if this is a delimeter skip it and break. */
            if( !bInString && strchr(pszDelimiters, *pszString) != NULL )
            {
                pszString++;
                break;
            }
            
            /* If this is a quote, and we are honouring constant
               strings, then process the constant strings, with out delim
               but don't copy over the quotes */
            if( bHonourStrings && *pszString == '"' )
            {
                if( nCSLTFlags & CSLT_PRESERVEQUOTES )
                {
                    pszToken[nTokenLen] = *pszString;
                    nTokenLen++;
                }

                if( bInString )
                {
                    bInString = FALSE;
                    continue;
                }
                else
                {
                    bInString = TRUE;
                    continue;
                }
            }

            /* Within string constants we allow for escaped quotes, but
               in processing them we will unescape the quotes */
            if( bInString && pszString[0] == '\\' && pszString[1] == '"' )
            {
                if( nCSLTFlags & CSLT_PRESERVEESCAPES )
                {
                    pszToken[nTokenLen] = *pszString;
                    nTokenLen++;
                }

                pszString++;
            }

            /* Within string constants a \\ sequence reduces to \ */
            else if( bInString 
                     && pszString[0] == '\\' && pszString[1] == '\\' )
            {
                if( nCSLTFlags & CSLT_PRESERVEESCAPES )
                {
                    pszToken[nTokenLen] = *pszString;
                    nTokenLen++;
                }
                pszString++;
            }

            if( nTokenLen >= nTokenMax-3 )
            {
                nTokenMax = nTokenMax * 2 + 10;
                pszToken = (char *) CPLRealloc( pszToken, nTokenMax );
            }

            pszToken[nTokenLen] = *pszString;
            nTokenLen++;
        }

        pszToken[nTokenLen] = '\0';

        /*
         * Add the token.
         */
        if( pszToken[0] != '\0' || bAllowEmptyTokens )
        {
            if( nRetLen >= nRetMax - 1 )
            {
                nRetMax = nRetMax * 2 + 10;
                papszRetList = (char **) 
                    CPLRealloc(papszRetList, sizeof(char*) * nRetMax );
            }

            papszRetList[nRetLen++] = CPLStrdup( pszToken );
            papszRetList[nRetLen] = NULL;
        }
    }

    /*
     * If the last token was empty, then we need to capture
     * it now, as the loop would skip it.
     */
    if( *pszString == '\0' && bAllowEmptyTokens && nRetLen > 0 
        && strchr(pszDelimiters,*(pszString-1)) != NULL )
    {
        if( nRetLen >= nRetMax - 1 )
        {
            nRetMax = nRetMax * 2 + 10;
            papszRetList = (char **) 
                CPLRealloc(papszRetList, sizeof(char*) * nRetMax );
        }

        papszRetList[nRetLen++] = CPLStrdup("");
        papszRetList[nRetLen] = NULL;
    }

    if( papszRetList == NULL )
        papszRetList = (char **) CPLCalloc(sizeof(char *),1);

    CPLFree( pszToken );

    return papszRetList;
}

/**********************************************************************
 *                       CSLTokenizeString()
 *
 * Tokenizes a string and returns a StringList with one string for
 * each token.
 **********************************************************************/
char    **CSLTokenizeString( const char *pszString )
{
    return CSLTokenizeString2( pszString, " ", CSLT_HONOURSTRINGS );
}
/************************************************************************/
/*                      CSLTokenizeStringComplex()                      */
/*                                                                      */
/*      Obsolete tokenizing api.                                        */
/************************************************************************/

char ** CSLTokenizeStringComplex( const char * pszString,
                                  const char * pszDelimiters,
                                  int bHonourStrings, int bAllowEmptyTokens )

{
    int         nFlags = 0;

    if( bHonourStrings )
        nFlags |= CSLT_HONOURSTRINGS;
    if( bAllowEmptyTokens )
        nFlags |= CSLT_ALLOWEMPTYTOKENS;

    return CSLTokenizeString2( pszString, pszDelimiters, nFlags );
}


/**********************************************************************
 *                       CSLCount()
 *
 * Return the number of lines in a Stringlist.
 **********************************************************************/
int CSLCount(char **papszStrList)
{
    int nItems=0;

    if (papszStrList)
    {
        while(*papszStrList != NULL)
        {
            nItems++;
            papszStrList++;
        }
    }

    return nItems;
}

/**********************************************************************
 *                       CSLDestroy()
 *
 * Free all memory used by a StringList.
 **********************************************************************/
void CPL_STDCALL CSLDestroy(char **papszStrList)
{
    char **papszPtr;

    if (papszStrList)
    {
        papszPtr = papszStrList;
        while(*papszPtr != NULL)
        {
            CPLFree(*papszPtr);
            papszPtr++;
        }

        CPLFree(papszStrList);
    }
}


// Мьютекс, мультипроц !!!!!! Это вам не Подбельский 
/************************************************************************/
/*                           CPLMutexHolder()                           */
/************************************************************************/

CPLMutexHolder::CPLMutexHolder( void **phMutex, double dfWaitInSeconds,
                                const char *pszFileIn, 
                                int nLineIn )

{
#ifndef MUTEX_NONE
    pszFile = pszFileIn;
    nLine = nLineIn;

#ifdef DEBUG_MUTEX
    CPLDebug( "MH", "Request %p for pid %d at %d/%s", 
              *phMutex, CPLGetPID(), nLine, pszFile );
#endif

    if( !CPLCreateOrAcquireMutex( phMutex, dfWaitInSeconds ) )
    {
        CPLDebug( "CPLMutexHolder", "failed to acquire mutex!" );
        hMutex = NULL;
    }
    else
    {
#ifdef DEBUG_MUTEX
        CPLDebug( "MH", "Acquired %p for pid %d at %d/%s", 
                  *phMutex, CPLGetPID(), nLine, pszFile );
#endif

        hMutex = *phMutex;
    }
#endif /* ndef MUTEX_NONE */
}

/************************************************************************/
/*                          ~CPLMutexHolder()                           */
/************************************************************************/

CPLMutexHolder::~CPLMutexHolder()

{
#ifndef MUTEX_NONE
    if( hMutex != NULL )
    {
#ifdef DEBUG_MUTEX
        CPLDebug( "MH", "Release %p for pid %d at %d/%s", 
                  hMutex, CPLGetPID(), nLine, pszFile );
#endif
        CPLReleaseMutex( hMutex );
    }
#endif /* ndef MUTEX_NONE */
}


/************************************************************************/
/*                           CPLCreateMutex()                           */
/************************************************************************/

void *CPLCreateMutex()

{
#ifndef MUTEX_NONE
    unsigned char *pabyMutex = (unsigned char *) CPLMalloc( 4 );

    pabyMutex[0] = 1;
    pabyMutex[1] = 'r';
    pabyMutex[2] = 'e';
    pabyMutex[3] = 'd';

    return (void *) pabyMutex;
#else
    return (void *) 0xdeadbeef;
#endif 
}



/************************************************************************/
/*                          CPLAcquireMutex()                           */
/************************************************************************/

int CPLAcquireMutex( void *hMutex, double dfWaitInSeconds )

{
#ifndef MUTEX_NONE
    unsigned char *pabyMutex = (unsigned char *) hMutex;

    // //CPLAssert( pabyMutex[1] == 'r' && pabyMutex[2] == 'e'   && pabyMutex[3] == 'd' );

    pabyMutex[0] += 1;

    return TRUE;
#else
    return TRUE;
#endif
}



/************************************************************************/
/*                      CPLCreateOrAcquireMutex()                       */
/************************************************************************/

int CPLCreateOrAcquireMutex( void **phMutex, double dfWaitInSeconds )

{
#ifndef MUTEX_NONE
    static void *hCOAMutex = NULL;

    /*
    ** ironically, creation of this initial mutex is not threadsafe
    ** even though we use it to ensure that creation of other mutexes
    ** is threadsafe. 
    */
    if( hCOAMutex == NULL )
    {
        hCOAMutex = CPLCreateMutex();
    }
    else
    {
        CPLAcquireMutex( hCOAMutex, dfWaitInSeconds );
    }

    if( *phMutex == NULL )
    {
        *phMutex = CPLCreateMutex();
        CPLReleaseMutex( hCOAMutex );
        return TRUE;
    }
    else
    {
        CPLReleaseMutex( hCOAMutex );

        int bSuccess = CPLAcquireMutex( *phMutex, dfWaitInSeconds );
        
        return bSuccess;
    }
#endif /* ndef MUTEX_NONE */

    return TRUE;
}

/************************************************************************/
/*                          CPLReleaseMutex()                           */
/************************************************************************/

void CPLReleaseMutex( void *hMutex )

{
#ifndef MUTEX_NONE
    unsigned char *pabyMutex = (unsigned char *) hMutex;

    // // CPLAssert( pabyMutex[1] == 'r' && pabyMutex[2] == 'e'                && pabyMutex[3] == 'd' );

    if( pabyMutex[0] < 1 )
        CPLDebug( "CPLMultiProc", 
                  "CPLReleaseMutex() called on mutex with %d as ref count!",
                  pabyMutex[0] );

    pabyMutex[0] -= 1;
#endif
}



} // namespace cpl
} //namespace fteo