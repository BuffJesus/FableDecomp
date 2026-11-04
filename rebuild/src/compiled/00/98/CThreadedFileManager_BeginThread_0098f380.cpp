#include "fable_gfuninitialise.h"
extern "C" __declspec(dllimport) void* __stdcall GetCurrentThread();
extern "C" __declspec(dllimport) int __stdcall SetThreadPriority(void*,int);

void __cdecl CThreadedFileManager::BeginThread(void* context)
{
    SetThreadPriority(GetCurrentThread(),1);
    static_cast<CThreadedFileManager*>(context)->ThreadMain();
}
