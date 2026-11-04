// Retail 0098E410; owner and const signature confirmed by FableWin PDB.
#include "fable_threaded_file_object.h"
struct _RTL_CRITICAL_SECTION;
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION*);

long CThreadedFileManager::GetTotalBytesPendingCount() const
{
    _RTL_CRITICAL_SECTION* section = reinterpret_cast<_RTL_CRITICAL_SECTION*>(const_cast<unsigned long*>(CriticalSection));
    EnterCriticalSection(section);
    unsigned long total = 0;
    typedef _STL::map<unsigned long, CFileObject*> ObjectMap;
    // The shared STLport subset has mutable iterators only; no values are changed.
    ObjectMap& objects = const_cast<ObjectMap&>(Objects);
    for (ObjectMap::iterator it = objects.begin(); it != objects.end(); ++it) {
        if (it->second->LoadingStatus != FILE_LOADING_STATUS_FINISHED)
            total += it->second->Desc.Size;
    }
    LeaveCriticalSection(section);
    return total;
}
