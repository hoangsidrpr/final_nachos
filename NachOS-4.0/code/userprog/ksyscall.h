/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__
#define __USERPROG_KSYSCALL_H__

#include "kernel.h"
#include "synchconsole.h"
#include <stdlib.h>

void SysHalt()
{
	kernel->interrupt->Halt();
}

int SysAdd(int op1, int op2)
{
	return op1 + op2;
}

char SysReadChar()
{
	return kernel->synchConsoleIn->GetChar();
}

void SysPrintChar(char character)
{
	kernel->synchConsoleOut->PutChar(character);
}

int SysRandomNum()
{
	return random();
}

char *SysReadString(int length)
{
	char *buffer = new char[length + 1];
	for (int i = 0; i < length; i++) {
		buffer[i] = SysReadChar();
		if (buffer[i]=='\n')
		{
			length=i;
			break;
		}
	}
	buffer[length] = '\0';
	return buffer;
}

void SysPrintString(char *buffer, int length)
{
	for (int i = 0; i < length; i++) {
		kernel->synchConsoleOut->PutChar(buffer[i]);
	}
}

int SysReadNum()
{
	int n = 0, num = 0, c_zeros = 0;
	char c, num_buffer[255];
	// Check each character is a suitable character like a number or word.
	do {
		c = kernel->synchConsoleIn->GetChar(); // Read each character.
		if (c == ' ' || c == '\n' || c == '\0')
			break;
		num_buffer[n++] = c;
	} while (true);
	// Check overflow: when num larger than int32 max value
	//                 or smaller than int32 min value it will return 0 in
	//                 result.
	if (n > 11)
		return 0;
	else if (n == 10) {
		for (int i = 0; i < 10; i++)
			if (num_buffer[i] > "2147483647"[i]) {
				return 0;
			}
	} else if (n == 11) {
		for (int i = 1; i < 11; i++)
			if (num_buffer[i] > "-2147483648"[i]) {
				return 0;
			}
	}

	bool negative = false, zero_lead = true;
	// if the first character is '-' -> negative = 1 if not negative=0
	if (num_buffer[0] == '-') {
		negative = true;
	}
	// The reason we need negative bool for skip the first character if num is
	// negative number.
	for (int i = negative; i < n; i++) {
		if (num_buffer[i] == '0' && zero_lead == true)
			c_zeros++;
		else
			zero_lead = false;
		if (num_buffer[i] < '0' ||
		    num_buffer[i] >
			    '9') // If any character in the string does not a
		// number, the result will be 0.
		{
			return 0;
		} else
			num = num * 10 + (num_buffer[i] - '0');
	}
	if (c_zeros > 0) // Avoid case: 002,01,-05,..etc
	{
		return 0;
	}
	if (negative)
		num = -num;
	return num;
}

void SysPrintNum(int num)
{
	if (num == 0)
		return kernel->synchConsoleOut->PutChar('0');
	// If num is negative -> read '-' first then tranfer num in positive num.
	if (num < 0) {
		kernel->synchConsoleOut->PutChar('-');
		num = -num;
	}
	// In case: num is a int32 max value print num
	if (num == 2147483647) {
		for (int i = 0; i < 10; i++) {
			kernel->synchConsoleOut->PutChar("2147483647"[i]);
		}
		return;
	}
	// In case: num is a int32 min value print num
	if (num == -2147483648) {
		for (int i = 0; i < 10; i++) {
			kernel->synchConsoleOut->PutChar("2147483648"[i]);
		}
		return;
	}

	char number_buffer[255];
	int n = 0;
	// Add each num character in number_buffer to print.
	while (num) {
		number_buffer[n++] = num % 10;
		num /= 10;
	}
	// Print number_buffer.
	for (int i = n - 1; i >= 0; i--) {
		kernel->synchConsoleOut->PutChar(number_buffer[i] + '0');
	}
}
bool SysCreateFile(char* fileName) {
    bool success;
    int fileNameLength = strlen(fileName);

    if (fileNameLength == 0) {
        DEBUG(dbgSys, "\nFile name can't be empty");
        success = false;

    } else if (fileName == NULL ) {
        DEBUG(dbgSys, "\nNot enough memory in system");
        success = false;

    } else {
        DEBUG(dbgSys, "\nFile's name read successfully");
        if (!kernel->fileSystem->Create(fileName)) {
            DEBUG(dbgSys, "\nError creating file");
            success = false;
        } else {
            success = true;
        }
    }

    return success;
}

int SysOpen_File(char* fileName, int type) {
    if (type != 0 && type != 1) return -1;

    int id = kernel->fileSystem->Open(fileName, type);
    if (id == -1) return -1;
    DEBUG(dbgSys, "\nOpened file");
    return id;
}

int SysClose_File(int id) { return kernel->fileSystem->Close(id); }

int SysRead_File(char* buffer, int charCount, int fileId) {
    if (fileId == 0) {
        return kernel->synchConsoleIn->GetString(buffer, charCount);
    }
    return kernel->fileSystem->Read(buffer, charCount, fileId);
}

int SysWrite_File(char* buffer, int charCount, int fileId) {
    if (fileId == 1) {
        return kernel->synchConsoleOut->PutString(buffer, charCount);
    }
    return kernel->fileSystem->Write(buffer, charCount, fileId);
}

int SysSeek_File(int seekPos, int fileId) {
    if (fileId <= 1) {
        DEBUG(dbgSys, "\nCan't seek in console");
        return -1;
    }
    return kernel->fileSystem->Seek(seekPos, fileId);
}

int SysRemove_File(char* filename) {
	if(kernel->fileSystem->isOpenFile(filename) == true) //Check whether the file's already opened or not
	{
		return -1;
	}
	else{
		bool result = kernel->fileSystem->Delete(filename);
		if (result == true){	// Delete Sucessful
			cout << "\nDeleted file '" << filename << "'.\n";
			return 0;
		}
		else{	
			cout << "\nFile '" << filename << "' not exists.\n";
			return -1;
		}
		return 0;
	}	
}

#endif /* ! __USERPROG_KSYSCALL_H__ */
