// CGameScriptInterface::EntitySetStategroupEnabled @ 0088f6b0
// __stdcall free-function model (ret 0xc, 3 dword params).
// self->vt slot 0x2c is a __fastcall getter returning an Entity*.

struct Entity;

struct Self;
struct SelfVtbl {
    void* slot[0x2c / 4];                 // 0x00..0x28
    Entity* (__fastcall* getEntity)(Self*); // +0x2c  (__fastcall: this in ecx)
};

struct Entity {
    char pad[0x10];
    char enabled;                          // +0x10
    void SetStategroup(int a2, int a3);    // __fastcall member
};

struct Self {
    SelfVtbl* vt;                          // +0x0
};

void __stdcall CGameScriptInterface_EntitySetStategroupEnabled(Self* self, int a2, int a3)
{
    Entity* e = self->vt->getEntity(self);
    if (e && e->enabled == 1) {
        e->SetStategroup(a2, a3);
    }
}