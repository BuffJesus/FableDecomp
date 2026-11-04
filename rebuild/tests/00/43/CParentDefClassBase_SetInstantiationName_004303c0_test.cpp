// Behaviour of CParentDefClassBase::SetInstantiationName 0x004303C0: stores the
// CDefString's table index at +0x25 and nothing else.
#include "fable_def_class_base.h"
#include <stdio.h>
#include <string.h>

int main()
{
    static unsigned char store[sizeof(CParentDefClassBase)];
    memset(store, 0xCC, sizeof(store));
    CParentDefClassBase* def = reinterpret_cast<CParentDefClassBase*>(store);
    CDefString name;
    name.TablePos = 0x2468;
    def->CParentDefClassBase::SetInstantiationName(name);
    long got;
    memcpy(&got, store + 0x25, 4);
    if (got != 0x2468) { printf("got %lx\n", got); return 1; }
    for (int i = 0; i < (int)sizeof(store); ++i)
        if ((i < 0x25 || i >= 0x29) && store[i] != 0xCC) { printf("byte %d touched\n", i); return 1; }
    printf("PASS_004303c0\n");
    return 0;
}