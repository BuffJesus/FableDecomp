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
    CBaseClass* object = new (storage) CBaseClass;
    if (object != reinterpret_cast<CBaseClass*>(storage)) return 1;
    if (*reinterpret_cast<void**>(storage) != g_testCBaseClassVTable) return 2;
    if (storage[4] != 0xCC) return 3;
    puts("CBASE_0099a2d0_TEST PASS");
    return 0;
}
