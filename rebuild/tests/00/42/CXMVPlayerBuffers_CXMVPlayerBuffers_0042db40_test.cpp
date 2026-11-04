#include <stdio.h>
#include <string.h>
#include <new>

#include "fable_game.h"
#include "test_support/base_class_stubs.h"

// Behaviour fixture for CXMVPlayerBuffers::CXMVPlayerBuffers @ 0x0042DB40:
// the CBaseClassNonCopyable base is constructed first on the same `this`,
// then InputBuffer / PlaybackBuffer / CurrentSize (+0x04/+0x08/+0x0C) are
// cleared and the CXMVPlayerBuffers vptr is installed; returns `this`.

static int g_baseCalls;
static void* g_baseThis;
CBaseClassNonCopyable::CBaseClassNonCopyable()
{
    ++g_baseCalls;
    g_baseThis = this;
}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CXMVPlayerBuffers::~CXMVPlayerBuffers() {}

static unsigned char g_storage[0x18];
static unsigned char g_other[0x10];

int main()
{
    memset(g_storage, 0xCD, sizeof(g_storage));
    CXMVPlayerBuffers* buffers = new (g_storage + 4) CXMVPlayerBuffers;
    CXMVPlayerBuffers* reference = new (g_other) CXMVPlayerBuffers;

    bool ok = (void*)buffers == (void*)(g_storage + 4);
    ok = ok && g_baseCalls == 2 && g_baseThis == g_other;
    void* vptr; memcpy(&vptr, g_storage + 4, 4);
    void* refVptr; memcpy(&refVptr, g_other, 4);
    ok = ok && vptr == refVptr && vptr != 0;
    for (int i = 8; i < 0x14; ++i)
        ok = ok && g_storage[i] == 0;
    for (int i = 0; i < 4; ++i)
        ok = ok && g_storage[i] == 0xCD && g_storage[0x14 + i] == 0xCD;
    if (ok) printf("FABLETLC_XMVPLAYERBUFFERS_CTOR_PASS\n");
    else printf("FAIL base=%d\n", g_baseCalls);
    return ok ? 0 : 1;
}