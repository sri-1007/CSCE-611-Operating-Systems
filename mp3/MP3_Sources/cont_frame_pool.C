/*
 File: ContFramePool.C
 
 Author:
 Date  : 
 
 */

/*--------------------------------------------------------------------------*/
/* 
 POSSIBLE IMPLEMENTATION
 -----------------------

 The class SimpleFramePool in file "simple_frame_pool.H/C" describes an
 incomplete vanilla implementation of a frame pool that allocates 
 *single* frames at a time. Because it does allocate one frame at a time, 
 it does not guarantee that a sequence of frames is allocated contiguously.
 This can cause problems.
 
 The class ContFramePool has the ability to allocate either single frames,
 or sequences of contiguous frames. This affects how we manage the
 free frames. In SimpleFramePool it is sufficient to maintain the free 
 frames.
 In ContFramePool we need to maintain free *sequences* of frames.
 
 This can be done in many ways, ranging from extensions to bitmaps to 
 free-lists of frames etc.
 
 IMPLEMENTATION:
 
 One simple way to manage sequences of free frames is to add a minor
 extension to the bitmap idea of SimpleFramePool: Instead of maintaining
 whether a frame is FREE or ALLOCATED, which requires one bit per frame, 
 we maintain whether the frame is FREE, or ALLOCATED, or HEAD-OF-SEQUENCE.
 The meaning of FREE is the same as in SimpleFramePool. 
 If a frame is marked as HEAD-OF-SEQUENCE, this means that it is allocated
 and that it is the first such frame in a sequence of frames. Allocated
 frames that are not first in a sequence are marked as ALLOCATED.
 
 NOTE: If we use this scheme to allocate only single frames, then all 
 frames are marked as either FREE or HEAD-OF-SEQUENCE.
 
 NOTE: In SimpleFramePool we needed only one bit to store the state of 
 each frame. Now we need two bits. In a first implementation you can choose
 to use one char per frame. This will allow you to check for a given status
 without having to do bit manipulations. Once you get this to work, 
 revisit the implementation and change it to using two bits. You will get 
 an efficiency penalty if you use one char (i.e., 8 bits) per frame when
 two bits do the trick.
 
 DETAILED IMPLEMENTATION:
 
 How can we use the HEAD-OF-SEQUENCE state to implement a contiguous
 allocator? Let's look a the individual functions:
 
 Constructor: Initialize all frames to FREE, except for any frames that you 
 need for the management of the frame pool, if any.
 
 get_frames(_n_frames): Traverse the "bitmap" of states and look for a 
 sequence of at least _n_frames entries that are FREE. If you find one, 
 mark the first one as HEAD-OF-SEQUENCE and the remaining _n_frames-1 as
 ALLOCATED.

 release_frames(_first_frame_no): Check whether the first frame is marked as
 HEAD-OF-SEQUENCE. If not, something went wrong. If it is, mark it as FREE.
 Traverse the subsequent frames until you reach one that is FREE or 
 HEAD-OF-SEQUENCE. Until then, mark the frames that you traverse as FREE.
 
 mark_inaccessible(_base_frame_no, _n_frames): This is no different than
 get_frames, without having to search for the free sequence. You tell the
 allocator exactly which frame to mark as HEAD-OF-SEQUENCE and how many
 frames after that to mark as ALLOCATED.
 
 needed_info_frames(_n_frames): This depends on how many bits you need 
 to store the state of each frame. If you use a char to represent the state
 of a frame, then you need one info frame for each FRAME_SIZE frames.
 
 A WORD ABOUT RELEASE_FRAMES():
 
 When we releae a frame, we only know its frame number. At the time
 of a frame's release, we don't know necessarily which pool it came
 from. Therefore, the function "release_frame" is static, i.e., 
 not associated with a particular frame pool.
 
 This problem is related to the lack of a so-called "placement delete" in
 C++. For a discussion of this see Stroustrup's FAQ:
 http://www.stroustrup.com/bs_faq2.html#placement-delete
 
 */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "cont_frame_pool.H"
#include "console.H"
#include "utils.H"
#include "assert.H"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */
ContFramePool* ContFramePool::head = NULL; 

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* METHODS FOR CLASS   C o n t F r a m e P o o l */
/*--------------------------------------------------------------------------*/

ContFramePool::ContFramePool(unsigned long _base_frame_no,
                             unsigned long _n_frames,
                             unsigned long _info_frame_no,
                             unsigned long _n_info_frames)
{
    base_frame_no = _base_frame_no;
	nframes = _n_frames;
	nFreeFrames = _n_frames;
	info_frame_no = _info_frame_no;
	ninfoframes = _n_info_frames;
	
/* If _info_frame_no is zero then we keep management info in the first
   frame, else we use the provided frame to keep management info */
   
   
    if(info_frame_no == 0) {
        bitmap = (unsigned char *) (base_frame_no * FRAME_SIZE);
    } else {
        bitmap = (unsigned char *) (info_frame_no * FRAME_SIZE);
    }
    
    /* Number of frames should be able to "fill" the bitmap!*/
    assert ((nframes % 8 ) == 0);
        
    /* Initializing all bits in the bitmap. Using 8 bits to represent the
	one frame, in which first two bits are used to store information regarding 
	allocation and Head*/
    for(int i=0; i < _n_frames; i++) {
        bitmap[i] = 0xFF;
    }
    
    /* To Mark the first Frame as being used for Kernel Pool */
    if(_info_frame_no == 0) {
        bitmap[0] = 0x7F;
        nFreeFrames--;
	 // bitmap[1] :  Is the second bit. Not assigning any value for it currently.		
    }
	
	/* Creating a LinkedLists of the Pools with Kernel Pool as the HEAD*/
	if(head == NULL)
	{
		head = this;   // Assigning the kernel pool to HEAD	 
		head->next = NULL;
	}
	else
	{
       ContFramePool *temp =NULL;
       for(temp= head ; temp->next !=NULL ;temp = temp->next);
       temp->next = this;
	   temp = this;
       temp->next = NULL;
	
	}	
    
    Console::puts("Frame Pool initialized\n");
}

unsigned long ContFramePool::get_frames(unsigned int _n_frames)
{
    	
	/* To check if frames are free to allocate and also less than the max number of frames */
	if(!((_n_frames <= nframes) && (_n_frames <= nFreeFrames))) 
	{
	    Console::puts("\n CANNOT ALLOCATE FRAMES MORE THAN AVAILABLE FRAMES\n ");
		assert(false);
	}
		   
    unsigned int i = 0;
	unsigned int start_frame_no = base_frame_no; 
	
	/* To check frames which are already allocated */
	x:
    while (bitmap[i] == 0x7F || bitmap[i] == 0x3F) {
		i++;
    }
	
	start_frame_no = base_frame_no + i;
		
	/* To check if frames are continuously available from the free frame*/
	
	for(unsigned int j = i; j<(_n_frames +i); j++)
	{
		if(!((start_frame_no+ _n_frames) <= (base_frame_no+ nframes)))
		{
			Console::puts("\n NO CONTINUOUS FREE FRAMES AVAILABLE\n");
			assert(false);
		}
		
		if((bitmap[j] == 0x7F) || (bitmap[j] == 0x3F))
		{
		   if(j < base_frame_no+nframes) // CHECK FOR MAX AVAILABLE 
		   {
			   i=j;
		       goto x; 		   // To start looking for the next free frame
		   }
		   else
		   {
			 Console::puts("ERROR: NO CONTINUOUS FREE FRAMES AVAILABLE ");
			 assert(false);
		   }			
		  
		}
	}
	
	setBitMap(i, _n_frames);
	return (start_frame_no);
}
	
	
	
void ContFramePool::setBitMap(unsigned int i, unsigned int _n_frames)	
{
	 /* Mark that frame as being used in the bitmap. Assuming two bits. One for allocation and one for head*/		
	
	unsigned char mask = 0x80;   
    unsigned int k = _n_frames;
	
	while(k != 0)
	{	
	   if(k == _n_frames)
	   {
		bitmap[i] = bitmap[i] ^ 0xC0;   
	   }
	   else
	   {
	   bitmap[i] = bitmap[i] ^ mask;
	   }
	   nFreeFrames--;
	   i++;
	   k--;
	} 	
		
}

void ContFramePool::mark_inaccessible(unsigned long _base_frame_no,
                                      unsigned long _n_frames)
{   
    int i ;
    for(i = _base_frame_no; i < _base_frame_no + _n_frames; i++){
	mark_inaccessible(i);		
    }
    nFreeFrames -= _n_frames;
}

void ContFramePool::mark_inaccessible(unsigned long _frame_no)
{
    // Range check.
	if(!((_frame_no >= base_frame_no) && (_frame_no < base_frame_no + nframes)))
	{
		Console::puts("\n OUT OF POOL BOUNDS. CANNOT MARK \n");
		assert(false);
	}
    
    unsigned int bitmap_index = (_frame_no - base_frame_no);
    unsigned char mask = 0xC0;
    
    assert((bitmap[bitmap_index] & mask) != 0); // check if already in use : TBT 
    
    // Update bitmap
    bitmap[bitmap_index] ^= mask;	
}



void ContFramePool::release_frames(unsigned long _first_frame_no)
{
	ContFramePool *temp = NULL;
	for(temp = head ;temp!= NULL; temp = temp->next)
	{
		 if((_first_frame_no >= temp->base_frame_no )&& (_first_frame_no <= (temp->base_frame_no + temp->nframes - 1)))
		 {
			temp->release_frame(_first_frame_no);
            break;		
		 }	 				
	
	}
}


void ContFramePool::release_frame(unsigned long _first_frame_no )
{

if(!(bitmap[_first_frame_no - base_frame_no] == 0x3F))
{
Console::puts("\n NOT HEAD FRAME. CANNOT RELEASE");
assert(false);	
}

unsigned int i = (_first_frame_no - base_frame_no);
 
  for(i = (_first_frame_no - base_frame_no); i<nframes ;)
  {
         bitmap[i] |= 0xC0; // resetting the bitmaps	   
         i++; 
		 nFreeFrames++; // Increasing the number of Free Frames
		 
		 if((bitmap[i] ^ 0xFF) == 0x80) 
				 continue;
		 else
			 break;
  }
}

unsigned long ContFramePool::needed_info_frames(unsigned long _n_frames)
{
 
 /* Considering 8 bits per frame*/
   unsigned long n_info_frames;
   n_info_frames = ((_n_frames *8) /(4*1024*8)) + (((_n_frames*8) % (4*1024*8)) >0 ? 1 : 0 );
   return n_info_frames;
}
