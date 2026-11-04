#define FABLE_THREADED_FILE_OBJECT_INLINE_CTOR
#include "fable_threaded_file_object.h"
struct _RTL_CRITICAL_SECTION;
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void* __stdcall GlobalAlloc(unsigned int,unsigned long);
extern "C" __declspec(dllimport) int __stdcall SetEvent(void*);

unsigned long CThreadedFileManager::AddObject(const CThreadedFileObjectDesc& desc,CThreadedFile& file)
{
    _RTL_CRITICAL_SECTION* section=reinterpret_cast<_RTL_CRITICAL_SECTION*>(CriticalSection);
    EnterCriticalSection(section);
    unsigned long uid=NextObjectUID++;
    CFileObject* object=new CFileObject;
    object->UID=uid;
    object->Desc=desc;
    object->PFile=&file;
    object->PManager=this;
    object->LoadingStatus=FILE_LOADING_STATUS_PENDING;
    object->Overlapped=static_cast<_OVERLAPPED*>(GlobalAlloc(0x40,20));
    Objects[uid]=object;
    AddObjectToPendingMap(object);
    SetEvent(Event);
    LeaveCriticalSection(section);
    return uid;
}
