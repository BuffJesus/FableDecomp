#include "fable_gfuninitialise.h"
struct _RTL_CRITICAL_SECTION;
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long);
extern "C" __declspec(dllimport) int __stdcall SetEvent(void*);
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void*);

void CThreadedFileManager::Uninitialise()
{
    if(Initialised) {
        Initialised=false;
        _RTL_CRITICAL_SECTION* section=reinterpret_cast<_RTL_CRITICAL_SECTION*>(CriticalSection);
        EnterCriticalSection(section);
        ExitThread=true;
        SetEvent(Event);
        while(!ThreadFinished) {
            LeaveCriticalSection(section);
            Sleep(0);
            EnterCriticalSection(section);
        }
        if(Event) {
            CloseHandle(Event);
            Event=0;
        }
        LeaveCriticalSection(section);
        DeleteCriticalSection(section);
    }
}
