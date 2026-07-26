struct SGObj {
    void* vt;      // +0x0
    char pad4[0xc];
    char kind;     // +0x10
    void Apply(int enabled);
};
struct Entity {
    void* vt;
};

typedef SGObj* (__fastcall *GetSGFn)(Entity*);

void __stdcall CGameScriptInterface_EntitySetAllStategroupsEnabled(Entity* entity, int enabled)
{
    SGObj* o = (*(GetSGFn*)((char*)(*(void**)entity) + 0x2c))(entity);
    if (o == 0) return;
    if (o->kind != 1) return;
    o->Apply(enabled);
}