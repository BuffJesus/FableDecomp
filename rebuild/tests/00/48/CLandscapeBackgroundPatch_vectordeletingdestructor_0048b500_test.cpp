#include <cstdio>

struct CLandscapeBackgroundPatch
{
    void dtor();
    int marker;
};

static int g_dtor_calls = 0;
static int g_delete_calls = 0;

__declspec(noinline) void CLandscapeBackgroundPatch::dtor()
{
    g_dtor_calls++;
}

void operator_delete_stub(void* p)
{
    (void)p;
    g_delete_calls++;
}

void* __fastcall vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx, unsigned int flags)
{
    self->dtor();
    if (flags & 1)
        operator_delete_stub(self);
    return self;
}

int main()
{
    CLandscapeBackgroundPatch obj;
    obj.marker = 0x1234;

    // flags & 1 == 0 : dtor only, no delete
    g_dtor_calls = 0; g_delete_calls = 0;
    void* r0 = vector_deleting_destructor(&obj, 0, 0);
    bool ok0 = (r0 == &obj) && (g_dtor_calls == 1) && (g_delete_calls == 0);

    // flags & 1 == 1 : dtor + delete
    g_dtor_calls = 0; g_delete_calls = 0;
    void* r1 = vector_deleting_destructor(&obj, 0, 1);
    bool ok1 = (r1 == &obj) && (g_dtor_calls == 1) && (g_delete_calls == 1);

    // flag bit high but low bit clear -> no delete
    g_dtor_calls = 0; g_delete_calls = 0;
    void* r2 = vector_deleting_destructor(&obj, 0, 2);
    bool ok2 = (r2 == &obj) && (g_dtor_calls == 1) && (g_delete_calls == 0);

    if (ok0 && ok1 && ok2)
        printf("VDD_PASS_OK\n");
    else
        printf("VDD_FAIL o0=%d o1=%d o2=%d\n", ok0, ok1, ok2);
    return (ok0 && ok1 && ok2) ? 0 : 1;
}