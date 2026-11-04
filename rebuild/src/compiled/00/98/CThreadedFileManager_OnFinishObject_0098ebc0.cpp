#include "fable_threaded_file_object.h"
struct _RTL_CRITICAL_SECTION;
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void* __stdcall GlobalFree(void*);

// Retail 0x0098EBC0: the completion callback runs outside the manager lock.
void CThreadedFileManager::OnFinishObject(CFileObject* object)
{
    if (object->Desc.PCallback) {
        _RTL_CRITICAL_SECTION* section=reinterpret_cast<_RTL_CRITICAL_SECTION*>(CriticalSection);
        LeaveCriticalSection(section);
        object->Desc.PCallback->OnFileReadCompletion(true);
        EnterCriticalSection(section);
        object->Desc.PCallback=0;
    }
    object->LoadingStatus=FILE_LOADING_STATUS_FINISHED;
    CurrentlyReadingObject=0;
    if (!object->Desc.QueueWhenFinished) {
        Objects.erase(object->UID);
        delete object;
    }
}
