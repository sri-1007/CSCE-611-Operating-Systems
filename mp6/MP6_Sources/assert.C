/* 
    File: assert.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University

    Date  : 05/01/23

    Implementation of the assert() function. 

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "assert.H"

#include "utils.H"
#include "console.H"

/*--------------------------------------------------------------------------*/
/* _assert() FUNCTION: gets called when assert() macro fails. */
/*--------------------------------------------------------------------------*/

void _assert (const char* _file, const int _line, const char* _message )  {
  /* Prints current file, line number, and failed assertion. */
  char temp[15];
  Console::puts("Assertion failed at file: ");
  Console::puts(_file);
  Console::puts(" line: ");
  int2str(_line, temp);
  Console::puts(temp);
  Console::puts(" assertion: ");
  Console::puts(_message);
  Console::puts("\n");
  abort();
}/* end _assert */
