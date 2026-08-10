#include <stdio.h>

struct CInputProcessManager {
    void* vptr;
};

static int g_helper_called = 0;
static int g_tail_called = 0;
static CInputProcessManager* g_last_self = 0;

void __fastcall Helper_af8780(CInputProcessManager* self)
{
    g_helper_called++;
    g_last_self = self;
}

void __fastcall Tail_f36c90(CInputProcessManager* self)
{
    g_tail_called++;
    g_last_self = self;
}

void __fastcall DtorIPM_A(CInputProcessManager* self)
{
    self->vptr = (void*)0x0129F638;
    Helper_af8780(self);
    Tail_f36c90(self);
}

int main()
{
    CInputProcessManager obj;
    obj.vptr = 0;
    DtorIPM_A(&obj);

    int ok = 1;
    if (obj.vptr != (void*)0x0129F638) ok = 0;
    if (g_helper_called != 1) ok = 0;
    if (g_tail_called != 1) ok = 0;
    if (g_last_self != &obj) ok = 0;

    if (ok) {
        printf("OK_00af89a0\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}