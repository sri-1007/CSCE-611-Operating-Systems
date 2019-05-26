/* 
    File: mem_pool.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 11/10/27

    Implementation of a contiguous-memory allocator.

    THIS IS A DUMMY IMPLEMENTATION ONLY! 
    IT HAS BEEN IMPLEMENTED AS POORLY AS HUMANLY POSSIBLE.
    DO NOT TRY TO USE OR IMITATE IN ANY WAY!!

*/

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "utils.H"
#include "console.H"

#include "mem_pool.H"

/*--------------------------------------------------------------------------*/
/* M e m o r y   P o o l  */
/*--------------------------------------------------------------------------*/

MemPool::MemPool(FramePool * _frame_pool, int _n_frames) {
  Console::puts("Allocating Memory Pool... ");
  start_address = _frame_pool->get_frame();
  for (int i = 1; i < _n_frames; i++) {
      unsigned long next_frame_addr = _frame_pool->get_frame();
  }
  Console::puts("done\n");
}     


unsigned long MemPool::allocate(unsigned long _size) {
  
  unsigned long return_address = start_address;
  start_address += _size;

  return return_address;

}
 

void MemPool::release(unsigned long   _start_address) {
   /* FOR NOW WE DON'T RELEASE MEMORY. */
}
