#include <stdio.h>

struct CInputProcessManager { void* vtbl; };

static int g_inner = 0;
static int g_base = 0;
static CInputProcessManager* g_last_base = 0;

extern "C" void __fastcall inner_dtor(CInputProcessManager* self) { g_inner++; }
extern "C" void __fastcall base_dtor(CInputProcessManager* self) { g_base++; g_last_base = self; }

void __fastcall DtorIPM_F(CInputProcessManager* self)
{
    self->vtbl = (void*)0x12a20fc;
    inner_dtor(self);
    base_dtor(self);
}

int main()
{
    CInputProcessManager obj;
    obj.vtbl = 0;
    DtorIPM_F(&obj);
    if (obj.vtbl != (void*)0x12a20fc) { printf("FAIL vtbl\n"); return 1; }
    if (g_inner != 1) { printf("FAIL inner\n"); return 1; }
    if (g_base != 1) { printf("FAIL base\n"); return 1; }
    if (g_last_base != &obj) { printf("FAIL this\n"); return 1; }
    printf("OK_00b49640\n");
    return 0;
}