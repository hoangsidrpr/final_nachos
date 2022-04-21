/* concatenate program */
#include "syscall.h"
#include "copyright.h"
typedef int OpenFileID;
int main()
{
	OpenFileID srcId;
	OpenFileID dstId;
	char source[255], dest[255];
	int filesz,srcsz, i;
	char c;
	
	PrintString("Input source file:");
	ReadString(source, 255);
	PrintString("Input destination file:");
	ReadString(dest, 255);
	PrintString(source);
	
	srcId = Open_File(source, 1);
	dstId = Open_File(dest, 0);
	if (srcId == -1 || dstId == -1)
	{
		int errorId = srcId == 0 ? 1 : 2;
		PrintString("Can not open file \n");
		Halt();
	}
	filesz = Seek_File(-1, srcId);
	Seek_File(0, srcId);
	Seek_File(-1, dstId);
	for (i = 0; i < filesz; ++i)
	{
		Read_File(&c, 1, srcId);
		Write_File(&c, 1, dstId);
	}
	//Close_File(srcId);
	//Close_File(dstId);
	Halt();
    }