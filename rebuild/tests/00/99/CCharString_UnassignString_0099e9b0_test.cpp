// Behaviour fixture for CCharString::UnassignString (0x0099E9B0).
// Links the compiled source object; records operator delete / delete[].
#define private public
#include "fable_string.h"
#undef private

// The header declares UnassignString private (mangled AAE); the fixture
// reaches it through the public spelling and an alias to the real symbol.
#pragma comment(linker, "/alternatename:?UnassignString@CCharString@@QAEXXZ=?UnassignString@CCharString@@AAEXXZ")

#include <stdio.h>
#include <string.h>

static int g_seq;
static void* g_arr; static int g_arrOrder; static int g_arrs;
static void* g_obj; static int g_objOrder; static int g_objs;
static unsigned long g_lenAtDelete;
static unsigned long g_capAtDelete;
static char* g_textAtDelete;

void operator delete[](void* p)
{
    g_arr = p; g_arrOrder = ++g_seq; ++g_arrs;
}

void operator delete(void* p)
{
    CCharStringData* d = static_cast<CCharStringData*>(p);
    g_textAtDelete = d->text;
    g_lenAtDelete = d->unknown04;
    g_capAtDelete = d->unknown08;
    g_obj = p; g_objOrder = ++g_seq; ++g_objs;
}

static void Reset()
{
    g_seq = g_arrs = g_objs = 0;
    g_arr = g_obj = 0;
}

struct Holder
{
    unsigned long before;
    CCharStringData* storage;
    unsigned long after;
};

static CCharString* AsString(Holder& h)
{
    return reinterpret_cast<CCharString*>(&h.storage);
}

int main()
{
    int fail = 0;
    char text[8];
    CCharStringData data;

    // Last owner: delete[] text, clear text/length, keep only the top
    // capacity bit and the flag byte, then delete the block.
    Reset();
    data.text = text; data.unknown04 = 5; data.unknown08 = 0x80000010; data.flags0C = 0x5A; data.owners = 1;
    Holder h1 = { 0x11111111, &data, 0x22222222 };
    AsString(h1)->UnassignString();
    if (h1.storage != 0 || h1.before != 0x11111111 || h1.after != 0x22222222) fail |= 1;
    if (g_arrs != 1 || g_arr != text || g_arrOrder != 1) fail |= 2;
    if (g_objs != 1 || g_obj != &data || g_objOrder != 2) fail |= 4;
    if (g_textAtDelete != 0 || g_lenAtDelete != 0 || g_capAtDelete != 0x80000000) fail |= 8;
    if (data.flags0C != 0x5A || data.owners != 0) fail |= 16;

    // Capacity without the top bit clears to zero.
    Reset();
    data.text = text; data.unknown04 = 5; data.unknown08 = 0x7FFFFFFF; data.owners = 1;
    Holder h2 = { 0, &data, 0 };
    AsString(h2)->UnassignString();
    if (g_capAtDelete != 0) fail |= 32;

    // No buffer: no delete[], block still released, length reset.
    Reset();
    data.text = 0; data.unknown04 = 3; data.unknown08 = 4; data.owners = 1;
    Holder h3 = { 0, &data, 0 };
    AsString(h3)->UnassignString();
    if (g_arrs != 0 || g_objs != 1 || g_lenAtDelete != 0) fail |= 64;

    // Shared: only the count drops.
    Reset();
    data.text = text; data.unknown04 = 3; data.unknown08 = 4; data.owners = 2;
    Holder h4 = { 0, &data, 0 };
    AsString(h4)->UnassignString();
    if (h4.storage != 0 || data.owners != 1 || g_arrs != 0 || g_objs != 0) fail |= 128;
    if (data.text != text || data.unknown04 != 3 || data.unknown08 != 4) fail |= 256;

    // Empty handle.
    Reset();
    Holder h5 = { 0, 0, 0 };
    AsString(h5)->UnassignString();
    if (h5.storage != 0 || g_arrs != 0 || g_objs != 0) fail |= 512;

    if (fail == 0)
        printf("UNASSIGN_STRING_OK\n");
    else
        printf("UNASSIGN_STRING_FAIL %d\n", fail);
    return 0;
}
