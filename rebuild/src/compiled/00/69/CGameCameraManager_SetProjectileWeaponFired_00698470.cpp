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

extern void __fastcall Handle(Inner* p);

void __fastcall CGameCameraManager_SetProjectileWeaponFired(CGameCameraManager* self)
{
    Inner* p = self->inner;
    int r = p->vptr->slot2(p);
    if (r == 0xf) {
        Handle(self->inner);
    }
}