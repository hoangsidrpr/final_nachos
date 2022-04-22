/* delete program */
#include "syscall.h"
int main()
{
	int fileId;
	char name[255];
	PrintString("Input file name:");
	ReadString(name, 255);
	if ( (fileId= Open_File(name, 0)) == -1) // Check file if it is not exist
	{
		PrintString("Can not open file ");
		return 0;
	}
	Remove_File(name); // Cannot remove while file is openning -> Print 'File was in use at index fileId'
	Close_File(fileId);
	Remove_File(name); // Remove success
	Halt();
}