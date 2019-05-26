/* 
    File: frame_pool.H

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 09/03/05

    Description: Management of the Free-Frame Pool.
    

*/

#ifndef _FRAME_POOL_H_                   // include file only once
#define _FRAME_POOL_H_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* F r a m e   P o o l  */
/*--------------------------------------------------------------------------*/

class FramePool {

public:

   FramePool();   
   /* Initializes the data structures needed for the management of the 
      free frame pool. This function must be called before the paging system 
      is initialized. */ 

   unsigned long get_frame(); 
   /* Allocates a frame from the frame pool. If successful, returns the physical 
      address of the frame. If fails, returns 0x0. */ 

   void release_frame(unsigned long _frame_address); 
   /* Releases frame back to the given frame pool. 
      The frame is identified by the physical address. */ 

};
#endif