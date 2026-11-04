// Behaviour fixture for CWideString::UnassignString (0x0099B4D0).
// Links the compiled source object; records the CRT free and operator delete.
#define private public
#include "fable_string.h"
#undef private

// The header declares UnassignString private (mangled AAE); the fixture
// reaches it through the public spelling and an alias to the real symbol.
#pragma comment(linker, "/alternatename:?UnassignString@CWideString@@QAEXXZ=?UnassignString@CWideString@@AAEXXZ")

#include <stdio.h>
#include <string.h>

static int g_seq;
static void* g_freed[4];
static int g_freeOrder[4];
static int g_frees;
static void* g_deleted[4];
static int g_deleteOrder[4];
static int g_deletes;

void operator delete(void* p)
{
    g_deleted[g_deletes] = p;
    g_deleteOrder[g_deletes++] = ++g_seq;
}

extern "C" void __cdecl free(void* p)
{
    g_freed[g_frees] = p;
    g_freeOrder[g_frees++] = ++g_seq;
}

static void Reset()
{
    g_seq = g_frees = g_deletes = 0;
    memset(g_freed, 0, sizeof(g_freed));
    memset(g_deleted, 0, sizeof(g_deleted));
}

struct Holder
{
    unsigned long before;
    CWideStringData* storage;
    unsigned long after;
};

static CWideString* AsString(Holder& h)
{
    return reinterpret_cast<CWideString*>(&h.storage);
}

int main()
{
    int fail = 0;
    wchar_t text[4];
    unsigned long dataRaw_[4];
    CWideStringData& data = *reinterpret_cast<CWideStringData*>(dataRaw_);

    // Last owner with a buffer: free(text) then delete(data), handle cleared.
    Reset();
    data.text = text; data.unknown04 = 7; data.unknown08 = 9; data.owners = 1;
    Holder h1 = { 0x11111111, &data, 0x22222222 };
    AsString(h1)->UnassignString();
    if (h1.storage != 0 || h1.before != 0x11111111 || h1.after != 0x22222222) fail |= 1;
    if (g_frees != 1 || g_freed[0] != text || g_freeOrder[0] != 1) fail |= 2;
    if (g_deletes != 1 || g_deleted[0] != &data || g_deleteOrder[0] != 2) fail |= 4;
    if (data.owners != 0) fail |= 8;

    // Last owner without a buffer: only the data block is deleted.
    Reset();
    data.text = 0; data.owners = 1;
    Holder h2 = { 0, &data, 0 };
    AsString(h2)->UnassignString();
    if (h2.storage != 0 || g_frees != 0 || g_deletes != 1 || g_deleted[0] != &data) fail |= 16;

    // Shared data: only the count drops, nothing is released.
    Reset();
    data.text = text; data.owners = 3;
    Holder h3 = { 0, &data, 0 };
    AsString(h3)->UnassignString();
    if (h3.storage != 0 || data.owners != 2 || g_frees != 0 || g_deletes != 0) fail |= 32;
    if (data.text != text) fail |= 64;

    // Already-null owners (<= 0 after decrement from 0) also release.
    Reset();
    data.text = text; data.owners = 0;
    Holder h4 = { 0, &data, 0 };
    AsString(h4)->UnassignString();
    if (data.owners != -1 || g_frees != 1 || g_deletes != 1) fail |= 128;

    // Empty handle: no-op.
    Reset();
    Holder h5 = { 0, 0, 0 };
    AsString(h5)->UnassignString();
    if (h5.storage != 0 || g_frees != 0 || g_deletes != 0) fail |= 256;

    if (fail == 0)
        printf("UNASSIGN_STRING_OK\n");
    else
        printf("UNASSIGN_STRING_FAIL %d\n", fail);
    return 0;
}
