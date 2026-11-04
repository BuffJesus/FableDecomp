#include "rebuild_abi.h"
#include <ctype.h>
extern "C" {
__declspec(dllimport) void* __stdcall CreateFileA(const char*,unsigned long,unsigned long,void*,unsigned long,unsigned long,void*);
__declspec(dllimport) unsigned long __stdcall GetLastError();
__declspec(dllimport) unsigned long __stdcall GetFileSize(void*,unsigned long*);
__declspec(dllimport) void* __stdcall GlobalAlloc(unsigned int,unsigned long);
__declspec(dllimport) int __stdcall ReadFile(void*,void*,unsigned long,unsigned long*,void*);
__declspec(dllimport) int __stdcall CloseHandle(void*);
__declspec(dllimport) void* __stdcall GlobalFree(void*);
}
static inline char* FindAssignment(char* begin,char* end)
{
    while(begin<end && *begin!='=')++begin;
    return begin;
}
void FABLE_FASTCALL InitializeMemorySystem()
{
    void* file=CreateFileA("boot.ini",0x80000000,1,0,3,0x08000000,0);
    if(file==reinterpret_cast<void*>(-1)) {GetLastError();return;}
    unsigned long size=GetFileSize(file,0);
    char* buffer=static_cast<char*>(GlobalAlloc(0x40,size));
    unsigned long read;
    ReadFile(file,buffer,size,&read,0);
    CloseHandle(file);
    char* end=buffer+size;
    char* next=buffer;
    while(next<end) {
        while(next<end && isspace(*next))++next;
        char* lineEnd=next;
        while(lineEnd<end && *lineEnd!='\n' && *lineEnd!='\r')++lineEnd;
        if(next+1<end && next[0]=='/' && next[1]=='/') {next=lineEnd;continue;}
        char* trimEnd=lineEnd;
        while(next<trimEnd && isspace(trimEnd[-1]))--trimEnd;
        char* equal=FindAssignment(next,trimEnd);
        if(next<equal && equal<trimEnd) {
            char* keyEnd=equal;
            while(next<keyEnd && isspace(keyEnd[-1]))--keyEnd;
            next=equal+1;
            while(next<trimEnd && isspace(*next))++next;
        }
        next=lineEnd;
    }
    GlobalFree(buffer);
}
