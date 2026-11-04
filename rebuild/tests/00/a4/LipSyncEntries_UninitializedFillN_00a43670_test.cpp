// Behaviour fixture for uninitialized_fill_n<CLipSyncDataBankEntry> (provider LipSyncEntries_UninitializedFillN_00a43670.cpp;
// it is declared, not defined, here): n copies of the value are constructed in raw storage, each sharing the
// value's data (one reference each), and the end of the filled range is returned.
#include <new>
#include "fable_lipsync_data_bank.h"
#include <stdio.h>
#include <string.h>

static int g_fail, g_deleted;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

void CIVCountedPointeeBase::DeleteCIVCountedPointeeBase() const {}
struct CPointee : CIVCountedPointeeBase
{
    virtual void DeleteCIVCountedPointeeBase() const { ++g_deleted; }
};
struct RawEntry { CIVCountedPointeeBase* Data; float Duration; };

int main()
{
    static CPointee data;
    data.IVRefCount = 1;
    RawEntry value = { &data, 2.5f };
    static RawEntry out[5];
    memset(out, 0xCD, sizeof(out));
    CLipSyncDataBankEntry* first = reinterpret_cast<CLipSyncDataBankEntry*>(out);
    const CLipSyncDataBankEntry& v = *reinterpret_cast<const CLipSyncDataBankEntry*>(&value);

    CLipSyncDataBankEntry* end = _STL::uninitialized_fill_n(first, 3, v);
    CHECK(end == first + 3);
    CHECK(out[0].Data == &data && out[2].Data == &data && out[1].Duration == 2.5f);
    CHECK(data.IVRefCount == 4 && g_deleted == 0);
    CHECK(*reinterpret_cast<unsigned long*>(&out[3]) == 0xCDCDCDCD);

    // Nothing to fill: nothing constructed.
    CHECK(_STL::uninitialized_fill_n(first + 3, 0, v) == first + 3 && data.IVRefCount == 4);

    // A value without data copies only the duration.
    RawEntry empty = { 0, -1.0f };
    end = _STL::uninitialized_fill_n(first + 3, 2, *reinterpret_cast<const CLipSyncDataBankEntry*>(&empty));
    CHECK(end == first + 5 && out[4].Data == 0 && out[3].Duration == -1.0f);

    printf(g_fail ? "FAIL\n" : "LIPSYNC_ENTRIES_FILL_OK\n");
    return g_fail != 0;
}
