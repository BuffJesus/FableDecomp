#include "engine/CGameScriptInterface.h"
#include <cstdio>
#include <cstring>

struct Provider { void* vt; };
struct CWorld { char pad[0x34]; Provider* prov; };

static char g_buf[0x200];
static void __fastcall vfn(Provider* p, void* vt, void** out) { *out = g_buf; }
static void* g_vt[16];

void __fastcall CGameScriptInterface_CameraSetCameraPreloadFlag(CGameScriptInterface* self, void* edx, char flag)
{
    void* local;
    Provider* prov = self->World->prov;
    void** vt = *(void***)prov;
    typedef void (__fastcall *fn_t)(Provider*, void*, void**);
    fn_t fn = (fn_t)vt[9];
    fn(prov, vt, &local);
    *((char*)local + 0x145) = flag;
}

int main() {
    memset(g_buf, 0, sizeof(g_buf));
    g_vt[9] = (void*)vfn;
    Provider prov; prov.vt = g_vt;
    CWorld owner; memset(&owner,0,sizeof(owner)); owner.prov = &prov;
    CGameScriptInterface self; memset(&self,0,sizeof(self)); self.World = &owner;
    CGameScriptInterface_CameraSetCameraPreloadFlag(&self, 0, (char)0x7);
    if (g_buf[0x145] == 0x7) { std::printf("CGameScriptInterface_0088ec90_TEST PASS\n"); return 0; }
    std::printf("FAIL got %d\n", (int)g_buf[0x145]); return 1;
}