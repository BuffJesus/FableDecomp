#include <stdio.h>

static int g_dtor_calls = 0;
static int g_delete_calls = 0;
static void* g_dtor_self = 0;
static void* g_delete_self = 0;

void __fastcall dtor_scalar_529ad0(void* self)
{
    g_dtor_calls++;
    g_dtor_self = self;
}
void __fastcall op_delete_4c73b0(void* p)
{
    g_delete_calls++;
    g_delete_self = p;
}

struct CEngineSubPrimitiveGameMesh
{
    void* vDeletingDtor(unsigned int flags)
    {
        dtor_scalar_529ad0(this);
        if (flags & 1)
            op_delete_4c73b0(this);
        return this;
    }
};

int main()
{
    CEngineSubPrimitiveGameMesh obj;

    // flags bit0 clear: dtor runs, no delete
    g_dtor_calls = g_delete_calls = 0;
    void* r0 = obj.vDeletingDtor(0);
    if (r0 != &obj) { printf("FAIL ret0\n"); return 1; }
    if (g_dtor_calls != 1) { printf("FAIL dtor0\n"); return 1; }
    if (g_delete_calls != 0) { printf("FAIL del0\n"); return 1; }
    if (g_dtor_self != &obj) { printf("FAIL dtorself0\n"); return 1; }

    // flags bit0 set: dtor runs, then delete
    g_dtor_calls = g_delete_calls = 0;
    void* r1 = obj.vDeletingDtor(1);
    if (r1 != &obj) { printf("FAIL ret1\n"); return 1; }
    if (g_dtor_calls != 1) { printf("FAIL dtor1\n"); return 1; }
    if (g_delete_calls != 1) { printf("FAIL del1\n"); return 1; }
    if (g_delete_self != &obj) { printf("FAIL delself1\n"); return 1; }

    // even flags (bit0 clear) -> no delete
    g_dtor_calls = g_delete_calls = 0;
    obj.vDeletingDtor(2);
    if (g_delete_calls != 0) { printf("FAIL del2\n"); return 1; }

    // odd flags (bit0 set) -> delete
    g_dtor_calls = g_delete_calls = 0;
    obj.vDeletingDtor(3);
    if (g_delete_calls != 1) { printf("FAIL del3\n"); return 1; }

    printf("VDD_OK\n");
    return 0;
}