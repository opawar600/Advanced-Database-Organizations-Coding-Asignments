# Advanced-Database-Organizations-Coding-Asignments

In this assignment, we implement a storage manager which can read and write blocks from a given file to the disk and also to the memory.

There are three parts to the code. One deals with creation of the files, the second reads content from the block and the last writes content to the specified block.

## File creation

- initStorageManager() : Initialize the storage manager

- createPageFile(char * fileName) : Creates a new page file with name as fileName. Initial file size is one page and the page is filled with "/0"

- openPageFile(char * fileName, SM_FileHandle * fHandle) : Opens page file named fileName. If the file is opened successfully, the fields of the file handle are initialized with the data of the opened file, else returns RC_FILE_NOT_FOUND.

- closePageFile(SM_FileHandle * fHandle) : Closes an already opened page file.

- destroyPageFile(char * fileName) : Deletes a page file

## Reading from a file

- readBlock(int pageNum, SM_FileHandle * fHandle, SM_PageHandle memPage) : Reads the pageNumth block from the page file and stores the content in the memory location pointed by memPage in the memory.

- getBlockPos(SM_FileHandle * fHandle) : Finds the current block position.

- readFirstBlock(SM_FileHandle * fHandle, SM_PageHandle memPage) : Reads the content from first block in the page file.

- readPreviousBlock(SM_FileHandle * fHandle, SM_PageHandle memPage) : Reads the previous block with respect to the curPagePos.

- readCurrentBlock(SM_FileHandle * fHandle, SM_PageHandle memPage) : Reads the block from the file where the curPagePos is.

- readNextBlock(SM_FileHandle * fHandle, SM_PageHandle memPage) : Reads the next block with respect to the curPagePos.

- readLastBlock(SM_FileHandle * fHandle, SM_PageHandle memPage) : Reads the content of last block in the page file.

## Writing to a file

- writeBlock()
- writeCurrentBlock()
- appendEmptyBlock()
- ensureCapacity()
