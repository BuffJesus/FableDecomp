// Independent behaviour test for the vector-deleting-destructor shape.
// Reproduces the semantics locally (destroy always; free only when flag bit0 set;
// return this) and verifies each branch, printing a stable pass token.
#include <stdio.h>

static int g_destroyed = 0;
static int g_freed = 0;

struct Mesh
{
    int marker;
    void Destroy()  { g_destroyed++; }
    void FreeThis() { g_freed++; }
    void* VectorDeletingDtor(unsigned int flags)
    {
        this->Destroy();
        if (flags & 1)
            this->FreeThis();
        return this;
    }
};

int main()
{
    Mesh m;
    m.marker = 0x1234;

    // flags = 0 : destroy, no free, returns this
    g_destroyed = g_freed = 0;
    void* r0 = m.VectorDeletingDtor(0);
    if (r0 != &m || g_destroyed != 1 || g_freed != 0) { printf("FAIL0\n"); return 1; }

    // flags = 1 : destroy + free, returns this
    g_destroyed = g_freed = 0;
    void* r1 = m.VectorDeletingDtor(1);
    if (r1 != &m || g_destroyed != 1 || g_freed != 1) { printf("FAIL1\n"); return 1; }

    // even flag with bit0 clear (2) : no free
    g_destroyed = g_freed = 0;
    void* r2 = m.VectorDeletingDtor(2);
    if (r2 != &m || g_destroyed != 1 || g_freed != 0) { printf("FAIL2\n"); return 1; }

    // odd flag (3) : bit0 set -> free
    g_destroyed = g_freed = 0;
    void* r3 = m.VectorDeletingDtor(3);
    if (r3 != &m || g_destroyed != 1 || g_freed != 1) { printf("FAIL3\n"); return 1; }

    printf("VDD_PARITY_OK\n");
    return 0;
}