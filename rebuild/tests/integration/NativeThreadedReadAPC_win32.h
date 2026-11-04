#pragma once
// Minimal x86 ABI for VC7.1, which has no Windows SDK headers in this install.
// ReadFileEx contract: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-readfileex
#include "fable_threaded_file_overlapped.h"
#include <stddef.h>
typedef unsigned long DWORD;
typedef long LONG;
typedef int BOOL;
typedef void* HANDLE;
typedef _OVERLAPPED OVERLAPPED;
#define WINAPI __stdcall
#define CALLBACK __stdcall
#define MEM_COMMIT 0x1000
#define MEM_RESERVE 0x2000
#define MEM_RELEASE 0x8000
#define PAGE_READWRITE 0x04
#define PAGE_EXECUTE_READ 0x20
#define GENERIC_READ 0x80000000UL
#define FILE_SHARE_READ 1
#define OPEN_EXISTING 3
#define FILE_FLAG_OVERLAPPED 0x40000000
#define INVALID_HANDLE_VALUE ((HANDLE)(LONG)-1)
#define WAIT_OBJECT_0 0
#define WAIT_TIMEOUT 258
#define WAIT_IO_COMPLETION 192
#define ERROR_SUCCESS 0
typedef void (CALLBACK* IOCompletion)(DWORD,DWORD,OVERLAPPED*);
extern "C" {
__declspec(dllimport) void* WINAPI VirtualAlloc(void*,size_t,DWORD,DWORD);
__declspec(dllimport) BOOL WINAPI VirtualFree(void*,size_t,DWORD);
__declspec(dllimport) BOOL WINAPI VirtualProtect(void*,size_t,DWORD,DWORD*);
__declspec(dllimport) BOOL WINAPI FlushInstructionCache(HANDLE,const void*,size_t);
__declspec(dllimport) HANDLE WINAPI GetCurrentProcess();
__declspec(dllimport) DWORD WINAPI GetLastError();
__declspec(dllimport) void WINAPI SetLastError(DWORD);
__declspec(dllimport) HANDLE WINAPI CreateEventA(void*,BOOL,BOOL,const char*);
__declspec(dllimport) BOOL WINAPI CloseHandle(HANDLE);
__declspec(dllimport) BOOL WINAPI SetEvent(HANDLE);
__declspec(dllimport) DWORD WINAPI WaitForSingleObject(HANDLE,DWORD);
__declspec(dllimport) DWORD WINAPI SleepEx(DWORD,BOOL);
__declspec(dllimport) void WINAPI Sleep(DWORD);
__declspec(dllimport) HANDLE WINAPI CreateFileA(const char*,DWORD,DWORD,void*,DWORD,DWORD,HANDLE);
__declspec(dllimport) BOOL WINAPI ReadFileEx(HANDLE,void*,DWORD,OVERLAPPED*,IOCompletion);
__declspec(dllimport) __declspec(noreturn) void WINAPI ExitProcess(unsigned int);
__declspec(dllimport) DWORD WINAPI GetTickCount();
}
