#include <cstdio>

static int g_dtor = 0;
static int g_free = 0;

struct CEngineSubPrimitiveGameMesh
{
    int marker;
    void inner_dtor();
    void member_free();
};

void CEngineSubPrimitiveGameMesh::inner_dtor() { g_dtor++; }
void CEngineSubPrimitiveGameMesh::member_free() { g_free++; }

void* __fastcall CEngineSubPrimitiveGameMesh_vector_deleting_destructor(
    CEngineSubPrimitiveGameMesh* self, int edx, unsigned int flags);

int main()
{
    CEngineSubPrimitiveGameMesh a;
    a.marker = 0x1234;

    // flags bit0 clear: dtor only, no free
    g_dtor = g_free = 0;
    void* r0 = CEngineSubPrimitiveGameMesh_vector_deleting_destructor(&a, 0, 0);
    bool ok0 = (r0 == &a) && (g_dtor == 1) && (g_free == 0);

    // flags bit0 set: dtor + free
    g_dtor = g_free = 0;
    void* r1 = CEngineSubPrimitiveGameMesh_vector_deleting_destructor(&a, 0, 1);
    bool ok1 = (r1 == &a) && (g_dtor == 1) && (g_free == 1);

    // even flags (bit0 clear) => no free
    g_dtor = g_free = 0;
    void* r2 = CEngineSubPrimitiveGameMesh_vector_deleting_destructor(&a, 0, 2);
    bool ok2 = (r2 == &a) && (g_dtor == 1) && (g_free == 0);

    if (ok0 && ok1 && ok2) {
        printf("VDD_OK\n");
        return 0;
    }
    printf("VDD_FAIL d=%d f=%d\n", g_dtor, g_free);
    return 1;
}