#include <stdio.h>

// Independent reimplementation mirroring the authored shape, with observable
// side effects so we can verify the control flow (dtor always; delete iff flag&1).

static int g_destroyed = 0;
static int g_freed = 0;

struct CEngineSubPrimitiveGameMesh
{
    int tag;
    void Destroy()    { g_destroyed++; }
    void FreeMemory() { g_freed++; }
    void* vector_deleting_destructor(unsigned int flags)
    {
        this->Destroy();
        if (flags & 1)
            this->FreeMemory();
        return this;
    }
};

int main()
{
    CEngineSubPrimitiveGameMesh a;
    a.tag = 0x1234;

    // flag bit0 clear: destroy only, no free, returns this
    g_destroyed = g_freed = 0;
    void* r0 = a.vector_deleting_destructor(0);
    if (g_destroyed != 1) { printf("FAIL destroy0\n"); return 1; }
    if (g_freed != 0)     { printf("FAIL free0\n");    return 1; }
    if (r0 != (void*)&a)  { printf("FAIL ret0\n");     return 1; }

    // flag bit0 set: destroy + free, returns this
    g_destroyed = g_freed = 0;
    void* r1 = a.vector_deleting_destructor(1);
    if (g_destroyed != 1) { printf("FAIL destroy1\n"); return 1; }
    if (g_freed != 1)     { printf("FAIL free1\n");    return 1; }
    if (r1 != (void*)&a)  { printf("FAIL ret1\n");     return 1; }

    // even flag (bit0 clear) => no free
    g_destroyed = g_freed = 0;
    a.vector_deleting_destructor(2);
    if (g_freed != 0)     { printf("FAIL free2\n");    return 1; }

    printf("VDD_PARITY_OK\n");
    return 0;
}