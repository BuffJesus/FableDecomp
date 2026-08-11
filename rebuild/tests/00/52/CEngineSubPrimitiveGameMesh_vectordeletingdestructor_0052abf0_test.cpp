#include <stdio.h>

struct CEngineSubPrimitiveGameMesh
{
    int tag;
    void destroy();
    void* vec_deleting_destructor(unsigned int flags);
};

static int g_destroyed = 0;
static int g_freed = 0;

void CEngineSubPrimitiveGameMesh::destroy()
{
    g_destroyed = this->tag;
}

void __fastcall mesh_free(void* self)
{
    g_freed = ((CEngineSubPrimitiveGameMesh*)self)->tag;
}

void* CEngineSubPrimitiveGameMesh::vec_deleting_destructor(unsigned int flags)
{
    this->destroy();
    if (flags & 1)
        mesh_free(this);
    return this;
}

int main()
{
    CEngineSubPrimitiveGameMesh m;
    m.tag = 77;

    g_destroyed = 0; g_freed = 0;
    void* r0 = m.vec_deleting_destructor(0);
    // flag 0: destroyed but NOT freed
    if (r0 != &m || g_destroyed != 77 || g_freed != 0) { printf("FAIL a\n"); return 1; }

    g_destroyed = 0; g_freed = 0;
    void* r1 = m.vec_deleting_destructor(1);
    // flag 1: destroyed AND freed
    if (r1 != &m || g_destroyed != 77 || g_freed != 77) { printf("FAIL b\n"); return 1; }

    g_destroyed = 0; g_freed = 0;
    void* r2 = m.vec_deleting_destructor(2);
    // flag 2: bit0 clear -> not freed
    if (r2 != &m || g_destroyed != 77 || g_freed != 0) { printf("FAIL c\n"); return 1; }

    printf("VDD_PARITY_OK\n");
    return 0;
}