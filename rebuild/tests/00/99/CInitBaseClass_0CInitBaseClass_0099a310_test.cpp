#include <new>
#include <stdio.h>
#include <string.h>
#include "fable_init_base_class.h"

long g_CInitBaseClassConstructed_013BCA18 = 0;
long g_CInitBaseClassValid_013BCA14 = 0;
CBaseClass::~CBaseClass() {}
CInitBaseClass::~CInitBaseClass() {}
void CInitBaseClass::Validate() const {}

int main()
{
    unsigned char storage[16];
    memset(storage, 0xCC, sizeof(storage));
    g_CInitBaseClassConstructed_013BCA18 = 41;
    g_CInitBaseClassValid_013BCA14 = 7;
    CInitBaseClass* object = new (storage) CInitBaseClass;
    void* vtable = *reinterpret_cast<void**>(storage);
    if (object != reinterpret_cast<CInitBaseClass*>(storage)) return 1;
    if (vtable == 0 || vtable == reinterpret_cast<void*>(0xCCCCCCCC)) return 2;
    if (storage[4] != 0) return 3;                     // Valid = false
    if (storage[5] != 0xCC || storage[8] != 0xCC) return 4;
    if (g_CInitBaseClassConstructed_013BCA18 != 42) return 5;
    if (g_CInitBaseClassValid_013BCA14 != 7) return 6;
    unsigned char second[8];
    new (second) CInitBaseClass;
    if (g_CInitBaseClassConstructed_013BCA18 != 43) return 7;
    if (*reinterpret_cast<void**>(second) != vtable) return 8;
    puts("FABLETLC_INITBASE_CTOR PASS");
    return 0;
}