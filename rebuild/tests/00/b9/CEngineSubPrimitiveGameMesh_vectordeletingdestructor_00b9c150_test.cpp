#include <stdio.h>

struct CEngineSubPrimitiveGameMesh;

static int g_dtor = 0;
static int g_del = 0;
static CEngineSubPrimitiveGameMesh* g_lastDel = 0;

void __fastcall SubPrim_ScalarDtor(CEngineSubPrimitiveGameMesh* self) { g_dtor++; }
void __fastcall SubPrim_Delete(CEngineSubPrimitiveGameMesh* self) { g_del++; g_lastDel = self; }

void* __fastcall CEngineSubPrimitiveGameMesh_vector_deleting_destructor(
        CEngineSubPrimitiveGameMesh* self, int edx, unsigned int flags);

int main()
{
    CEngineSubPrimitiveGameMesh* obj = (CEngineSubPrimitiveGameMesh*)0x1234;

    // flags bit0 clear -> dtor only, no delete
    g_dtor = g_del = 0; g_lastDel = 0;
    void* r0 = CEngineSubPrimitiveGameMesh_vector_deleting_destructor(obj, 0, 0);
    bool ok0 = (r0 == obj) && (g_dtor == 1) && (g_del == 0);

    // flags bit0 set -> dtor + delete
    g_dtor = g_del = 0; g_lastDel = 0;
    void* r1 = CEngineSubPrimitiveGameMesh_vector_deleting_destructor(obj, 0, 1);
    bool ok1 = (r1 == obj) && (g_dtor == 1) && (g_del == 1) && (g_lastDel == obj);

    // even flag (bit0 clear, higher bits) -> no delete
    g_dtor = g_del = 0;
    void* r2 = CEngineSubPrimitiveGameMesh_vector_deleting_destructor(obj, 0, 2);
    bool ok2 = (r2 == obj) && (g_dtor == 1) && (g_del == 0);

    if (ok0 && ok1 && ok2)
        printf("VDD_PARITY_OK\n");
    else
        printf("VDD_PARITY_FAIL d=%d del=%d\n", g_dtor, g_del);
    return (ok0 && ok1 && ok2) ? 0 : 1;
}