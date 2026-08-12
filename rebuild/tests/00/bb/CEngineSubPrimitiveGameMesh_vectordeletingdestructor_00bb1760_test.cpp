#include <cstdio>

static int g_dtor = 0;
static int g_del = 0;

extern "C" void __fastcall SubPrim_dtor(void* self) { (void)self; g_dtor++; }
extern "C" void __fastcall SubPrim_opdelete(void* self) { (void)self; g_del++; }

struct CEngineSubPrimitiveGameMesh
{
    void* vector_deleting_destructor(unsigned int flags)
    {
        SubPrim_dtor(this);
        if (flags & 1)
            SubPrim_opdelete(this);
        return this;
    }
};

int main()
{
    CEngineSubPrimitiveGameMesh obj;

    g_dtor = g_del = 0;
    void* r1 = obj.vector_deleting_destructor(0);
    // no delete when flag&1 == 0
    bool ok1 = (r1 == (void*)&obj) && g_dtor == 1 && g_del == 0;

    g_dtor = g_del = 0;
    void* r2 = obj.vector_deleting_destructor(1);
    // delete when flag&1 == 1
    bool ok2 = (r2 == (void*)&obj) && g_dtor == 1 && g_del == 1;

    g_dtor = g_del = 0;
    void* r3 = obj.vector_deleting_destructor(2);
    // bit 1 set but bit 0 clear -> no delete
    bool ok3 = (r3 == (void*)&obj) && g_dtor == 1 && g_del == 0;

    if (ok1 && ok2 && ok3)
        printf("VDD_OK\n");
    else
        printf("VDD_FAIL d=%d del=%d\n", g_dtor, g_del);
    return (ok1 && ok2 && ok3) ? 0 : 1;
}