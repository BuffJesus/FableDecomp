// Behaviour fixture for CLipSyncDataBank's entry table (provider CLipSyncDataBank_Entries_00a42c00.cpp: CloseData,
// the destructor, Close, NotifyEntryAdded, NotifyBankSize and the vector<CLipSyncDataBankEntry> members they
// instantiate; it defines none of them). The bank is raw storage at the retail offsets the header pins; entry data
// are counting pointees. The CBankFile members the bank forwards to, the other vtable slots and the out-of-line
// uninitialized_fill_n (its own unit) are test doubles.
#include <new>
#include "fable_lipsync_data_bank.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int g_fail, g_closes, g_added, g_asyncDtors, g_deleted, g_fills;
static unsigned long g_addedId, g_addedIndex;
static const void* g_addedData;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

void CIVCountedPointeeBase::DeleteCIVCountedPointeeBase() const {}
struct CPointee : CIVCountedPointeeBase
{
    virtual void DeleteCIVCountedPointeeBase() const { ++g_deleted; }
};

void CBankFile::Close() { ++g_closes; }
void CBankFile::NotifyEntryAdded(unsigned long id, unsigned long index, const CArray<char>& data)
{
    ++g_added; g_addedId = id; g_addedIndex = index; g_addedData = &data;
}
template <class T>
T* FABLE_FASTCALL _STL::uninitialized_fill_n(T* first, size_t count, const T& value)
{
    ++g_fills;
    for (; count > 0; --count, ++first)
        new (first) T(value);
    return first;
}
template CLipSyncDataBankEntry* FABLE_FASTCALL _STL::uninitialized_fill_n<CLipSyncDataBankEntry>(
    CLipSyncDataBankEntry*, size_t, const CLipSyncDataBankEntry&);

// The other vtable slots are never reached here (link-level doubles, as in CVertexShader_Lifetime's fixture);
// the async bank's destructor is a counting fastcall double (`this` in ecx, no stack arguments).
extern "C" void NeverCalledLipSyncSlot() { printf("FAIL unreached slot\n"); abort(); }
extern "C" void __fastcall LipSyncAsyncBankDestructor(void*) { ++g_asyncDtors; }
#pragma comment(linker, "/alternatename:??1CBankFileAsync@@UAE@XZ=@LipSyncAsyncBankDestructor@4")
#pragma comment(linker, "/alternatename:?OpenReadOnly@CBankFileAsync@@UAE_NABVCCharString@@K@Z=_NeverCalledLipSyncSlot")
#pragma comment(linker, "/alternatename:?OpenReadWrite@CBankFileAsync@@UAEXABVCCharString@@KK@Z=_NeverCalledLipSyncSlot")
#pragma comment(linker, "/alternatename:?OpenFileReadOnly@CBankFileAsync@@UAE_NABVCWideString@@K@Z=_NeverCalledLipSyncSlot")
#pragma comment(linker, "/alternatename:?OpenFileReadWrite@CBankFileAsync@@UAEXABVCWideString@@KK@Z=_NeverCalledLipSyncSlot")
#pragma comment(linker, "/alternatename:?AddEntryMultipleFiles@CBankFile@@UAEXABVCCharString@@KABV?$CArray@VCCharString@@@@V?$CCountedPointer@VCBankStateBlock@@@@@Z=_NeverCalledLipSyncSlot")
#pragma comment(linker, "/alternatename:?Update@CLipSyncDataBank@@UAEXXZ=_NeverCalledLipSyncSlot")
#pragma comment(linker, "/alternatename:?GetDataFormatVersion@CLipSyncDataBank@@UAEKXZ=_NeverCalledLipSyncSlot")
#pragma comment(linker, "/alternatename:?AdjustUpdateEntries@CBankFile@@MAEKXZ=_NeverCalledLipSyncSlot")
#pragma comment(linker, "/alternatename:?NotifyBankOpened@CBankFile@@MAEXXZ=_NeverCalledLipSyncSlot")
#pragma comment(linker, "/alternatename:?NotifyEntryUpdated@CBankFile@@MAEXKKABV?$CArray@D@@@Z=_NeverCalledLipSyncSlot")
#pragma comment(linker, "/alternatename:?NotifyEntryRemoved@CBankFile@@MAEXK@Z=_NeverCalledLipSyncSlot")
#pragma comment(linker, "/alternatename:?NotifyPostEntriesLoaded@CBankFile@@MAEXXZ=_NeverCalledLipSyncSlot")
#pragma comment(linker, "/alternatename:?PrepareDataForInclusion@CLipSyncDataBank@@MAEXKABV?$CArray@VCCharString@@@@KV?$CCountedPointer@VCBankStateBlock@@@@AAVCDataOutputStream@@2@Z=_NeverCalledLipSyncSlot")
#pragma comment(linker, "/alternatename:?IsDiskFile@CLipSyncDataBank@@MAE_NKK@Z=_NeverCalledLipSyncSlot")
#pragma comment(linker, "/alternatename:?GetLastModifiedTime@CBankFile@@MAE?AVCDateAndTime@@ABVCCharString@@@Z=_NeverCalledLipSyncSlot")
#pragma comment(linker, "/alternatename:?GetEnumerationType@CLipSyncDataBank@@MBE?AVCCharString@@K@Z=_NeverCalledLipSyncSlot")
#pragma comment(linker, "/alternatename:?Close@CBankFileAsync@@UAEXXZ=_NeverCalledLipSyncSlot")

// Constructor doubles: the base and resource-bank constructors are fastcall stand-ins (`this` in ecx, returned);
// SetNonCached and the counted pointer's Reset are plain member doubles.
static int g_asyncCtors, g_resourceBankCtors, g_nonCachedCalls, g_resets;
static bool g_nonCached = true;
static const void* g_resetThis;
static CResourceBank* g_resetValue;
extern "C" void* __fastcall LipSyncAsyncBankConstructor(void* self)
{
    ++g_asyncCtors;
    memset(self, 0xCD, sizeof(CLipSyncDataBank));
    return self;
}
extern "C" void* __fastcall LipSyncResourceBankConstructor(void* self) { ++g_resourceBankCtors; return self; }
#pragma comment(linker, "/alternatename:??0CBankFileAsync@@QAE@XZ=@LipSyncAsyncBankConstructor@4")
#pragma comment(linker, "/alternatename:??0CResourceBank@@QAE@XZ=@LipSyncResourceBankConstructor@4")
void CBankFileAsync::SetNonCached(bool nonCached) { ++g_nonCachedCalls; g_nonCached = nonCached; }
template <> void CCountedPointer<CResourceBank>::Reset(CResourceBank* data)
{
    ++g_resets; g_resetThis = this; g_resetValue = data;
}

struct CLipSyncDataBankTest : CLipSyncDataBank
{
    void Added(unsigned long id, unsigned long i, const CArray<char>& d) { CLipSyncDataBank::NotifyEntryAdded(id, i, d); }
    void Size(unsigned long n, const CArray<CBankFileDataTypeCount>& c) { CLipSyncDataBank::NotifyBankSize(n, c); }
};

// Entries (+0x164) as STLport's start/finish/end-of-storage; an entry is { CData* Data; float Duration; }.
struct RawEntry { CIVCountedPointeeBase* Data; float Duration; };
static unsigned char storage[sizeof(CLipSyncDataBank)];
static RawEntry*& Start() { return *reinterpret_cast<RawEntry**>(storage + 0x164); }
static RawEntry*& Finish() { return *reinterpret_cast<RawEntry**>(storage + 0x168); }
static RawEntry*& EndOfStorage() { return *reinterpret_cast<RawEntry**>(storage + 0x16C); }
static size_t Count() { return Finish() - Start(); }

int main()
{
    memset(storage, 0, sizeof(storage));
    CLipSyncDataBankTest* bank = reinterpret_cast<CLipSyncDataBankTest*>(storage);
    static CPointee a, b, c;
    // NotifyBankSize ignores the per-type counts; an empty array's storage stands in for one.
    static unsigned char countsStorage[sizeof(CArray<char>)];
    const CArray<CBankFileDataTypeCount>& counts = *reinterpret_cast<const CArray<CBankFileDataTypeCount>*>(countsStorage);
    // An entry payload: CArray<char> storage over 8 bytes that begin with the duration.
    static float bytes[2];
    static char* payloadStorage[3] = { (char*)bytes, (char*)(bytes + 2), (char*)(bytes + 2) };
    const CArray<char>& payload = *reinterpret_cast<const CArray<char>*>(payloadStorage);
    bytes[0] = 1.5f;

    // NotifyBankSize: room for every entry, all empty.
    bank->Size(3, counts);
    CHECK(Count() == 3 && Start()[0].Data == 0 && Start()[2].Duration == 0.0f);
    CHECK(EndOfStorage() - Start() >= 3);

    // NotifyEntryAdded past the end grows to index + 1 and records the duration; the base sees the call.
    bank->Added(7, 5, payload);
    CHECK(Count() == 6 && Start()[5].Duration == 1.5f && Start()[4].Duration == 0.0f);
    CHECK(g_added == 1 && g_addedId == 7 && g_addedIndex == 5 && g_addedData == &payload);
    bytes[0] = -2.25f;
    bank->Added(8, 1, payload);
    CHECK(Count() == 6 && Start()[1].Duration == -2.25f && g_added == 2);

    // Shrinking releases the cut entries' data.
    a.IVRefCount = 1; Start()[4].Data = &a;
    b.IVRefCount = 2; Start()[5].Data = &b;
    bank->Size(4, counts);
    CHECK(Count() == 4 && g_deleted == 1 && b.IVRefCount == 1 && a.IVRefCount == 0);

    // Growing inside the capacity appends empty entries through uninitialized_fill_n.
    const int fills = g_fills;
    bank->Size(6, counts);
    CHECK(Count() == 6 && g_fills == fills + 1 && Start()[5].Data == 0 && Start()[5].Duration == 0.0f);

    // Close releases every entry, then closes the bank file.
    c.IVRefCount = 1; Start()[0].Data = &c;
    b.IVRefCount = 1; Start()[2].Data = &b;
    bank->CLipSyncDataBank::Close();
    CHECK(Count() == 0 && g_deleted == 3 && g_closes == 1);

    // The destructor releases the entries and their storage, then runs the async bank's destructor.
    bank->Size(2, counts);
    a.IVRefCount = 1; Start()[1].Data = &a;
    bank->CLipSyncDataBank::~CLipSyncDataBank();
    CHECK(g_deleted == 4 && g_asyncDtors == 1 && g_closes == 1);

    // Construction: an empty entry table, no skipping, no phoneme bank, cached reads, and a fresh resource bank.
    static unsigned char built[sizeof(CLipSyncDataBank)];
    CLipSyncDataBank* fresh = new (built) CLipSyncDataBank;
    CHECK(fresh == reinterpret_cast<CLipSyncDataBank*>(built) && g_asyncCtors == 1);
    CHECK(*reinterpret_cast<void**>(built + 0x164) == 0 && *reinterpret_cast<void**>(built + 0x168) == 0);
    CHECK(*reinterpret_cast<void**>(built + 0x16C) == 0);
    CHECK(built[0x178] == 0 && *reinterpret_cast<void**>(built + 0x17C) == 0);
    CHECK(*reinterpret_cast<unsigned long*>(built) != 0xCDCDCDCD);
    CHECK(g_nonCachedCalls == 1 && !g_nonCached);
    CHECK(g_resourceBankCtors == 1 && g_resets == 1 && g_resetThis == built + 0x170 && g_resetValue != 0);
    free(g_resetValue);

    printf(g_fail ? "FAIL\n" : "CLIPSYNCDATABANK_ENTRIES_OK\n");
    return g_fail != 0;
}
