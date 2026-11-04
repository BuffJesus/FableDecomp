#include "fable_threaded_file_object.h"
struct _RTL_CRITICAL_SECTION;
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION*);

bool CThreadedFileManager::IsObjectFinished(unsigned long uid) const
{
    _RTL_CRITICAL_SECTION* section=reinterpret_cast<_RTL_CRITICAL_SECTION*>(const_cast<unsigned long*>(CriticalSection));
    EnterCriticalSection(section);
    _STL::map<unsigned long,CFileObject*>::const_iterator it=Objects.find(uid);
    bool finished=it==Objects.end() || it->second->LoadingStatus==FILE_LOADING_STATUS_FINISHED;
    LeaveCriticalSection(section);
    return finished;
}

bool CThreadedFileManager::ObjectExists(unsigned long uid) const
{
    _RTL_CRITICAL_SECTION* section=reinterpret_cast<_RTL_CRITICAL_SECTION*>(const_cast<unsigned long*>(CriticalSection));
    EnterCriticalSection(section);
    _STL::map<unsigned long,CFileObject*>::const_iterator end=Objects.end();
    _STL::map<unsigned long,CFileObject*>::const_iterator it=Objects.find(uid);
    LeaveCriticalSection(section);
    if (it != end)
        return true;
    return false;
}

bool CThreadedFile::IsObjectFinished(unsigned long uid) const
{
    return g_FableThreadedFileManager_013BC870.IsObjectFinished(uid);
}

bool CThreadedFile::ObjectExists(unsigned long uid) const
{
    return g_FableThreadedFileManager_013BC870.ObjectExists(uid);
}
