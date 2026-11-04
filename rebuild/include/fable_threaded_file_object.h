#pragma once
// fable_threaded_file.h first: it specialises CCountedPointer<CThreadedFile>, which
// fable_game_text_bank.h instantiates.
#include "fable_threaded_file.h"
#include "fable_gfuninitialise.h"
#include "fable_game_text_bank.h"
// CThreadedFileManager::CFileObject, retail 0x40 bytes (operator new(0x40) at 0x0098F10D).
// FableWin and retail 0098DF00/0098DF20: this object owns its Overlapped block.
struct _OVERLAPPED;
extern "C" __declspec(dllimport) void* __stdcall GlobalFree(void*);
class CThreadedFileManager::CFileObject
{
public:
#ifdef FABLE_THREADED_FILE_OBJECT_INLINE_CTOR
    CFileObject() : Overlapped(0) {}
#else
    CFileObject();
#endif
    ~CFileObject() { if(Overlapped) GlobalFree(Overlapped); }
    CThreadedFileObjectDesc Desc;          // +0x00
    unsigned long UID;                     // +0x18
    EFileLoadingStatus LoadingStatus;      // +0x1C
    _OVERLAPPED* Overlapped;               // +0x20
    CThreadedFileManager* PManager;        // +0x24
    CThreadedFile* PFile;                  // +0x28
    double ReadStartTimeStamp;             // +0x30
    double RequestTimeStamp;               // +0x38
private:
    CFileObject(const CFileObject&);
    const CFileObject& operator=(const CFileObject&);
};

// Pending queues sort by the file's physical sort key, then file position, then object
// address (less<> 0x0098F570; inlined into the pending _M_find 0x0098FE70).
inline bool CThreadedFileManager::CPendingObjectSortKey::operator<(const CPendingObjectSortKey& other) const
{
    unsigned long sortKey=Object->PFile->physicalSortKey_;
    unsigned long otherSortKey=other.Object->PFile->physicalSortKey_;
    if (sortKey < otherSortKey) return true;
    if (sortKey > otherSortKey) return false;
    if (Object->Desc.FilePos < other.Object->Desc.FilePos) return true;
    if (Object->Desc.FilePos > other.Object->Desc.FilePos) return false;
    return Object < other.Object;
}

// The uid map's find stays out of line in retail (0x009902D0, hidden result slot);
// its body lives in 00/99/STLThreadedUIDTree_find_009902d0.cpp.
namespace _STL {
template<> _Rb_tree<unsigned long,
    pair<const unsigned long,CThreadedFileManager::CFileObject*>,
    _Select1st<pair<const unsigned long,CThreadedFileManager::CFileObject*> >,
    less<unsigned long>,
    allocator<pair<const unsigned long,CThreadedFileManager::CFileObject*> > >::const_iterator
_Rb_tree<unsigned long,
    pair<const unsigned long,CThreadedFileManager::CFileObject*>,
    _Select1st<pair<const unsigned long,CThreadedFileManager::CFileObject*> >,
    less<unsigned long>,
    allocator<pair<const unsigned long,CThreadedFileManager::CFileObject*> > >::find(const unsigned long&) const;
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

inline void CThreadedFileManager::AddObjectToPendingMap(CFileObject* object)
{
    const unsigned long device=object->PFile->deviceId_;
    CPendingObjectSortKey key;
    key.Object=object;
    DeviceQueue[device][object->Desc.Priority].Objects[key]=object;
}

