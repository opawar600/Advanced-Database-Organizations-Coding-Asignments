#include<stdio.h>	//Standard Input Output Operations
#include<stdlib.h>	// Standard library for memory allocation and conversion
#include<string.h>	//Library for string operations

#include "storage_mgr.h"


FILE * file_pointer;


//The page handler of storage manager is initialized to NULL.

extern void initStorageManager (void)
{

}


/* Create a new page file fileName with initial file size of one page.
   The method fills the single page with '\0' bytes. */

extern RC createPageFile(char *fileName)
{
	file_pointer = fopen(fileName,"w+");

	if(file_pointer == NULL)
	{							//Make sure if the file exists
		return RC_FILE_NOT_FOUND;			//Display error message the file is not found
	}

	else
	{
		SM_PageHandle empty_page = (SM_PageHandle)calloc(PAGE_SIZE,sizeof(char));

		int check = fwrite(empty_page,sizeof(char),PAGE_SIZE, file_pointer);

		if(check >= PAGE_SIZE)
		{
			fclose(file_pointer);
	    		free(empty_page);
      			return RC_OK;						//Page file created successfully
		}
	}
}


/* An existing page file should open. This returns RC_FILE_NOT_FOUND if the file does not exist. The second parameter is an existing file handle.
 When file open is successful, then the fields of this file handle should be initialized with the information about the opened file.
 For example, we have to read the total number of pages that are stored in the file from disk. */

extern RC openPageFile(char *fileName, SM_FileHandle *fHandle)
{
	file_pointer = fopen(fileName,"r");

	if(file_pointer == NULL)
	{								//Make sure if the file exists
		return RC_FILE_NOT_FOUND;				//Display error message the file is not found
	}

	else
	{
		fHandle->fileName = fileName;			//Give the details about the opened file to the file handler
		fHandle->curPagePos = 0;
		fseek(file_pointer,0L,SEEK_END);
		int SIZE = ftell(file_pointer);
		fHandle->totalNumPages = SIZE/PAGE_SIZE;
		fHandle->mgmtInfo = file_pointer;
		fclose(file_pointer);

		return RC_OK;						//File opened successfully
	}

}


// Closes an already opened page file.

extern RC closePageFile(SM_FileHandle *fHandle)
{
	if(file_pointer != NULL)
	{
		file_pointer = NULL;
	}
	return RC_OK;					//File closed successfully
}


// Deletes a page file

extern RC destroyPageFile(char *fileName)
{
	file_pointer = fopen(fileName,"r");
	if(file_pointer == NULL)
	{
		return RC_FILE_NOT_FOUND;
	}

	remove(fileName);
	return RC_OK;					//File deleted successfully
}


/* The method reads the pageNumth block from a file and stores its content in the memory pointed to by the memPage page handle.
   If the file has less than pageNum pages, the method should return RC_READ_NON_EXISTING_PAGE. */

extern RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{

	if(pageNum > fHandle->totalNumPages && pageNum < 0 )	//Validation of page number

	{
		return RC_READ_NON_EXISTING_PAGE;			//Return error message on invalid page
	}

	file_pointer = fopen(fHandle->fileName,"r");

	if(file_pointer == NULL)
	{
		return RC_FILE_NOT_FOUND;				//File does not exist
	}

	if(fseek(file_pointer, pageNum*PAGE_SIZE,SEEK_SET) != 0)
	{
		return RC_FILE_NOT_FOUND;
	}

	fread(memPage,sizeof(char),PAGE_SIZE, file_pointer);	 //Reads the contents of the block and stores them in the block of memory pointed by memPage.
	fHandle->curPagePos = pageNum;
	fclose(file_pointer);
	return RC_OK;					//Given block read without any errors successfully.
}


// This method returns the current page position in a file.

extern int getBlockPos(SM_FileHandle *fHandle)
{
	int cur_pos;

	cur_pos = fHandle->curPagePos;
	return cur_pos;
}


// Reads the content from first block in the page file.

extern RC readFirstBlock(SM_FileHandle *fHandle , SM_PageHandle memPage)
{
	file_pointer = fopen(fHandle->fileName,"r");

	if(file_pointer == NULL)
	{
		return RC_FILE_NOT_FOUND;
	}

       readBlock(0,fHandle,memPage);

	fclose(file_pointer);
	return RC_OK;						//First block read successfully
}

// Reads the content of last block in the page file.

extern RC readLastBlock(SM_FileHandle *fHandle , SM_PageHandle memPage)
{
	file_pointer = fopen(fHandle->fileName,"r");

	if(file_pointer == NULL)
	{
		return RC_FILE_NOT_FOUND;
	}

	int count_pages = fHandle->totalNumPages;

	if(fHandle->mgmtInfo == NULL)
	{
		return RC_FILE_NOT_FOUND;
	}


	readBlock(count_pages,fHandle,memPage);

	fclose(file_pointer);
	return RC_OK;						//Read from last block successfully.
}


/*Reads the block from the file where the curPagePos is.
Should return RC_FILE_NOT_FOUND if the file does not exists.*/

extern RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	file_pointer = fopen(fHandle->fileName,"r");
	if(file_pointer == NULL)
	{
		return RC_FILE_NOT_FOUND;
	}

	int current_pg = getBlockPos(fHandle);
	readBlock(current_pg,fHandle,memPage);
	fclose(file_pointer);
	return RC_OK;					//Read from current block successfully.

}


/* Reads the previous block with respect to the curPagePos. Should return RC_READ_NON_EXISTING_PAGE if the method tries to read the block previous to first block*/

extern RC readPreviousBlock(SM_FileHandle *fHandle , SM_PageHandle memPage)
{
	file_pointer = fopen(fHandle->fileName,"r");
	if(file_pointer == NULL)
	{
		return RC_FILE_NOT_FOUND;
	}

	if(fHandle->curPagePos <= PAGE_SIZE)
	{
		return RC_READ_NON_EXISTING_PAGE;
	}

	int current_pg = getBlockPos(fHandle);
	int Prev_page = current_pg - 1;				//Get to the previous block by decrementing the current page
	readBlock(Prev_page,fHandle,memPage);

	fclose(file_pointer);
	return RC_OK;					//Successfully read from the previous block
}


/*Reads the next block with respect to the curPagePos.
Should return RC_READ_NON_EXISTING_PAGE if the curPagePos is at the last and there is no block ahead of the page.*/

extern RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
	if(file_pointer == NULL)
	{
		return RC_FILE_NOT_FOUND;
	}

	if(fHandle->curPagePos == PAGE_SIZE)
	{
		return RC_READ_NON_EXISTING_PAGE;
	}

	int current_pg = getBlockPos(fHandle);
	int next_page = current_pg + 1;				//Move the next block by incrementing.
	readBlock(next_page,fHandle,memPage);

	fclose(file_pointer);
	return RC_OK;						//Next block read successfully.
}


// Write a page to disk using either the current position or an absolute position.

extern RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{

	if(pageNum > fHandle->totalNumPages || pageNum <0)
	{
		return RC_WRITE_FAILED;
	}

	file_pointer = fopen(fHandle->fileName,"r+");			//Open fileName file for writing operations

	if(file_pointer == NULL)
	{
		return RC_FILE_NOT_FOUND;
	}

	if(fseek(file_pointer, pageNum*PAGE_SIZE,SEEK_SET) != 0)
	{
		return RC_FILE_NOT_FOUND;
	}

	else
	{
              fwrite(memPage,sizeof(char),strlen(memPage), file_pointer);		//Writes content to the block.
		fHandle->curPagePos = pageNum;

		fclose(file_pointer);
		return RC_OK;				//Content Written to the block successfully.
  	}
}


// Write a page to disk using either the current position or an absolute position.

extern RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	file_pointer = fopen(fHandle->fileName,"r+");
	if(file_pointer == NULL)
	{
		return RC_FILE_NOT_FOUND;
	}

	int current_pg = getBlockPos(fHandle);
	fHandle->totalNumPages++;
	writeBlock(current_pg,fHandle,memPage);			//Write to current block

	fclose(file_pointer);
	return RC_OK;						//Current Block written successfully
}


// Increase the number of pages in the file by one. The new last page should be filled with zero bytes.

extern RC appendEmptyBlock (SM_FileHandle *fHandle)
{
	file_pointer = fopen(fHandle->fileName,"r+");
	if(file_pointer == NULL)
	{
		return RC_FILE_NOT_FOUND;
	}

	int total_pages = fHandle->totalNumPages;

	fseek(file_pointer,total_pages*PAGE_SIZE,SEEK_SET);	//Find the end of file.
	char x = 0 ;
	int i;

	for(i=0;i<PAGE_SIZE;i++)
	{
		fwrite(&x,sizeof(x),1, file_pointer);
	}

	fHandle->totalNumPages++;
	fclose(file_pointer);
	return RC_OK;					//Empty block appended successfully
}


//If the file is having less pages than that of the total number of pages, then increase the number of pages to fulfil the capacity.

extern RC ensureCapacity(int numberOfPages, SM_FileHandle *fHandle)
{
	int i;
  	int pages = numberOfPages - fHandle->totalNumPages;		//check whether the numPages is less than the total number of pages
  	if(pages > 0)
	{
    		for(i=0;i<pages;i++)
        	appendEmptyBlock(fHandle);
  	}
  	if(fHandle->totalNumPages == numberOfPages)
	{
    		return RC_OK;
  	}
}
