// lib_memory_allocator.cpp support members, retail 0x009F1E80-0x009F20A0 in
// Ego_r's order: the debug-header helpers, the tracking ID, the allocator's own
// VirtualAlloc/VirtualFree storage, the CRT wrappers, the synchronisation trio,
// the per-size-class peeks and the (empty) stack-tracker hooks.
#define FABLE_MEMORY_ALLOCATOR_OUT_OF_LINE_SUPPORT
#include "fable_memory_allocator.h"
#include <stdlib.h>
#include <string.h>

extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*, unsigned long, unsigned long, unsigned long);
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(_RTL_CRITICAL_SECTION*);
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(_RTL_CRITICAL_SECTION*);

// Retail 009F1E80: inside the special range the ID is the 16MB block number,
// elsewhere the byte stored just ahead of the allocation's header.
unsigned char CMemoryAllocator::GetAllocIDFromPMem(void* pmem)
{
    if ((unsigned long)pmem >= g_FableAllocatorRangeStart_013CAAA4 && (unsigned long)pmem < g_FableAllocatorRangeEnd_013CAAA8)
        return (unsigned char)(((unsigned long)pmem - g_FableAllocatorRangeStart_013CAAA4) >> 24);
    return ((unsigned char*)pmem)[-5];
}

// Retail 009F1EB0.
void* CMemoryAllocator::GetPMemFromPMemIncDebugHeader(void* pmem)
{
    return (char*)pmem + sizeof(CDebugHeader);
}

// Retail 009F1EC0.
void* CMemoryAllocator::GetPMemIncDebugHeaderFromPMem(void* pmem)
{
    return (char*)pmem - sizeof(CDebugHeader);
}

// Retail 009F1ED0.
void CMemoryAllocator::InitDebugHeader(CDebugHeader& header)
{
    header.Guard = -1;
}

// Retail 009F1EE0.
void CMemoryAllocator::InitDebugRecord(CDebugRecord& record, long size)
{
    record.Guard = -1;
    record.TrackingID = CurrentTrackingID;
    record.Size = size;
}

// Retail 009F26F0: the plain record, then the source file's name (without its
// directory) and line; line -1 means no source location.
void CMemoryAllocator::InitTrackingDebugRecord(CTrackingDebugRecord& record, const char* file, long line, const char*, long size)
{
    InitDebugRecord((CDebugRecord&)record, size);
    if (line == -1)
    {
        record.Filename[0] = 0;
        record.Line = -1;
        return;
    }
    bool unknown_file = _stricmp(file, "Unknown") == 0;
    (void)unknown_file;
    char name[256];
    char extension[256];
    _splitpath(file, 0, 0, name, extension);
    strcat(name, extension);
    strncpy(record.Filename, name, sizeof(record.Filename) - 1);
    record.Line = line;
}

// Retail 009F1F20.
void CMemoryAllocator::SetCurrentTrackingID(unsigned long id)
{
    CurrentTrackingID = (unsigned char)id;
}

// Retail 009F1F30.
unsigned long CMemoryAllocator::GetCurrentTrackingID()
{
    return CurrentTrackingID;
}

// Retail 009F1F50.
void CMemoryAllocator::InternalValidate()
{
}

// Retail 009F1F60.
CMemoryAllocator::CDebugHeader& CMemoryAllocator::GetDebugHeader(void* pmem)
{
    return *((CDebugHeader*)pmem - 1);
}

// Retail 009F1F70: the allocator object lives in its own committed pages.
void* FABLE_FASTCALL CMemoryAllocator::operator new(unsigned int size)
{
    return VirtualAlloc(0, size, 0x3000, 4);
}

// Retail 009F1F90.
void FABLE_FASTCALL CMemoryAllocator::operator delete(void* area)
{
    VirtualFree(area, 0, 0x8000);
}

// Retail 009F1FA0.
void* FABLE_FASTCALL GFCalloc(unsigned int count, unsigned int size)
{
    return calloc(count, size);
}

// Retail 009F1FB0.
void* FABLE_FASTCALL GFMalloc(unsigned int size)
{
    return malloc(size);
}

// Retail 009F1FC0.
void FABLE_FASTCALL GFFree(void* memory)
{
    free(memory);
}

// Retail 009F1FD0.
void* FABLE_FASTCALL GFRealloc(void* memory, unsigned int size)
{
    return realloc(memory, size);
}

// Retail 009F1FE0.
void FABLE_FASTCALL CMemoryAllocator::InitSynchronisation()
{
    InitializeCriticalSection(&g_CMemoryAllocatorCriticalSection_013CAA70);
}

// Retail 009F1FF0.
void FABLE_FASTCALL CMemoryAllocator::UninitSynchronisation()
{
    DeleteCriticalSection(&g_CMemoryAllocatorCriticalSection_013CAA70);
}

// Retail 009F2000.
void FABLE_FASTCALL CMemoryAllocator::EnterSynchronisation()
{
    EnterCriticalSection(&g_CMemoryAllocatorCriticalSection_013CAA70);
}

// Retail 009F2010.
void FABLE_FASTCALL CMemoryAllocator::LeaveSynchronisation()
{
    LeaveCriticalSection(&g_CMemoryAllocatorCriticalSection_013CAA70);
}

// Retail 009F2020.
long CMemoryAllocator::PeekAllocatorMemoryControlled(long index) const
{
    return AllocatorInfo[index].PAllocator->GetTotalMemoryControlledByAllocator();
}

// Retail 009F2040.
long CMemoryAllocator::PeekAllocatorMemoryAllocated(long index) const
{
    return AllocatorInfo[index].PAllocator->GetTotalMemoryAllocated();
}

// Retail 009F2060: size classes with a maximum size are fixed-size.
bool CMemoryAllocator::IsAllocatorFixedSize(unsigned long index) const
{
    return AllocatorInfo[index].MaxAllocSize > 0;
}

// Retail 009F2080.
void CMemoryAllocator::SaveStackMemoryTrackerLog(const char*)
{
}

// Retail 009F2090.
void CMemoryAllocator::StackMemoryTrackerStartAutosavingMaxMemory(const char*)
{
}

// Retail 009F20A0.
void CMemoryAllocator::StackMemoryTrackerStopAutosavingMaxMemory()
{
}

// Retail 009F2830: the record ends the allocation. FableWin's body computes the
// offset once more before using it (the extra GetAllocationSize call survives).
CMemoryAllocator::CDebugRecord& CMemoryAllocator::GetDebugRecord(void* pmem)
{
    unsigned long offset = GetAllocationSize(pmem) - sizeof(CDebugRecord);
    (void)offset;
    return *(CDebugRecord*)((char*)pmem + GetAllocationSize(pmem) - sizeof(CDebugRecord));
}

// Retail 009F2850.
CMemoryAllocator::CTrackingDebugRecord& CMemoryAllocator::GetTrackingDebugRecord(void* pmem)
{
    return *(CTrackingDebugRecord*)((char*)pmem + GetAllocationSize(pmem) - sizeof(CTrackingDebugRecord));
}

// Retail 009F29C0: both guard words intact.
bool CMemoryAllocator::CheckChecksums(void* pmem)
{
    if (GetDebugHeader(pmem).Guard != -1)
        return false;
    CDebugRecord* record = &GetDebugRecord(pmem);
    if (record->Guard != -1)
        return false;
    return true;
}
