// Retail 0098ECD0. Near match; worker submission and scheduling.
#include <string.h>
#include "fable_threaded_file_object.h"
#include "fable_threaded_file_overlapped.h"
#include "fable_file_installer.h"
double FABLE_FASTCALL GFGetTime();
extern "C" {
__declspec(dllimport) void __stdcall EnterCriticalSection(void*);
__declspec(dllimport) void __stdcall LeaveCriticalSection(void*);
__declspec(dllimport) unsigned long __stdcall WaitForSingleObjectEx(void*,unsigned long,int);
__declspec(dllimport) unsigned long __stdcall SleepEx(unsigned long,int);
__declspec(dllimport) int __stdcall SetEvent(void*);
__declspec(dllimport) int __stdcall ReadFileEx(void*,void*,unsigned long,_OVERLAPPED*,void (__stdcall*)(unsigned long,unsigned long,_OVERLAPPED*));
__declspec(dllimport) int __stdcall WriteFileEx(void*,const void*,unsigned long,_OVERLAPPED*,void (__stdcall*)(unsigned long,unsigned long,_OVERLAPPED*));
}

void CThreadedFileManager::ProcessObjects()
{
    CFileObject* object;
    double now;
    EnterCriticalSection(CriticalSection);
    if (CurrentlyReadingObject) {
        if (CurrentlyReadingObject->LoadingStatus == FILE_LOADING_STATUS_READING) {
            LeaveCriticalSection(CriticalSection);
            goto wait_for_event;
        }
        OnFinishObject(CurrentlyReadingObject);
    }
    if (BlockPendingObjectProcessingRefCount) {
        LeaveCriticalSection(CriticalSection);
        goto wait_for_event;
    }
    object = 0;
    now = GFGetTime();
    for (int priority = THREADED_FILE_PRIORITY_COUNT-1; priority >= 0 && !object; --priority) {
        EThreadedFileDeviceID& device = CurrentDeviceAtPriorityLevel[priority];
        if (now < CurrentDeviceEndOfTimeSlice[priority]) {
            CPendingObjectMap& queue = DeviceQueue[device][priority];
            if (queue.CurrentPosition == queue.Objects.end())
                queue.CurrentPosition = queue.Objects.begin();
            if (queue.CurrentPosition != queue.Objects.end())
                object = queue.CurrentPosition->second;
        }
        if (!object) {
            device = static_cast<EThreadedFileDeviceID>(device+1);
            if (device == THREADED_FILE_DEVICE_ID_COUNT) device = THREADED_FILE_DEVICE_ID_FIRST;
            for (int attempt = 0; attempt < THREADED_FILE_DEVICE_ID_COUNT; ++attempt) {
                CPendingObjectMap& queue = DeviceQueue[device][priority];
                if (queue.CurrentPosition == queue.Objects.end()) {
                    queue.CurrentPosition = queue.Objects.begin();
                    if (queue.CurrentPosition == queue.Objects.end()) {
                        device = static_cast<EThreadedFileDeviceID>(device+1);
                        if (device == THREADED_FILE_DEVICE_ID_COUNT) device = THREADED_FILE_DEVICE_ID_FIRST;
                        continue;
                    }
                }
                object = queue.CurrentPosition->second;
                CurrentDeviceEndOfTimeSlice[priority] = now + DeviceTimeSlice[device];
                break;
            }
        }
    }
    if (!object) {
        LeaveCriticalSection(CriticalSection);
        goto wait_for_event;
    }
    if (object->PFile->openedForWrite_) {
        memset(object->Overlapped,0,sizeof(_OVERLAPPED));
        object->Overlapped->Offset = object->Desc.FilePos;
        object->Overlapped->hEvent = object;
        RemoveObjectFromPendingMap(object);
        CurrentlyReadingObject = object;
        object->LoadingStatus = FILE_LOADING_STATUS_READING;
        LeaveCriticalSection(CriticalSection);
        if (g_CFileInstallerSingleton_013CA818.object) CFileInstallerSingleton::Get().Pause();
        if (!WriteFileEx(object->PFile->fileHandle_,object->Desc.PDst,object->Desc.Size,object->Overlapped,OnFileReadCompletion)) {
            if (m_harddiskErrorCallback) m_harddiskErrorCallback("");
            if (g_CFileInstallerSingleton_013CA818.object) CFileInstallerSingleton::Get().Unpause();
        }
    } else {
        bool failed = false;
        if (object->Desc.PCallback) {
            EThreadedFilePriority priority = object->Desc.Priority;
            object->LoadingStatus = FILE_LOADING_STATUS_PENDING_LOCKED;
            LeaveCriticalSection(CriticalSection);
            if (!object->Desc.PCallback->OnFileReadBegin(&object->Desc.PDst,priority)) failed = true;
            EnterCriticalSection(CriticalSection);
        }
        if (failed) {
            object->LoadingStatus = FILE_LOADING_STATUS_PENDING;
            LeaveCriticalSection(CriticalSection);
            SetEvent(Event);
            SleepEx(1,1);
            return;
        }
        memset(object->Overlapped,0,sizeof(_OVERLAPPED));
        object->Overlapped->Offset = object->Desc.FilePos;
        object->Overlapped->hEvent = object;
        RemoveObjectFromPendingMap(object);
        CurrentlyReadingObject = object;
        object->LoadingStatus = FILE_LOADING_STATUS_READING;
        LeaveCriticalSection(CriticalSection);
        if (g_CFileInstallerSingleton_013CA818.object) CFileInstallerSingleton::Get().Pause();
        ReadFileEx(object->PFile->fileHandle_,object->Desc.PDst,object->Desc.Size,object->Overlapped,OnFileReadCompletion);
    }
wait_for_event:
    WaitForSingleObjectEx(Event,0xffffffffUL,1);
}
