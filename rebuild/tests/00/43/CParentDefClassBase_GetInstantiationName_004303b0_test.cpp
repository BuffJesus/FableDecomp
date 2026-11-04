// Behaviour of CParentDefClassBase::GetInstantiationName 0x004303B0: returns
// (through the hidden result pointer) a CDefString with the table index at +0x25.
#include "fable_def_class_base.h"
#include <stdio.h>
#include <string.h>

int main()
{
    static unsigned char store[sizeof(CParentDefClassBase)];
    memset(store, 0xCC, sizeof(store));
    long index = 0x1234567;
    memcpy(store + 0x25, &index, 4);
    const CParentDefClassBase* def = reinterpret_cast<const CParentDefClassBase*>(store);
    CDefString name = def->CParentDefClassBase::GetInstantiationName();
    if (name.TablePos != 0x1234567) { printf("got %lx\n", name.TablePos); return 1; }
    index = -1;
    memcpy(store + 0x25, &index, 4);
    if (def->CParentDefClassBase::GetInstantiationName().TablePos != -1) { puts("unnamed"); return 1; }
    printf("OK_0x004303b0\n");
    return 0;
}