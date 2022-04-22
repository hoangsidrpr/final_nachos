#ifndef FILETABLE_H
#define FILETABLE_H
#include "openfile.h"
#include "sysdep.h"
#include <string.h>

#define FILE_MAX 10
#define CONSOLE_IN 0
#define CONSOLE_OUT 1
#define MODE_READWRITE 0
#define MODE_READ 1
#define MODE_WRITE 2

class FileTable {
   private:
    OpenFile** openFile;
    int* fileOpenMode;
    char** FileName;

   public:
    FileTable() {
        openFile = new OpenFile*[FILE_MAX];
        FileName = new char*[FILE_MAX];
        fileOpenMode = new int[FILE_MAX];
        fileOpenMode[CONSOLE_IN] = MODE_READ;
        fileOpenMode[CONSOLE_OUT] = MODE_WRITE;
         // Taclke I/O from console
    }

    int Insert(char* fileName, int openMode) {
        int freeIndex = -1;
        int fileDescriptor = -1;
        for (int i = 2; i < FILE_MAX; i++) {
            if (openFile[i] == NULL) {
                freeIndex = i;
                break;
            }
        }

        if (freeIndex == -1) { // Full memory
            return -1;
        }

        if (openMode == MODE_READWRITE)
            fileDescriptor = OpenForReadWrite(fileName, FALSE); // MODE: Open file for Read and Write - fileDescriptor = 0 
        if (openMode == MODE_READ)
            fileDescriptor = OpenForRead(fileName, FALSE); // MODE: Open file for only Read -  fileDescriptor = 1

        if (fileDescriptor == -1) return -1; // File not exist
        openFile[freeIndex] = new OpenFile(fileDescriptor); // After checking all condition -> Open new file with index=freeIndex
        fileOpenMode[freeIndex] = openMode; // Save openMode -> fileOpenMode array with index=freeIndex
        FileName[freeIndex] =fileName; // save fileName -> FileName array with index=freeIndex
        cout << "\nOpen file '" << fileName << "' at index " << freeIndex << ".\n";
        return freeIndex;
    }

    int Remove(int index) {
        if (index < 2 || index >= FILE_MAX) return -1; // invalid index
        if (openFile[index]!=NULL) {
            delete openFile[index];
            openFile[index] = NULL;
            delete FileName[index];
            FileName[index] = NULL;  
            cout << "\nClosed file at id " << index << ".\n";
            return 0;
        }
        else
        return -1;
    }

    int Read(char* buffer, int charCount, int index) {
        if (index >= FILE_MAX) return -1;
        if (openFile[index] == NULL) return -1;
        int result = openFile[index]->Read(buffer, charCount);
        // if we cannot read enough bytes, we should return -2
        if (result != charCount) return -2;
        return result;
    }

    int Write(char* buffer, int charCount, int index) {
        if (index >= FILE_MAX) return -1;
        if (openFile[index] == NULL || fileOpenMode[index] == MODE_READ)
            return -1;
        return openFile[index]->Write(buffer, charCount);
    }

    int Seek(int pos, int index) 
    {
        if (index <= 1 || index >= FILE_MAX) return -1;
        if (openFile[index] == NULL) return -1;
        // use seek(-1) to move to the end of file
        if (pos == -1) pos = openFile[index]->Length();
        if (pos < 0 || pos > openFile[index]->Length()) return -1;
        return openFile[index]->Seek(pos);
    }

    bool isOpen(char* name){ // check file open or not for remove 
        for (int i = 2; i < FILE_MAX; i++)
            if (FileName[i] != NULL) // Check if have file from FileName[i] is open
                if (strcmp(name, FileName[i]) == 0){ // Check FileName[i] has been called 'name'
                    cout << "\nFile '" << name << "' is in use!\n";
                    return true;
                }

        return false;
    }
    ~FileTable() {
        for (int i = 0; i < FILE_MAX; i++) {
            if (openFile[i]) delete openFile[i];
            if (FileName[i]) delete FileName[i];
        }
        delete[] openFile;
        delete[] fileOpenMode;
        delete[] FileName;
    }
};

#endif