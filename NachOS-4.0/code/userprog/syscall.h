/* syscalls.h
 * 	Nachos system call interface.  These are Nachos kernel operations
 * 	that can be invoked from user programs, by trapping to the kernel
 *	via the "syscall" instruction.
 *
 *	This file is included by user programs and by the Nachos kernel.
 *
 * Copyright (c) 1992-1993 The Regents of the University of California.
 * All rights reserved.  See copyright.h for copyright notice and limitation
 * of liability and disclaimer of warranty provisions.
 */

#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "copyright.h"
#include "errno.h"
/* system call codes -- used by the stubs to tell the kernel which system call
 * is being asked for
 */
#define SC_Halt 0
#define SC_Exit 1
#define SC_Exec 2
#define SC_Join 3
//Syscall Thao tác trên tập tin
#define SC_Create 4
#define SC_Remove 5
#define SC_Open 6
#define SC_Read 7
#define SC_Write 8
#define SC_Seek 9
#define SC_Close 10
//Syscall đa chương
#define SC_ThreadFork 11
#define SC_ThreadYield 12
#define SC_ExecV 13
#define SC_ThreadExit 14
#define SC_ThreadJoin 15

#define SC_Add 42
#define SC_ReadNum 43
#define SC_PrintNum 44
#define SC_ReadChar 45
#define SC_PrintChar 46
#define SC_RandomNum 47
#define SC_ReadString 48
#define SC_PrintString 49
#define SC_CreateFile 50
#define SC_Read_File 51
#define SC_Write_File 52
#define SC_Open_File 53
#define SC_Close_File 54
#define SC_Seek_File 55
#define SC_Remove_File 56

#ifndef IN_ASM

/* The system call interface.  These are the operations the Nachos
 * kernel needs to support, to be able to run user programs.
 *
 * Each of these is invoked by a user program by simply calling the
 * procedure; an assembly language stub stuffs the system call code
 * into a register, and traps to the kernel.  The kernel procedures
 * are then invoked in the Nachos kernel, after appropriate error checking,
 * from the system call entry point in exception.cc.
 */

/* Stop Nachos, and print out performance stats */
void Halt();

/*
 * Add the two operants and return the result
 */

int Add(int op1, int op2);
// 3.
int ReadNum();
// 4.
void PrintNum(int num);
// 5.
char ReadChar();
// 6.
void PrintChar(char character);
// 7.
int RandomNum();
// 8.
void ReadString(char *buffer, int length);
// 9.
void PrintString(char *buffer);

typedef int OpenFileId;

/* when an address space starts up, it has two open files, representing
 * keyboard input and display output (in UNIX terms, stdin and stdout).
 * Read and Write can be used directly on these, without first opening
 * the console device.
 */

#define _ConsoleInput 0
#define _ConsoleOutput 1

/* Create a Nachos file, with name "name" */
/* Note: Create does not open the file.   */
/* Return 1 on success, negative error code on failure */
int Createfile(char *name);

/* Remove a Nachos file, with name "name" */
int Remove_File(char *name);

/* Open the Nachos file "name", and return an "OpenFileId" that can
 * be used to read and write to the file.
 */
OpenFileId Open_File(char *name,int type);

/* Write "size" bytes from "buffer" to the open file.
 * Return the number of bytes actually read on success.
 * On failure, a negative error code is returned.
 */
int Write_File(char *buffer, int size, OpenFileId id);

/* Read "size" bytes from the open file into "buffer".
 * Return the number of bytes actually read -- if the open file isn't
 * long enough, or if it is an I/O device, and there aren't enough
 * characters to read, return whatever is available (for I/O devices,
 * you should always wait until you can return at least one character).
 */
int Read_File(char *buffer, int size, OpenFileId id);

/* Set the seek position of the open file "id"
 * to the byte "position".
 */
int Seek_File(int position, OpenFileId id);

/* Close the file, we're done reading and writing to it.
 * Return 1 on success, negative error code on failure
 */
int Close_File(OpenFileId id);

void Exit(int status);

#endif /* IN_ASM */

#endif /* SYSCALL_H */
