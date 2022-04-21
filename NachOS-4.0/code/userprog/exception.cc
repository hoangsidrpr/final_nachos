// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"
#define MaxFileNameLength 32
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions
//	is in machine.h.
//----------------------------------------------------------------------

void move_pc()
{
	/* set previous programm counter (debugging only)*/
	kernel->machine->WriteRegister(PrevPCReg,
				       kernel->machine->ReadRegister(PCReg));

	/* set programm counter to next instruction (all Instructions are 4 byte *
     * wide)*/
	kernel->machine->WriteRegister(
		PCReg, kernel->machine->ReadRegister(NextPCReg));

	/* set next programm counter for brach execution */
	kernel->machine->WriteRegister(
		NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
}

char *stringUser2Kernel(int addr, int convert_length = -1)
{
	// Convert user string to kernel string
	// addr           : addess of user string
	// convert_length : desired length of string to convert
	//                  default will be the length of current string
	int length = 0;
	char *str;
	int oneChar;

	if (convert_length == -1) {
		// if convert_length takes default val as -1, this loop will loop
		// to the end the string - oneChar = '\0'
		do {
			kernel->machine->ReadMem(addr + length, 1, &oneChar);
			length++;
		} while (oneChar != '\0');
	} else {
		// or it will loop for ${convert_length} times
		do {
			kernel->machine->ReadMem(addr + length, 1, &oneChar);
			length++;
		} while (length != convert_length);
	}

	str = new char[length];
	for (int i = 0; i < length; i++) {
		int oneChar;
		kernel->machine->ReadMem(addr + i, 1, &oneChar);
		str[i] = (unsigned char)
			oneChar; // copy char by char to kernel space
	}

	return str;
}

void StringKernel2User(char *str, int addr, int convert_length = -1)
{
	// Function to convert Kernel
	// str            : string to convert.
	// addr           : address of kernel string
	// convert_length : desired length of string to convert
	//                  default will be the length of current string
	int length;
	if (convert_length == -1) {
		length =
			strlen(str); // Leave convert_length as default param to
			// convert all characters of sys string
	} else {
		length = convert_length; // Desired length to perform conversion
	}

	for (int i = 0; i < length; i++) {
		kernel->machine->WriteMem(
			addr + i, 1,
			str[i]); // Copy char by char to user space
	}

	kernel->machine->WriteMem(addr + length, 1,
				  '\0'); // Every string ends with '\0'
}

#define MAX_READ_STRING_LENGTH 255
void call_SC_ReadString()
{
	int memPtr =
		kernel->machine->ReadRegister(4); // read address of C-string
	int length =
		kernel->machine->ReadRegister(5); // read length of C-string

	if (length > MAX_READ_STRING_LENGTH) { // avoid allocating large memory
		DEBUG(dbgSys,
		      "String length exceeds " << MAX_READ_STRING_LENGTH);
		return move_pc();
	}

	char *buffer = SysReadString(length); // ReadString and store to buffer
	StringKernel2User(buffer,
			  memPtr); // Read string in kernel space -> userspace

	delete[] buffer;
	return move_pc();
}

void call_SC_PrintString()
{
	int memPtr =
		kernel->machine->ReadRegister(4); // read address of C-string
	char *buffer = stringUser2Kernel(memPtr);

	SysPrintString(buffer, strlen(buffer));
	delete[] buffer;
	return move_pc();
}

void call_SC_Halt()
{
	DEBUG(dbgSys, "Shutdown, initiated by user program.\n");
	SysHalt();
	ASSERTNOTREACHED();
}

void call_SC_Add()
{
	DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + "
			     << kernel->machine->ReadRegister(5) << "\n");

	/* Process SysAdd Systemcall*/
	int result;
	result = SysAdd(/* int op1 */ (int)kernel->machine->ReadRegister(4),
			/* int op2 */ (int)kernel->machine->ReadRegister(5));

	DEBUG(dbgSys, "Add returning with " << result << "\n");
	/* Prepare Result */
	kernel->machine->WriteRegister(2, (int)result);

	/* Modify return point */
	return move_pc();
}

void call_SC_ReadNum()
{
	int result = SysReadNum();
	kernel->machine->WriteRegister(2, result);
	return move_pc();
}

void call_SC_PrintNum()
{
	int character = kernel->machine->ReadRegister(4);
	SysPrintNum(character);
	return move_pc();
}

void call_SC_ReadChar()
{
	char result = SysReadChar();
	kernel->machine->WriteRegister(2, (int)result);
	return move_pc();
}

void call_SC_PrintChar()
{
	char character = (char)kernel->machine->ReadRegister(4);
	SysPrintChar(character);
	return move_pc();
}

void call_SC_RandomNum()
{
	int result;
	result = SysRandomNum();
	kernel->machine->WriteRegister(2, result);
	return move_pc();
}
//---CREATE_FILE---
void call_SC_CreateFile() {
    int virtAddr = kernel->machine->ReadRegister(4);
    char* fileName = stringUser2Kernel(virtAddr);

    if (SysCreateFile(fileName))
        kernel->machine->WriteRegister(2, 0);
    else
        kernel->machine->WriteRegister(2, -1);

    delete[] fileName;
    return move_pc();
}
void call_SC_Remove_File()
{
	int virtAddr = kernel->machine->ReadRegister(4); // Lay dia chi cua tham so name tu thanh ghi so 4
    char* fileName = stringUser2Kernel(virtAddr); // Copy chuoi tu vung nho User Space sang System Space
	
	kernel->machine->WriteRegister(2, SysRemove_File(fileName));

	return move_pc();
}
void call_SC_Open_File() {
    int virtAddr = kernel->machine->ReadRegister(4);
    char* fileName = stringUser2Kernel(virtAddr);
    int type = kernel->machine->ReadRegister(5);

    kernel->machine->WriteRegister(2, SysOpen_File(fileName, type));

    delete fileName;
    return move_pc();
}

void call_SC_Close_File() {
    int id = kernel->machine->ReadRegister(4);
    kernel->machine->WriteRegister(2, SysClose_File(id));

    return move_pc();
}

void call_SC_Read_File() {
    int virtAddr = kernel->machine->ReadRegister(4);
    int charCount = kernel->machine->ReadRegister(5);
    char* buffer = stringUser2Kernel(virtAddr, charCount);
    int fileId = kernel->machine->ReadRegister(6);

    DEBUG(dbgFile,
          "Read " << charCount << " chars from file " << fileId << "\n");

    kernel->machine->WriteRegister(2, SysRead_File(buffer, charCount, fileId));
    StringKernel2User(buffer, virtAddr, charCount);

    delete[] buffer;
    return move_pc();
}

void call_SC_Write_File() {
    int virtAddr = kernel->machine->ReadRegister(4);
    int charCount = kernel->machine->ReadRegister(5);
    char* buffer = stringUser2Kernel(virtAddr, charCount);
    int fileId = kernel->machine->ReadRegister(6);

    DEBUG(dbgFile,
          "Write " << charCount << " chars to file " << fileId << "\n");

    kernel->machine->WriteRegister(2, SysWrite_File(buffer, charCount, fileId));
    StringKernel2User(buffer, virtAddr, charCount);

    delete[] buffer;
    return move_pc();
}
void call_SC_Seek_File() {
    int seekPos = kernel->machine->ReadRegister(4);
    int fileId = kernel->machine->ReadRegister(5);

    kernel->machine->WriteRegister(2, SysSeek_File(seekPos, fileId));

    return move_pc();
}
void ExceptionHandler(ExceptionType which)
{
	int type = kernel->machine->ReadRegister(2);

	DEBUG(dbgSys,
	      "Received Exception " << which << " type: " << type << "\n");

	//	"which" is the kind of exception.  The list of possible exceptions
	//	is in machine.h.

	switch (which) {
	case NoException: // switch -> SystemMode [kernel control]
		kernel->interrupt->setStatus(SystemMode);
		DEBUG(dbgSys, "Switch to system mode\n");
		break;
	case PageFaultException:
	case ReadOnlyException:
	case BusErrorException:
	case AddressErrorException:
	case OverflowException:
	case IllegalInstrException:
	case NumExceptionTypes:
		cerr << "Error " << which << " occurs\n";
		SysHalt();
		ASSERTNOTREACHED();
	case SyscallException:
		switch (type) {
		case SC_Halt:
			return call_SC_Halt();
		case SC_Add:
			return call_SC_Add();
		case SC_ReadNum:
			return call_SC_ReadNum();
		case SC_PrintNum:
			return call_SC_PrintNum();
		case SC_ReadChar:
			return call_SC_ReadChar();
		case SC_PrintChar:
			return call_SC_PrintChar();
		case SC_RandomNum:
			return call_SC_RandomNum();
		case SC_ReadString:
			return call_SC_ReadString();
		case SC_PrintString:
			return call_SC_PrintString();
		case SC_CreateFile:
			return call_SC_CreateFile();
		case SC_Open_File:
			return call_SC_Open_File();
		case SC_Close_File:
			return call_SC_Close_File();
		case SC_Read_File:
			return call_SC_Read_File();
		case SC_Write_File:
			return call_SC_Write_File();
		case SC_Seek_File:
			return call_SC_Seek_File();
		case SC_Remove_File:
			return call_SC_Remove_File();
		default:
			cerr << "Unexpected system call " << type << "\n";
			break;
		}
		break;
	default:
		cerr << "Unexpected user mode exception" << (int)which << "\n";
		break;
	}
	ASSERTNOTREACHED();
}