#include <cstdio>

// Standalone: provide the vtable symbol locally so the test links.
void* const CSystemRegistry_vftable[1] = { 0 };

struct CSystemRegistry
{
    void* vtbl;
    int   pad4;
    void* buffer;
    void dtor();
};

void CSystemRegistry::dtor()
{
    if (buffer)
        operator delete(buffer);
    vtbl = (void*)CSystemRegistry_vftable;
}

int main()
{
    // Case 1: buffer non-null -> freed, vtbl reset
    {
        CSystemRegistry r;
        r.vtbl = 0;
        r.buffer = operator new(64);
        r.dtor();
        if (r.vtbl != (void*)CSystemRegistry_vftable) { printf("FAIL1\n"); return 1; }
    }
    // Case 2: buffer null -> no free attempted, vtbl reset
    {
        CSystemRegistry r;
        r.vtbl = 0;
        r.buffer = 0;
        r.dtor();
        if (r.vtbl != (void*)CSystemRegistry_vftable) { printf("FAIL2\n"); return 1; }
    }
    printf("DTOR_OK\n");
    return 0;
}