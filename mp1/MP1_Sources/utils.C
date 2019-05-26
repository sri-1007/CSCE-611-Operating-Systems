/* 
    File: utils.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University

    Date  : 09/02/12

*/

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

char * memcpy(char * _dest, const char * _src, const int _count) {
    for (int i = 0; i < _count; i++) {
	*_dest = *_src;
        _dest++;
  	_src++;
    }
    
    return _dest-1;
}

char *memset(char * _dest, const char _val, const int _count) {
    for (int i = 0; i < _count; i++) {
	*_dest = _val;
	_dest++;
    }
    return _dest-1;
}

unsigned short *memsetw(      unsigned short * _dest, 
                        const unsigned short   _val, 
                        const          int     _count) {
    for (int i = 0; i < _count; i++) {
	*_dest = _val;
	_dest++;
    }
    return _dest-1;    
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
        char    c, temp[11];

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
        char    c, temp[11];

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

/*--------------------------------------------------------------------------*/
/* POERT I/O OPERATIONS  */ 
/*--------------------------------------------------------------------------*/

/* We will use this later on for reading from the I/O ports to get data
*  from devices such as the keyboard. We are using what is called
*  'inline assembly' in these routines to actually do the work */
char inportb (unsigned short _port) {
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

/* We will use this to write to I/O ports to send bytes to devices. This
*  will be used in the next tutorial for changing the textmode cursor
*  position. Again, we use some inline assembly for the stuff that simply
*  cannot be done in C */
void outportb (unsigned short _port, char _data) {
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

