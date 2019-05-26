/*
     File        : file.C

     Author      : Riccardo Bettati
     Modified    : 2017/05/01

     Description : Implementation of simple File class, with support for
                   sequential read/write operations.
*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "assert.H"
#include "console.H"
#include "file.H"
#include "file_system.H"

extern FileSystem* FILE_SYSTEM;
/*--------------------------------------------------------------------------*/
/* CONSTRUCTOR */
/*--------------------------------------------------------------------------*/

File::File(unsigned int* _startInfoBlock, unsigned int* _startDataBlock) {
    /* We will need some arguments for the constructor, maybe pointer to disk
     block with file management and allocation data. */
    Console::puts("In file constructor.\n");
	
	fileSize = 512;
	
	startInfoBlock = *_startInfoBlock;
	startDataBlock = *_startDataBlock;
	
	current_pos = 0; //indicates the current start position. Useful in cases where not all 512 bytes are occupied.
	current_block = startDataBlock;
	
	fileEndBlock = startDataBlock; // To keep track if file size exceeds 512
    dataBlockCount = 1;
}

/*--------------------------------------------------------------------------*/
/* FILE FUNCTIONS */
/*--------------------------------------------------------------------------*/

int File::Read(unsigned int _n, char * _buf) {
    Console::puts("reading from file\n");
    
	unsigned char* diskReadBuff = new unsigned char[512];
	unsigned int read_position = 0; //tracks the read position
	
	while(read_position < _n)
	{
		FILE_SYSTEM->disk->read(current_block,diskReadBuff);
		
		unsigned int readBytesRemaining = _n -read_position;
		unsigned int blockBytesRemaining = 508 - current_pos;
		
		if(readBytesRemaining <= blockBytesRemaining)
		{
			memcpy(_buf + read_position , diskReadBuff+current_pos,readBytesRemaining); // we just copy the remaining requested bytes
			read_position = read_position + readBytesRemaining;
			current_pos = current_pos + readBytesRemaining;
			
			if(read_position == _n)
				return _n;	
		}
		else
		{
			 memcpy(_buf + read_position , diskReadBuff+current_pos,blockBytesRemaining);  //copying the disk read data buffer to the request read buffer
			 read_position = read_position + blockBytesRemaining;
			 current_pos = 0;
			 memcpy(&current_block , diskReadBuff+508, 4); //shifting the current block to the next block
			
		}
	}

return _n;
}


void File::Write(unsigned int _n, const char * _buf) 
{
    Console::puts("writing to file\n");

	unsigned char* diskWriteBuff = new unsigned char[512];
	unsigned int write_pos = 0;
	
	while(write_pos < _n)
	{
		
		FILE_SYSTEM->disk->read(current_block,diskWriteBuff);
		
		unsigned int writeBytesRemaining = _n-write_pos;
		unsigned int blockBytesRemaining = 508 - current_pos;
		
		if(writeBytesRemaining < blockBytesRemaining)
		{
		memcpy(diskWriteBuff+current_pos,_buf+write_pos,writeBytesRemaining);
        FILE_SYSTEM->disk->write(current_block,diskWriteBuff);
		
		write_pos += writeBytesRemaining;		
		current_pos += writeBytesRemaining;
		
		if(current_pos == _n)
			Console::puts("WRITE COMPLETE \n");
				
		}
		else
		{
			
		memcpy(diskWriteBuff+current_pos,_buf+write_pos,blockBytesRemaining);
        FILE_SYSTEM->disk->write(current_block,diskWriteBuff);

        write_pos += blockBytesRemaining;
		current_pos = 0;
		
		current_block = FILE_SYSTEM->freeBlock;
		FILE_SYSTEM->disk->read(FILE_SYSTEM->freeBlock,diskWriteBuff);
		memcpy(&(FILE_SYSTEM->freeBlock),diskWriteBuff+508,4);
		dataBlockCount += 1;
			
			
		}
		
	}	
	
	    if(((dataBlockCount - 1)*512 +current_pos ) > fileSize)
		{
			
		Console::puts("\n FILE SIZE MAX REACHED. INCREASING THE FILE SIZE \n");
		fileSize = dataBlockCount*512 + current_pos;
		
		fileEndBlock = current_block;
			
		}
}

void File::Reset() {
    Console::puts("reset current position in file\n");
    
	current_pos = 0;
	current_block = startDataBlock;
    
}

void File::Rewrite() {
    Console::puts("erase content of file\n");
	
	unsigned char* diskWriteBuff = new unsigned char[512];
    unsigned char* diskReadBuff = new unsigned char[512];
	
	unsigned int temp_curr_block = startDataBlock;
	int temp_next_block = 0;
	
	for(unsigned int i =0; i<dataBlockCount;i++)
	{
	    if(temp_next_block == -1)
			break; // Last block in the FS
	    
		FILE_SYSTEM->disk->read(temp_curr_block,diskReadBuff);
		memcpy(&temp_next_block,diskReadBuff+508,4);
		
		memcpy(diskWriteBuff+508,&(FILE_SYSTEM->freeBlock),4);
		FILE_SYSTEM->disk->write(temp_curr_block,diskWriteBuff);
		
		if(startDataBlock != temp_curr_block)
			FILE_SYSTEM->freeBlock = temp_curr_block;
		
		temp_curr_block = temp_next_block;
	}
	
	fileSize = 512;
	
	current_pos = 0; //indicates the current start position. Useful in cases where not all 512 bytes are occupied.
	current_block = startDataBlock;
	
	fileEndBlock = startDataBlock; // To keep track if file size exceeds 512
    dataBlockCount = 1;
	
}


bool File::EoF() {
    Console::puts("testing end-of-file condition\n");
    return (dataBlockCount*512 + current_pos ) == fileSize;
}
