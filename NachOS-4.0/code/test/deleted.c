/* cat program */
#include "syscall.h"
typedef int OpenFileID;
int main()
{
	OpenFileID fileId;
	char name[255];
	PrintString("Input file name:");
	ReadString(name, 4);
    PrintString(name);
    if ( (fileId= Open_File(name, 0)) == -1)
	{
		PrintString("Can not open file ");
		return 0;
	}
    Remove_File(name);
	PrintString(fileId);
    Close_File(fileId);
	return 0;
	//Halt();
}