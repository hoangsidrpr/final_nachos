/* concatenate program */
#include "syscall.h"

int main()
{
	int srcId, dstId;
	char source[255], dest[255];
	int filesz,srcsz, i;
	char c;
	
	
	PrintString("Input source file:");
	ReadString(source, 255); // Call ReadString function for read source filename
	PrintString("\nYour source file: ");
	PrintString(source);
	PrintString("\nInput destination file:");
	ReadString(dest, 255); // Call ReadString function for read destination filename
	PrintString("\nYour destination file: ");
	PrintString(dest);
	PrintString("\n");
	
	srcId = Open_File(source, 1); // open for only read
	dstId = Open_File(dest, 0); // open for read and write
	if (srcId == -1 || dstId == -1)
	{
		int errorId = srcId == 0 ? 1 : 2;
		PrintString("Can not open file \n");
		Halt();
	}
	filesz = Seek_File(-1, srcId); // count number of character in file -> filesz
	Seek_File(0, srcId); // move the pointer at the first of file
	Seek_File(-1, dstId); // move the pointer at the end of file
	for (i = 0; i < filesz; ++i)
	{
		Read_File(&c, 1, srcId);
		Write_File(&c, 1, dstId);
	}
	//Close_File(srcId);
	//Close_File(dstId);
	Halt();
    }