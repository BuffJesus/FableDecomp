#include "fable_threaded_file_object.h"
struct _RTL_CRITICAL_SECTION;
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION*);

// Retail 0x0098EFF0: only objects still waiting in a queue (pending or locked) move queues.
void CThreadedFileManager::SetObjectPriority(unsigned long uid, EThreadedFilePriority priority)
{
    _RTL_CRITICAL_SECTION* section=reinterpret_cast<_RTL_CRITICAL_SECTION*>(CriticalSection);
    EnterCriticalSection(section);
    _STL::map<unsigned long, CFileObject*>::iterator it=Objects.find(uid);
    if (it==Objects.end()) {
        LeaveCriticalSection(section);
        return;
    }
    CFileObject* object=it->second;
    if (object->Desc.Priority!=priority &&
        (object->LoadingStatus==FILE_LOADING_STATUS_PENDING || object->LoadingStatus==FILE_LOADING_STATUS_PENDING_LOCKED)) {
        RemoveObjectFromPendingMap(object);
        object->Desc.Priority=priority;
        AddObjectToPendingMap(object);
    }
    LeaveCriticalSection(section);
}
