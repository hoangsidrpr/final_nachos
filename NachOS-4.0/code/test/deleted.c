/* delete program */
#include "syscall.h"
int main()
{
	int fileId;
	int filesz,i;
	char name[255];
	PrintString("Input file name:");
	ReadString(name, 255);
	if ( (fileId= Open_File(name, 0)) == -1)
	{
		PrintString("Can not open file ");
		return 0;
	}
	Remove_File(name);
	Close_File(fileId);
	Remove_File(name);
	Halt();
}