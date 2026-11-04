#include <stdio.h>
#include <string.h>
#include "fable_init_base_class.h"

// The compiled vtables, reached without spelling a mangled identifier.
#pragma comment(linker, "/alternatename:_g_testCInitBaseClassVTable=??_7CInitBaseClass@@6B@")
#pragma comment(linker, "/alternatename:_g_testCBaseVTable=??_7CBaseClass@@6B@")
extern "C" void* g_testCInitBaseClassVTable[];
extern "C" void* g_testCBaseVTable[];

long g_CInitBaseClassConstructed_013BCA18 = 0;
long g_CInitBaseClassValid_013BCA14 = 0;
void CInitBaseClass::Validate() const {}

int main()
{
    const unsigned char before[] = { 1, 0, 2 };
    const unsigned char after[] = { 0, 0, 2 };
    const long counter[] = { 2, 3, 3 };
    for (int i = 0; i < 3; ++i)
    {
        unsigned char storage[12];
        memset(storage, 0x3C, sizeof(storage));
        *reinterpret_cast<void**>(storage) = g_testCInitBaseClassVTable;
        storage[4] = before[i];
        g_CInitBaseClassValid_013BCA14 = 3;
        CInitBaseClass* object = reinterpret_cast<CInitBaseClass*>(storage);
        object->CInitBaseClass::~CInitBaseClass();
        // Unwinds to the CBase vtable after the inlined Uninitialise.
        if (*reinterpret_cast<void**>(storage) != g_testCBaseVTable) return 1 + i;
        if (storage[4] != after[i]) return 10 + i;
        if (g_CInitBaseClassValid_013BCA14 != counter[i]) return 20 + i;
        if (storage[5] != 0x3C || storage[8] != 0x3C) return 30 + i;
    }
    puts("PASS_INITBASE_DTOR");
    return 0;
}
