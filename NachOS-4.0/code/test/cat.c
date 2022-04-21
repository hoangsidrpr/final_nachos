/* cat program */
#include "syscall.h"
int main()
{
	int fileId;
	int filesz,i;
	char c;
	char name[255];
	PrintString("Input file name:");
	ReadString(name, 255);
	if ( (fileId= Open_File(name, 1)) == -1)
	{
		PrintString("Can not open file ");
		return 0;
	}
	filesz = Seek_File(-1, fileId);
	Seek_File(0, fileId);
	
	for (i=0 ; i < filesz; ++i)
	{
		Read_File(&c, 1, fileId);
		PrintChar(c);
	} 
	Close_File(fileId);
	Halt();
}