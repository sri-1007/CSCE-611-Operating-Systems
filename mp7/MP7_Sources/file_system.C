/*
     File        : file_system.C

     Author      : Riccardo Bettati
     Modified    : 2017/05/01

     Description : Implementation of simple File System class.
                   Has support for numerical file identifiers.
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
#include "file_system.H"


/*--------------------------------------------------------------------------*/
/* CONSTRUCTOR */
/*--------------------------------------------------------------------------*/

file_blockNode* FileSystem::fileListHead;
file_blockNode* FileSystem::fileListTail;
unsigned int FileSystem::freeBlock = 0;
unsigned int FileSystem::file_count = 0;
unsigned int FileSystem::size = 0;

FileSystem::FileSystem() {
    Console::puts("In file system constructor.\n");
    
	fileListHead = new file_blockNode();
	fileListTail = fileListHead;
	
	file_count = 0;
	
}

/*--------------------------------------------------------------------------*/
/* FILE SYSTEM FUNCTIONS */
/*--------------------------------------------------------------------------*/

bool FileSystem::Mount(SimpleDisk * _disk) {
    Console::puts("mounting file system form disk\n");
	
/* Reading the disk and initializing the FS class variables from the data in the Super Block*/	
	
	disk = _disk;
	unsigned char *buf = new unsigned char[512];
	
	disk->read(0,buf);
	memcpy(&size, buf, 4);  // first 4 bytes denotes the size of the FS
	memcpy(&freeBlock,buf+4,4); // next 4 bytes denotes the start of the FreeBlocks number
	memcpy(&file_count, buf+8, 4); // next 4 bytes denotes the number of files
		
return true;	
}

bool FileSystem::Format(SimpleDisk * _disk, unsigned int _size) {
    
	Console::puts("formatting disk\n");
   
   unsigned int numBlocks = _size/BLOCK_SIZE; // Assuming _size is a multiple of BLOCK_SIZE
   unsigned char *buf = new unsigned char[512];
   
   /* First three blocks reserved. 
	   First block: Super block, second block : Directory block, third block : free block */
	   
	/* 1. Initializing these reserved blocks except first block as finally we save the 
	size of this File system in the first block, ie the Super block*/   
	
	for(unsigned int block = 1; block< RESERVED_FS_BLOCKSIZE; block++)
		_disk->write(block, buf);
	
	/*2 Initializing the Free blocks starting with buffers whose last 4 bytes store the next free block number. 
	Initializing only till last as Last block number will be initialized with -1*/
	
	unsigned int block;
	
    for(block = RESERVED_FS_BLOCKSIZE ; block < numBlocks - 1 ; block++)
	{
		unsigned int nextBlock = block+1;
		memcpy(buf +508, &nextBlock,4); //copies last 4 bytes as the next free block number
		_disk->write(block,buf);
	}
	
    /*3. Initializing the last block with -1 as the last block does not point to any further blocks*/
	int lastBlockLimit = -1;
	memcpy(buf +508, &lastBlockLimit,4); 
	_disk->write(block,buf);
	
	/* 4. Initialize the first block. Super Block*/
	  /* 4.a size of the File system*/
	unsigned int temp = _size; 
	memcpy(buf +0, &temp,4);
	  
	  /*4.b first Free Block*/
	temp = RESERVED_FS_BLOCKSIZE;
	memcpy(buf +4, &temp,4);
	
	 /*4.c Number of files in the FS*/
	 temp =0;
	 memcpy(buf +8, &temp,4);
	 
	 /*4.d Initialize the remaining bytes to 0*/
	  memcpy(buf +12, 0,500);
	  
	_disk->write(0,buf) ; // Writing the SUPER BLOCK 
  
return true;   
}

File * FileSystem::LookupFile(int _file_id) {
 Console::puts("looking up file\n");
 
 file_blockNode *newNode = fileListHead;
  
 if(newNode == NULL)
 {
	Console::puts("\n NO files in LIST \n");
    return NULL;	
	 
 } 
 else
 { 
	for(unsigned int i =0 ;i<file_count ;i++)
    {
       if(newNode->fileID == _file_id)
		   return newNode->file;
	   newNode = newNode->next;
    }
 }
 
}


void FileSystem::addToFileList(File *newFile,unsigned int newfileID)
{
	file_blockNode *node = new file_blockNode();
	node->fileID = newfileID;
	node->file = newFile;
	node->next = NULL;
	
	if(file_count == 0)
	{
	   fileListHead = node;
       fileListTail = node;	   
	}
	else
	{
	    fileListTail->next = node;
        fileListTail = node;		
	}	
	
}

bool FileSystem::CreateFile(int _file_id) {
	
	unsigned char* buf = new unsigned char[512];
	unsigned int startInfoBlock = freeBlock; // firt free block is given as the start block for file creation.
	unsigned int startDataBlock;
	
	disk->read(freeBlock,buf);
	memcpy(&startDataBlock,buf+508,4); //datablocks of the file start from next block which is stored in last 4 bytes of the first free Block
	
	disk->read(startDataBlock,buf);
	/*Rewriting the freeBlock variable with the next block after allocating two blocks for this file*/
	memcpy(&freeBlock,buf+508,4);

	/*Creating New file by sending this block numbers*/
	File *newCreatedFile = new File(&startInfoBlock, &startDataBlock);
		
	/*Now saving this File entry in the File List*/
	addToFileList(newCreatedFile,_file_id);
	file_count += 1;
	
    Console::puts("creating file\n");
return true;	
}


void FileSystem::removeFromFileList(unsigned int fileID)
{
 file_blockNode *prev  = NULL;//fileListHead;//NULL;
 file_blockNode *curr  = fileListHead;//fileListHead->next;//fileListHead;
 
 if(curr == NULL)
 {
   Console::puts("FILE LIST EMPTY. NOTHING TO REMOVE \n");
    return;   
 }
 
 for(unsigned int i = 0; i<file_count;i++)
 {
	
   if(curr->fileID == fileID)
   {	   
	  unsigned int fileDataBlock = 0;
	  
	  unsigned char* read_buf = new unsigned char[512];
	  unsigned char*write_buf = new unsigned char[512];
	  
	  unsigned int fileInfoBlock = curr->file->getInfoBlock(); 
	  
	  disk->read(fileInfoBlock,read_buf); //trying to get the block to get last 4 bytes which has info about next block from where data is stored
	  memcpy(&fileDataBlock, read_buf+508, 4);
	  
	  memcpy(write_buf+508, &freeBlock, 4);
	  disk->write(fileInfoBlock, write_buf);
	  
	  /*Initializing this fileInfo Block as freeBlock and updating the start Freeblock number of the class*/
	  freeBlock = fileInfoBlock; //updating the first available free block
	  /*Now updating the data blocks also. Freeing them */
	  unsigned int next_data_block = 0;
	  
	  for(unsigned int j = 0; j < curr->file->getDataBlockCount(); j++)
	   {
			disk->read(fileDataBlock, read_buf);
			memcpy(&next_data_block, read_buf+508, 4);

			memcpy(write_buf+508, &freeBlock, 4);
			disk->write(fileDataBlock, write_buf);
			
			freeBlock = fileDataBlock;
				
			fileDataBlock = next_data_block;	
	   }	   	  
	break;
   }
   prev = curr;
   curr = curr->next;   	 
 } 
 
 /*If node to be removed is the last node in the list*/
 if(curr->next == NULL) 
 {
    if(prev != NULL)
      prev->next = NULL;		
    fileListTail = prev; 
	 
 }
 else
 {
   if(prev != NULL)
     prev->next = curr->next ; // current node reference is removed from the LL.	  
   else
	  fileListHead = curr->next;
 }
 	
}

bool FileSystem::DeleteFile(int _file_id) 
{

Console::puts("deleting file\n");
    
removeFromFileList(_file_id);	
file_count -= 1;
return true;	
}


