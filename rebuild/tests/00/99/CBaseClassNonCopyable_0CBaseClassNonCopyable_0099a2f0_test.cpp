#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_base_class.h"

// The compiled vtables, reached without spelling a mangled identifier.
#pragma comment(linker, "/alternatename:_g_testCBaseClassVTable=??_7CBaseClass@@6B@")
#pragma comment(linker, "/alternatename:_g_testNonCopyableVTable=??_7CBaseClassNonCopyable@@6B@")
extern "C" void* g_testCBaseClassVTable[];
extern "C" void* g_testNonCopyableVTable[];

int main()
{
    unsigned char storage[8];
    memset(storage, 0xCC, sizeof(storage));
    CBaseClassNonCopyable* object = new (storage) CBaseClassNonCopyable;
    if (object != reinterpret_cast<CBaseClassNonCopyable*>(storage)) return 1;
    // Only the most-derived table survives; the inlined base store is dead.
    if (*reinterpret_cast<void**>(storage) != g_testNonCopyableVTable) return 2;
    if (storage[4] != 0xCC) return 3;
    puts("CBASE_CTOR_TEST_OK");
    return 0;
}
