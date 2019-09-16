# Advanced-Database-Organizations-Coding-Asignments

In this assignment, we implement a storage manager which can read and write blocks from a given file to the disk and also to the memory.

There are three parts to the code. One deals with creation of the files, the second reads content from the block and the last writes content to the specified block.

## File creation

- initStorageManager() : Initialize the storage manager
- createPageFile()
- openPageFile()
- closePageFile()
- destroyPageFile()

## Reading from a file

- readBlock()
- getBlockPos()
- readFirstBlock()
- readPreviousBlock()
- readCurrentBlock()
- readNextBlock()
- readLastBlock()

## Writing to a file

- writeBlock()
- writeCurrentBlock()
- appendEmptyBlock()
- ensureCapacity()
