/* 
    File: utils.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University

    Date  : 09/02/12

*/

/* Some of the code comes from Brandon Friesens OS Tutorial: 
*  bkerndev - Bran's Kernel Development Tutorial
*  By:   Brandon F. (friesenb@gmail.com)
*  Desc: Interrupt Descriptor Table management
*
*  Notes: No warranty expressed or implied. Use at own risk. */


/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

  /* -- (none ) -- */

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
/* ABORT (USED e.g. IN _ASSERT()  */ 
/*--------------------------------------------------------------------------*/

void abort() {
  for(;;);
}

/*--------------------------------------------------------------------------*/
/* MEMORY OPERATIONS  */ 
/*--------------------------------------------------------------------------*/

void *memcpy(void *dest, const void *src, int count)
{
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for(; count != 0; count--) *dp++ = *sp++;
    return dest;
}

void *memset(void *dest, char val, int count)
{
    char *temp = (char *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, int count)
{
    unsigned short *temp = (unsigned short *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

/*--------------------------------------------------------------------------*/
/* STRING OPERATIONS  */ 
/*--------------------------------------------------------------------------*/


int strlen(const char *_str) {
    /* This loops through character array 'str', returning how
    *  many characters it needs to check before it finds a 0.
    *  In simple words, it returns the length in bytes of a string */
    int len = 0;
    while (*_str != 0) {
	_str++;
	len++;
    }
    return len;
}
void strcpy(char* _dst, char* _src) {
    while (*_src != 0) {
        *_dst = *_src;
        _dst++;
        _src++;
    }
    *_dst = 0;  // put terminating 0 at end.
}

void int2str(int _num, char * _str) {
        /* -- THIS IMPLEMENTATION IS ONE PRETTY BAD HACK. */
        int     i;
        char    temp[11];

        temp[0] = '\0';
        for(i = 1; i <= 10; i++)  {
                temp[i] = _num % 10 + '0';
                _num /= 10;
        }
        for(i = 10; temp[i] == '0'; i--);
        if( i == 0 )
                i++;
        while( i >= 0 )
                *_str++ = temp[i--];
}


void uint2str(unsigned int _num, char * _str) {
        /* -- THIS IS A BAD HACK AS WELL. */
        int     i;
        char    temp[11];

        temp[0] = '\0';
        for(i = 1; i <= 10; i++)  {
                temp[i] = _num % 10 + '0';
                _num /= 10;
        }
        for(i = 10; temp[i] == '0'; i--);
        if( i == 0 )
                i++;
        while( i >= 0 )
                *_str++ = temp[i--];
}

