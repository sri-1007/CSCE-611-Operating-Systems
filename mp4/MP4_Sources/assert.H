/* 
    File: assert.H

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University

    Date  : 05/01/23

    Header file for the "assert" macro.


*/

#ifndef __assert_H__
#define __assert_H__
 
/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "utils.H"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* "ASSERT" MACRO */ 
/*--------------------------------------------------------------------------*/

/* NOTE: The "assert" macros can be turned off by giving the -DNDEBUG
   argument when compiling. */

#ifdef assert
#  undef assert
#endif

void _assert ( const char* _file, const int _line, const char* _message );
 
#ifdef NDEBUG        
#  define assert( m ) ( ( void ) 0 )
#else
#  define assert( m )                           \
   if ( !(m) ) _assert( __FILE__, __LINE__, #m );
#endif

#endif
