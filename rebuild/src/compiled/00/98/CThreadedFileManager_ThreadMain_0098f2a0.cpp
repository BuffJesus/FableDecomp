#include "fable_threaded_file_object.h"
struct _RTL_CRITICAL_SECTION;
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void* __stdcall GlobalFree(void*);

void CThreadedFileManager::ThreadMain()
{
    while(!ExitThread) ProcessObjects();
    for(_STL::map<unsigned long,CFileObject*>::iterator it=Objects.begin();it!=Objects.end();++it) {
        CFileObject* object=it->second;
        if(object) {
            delete object;
        }
    }
    _RTL_CRITICAL_SECTION* section=reinterpret_cast<_RTL_CRITICAL_SECTION*>(CriticalSection);
    EnterCriticalSection(section);
    ThreadFinished=true;
    LeaveCriticalSection(section);
}
