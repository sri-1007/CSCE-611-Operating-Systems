/*
     File        : utils.H

     Author      : Riccardo Bettati
     Modified    : 09/02/12

     Description : Various definitions (NULL) and
                   utility functions (e.g. abort, memory and 
                   string functions).
*/

#ifndef _utils_h_
#define _utils_h_

/*---------------------------------------------------------------*/
/* GENERAL CONSTANTS */
/*---------------------------------------------------------------*/

#ifndef NULL
#   define NULL 0
#endif

/*---------------------------------------------------------------*/
/* ABORT */
/*---------------------------------------------------------------*/

void abort();
/* Stop execution. */

/*---------------------------------------------------------------*/
/* SIMPLE MEMORY OPERATIONS */
/*---------------------------------------------------------------*/

void *memcpy(void *dest, const void *src, int count);
/* Copy _count bytes from _src to _dest. (No check for uverlapping) */

void *memset(void *dest, char val, int count);
/* Set _count bytes to value _val, starting from location _dest. */

unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);
/* Same as above, but operations are 16-bit wide. */

/*---------------------------------------------------------------*/
/* SIMPLE STRING OPERATIONS (STRINGS ARE NULL-TERMINATED) */
/*---------------------------------------------------------------*/

int strlen(const char * _str);
/* Determine the length of null-terminated string. */

void strcpy(char * _dst, char * _src);
/* Copy null-terminated string from _src to _dst. */

void int2str(int _num, char * _str);
/* Convert int to null-terminated string. */

void uint2str(unsigned int _num, char * _str);
/* Convert unsigned int to null-terminated string. */

/*---------------------------------------------------------------*/
/* PORT I/O OPERATIONS */
/*---------------------------------------------------------------*/

char inportb  (unsigned short _port);
unsigned short inportw (unsigned short _port);
/* Read data from input port _port.*/

void outportb (unsigned short _port, char _data);
void outportw (unsigned short _port, unsigned short _data);
/* Write _data to output port _port.*/


#endif



