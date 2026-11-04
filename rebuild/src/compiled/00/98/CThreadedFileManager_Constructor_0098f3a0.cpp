#include "fable_gfuninitialise.h"
#include <process.h>
struct _RTL_CRITICAL_SECTION;
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void* __stdcall CreateEventW(void*,int,int,const unsigned short*);

// Keep the constructor visible here without instantiating contextual map
// destructors in every translation unit that includes the manager declaration.
inline CThreadedFileManager::CPendingObjectMap::CPendingObjectMap()
    : CurrentPosition(static_cast<_STL::_Rb_tree_node_base*>(0))
{
}

CThreadedFileManager::CThreadedFileManager()
{
    InitializeCriticalSection(reinterpret_cast<_RTL_CRITICAL_SECTION*>(CriticalSection));
    BlockPendingObjectProcessingRefCount=0;
    Initialised=true;
    ThreadFinished=false;
    ExitThread=false;
    Event=CreateEventW(0,0,0,0);
    NextObjectUID=1;
    CurrentlyReadingObject=0;
    for(unsigned long device=0;device<THREADED_FILE_DEVICE_ID_COUNT;++device) {
        DeviceTimeSlice[device]=0.2f;
        for(unsigned long priority=0;priority<THREADED_FILE_PRIORITY_COUNT;++priority) {
            DeviceQueue[device][priority].CurrentPosition=DeviceQueue[device][priority].Objects.end();
            CurrentDeviceEndOfTimeSlice[priority]=0.0;
            CurrentDeviceAtPriorityLevel[priority]=THREADED_FILE_DEVICE_ID_FIRST;
        }
    }
    _beginthread(&BeginThread,0x3000,this);
}
