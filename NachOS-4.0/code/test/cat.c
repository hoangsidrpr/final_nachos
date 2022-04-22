/* cat program */
#include "syscall.h"
int main()
{
	int fileId, filesz,i;
	char c, name[255];
	PrintString("Input file name:");
	ReadString(name, 255);
	if ( (fileId= Open_File(name, 1)) == -1) // Check file if it is not exist
	{
		PrintString("Can not open file ");
		return 0;
	}
	filesz = Seek_File(-1, fileId); // count number of character in file -> filesz
	Seek_File(0, fileId); // move the pointer at the first of file

	for (i=0 ; i < filesz; ++i)
	{
		Read_File(&c, 1, fileId); // Read file from filename
		PrintChar(c); //Show the result
	} 
	Close_File(fileId);
	Halt();
}