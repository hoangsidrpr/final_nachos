/* createfile syscall  */
#include "syscall.h"

int main() {
    char fileName[256], mess[255];
    int length,open;
    
    PrintString("Enter file's name's length: ");
    length = ReadNum();

    PrintString("Enter file's name: ");
    ReadString(fileName, length);

    if (CreateFile(fileName) == 0) {
        PrintString("File ");
        PrintString(fileName);
        PrintString(" created successfully!\n");
		
    } else
        PrintString("Create file failed\n");
    Halt();
}