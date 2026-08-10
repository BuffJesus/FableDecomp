#include <cstdio>

struct CInputProcessManager { void** vtbl; };

static int g_member = 0;
static int g_base = 0;
static void* g_vt = 0;

extern "C" void __fastcall MemberDtor_686df0(void* self){ g_member++; }
extern "C" void __fastcall BaseDtor_686830(void* self){ g_base++; }

extern const void* const g_vtbl_CInputProcessManager[1] = {0};

extern "C" void __fastcall DtorInputProcessManager(CInputProcessManager* self, int edx);

int main(){
    CInputProcessManager o; o.vtbl = 0;
    DtorInputProcessManager(&o, 0);
    if (o.vtbl != (void**)&g_vtbl_CInputProcessManager) { printf("BAD vtbl\n"); return 1; }
    if (g_member != 1) { printf("BAD member\n"); return 1; }
    if (g_base != 1) { printf("BAD base\n"); return 1; }
    printf("OK_007e2760\n");
    return 0;
}