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
    memset(storage, 0x5A, sizeof(storage));
    *reinterpret_cast<void**>(storage) = storage;
    reinterpret_cast<CBaseClass*>(storage)->CBaseClass::~CBaseClass();
    if (*reinterpret_cast<void**>(storage) != g_testCBaseClassVTable) return 1;
    if (storage[4] != 0x5A) return 2;
    puts("FABLETLC_CBASE_RESTORE_A_BEHAVIOR PASS");
    return 0;
}
