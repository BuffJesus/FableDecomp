#include "fable_threaded_file.h"
extern "C" __declspec(dllimport) unsigned long __stdcall SetFilePointer(void*,long,long*,unsigned long);
extern "C" __declspec(dllimport) int __stdcall SetEndOfFile(void*);
extern "C" __declspec(dllimport) unsigned long __stdcall GetFileSize(void*,unsigned long*);

bool CThreadedFile::ResizeFile(unsigned long length)
{
    SetFilePointer(fileHandle_,length,0,0);
    SetEndOfFile(fileHandle_);
    length_=GetFileSize(fileHandle_,0);
    if (length == length_)
        return true;
    return false;
}
