#include "fable_threaded_file_object.h"
struct _RTL_CRITICAL_SECTION;
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void* __stdcall GlobalFree(void*);

// The uid map's find is out of line in retail (0x009902D0, hidden result slot).
namespace _STL {
template<> _Rb_tree<unsigned long,
    pair<const unsigned long,CThreadedFileManager::CFileObject*>,
    _Select1st<pair<const unsigned long,CThreadedFileManager::CFileObject*> >,
    less<unsigned long>,
    allocator<pair<const unsigned long,CThreadedFileManager::CFileObject*> > >::iterator
_Rb_tree<unsigned long,
    pair<const unsigned long,CThreadedFileManager::CFileObject*>,
    _Select1st<pair<const unsigned long,CThreadedFileManager::CFileObject*> >,
    less<unsigned long>,
    allocator<pair<const unsigned long,CThreadedFileManager::CFileObject*> > >::find(const unsigned long&);
}

// Retail 0x0098EB20: objects still being read (locked, reading or completed) stay queued.
bool CThreadedFileManager::RemoveObject(unsigned long uid)
{
    _RTL_CRITICAL_SECTION* section=reinterpret_cast<_RTL_CRITICAL_SECTION*>(CriticalSection);
    EnterCriticalSection(section);
    _STL::map<unsigned long, CFileObject*>::iterator it=Objects.find(uid);
    if (it!=Objects.end()) {
        CFileObject* object=it->second;
        if (object->LoadingStatus==FILE_LOADING_STATUS_PENDING)
            RemoveObjectFromPendingMap(object);
        if (object->LoadingStatus==FILE_LOADING_STATUS_PENDING_LOCKED ||
            object->LoadingStatus==FILE_LOADING_STATUS_READING ||
            object->LoadingStatus==FILE_LOADING_STATUS_READ_COMPLETED) {
            LeaveCriticalSection(section);
            return false;
        }
        delete object;
        Objects.erase(it);
    }
    LeaveCriticalSection(section);
    return true;
}
