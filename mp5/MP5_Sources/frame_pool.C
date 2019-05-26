/* 
    File: frame_pool.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 09/05/13

    Implementation of the manager for the Free-Frame Pool.

    THIS IS A DUMMY IMPLEMENTATION ONLY! 
    IT HAS BEEN IMPLEMENTED AS POORLY AS HUMANLY POSSIBLE.
    DO NOT TRY TO USE OR IMITATE IN ANY WAY!!

    NOTE: THIS IMPLEMENTATION SUPPORTS THE CREATION OF ONLY ONE FRAME POOL!!

*/

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "utils.H"
#include "machine.H"
#include "console.H"

#include "frame_pool.H"

/*--------------------------------------------------------------------------*/
/* LOCAL VARIABLES */
/*--------------------------------------------------------------------------*/

static unsigned long next_free_frame;

/*--------------------------------------------------------------------------*/
/* F r a m e   P o o l  */
/*--------------------------------------------------------------------------*/

FramePool::FramePool() {
  next_free_frame = 0x200000; /* 2 MB */
}     


unsigned long FramePool::get_frame() {
/* Allocates a frame from the frame pool. If successful, returns the physical 
   address of the frame. If fails, returns 0x0. */ 

//  Console::puts("FramePool:next_free_frame = "); Console::putui(next_free_frame); Console::puts("\n");
  unsigned long new_frame = next_free_frame;

  next_free_frame += Machine::PAGE_SIZE;

  return new_frame;

}
 

void FramePool::release_frame(unsigned long   _frame_address) {
/* Releases frame back to the given frame pool. 
   The frame is identified by the physical address. */ 

   /* FOR NOW WE DON'T RELEASE FRAMES. */
}
