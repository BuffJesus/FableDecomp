#include "fable_threaded_file_object.h"
struct _RTL_CRITICAL_SECTION;
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) int __stdcall SetEvent(void*);
extern "C" __declspec(dllimport) unsigned long __stdcall SleepEx(unsigned long,int);

void CThreadedFileManager::WaitForObjectToFinish(unsigned long uid)
{
    _RTL_CRITICAL_SECTION* section=reinterpret_cast<_RTL_CRITICAL_SECTION*>(CriticalSection);
    EnterCriticalSection(section);
    _STL::map<unsigned long,CFileObject*>::iterator initial=Objects.find(uid);
    if(initial==Objects.end() || initial->second->LoadingStatus==FILE_LOADING_STATUS_FINISHED) {
        LeaveCriticalSection(section);
        return;
    }
    SetObjectPriority(uid,THREADED_FILE_PRIORITY_BLOCKING);
    for(;;) {
        _STL::map<unsigned long,CFileObject*>::iterator current=Objects.find(uid);
        if(current==Objects.end() || current->second->LoadingStatus==FILE_LOADING_STATUS_FINISHED) {
            LeaveCriticalSection(section);
            return;
        }
        LeaveCriticalSection(section);
        SetEvent(Event);
        SleepEx(0,1);
        EnterCriticalSection(section);
    }
}
