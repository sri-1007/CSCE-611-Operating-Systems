/*
    File: mem_pool.H

    Author: R. Bettati, Nikhil Gupta
            Department of Computer Science
            Texas A&M University
    Date  : 11/27/2011

    Description: Management of the Contiguous-Memory Pool

    This Memory Pool operates on physical memory only. With
    few changes it can be adapted to virtual memory as well (see
    VMPool for this.)

*/

#ifndef _MEM_POOL_H_                   // include file only once
#define _MEM_POOL_H_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "utils.H"
#include "frame_pool.H"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

/* (none) */

/*--------------------------------------------------------------------------*/
/* M e m  P o o l  */
/*--------------------------------------------------------------------------*/

class MemPool { /* Contiguous-Memory Pool */

private:
   unsigned long start_address;

public:
   MemPool(FramePool * _frame_pool, int _n_frames);
   /* Allocates n_frames frames from the given frame pool for this memory pool. */

   unsigned long allocate(unsigned long _size);
   /* Allocates a region of _size bytes of memory from the
    * memory pool. If successful, returns the virtual address of the
    * start of the allocated region of memory. If fails, returns 0. */

   void release(unsigned long _start_address);
   /* Releases a region of previously allocated memory. The region
    * is identified by its start address, which was returned when the
    * region was allocated. */
};

#endif
