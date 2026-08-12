#include <cstdio>

static int g_dtor = 0;
static int g_free = 0;

struct CEngineSubPrimitiveGameMesh {
    int payload;
    void dtor_helper();
    void free_helper();
    void* vector_deleting_destructor(unsigned int flags);
};

void CEngineSubPrimitiveGameMesh::dtor_helper() { g_dtor++; }
void CEngineSubPrimitiveGameMesh::free_helper() { g_free++; }

void* CEngineSubPrimitiveGameMesh::vector_deleting_destructor(unsigned int flags)
{
    this->dtor_helper();
    if (flags & 1)
        this->free_helper();
    return this;
}

int main()
{
    CEngineSubPrimitiveGameMesh obj;
    obj.payload = 42;

    // flags=0: dtor runs, free does NOT
    g_dtor = 0; g_free = 0;
    void* r0 = obj.vector_deleting_destructor(0);
    bool ok0 = (r0 == &obj) && (g_dtor == 1) && (g_free == 0);

    // flags=1: dtor runs, free runs
    g_dtor = 0; g_free = 0;
    void* r1 = obj.vector_deleting_destructor(1);
    bool ok1 = (r1 == &obj) && (g_dtor == 1) && (g_free == 1);

    // flags=2: bit0 clear -> free must NOT run
    g_dtor = 0; g_free = 0;
    void* r2 = obj.vector_deleting_destructor(2);
    bool ok2 = (r2 == &obj) && (g_dtor == 1) && (g_free == 0);

    // flags=3: bit0 set -> free runs
    g_dtor = 0; g_free = 0;
    void* r3 = obj.vector_deleting_destructor(3);
    bool ok3 = (r3 == &obj) && (g_dtor == 1) && (g_free == 1);

    if (ok0 && ok1 && ok2 && ok3) {
        printf("VDD_OK\n");
        return 0;
    }
    printf("VDD_FAIL d=%d f=%d\n", g_dtor, g_free);
    return 1;
}