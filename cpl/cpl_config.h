
/* Define if you don't have vprintf but do have _doprnt.  */
#undef HAVE_DOPRNT

/* Define if you have the vprintf function.  */
#define HAVE_VPRINTF
#define HAVE_VSNPRINTF
//#define vsnprintf _vsnprintf

//#define HAVE_SNPRINTF
//#define snprintf _snprintf

/* Define if you have the ANSI C header files.  */
#ifndef STDC_HEADERS
#  define STDC_HEADERS
#endif

/* Define if you have the <fcntl.h> header file.  */
#undef HAVE_FCNTL_H

/* Define if you have the <unistd.h> header file.  */
#undef HAVE_UNISTD_H

/* Define if you have the <stdint.h> header file.  */
#undef HAVE_STDINT_H

#undef HAVE_LIBDL 

#undef HAVE_DLFCN_H
#undef HAVE_DBMALLOC_H
#undef HAVE_LIBDBMALLOC
#undef WORDS_BIGENDIAN

#undef CPL_DISABLE_DLL

/* Define to 1 if you have the <errno.h> header file. */
#define HAVE_ERRNO_H 1
