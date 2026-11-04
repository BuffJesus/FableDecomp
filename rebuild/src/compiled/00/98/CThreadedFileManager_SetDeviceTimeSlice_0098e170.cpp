#include "fable_gfuninitialise.h"
#include "fable_threaded_file.h"
extern "C" __declspec(dllimport) int __stdcall SetEvent(void*);
extern "C" __declspec(dllimport) unsigned long __stdcall SleepEx(unsigned long,int);

void CThreadedFileManager::SetDeviceTimeSlice(EThreadedFileDeviceID device,float timeSlice)
{
    DeviceTimeSlice[device]=timeSlice;
}

void CThreadedFileManager::KeepUpdating()
{
    SetEvent(Event);
    SleepEx(0,1);
}

bool CThreadedFile::IsOpenForWrite() const
{
    return openedForWrite_;
}

EThreadedFileDeviceID CThreadedFile::PeekDeviceID() const
{
    return static_cast<EThreadedFileDeviceID>(deviceId_);
}

unsigned long CThreadedFile::PeekPhysicalSortKey() const
{
    return physicalSortKey_;
}

void CThreadedFile::KeepUpdating()
{
    g_FableThreadedFileManager_013BC870.KeepUpdating();
}
