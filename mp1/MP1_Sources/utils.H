/*
     File        : utils.H

     Author      : Riccardo Bettati
     Modified    : 2017/05/02

     Description : Various definitions (NULL) and
                   utility functions (e.g. abort, memory and 
                   string functions).
*/

#ifndef _utils_H_
#define _utils_H_

/*---------------------------------------------------------------*/
/* GENERAL CONSTANTS */

#ifndef NULL
#   define NULL 0
#endif

/*---------------------------------------------------------------*/
/* ABORT */

void abort();
/* Stop execution. */

/*---------------------------------------------------------------*/
/* SIMPLE MEMORY OPERATIONS */

char * memcpy(char * _dest, const char * _src, const int _count);
/* Copy _count bytes from _src to _dest. (No check for uverlapping) */

char *memset(char * _dest, char _val, const int _count);
/* Set _count bytes to value _val, starting from location _dest. */

unsigned short *memsetw(      unsigned short * _dest, 
                        const unsigned short   _val, 
                        const          int     _count);
/* Same as above, but operations are 16-bit wide. */

/*---------------------------------------------------------------*/
/* SIMPLE STRING OPERATIONS (STRINGS ARE NULL-TERMINATED) */

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

char inportb  (unsigned short _port);
/* Read data from input port _port.*/

void outportb (unsigned short _port, char _data);
/* Write _data to output port _port.*/


#endif



