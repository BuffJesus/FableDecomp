#include <stdio.h>

static int g_destroyed = 0;
static int g_freed = 0;

struct CEngineSubPrimitiveGameMesh
{
    int tag;
    void Destroy();
    void Free();
};

void CEngineSubPrimitiveGameMesh::Destroy() { tag = 0xDEAD; g_destroyed++; }
void CEngineSubPrimitiveGameMesh::Free()    { g_freed++; }

void* __fastcall CEngineSubPrimitiveGameMesh_vector_deleting_destructor(
    CEngineSubPrimitiveGameMesh* self, int edx, unsigned int flags);

int main()
{
    CEngineSubPrimitiveGameMesh a; a.tag = 1;
    void* r1 = CEngineSubPrimitiveGameMesh_vector_deleting_destructor(&a, 0, 0);

    CEngineSubPrimitiveGameMesh b; b.tag = 2;
    void* r2 = CEngineSubPrimitiveGameMesh_vector_deleting_destructor(&b, 0, 1);

    int ok = (r1 == &a) && (r2 == &b) && (g_destroyed == 2) && (g_freed == 1)
             && (a.tag == 0xDEAD) && (b.tag == 0xDEAD);
    if (ok) printf("VDD_PARITY_OK\n");
    else    printf("VDD_FAIL d=%d f=%d\n", g_destroyed, g_freed);
    return ok ? 0 : 1;
}