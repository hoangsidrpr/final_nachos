/* createfile syscall  */
#include "syscall.h"
#include "copyright.h"
typedef int OpenFileID;
int main() {
    char fileName[256];
    char mess[255];
    int length;
    OpenFileID open; 
    
    PrintString("Enter file's name's length: ");
    length = ReadNum();

    PrintString("Enter file's name: ");
    ReadString(fileName, length);

    if (CreateFile(fileName) == 0) {
        PrintString("File ");
        PrintString(fileName);
        PrintString(" created successfully!\n");
        open = Open_File(fileName, 0);
		if (open==-1) PrintString("Can not open file\n");
		Write_File("123", 3, open);
		Seek_File(0, open);
		Read_File(mess, 3, open);
	 
        PrintString(mess);
		
        Close_File(open);
    } else
        PrintString("Create file failed\n");
    Halt();
}