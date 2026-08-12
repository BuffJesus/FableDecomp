#include <cstdio>

struct CLandscapeBackgroundPatch;

static int g_dtor_calls = 0;
static int g_delete_calls = 0;
static void* g_last_deleted = 0;

void __fastcall CLandscapeBackgroundPatch_scalar_dtor(CLandscapeBackgroundPatch* self)
{
    (void)self;
    g_dtor_calls++;
}
void __cdecl op_delete(void* p)
{
    g_delete_calls++;
    g_last_deleted = p;
}

struct CLandscapeBackgroundPatch
{
    int dummy;
    void* __fastcall vector_deleting_destructor(unsigned int flags)
    {
        CLandscapeBackgroundPatch_scalar_dtor(this);
        if (flags & 1)
            op_delete(this);
        return this;
    }
};

int main()
{
    CLandscapeBackgroundPatch obj;
    obj.dummy = 0x1234;

    // flags bit0 clear: dtor runs, no delete, returns this
    g_dtor_calls = g_delete_calls = 0; g_last_deleted = 0;
    void* r1 = obj.vector_deleting_destructor(0);
    bool ok1 = (g_dtor_calls == 1) && (g_delete_calls == 0) && (r1 == (void*)&obj);

    // flags bit0 set: dtor runs, delete runs with this, returns this
    g_dtor_calls = g_delete_calls = 0; g_last_deleted = 0;
    void* r2 = obj.vector_deleting_destructor(1);
    bool ok2 = (g_dtor_calls == 1) && (g_delete_calls == 1) &&
               (g_last_deleted == (void*)&obj) && (r2 == (void*)&obj);

    // higher bits set but bit0 clear: no delete
    g_dtor_calls = g_delete_calls = 0;
    obj.vector_deleting_destructor(2);
    bool ok3 = (g_dtor_calls == 1) && (g_delete_calls == 0);

    if (ok1 && ok2 && ok3)
        printf("VDD_PASS_OK\n");
    else
        printf("VDD_FAIL\n");
    return (ok1 && ok2 && ok3) ? 0 : 1;
}