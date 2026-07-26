#include <cstdio>

struct Inner;
struct InnerVtbl {
    int (__fastcall *slot0)(Inner*);
    int (__fastcall *slot1)(Inner*);
    int (__fastcall *slot2)(Inner*);
};
struct Inner {
    InnerVtbl* vptr;
};
struct CGameCameraManager {
    char pad[0x98];
    Inner* inner;
};

int g_handled = 0;
void __fastcall Handle(Inner* p)
{
    (void)p;
    g_handled = 1;
}

void __fastcall CGameCameraManager_SetProjectileWeaponFired(CGameCameraManager* self)
{
    Inner* p = self->inner;
    int r = p->vptr->slot2(p);
    if (r == 0xf) {
        Handle(self->inner);
    }
}

int g_ret = 0xf;
int __fastcall my_slot2(Inner* p) { (void)p; return g_ret; }

int main()
{
    InnerVtbl vt;
    vt.slot0 = 0; vt.slot1 = 0; vt.slot2 = my_slot2;
    Inner inr;
    inr.vptr = &vt;
    CGameCameraManager mgr;
    mgr.inner = &inr;

    g_ret = 0xf; g_handled = 0;
    CGameCameraManager_SetProjectileWeaponFired(&mgr);
    if (g_handled != 1) { std::printf("FAIL: handle not called on 0xf\n"); return 1; }

    g_ret = 7; g_handled = 0;
    CGameCameraManager_SetProjectileWeaponFired(&mgr);
    if (g_handled != 0) { std::printf("FAIL: handle called on non-0xf\n"); return 1; }

    std::printf("CGameCameraManager_00698470_TEST PASS\n");
    return 0;
}