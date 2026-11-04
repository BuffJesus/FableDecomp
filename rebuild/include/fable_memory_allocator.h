#pragma once

// CMemoryAllocator: the engine heap singleton and recovered native allocator ABI.

#include "rebuild_abi.h"
#include <string.h>

class CMemoryAllocator;
struct InstallerBufferChunk;
void FABLE_FASTCALL FableInstallerDestroyBufferRange(InstallerBufferChunk*,InstallerBufferChunk*,void*);
void FABLE_CDECL FableMemoryAllocatorShutdown_009f2d20();

// Win32 critical-section imports (retail IAT slots 0x0143FE0C / 0x0143FE10);
// compatible with <windows.h>'s declarations.
struct _RTL_CRITICAL_SECTION;
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION* criticalSection);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION* criticalSection);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);

// MemoryAllocator.cpp's file-static CRITICAL_SECTION "CriticalSection"
// (FableWin), retail 0x013CAA70. Declared extern here so each reconstructed
// member shares the one object.
extern _RTL_CRITICAL_SECTION g_CMemoryAllocatorCriticalSection_013CAA70;

// FableWin CMemoryAllocatorBase: the per-size-class allocator interface.
// Slot order from FableWin's ??_7CMemoryAllocatorFixedSize@@6B@ (the base
// vtable is all _purecall except slot 13, ConsolidateFreeAreas); retail
// CMemoryAllocator::ConsolidateFreeAreas 0x009F2660 calls slot 13 ([vptr+0x34]).
class CMemoryAllocatorBase
{
public:
    CMemoryAllocatorBase(unsigned long alignment); // retail 00A65AD0
    struct CPerformanceStats {
        CPerformanceStats() {
            AllocsThisFrame=0;TotalAllocs=0;TotalFrames=0;MaxAllocsPerFrame=0;
            for(unsigned long i=0;i<256;++i)TotalAllocsPerID[i]=0;
        }
        unsigned long AllocsThisFrame,TotalAllocs,TotalFrames,MaxAllocsPerFrame;
        unsigned long TotalAllocsPerID[256];
        unsigned long TotalAllocsPerIDLastFrame[256];
        unsigned long TotalAllocsPerIDThisFrame[256];
    };
    static void* FABLE_FASTCALL operator new(size_t size); // retail 00A66730
    static void FABLE_FASTCALL operator delete(void* area); // retail 00A66750
    virtual void* Allocate(unsigned long size) = 0;                                  // 0
    virtual void Deallocate(void* area) = 0;                                         // 1
    virtual unsigned long GetAllAllocatedAreas(void** areas, unsigned long max) = 0; // 2
    virtual unsigned long GetAllocationSize(void* area) = 0;                         // 3
    virtual unsigned long GetAllocationSizeIncludingHeader(void* area) = 0;          // 4
    virtual unsigned long GetLargestFreeAreaSize() = 0;                              // 5
    virtual unsigned long GetTotalMemoryControlledByAllocator() = 0;                 // 6
    virtual unsigned long GetTotalMemoryAllocated() = 0;                             // 7
    virtual unsigned long GetNoFreeAreas() = 0;                                      // 8
    virtual unsigned long GetNoAllocatedAreas() = 0;                                 // 9
    virtual float GetFragmentation() = 0;                                            // 10
    virtual void* GetAllocatedArea(long index) = 0;                                  // 11
    virtual unsigned long GetHeaderOverhead() = 0;                                   // 12
    virtual void ConsolidateFreeAreas();                                             // 13
    virtual void Validate() = 0;                                                     // 14
    unsigned long Align(unsigned long size) const;
    void SetCurrentAlignment();
    // The donor base has no virtual destructor. Its trivial destruction lets
    // allocator shutdown release the object through the class-specific delete.
protected:
    CPerformanceStats PerformanceStats; // +004
    unsigned long Alignment;           // +C14
    unsigned long AlignmentShift;      // +C18
};
FABLE_STATIC_ASSERT(sizeof(CMemoryAllocatorBase::CPerformanceStats)==0xc10);
FABLE_STATIC_ASSERT(sizeof(CMemoryAllocatorBase)==0xc1c);

// The singleton pointer ?Get@CMemoryAllocator@@SIPAV1@XZ returns (0x013CAA90).
extern CMemoryAllocator* g_FableMemoryAllocator_013CAA90;
// Retail special allocation range; donor variable names remain unidentified.
extern unsigned long g_FableAllocatorRangeStart_013CAAA4;
extern unsigned long g_FableAllocatorRangeEnd_013CAAA8;

class CMemoryAllocator
{
public:
    // Ego_r identifies this lookup record as a nested allocator type.
    struct CAllocSizeToAllocator {
        CMemoryAllocatorBase* PAllocator;
        unsigned char ID;
    };
    // FableWin ?Get@CMemoryAllocator@@SIPAV1@XZ; inline in retail
    // (`mov ecx, [0x013CAA90]` at each call site).
    static CMemoryAllocator* FABLE_FASTCALL Get()
    {
        return g_FableMemoryAllocator_013CAA90;
    }

    // Ego_r lib_memory_allocator.cpp support members, retail 0x009F1E80-0x009F20A0
    // (CMemoryAllocator_OutOfLineSupport_009f1f90.cpp). The tracking ID is the
    // byte at +414; the stack tracker hooks are empty in this build.
    void SetCurrentTrackingID(unsigned long id);
    unsigned long GetCurrentTrackingID();
    void InternalValidate();
    long PeekAllocatorMemoryControlled(long index) const;
    long PeekAllocatorMemoryAllocated(long index) const;
    bool IsAllocatorFixedSize(unsigned long index) const;
    void SaveStackMemoryTrackerLog(const char* filename);
    void StackMemoryTrackerStartAutosavingMaxMemory(const char* filename);
    void StackMemoryTrackerStopAutosavingMaxMemory();

    // FableWin ?ConsolidateFreeAreas@CMemoryAllocator@@QAEXXZ (0x009F2660) and
    // ?ClearPerformanceStats@CMemoryAllocator@@QAEXXZ (0x009F26B0).
    void ConsolidateFreeAreas();
    void ClearPerformanceStats();

    // FableWin ?GetTotalMemoryAllocated@CMemoryAllocator@@QAEKXZ (retail
    // 0x009F2450; body matches Ego_r's 0x00B45FD0 — the manifest's
    // GetHeaderOverhead label is a propagation) and ?NewFrame@@QAEXXZ
    // (retail 0x009F26D0, after ClearPerformanceStats as in Ego_r).
    // Callers: CMainGameComponent::InitDefinitions 0x00416005 and
    // SignalFrameUpdate 0x00416047.
    unsigned long GetTotalMemoryControlledByAllocator(); // retail 009F2400
    unsigned long GetTotalMemoryAllocated();
    unsigned long GetTotalFixedSizeMemoryAllocated();    // retail 009F24A0
    unsigned long GetNoFreeAreas();                      // retail 009F2500
    unsigned long GetNoAllocatedAreas();
    float GetFragmentation();                            // retail 009F25A0
    unsigned long GetHeaderOverhead();                   // retail 009F2610
    unsigned long GetTotalMemoryUsedWithTrackingID(unsigned char id); // retail 009F27E0
    void Validate();                                     // retail 009F2810
    unsigned long GetNoAllocations(); // retail 009F1F10; singleton allocation counter
    unsigned long GetAllocationSize(void* allocation); // retail 009F2390
    static void* FABLE_FASTCALL Allocate(unsigned long size,unsigned long category,const char* file,unsigned long line,const char* function); // 009F2B90
    static void* FABLE_FASTCALL AllocateAligned(unsigned long size,long alignment); // 009F2BE0
    static void FABLE_FASTCALL DeallocateAligned(void* allocation); // 009F2B20
    static void FABLE_FASTCALL Deallocate(void* allocation); // inline in retail callers
    static void* FABLE_FASTCALL Allocate(unsigned long,unsigned long);
    void NewFrame();
    void OnFailedAllocation(); // retail 009F1F00; no-op in shipping build
    void InternalValidateArea(void* area); // retail 009F1F40; shipping no-op

    // Ego_r CMemoryAllocator::CAllocatorInfo (0x1C).
    struct CAllocatorInfo
    {
        long MaxAllocSize;
        long MinChunkSize;
        long InitialSize;
        long Alignment;
        unsigned long Flags;
        unsigned long InternalFlags;
        CMemoryAllocatorBase* PAllocator;       // +0x18
    };

protected:
    // Ego_r CDebugHeader / CDebugRecord: a guard word ahead of each debug
    // allocation, and the guard, tracking ID and size of a tracked one (sizes
    // from the retail writers; the field names are ours).
    class CDebugHeader {
    public:
        long Guard;
    };
    class CDebugRecord {
    public:
        long Guard;
        unsigned long TrackingID;
        long Size;
        unsigned long Unknown0C[2];
    };
    // 0x48 bytes (GetTrackingDebugRecord's offset); InitTrackingDebugRecord
    // 0x009F26F0 fills the guard, ID, size, a 47-character file name at +10
    // and the line at +40.
    class CTrackingDebugRecord {
    public:
        long Guard;
        unsigned long TrackingID;
        long Size;
        unsigned long Unknown0C;
        char Filename[0x30];
        long Line;
        unsigned long Unknown44;
    };
    CDebugRecord& GetDebugRecord(void* pmem);                  // 009F2830
    CTrackingDebugRecord& GetTrackingDebugRecord(void* pmem);  // 009F2850
    bool CheckChecksums(void* pmem);                           // 009F29C0
    void InitTrackingDebugRecord(CTrackingDebugRecord& record, const char* file, long line,
                                 const char* function, long size); // 009F26F0
    unsigned char GetAllocIDFromPMem(void* pmem);              // 009F1E80
    void* GetPMemFromPMemIncDebugHeader(void* pmem);           // 009F1EB0
    void* GetPMemIncDebugHeaderFromPMem(void* pmem);           // 009F1EC0
    void InitDebugHeader(CDebugHeader& header);                // 009F1ED0
    void InitDebugRecord(CDebugRecord& record, long size);     // 009F1EE0
    CDebugHeader& GetDebugHeader(void* pmem);                  // 009F1F60
    static void* FABLE_FASTCALL operator new(size_t size); // 009F1F70
    static void FABLE_FASTCALL InitSynchronisation();          // 009F1FE0
    static void FABLE_FASTCALL UninitSynchronisation();        // 009F1FF0
    void InitDebugInfo(void* allocation,bool debug,long requestedSize); // 009F28D0
    void* InternalAllocate(long size,unsigned long category,const char* file,long line,const char* function); // 009F2AC0
    void* InternalAllocate(long size,unsigned long category,bool debug); // 009F2A40
    void InternalDeallocate(void* allocation); // 009F2920
    CMemoryAllocator(void* heap); // retail 009F20B0
    ~CMemoryAllocator(); // retail 009F2310; donor protected destructor
    static void FABLE_FASTCALL Create(); // retail 009F2870
    static void FABLE_FASTCALL Destroy(); // retail 009F2A00
    static unsigned long MemoryUseBeforeAllocator; // retail 013CAA88
#ifdef FABLE_MEMORY_ALLOCATOR_OUT_OF_LINE_SUPPORT
    // Retail array destruction calls these support routines out of line.
    static void FABLE_FASTCALL operator delete(void* area);
    static void FABLE_FASTCALL EnterSynchronisation();
    static void FABLE_FASTCALL LeaveSynchronisation();
#else
    static void FABLE_FASTCALL operator delete(void* area) // retail 009F1F90
    {
        VirtualFree(area,0,0x8000);
    }
    // FableWin ?EnterSynchronisation@CMemoryAllocator@@KIXXZ /
    // ?LeaveSynchronisation@CMemoryAllocator@@KIXXZ; inline in retail.
    static void FABLE_FASTCALL EnterSynchronisation()
    {
        EnterCriticalSection(&g_CMemoryAllocatorCriticalSection_013CAA70);
    }

    static void FABLE_FASTCALL LeaveSynchronisation()
    {
        LeaveCriticalSection(&g_CMemoryAllocatorCriticalSection_013CAA70);
    }
#endif

    // ?AllocatorInfo@CMemoryAllocator@@1PAUCAllocatorInfo@1@A (FableWin:
    // protected static array; retail constructor initializes 30 size classes;
    // AllocatorExists[15] counts tracking categories, not this table).
    // Retail 0x01396868 (ConsolidateFreeAreas walks PAllocator from 0x01396880).
    static CAllocatorInfo AllocatorInfo[30];

private:
    friend void FABLE_FASTCALL FableInstallerDestroyBufferRange(InstallerBufferChunk*,InstallerBufferChunk*,void*);
    friend void FABLE_CDECL FableMemoryAllocatorShutdown_009f2d20();
    // Ego_r member names and offsets; retail constructor/destructor agree.
    unsigned long MemoryAllocatedPerTrackingID[256]; // +000
    unsigned long MemoryAllocated;                   // +400
    void** MemoryAuditBuffer;                        // +404
    void* Heap;                                      // +408
    unsigned long NoAllocations;                     // +40C
    unsigned long NoAllocators;                      // +410
    unsigned char CurrentTrackingID;                 // +414
    bool AllocatorExists[15];                        // +415
    long MaxFixedAllocationSizes[15];                // +424
    CAllocSizeToAllocator* AllocSizeToAllocator[15];  // +460
};
FABLE_STATIC_ASSERT(sizeof(CMemoryAllocator)==0x49c);
typedef CMemoryAllocator::CAllocSizeToAllocator CAllocSizeToAllocator;
FABLE_STATIC_ASSERT(sizeof(CAllocSizeToAllocator)==8);
extern unsigned long g_FableAllocatorMemoryBaseline_013CAAB0;
// Ego_r's CRT wrappers (?GFCalloc@@YIPAXII@Z etc.), retail 0x009F1FA0-0x009F1FD0.
void* FABLE_FASTCALL GFCalloc(unsigned int count, unsigned int size);
void* FABLE_FASTCALL GFMalloc(unsigned int size);
void FABLE_FASTCALL GFFree(void* memory);
void* FABLE_FASTCALL GFRealloc(void* memory, unsigned int size);
extern bool g_FableAllocatorShutdownRequested_013CAA94;

// Inline in retail callers (e.g. CBankFileAsyncData::SetupBuffer 009D52D0).
__forceinline void* FABLE_FASTCALL CMemoryAllocator::Allocate(unsigned long size,unsigned long category){if(!g_FableMemoryAllocator_013CAA90)Create();EnterSynchronisation();void* p=g_FableMemoryAllocator_013CAA90->InternalAllocate((long)size,category,false);LeaveSynchronisation();return p;}
