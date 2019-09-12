//===========================================================================
//@2017 Fixosoft    FIXOSOFT FTEO C++ NATIVE LIBRARY
//===========================================================================
/*//////////////////////////////////////////////////////////////////////////*
 
* fteo_cpl.h - заголовник для функций библиотеки CPL - Common Portability Library

* Purpose cpl:  Convenience functions declarations. Удобные функции, лучше не скажешь.
*               This is intended to remain light weight.
* Author cpl :  Frank Warmerdam, warmerdam@pobox.com
 
 
///////////////////////////////////////////////////////////////////////////*/

#include <stdio.h>  // FILE etc
#include <stdlib.h> // alloc etc
#include <string.h>
#include <stdarg.h> // va_start etc

#ifndef _INC_FTEO_CPL
#define _INC_FTEO_CPL

namespace fteo
{
  namespace cpl
{

#ifdef VSI_DEBUG

#ifndef DEBUG
#  define DEBUG
#endif

//#include "cpl_error.h"

#define VSIDebug4(f,a1,a2,a3,a4)   CPLDebug( "VSI", f, a1, a2, a3, a4 );
#define VSIDebug3( f, a1, a2, a3 ) CPLDebug( "VSI", f, a1, a2, a3 );
#define VSIDebug2( f, a1, a2 )     CPLDebug( "VSI", f, a1, a2 );
#define VSIDebug1( f, a1 )         CPLDebug( "VSI", f, a1 );
#else
#define VSIDebug4( f, a1, a2, a3, a4 ) {}
#define VSIDebug3( f, a1, a2, a3 ) {}
#define VSIDebug2( f, a1, a2 )     {}
#define VSIDebug1( f, a1 )         {}
#endif


#ifndef FALSE
#  define FALSE 0
#endif

#ifndef TRUE
#  define TRUE  1
#endif

#ifndef MAX
#  define MIN(a,b)      ((a<b) ? a : b)
#  define MAX(a,b)      ((a>b) ? a : b)
#endif


typedef unsigned char   GByte;
typedef int    GBool;
typedef unsigned long   GUInt32;

#if UINT_MAX == 65535
typedef long            GInt32;
typedef unsigned long   GUInt32;
#else
typedef int             GInt32;
//typedef unsigned int    GUInt32;
#endif


#ifndef CPL_DLL
 #if defined(_MSC_VER) && !defined(CPL_DISABLE_DLL)
 #  define CPL_DLL     __declspec(dllexport)
   #else
     #  define CPL_DLL
  #endif
#endif

#ifndef CPL_THREADLOCAL 
#  define CPL_THREADLOCAL 
#endif


/*
#ifdef DEBUG
#  define CPLAssert(expr)  ((expr) ? (void)(0) : _CPLAssert(#expr,__FILE__,__LINE__))
#else
#  define CPLAssert(expr)
#endif
*/
 #ifndef CPL_STDCALL
#if defined(_MSC_VER) && !defined(CPL_DISABLE_STDCALL)
#  define CPL_STDCALL     __stdcall
#else
#  define CPL_STDCALL
#endif
#endif


/*---------------------------------------------------------------------
 *
 *                      Сокращенный VSI
 *
 *--------------------------------------------------------------------*/
int  VSIFEof( FILE * fp );
void VSIFree( void * pData );
FILE *VSIFOpen( const char * pszFilename, const char * pszAccess );
int CPL_DLL     VSIFClose( FILE * );
long CPL_DLL    VSIFTell( FILE * );
char CPL_DLL   *VSIFGets( char *, int, FILE * );
int CPL_DLL     VSIFSeek( FILE *, long, int );
void CPL_DLL    VSIRewind( FILE * );
char *VSIStrdup( const char * pszString );


char ** CSLTokenizeStringComplex( const char * pszString,
                                  const char * pszDelimiters,
                                  int bHonourStrings, int bAllowEmptyTokens );
char **CSLTokenizeString( const char *pszString );                                  

int   CSLCount(char **papszStrList);
/* ==================================================================== */
/*      Memory allocation                                               */
/* ==================================================================== */

void CPL_DLL   *VSICalloc( size_t, size_t );
void CPL_DLL   *VSIMalloc( size_t );
void CPL_DLL   *VSIRealloc( void *, size_t );

/*------------------------------------------------------------------------------------
 *
 *                      Сокращенный CPL
 *
 *------------------------------------------------------------------------------------*/

#ifndef EQUAL
#if defined(WIN32) || defined(WIN32CE)
#  define EQUALN(a,b,n)           (strnicmp(a,b,n)==0)
#  define EQUAL(a,b)              (stricmp(a,b)==0)
#else
#  define EQUALN(a,b,n)           (strncasecmp(a,b,n)==0)
#  define EQUAL(a,b)              (strcasecmp(a,b)==0)
#endif
#endif


#define CPLFree VSIFree
#if !defined(HAVE_VSNPRINTF)
#  define DEFAULT_LAST_ERR_MSG_SIZE 20000
#else
#  define DEFAULT_LAST_ERR_MSG_SIZE 500
#endif

// -- multiproc:
static void *hConfigMutex = NULL;
static volatile char **papszConfigOptions = NULL;

//#ifdef __cplusplus
#define CPLMutexHolderD(x)  CPLMutexHolder oHolder(x,1000.0,__FILE__,__LINE__);
class CPL_DLL CPLMutexHolder
{
  private:
    void       *hMutex;
    const char *pszFile;
    int         nLine;

  public:

    CPLMutexHolder( void **phMutex, double dfWaitInSeconds = 1000.0,
                    const char *pszFile = __FILE__,
                    int nLine = __LINE__ );
    ~CPLMutexHolder();
};
//#endif // def __cplusplus 
// -- multiproc:
void  CPL_DLL *CPLCreateMutex();
int   CPL_DLL CPLCreateOrAcquireMutex( void **, double dfWaitInSeconds );
int   CPL_DLL CPLAcquireMutex( void *hMutex, double dfWaitInSeconds );
void  CPL_DLL CPLReleaseMutex( void *hMutex );

#define CTLS_RLBUFFERINFO     		1         /* cpl_conv.cpp */
#define CTLS_ERRORCONTEXT           5         /* cpl_error.cpp */
#define CTLS_MAX                    32         



void CPL_DLL * CPLGetTLS( int nIndex );
void CPL_DLL   CPLSetTLS( int nIndex, void *pData, int bFreeOnExit );

 typedef enum
{
    CE_None = 0,
    CE_Debug = 1,
    CE_Warning = 2,
    CE_Failure = 3,
    CE_Fatal = 4
} CPLErr;

typedef void (CPL_STDCALL *CPLErrorHandler)(CPLErr, int, const char*);
static void *hErrorMutex = NULL;
void CPL_DLL CPL_STDCALL CPLDefaultErrorHandler( CPLErr, int, const char * );
static CPLErrorHandler pfnErrorHandler = CPLDefaultErrorHandler;
const char CPL_DLL * CPL_STDCALL CPLGetConfigOption( const char *, const char * );
const char CPL_DLL *  CSLFetchNameValue(char **papszStrList, const char *pszName);
void       CPL_DLL   CPL_STDCALL CPLDebug( const char *, const char *, ... );

typedef struct errHandler
{
    struct errHandler   *psNext;
    CPLErrorHandler     pfnHandler;
} CPLErrorHandlerNode;

typedef struct {
    int     nLastErrNo;
    CPLErr  eLastErrType;
    CPLErrorHandlerNode *psHandlerStack;
    int     nLastErrMsgMax;
    char    szLastErrMsg[DEFAULT_LAST_ERR_MSG_SIZE];
} CPLErrorContext;
 
 
char *CPLStrdup( const char * pszString);
void CPL_STDCALL CSLDestroy(char **papszStrList);


//void CPL_DLL CPL_STDCALL CSLDestroy(char **papszStrList);
char *GetBasename(const char *pszFname);

void CPL_DLL CPLError(CPLErr eErrClass, int err_no, const char *fmt, ...);
void CPL_DLL CPLErrorV(CPLErr, int, const char *, va_list );
static char *CPLReadLineBuffer( int nRequiredSize );
const  char CPL_DLL *CPLReadLine( FILE * );
char CPL_DLL *CPLFGets( char *, int, FILE *);
void CPL_DLL CPLVerifyConfiguration(void);
int  CPL_DLL CSLFindString( char **, const char * );
const char CPL_DLL *CPLSPrintf(const char *fmt, ...);
char CPL_DLL **CSLTokenizeString2( const char *pszString, 
                                   const char *pszDelimeter, 
                                   int nCSLTFlags );
// ==================================================================== 
//      Well known error codes.                                         
// ==================================================================== 

#define CPLE_None                       0
#define CPLE_AppDefined                 1
#define CPLE_OutOfMemory                2
#define CPLE_FileIO                     3
#define CPLE_OpenFailed                 4
#define CPLE_IllegalArg                 5
#define CPLE_NotSupported               6

#define CPLE_AssertionFailed            7

#define CPLE_NoWriteAccess              8
#define CPLE_UserInterrupt              9

/* 100 - 299 reserved for GDAL */

/* -------------------------------------------------------------------- */
/*      Safe malloc() API.  Thin cover over VSI functions with fatal    */
/*      error reporting if memory allocation fails.                     */
/* -------------------------------------------------------------------- */
void CPL_DLL *CPLCalloc( size_t, size_t );
void CPL_DLL *CPLRealloc( void *, size_t );
void CPL_DLL *CPLMalloc( size_t );

#define CSLT_HONOURSTRINGS      0x0001
#define CSLT_ALLOWEMPTYTOKENS   0x0002
#define CSLT_PRESERVEQUOTES     0x0004
#define CSLT_PRESERVEESCAPES    0x0008






} // namespace cpl
} //namespace fteo
#endif  /* _INC_FTEO_CPL */