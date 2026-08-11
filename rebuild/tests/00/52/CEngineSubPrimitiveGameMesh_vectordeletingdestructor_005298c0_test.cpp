#include <cstdio>

// Standalone behaviour model of the vector-deleting-destructor:
// call scalar dtor always; call deallocator only when (flags & 1);
// return this.

static int g_destroyed = 0;
static int g_freed = 0;

struct CEngineSubPrimitiveGameMesh
{
    int tag;
    void Destroy()  { g_destroyed++; }
    void FreeSelf() { g_freed++; }
    void *VecDelDtor(unsigned int flags)
    {
        Destroy();
        if (flags & 1)
            FreeSelf();
        return this;
    }
};

int main()
{
    CEngineSubPrimitiveGameMesh obj;
    obj.tag = 7;

    // flags bit0 clear: destroy only, no free
    g_destroyed = g_freed = 0;
    void *r0 = obj.VecDelDtor(0);
    if (r0 != &obj)       { printf("FAIL ret0\n"); return 1; }
    if (g_destroyed != 1) { printf("FAIL dtor0\n"); return 1; }
    if (g_freed != 0)     { printf("FAIL free0\n"); return 1; }

    // flags bit0 set: destroy then free
    g_destroyed = g_freed = 0;
    void *r1 = obj.VecDelDtor(1);
    if (r1 != &obj)       { printf("FAIL ret1\n"); return 1; }
    if (g_destroyed != 1) { printf("FAIL dtor1\n"); return 1; }
    if (g_freed != 1)     { printf("FAIL free1\n"); return 1; }

    // even flag (bit0 clear) -> no free
    g_destroyed = g_freed = 0;
    obj.VecDelDtor(2);
    if (g_freed != 0)     { printf("FAIL free2\n"); return 1; }

    printf("VECDELDTOR_OK\n");
    return 0;
}