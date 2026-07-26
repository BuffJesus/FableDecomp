#include <cstdio>
#include <cstring>

struct Provider { void* vt; };
struct Owner { char pad[0x34]; Provider* prov; };
struct Self { char pad[4]; Owner* owner; };

static char g_buf[0x200];
static void __fastcall vfn(Provider* p, void* vt, void** out) { *out = g_buf; }
static void* g_vt[16];

void __fastcall CGameScriptInterface_CameraSetCameraPreloadFlag(Self* self, void* edx, char flag)
{
    void* local;
    Provider* prov = self->owner->prov;
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
    Owner owner; memset(&owner,0,sizeof(owner)); owner.prov = &prov;
    Self self; memset(&self,0,sizeof(self)); self.owner = &owner;
    CGameScriptInterface_CameraSetCameraPreloadFlag(&self, 0, (char)0x7);
    if (g_buf[0x145] == 0x7) { std::printf("CGameScriptInterface_0088ec90_TEST PASS\n"); return 0; }
    std::printf("FAIL got %d\n", (int)g_buf[0x145]); return 1;
}