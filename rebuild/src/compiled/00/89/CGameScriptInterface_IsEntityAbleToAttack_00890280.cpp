struct Entity {
    void* vt;
    unsigned char pad[0x91 - 4];
    unsigned char flags; // +0x91
};

struct CGameScriptInterface;
typedef Entity* (__fastcall* GetEntityFn)(CGameScriptInterface*);
struct CGameScriptInterface {
    void** vt;
};

extern char __fastcall Entity_CanAttack(Entity* self);

bool __stdcall CGameScriptInterface_IsEntityAbleToAttack(CGameScriptInterface* self)
{
    Entity* e = ((GetEntityFn*)self->vt)[0x2c / 4](self);
    if (e != 0 && (e->flags & 0x1) == 0)
        return Entity_CanAttack(e);
    return false;
}