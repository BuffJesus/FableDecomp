struct Entity;
struct ArgObj;
struct DefObj;

struct ArgVtbl {
    void* pad[0x2c / 4];
    Entity* (__fastcall* GetTargetEntity)(void* self);
};
struct ArgObj {
    ArgVtbl* vtbl;
};

struct Entity {
    unsigned char pad00[0x6c];
    unsigned char m_Flags6c;
    unsigned char pad6d[0x91 - 0x6d];
    unsigned char m_Flags91;
    int  Helper_ffdd1230();
    DefObj* Helper_ffc36a50(void* pLocalA, int a);
};

struct DefObj {
    void** Helper_11bd80();
};

struct GObj {
    void Helper_143a70(void* pLocalB, void* value);
};

struct Local10 {
    void* m_data;
    void Dtor_10dba0();
};

// 'this' object: virtual dispatch through slot at byte 0x734.
struct SelfVtbl;
struct SelfObj {
    SelfVtbl* vtbl;
};
struct SelfVtbl {
    unsigned char pad[0x734];
    // member-function pointer occupying the slot at +0x734
    void (SelfObj::* ResetCombatType)(ArgObj* arg, void* pLocal);
};

extern GObj g_obj13ca828;

void __fastcall CGameScriptInterface_EntityResetCombatTypeToDefault(
    void* self_ecx, unsigned long /*edx*/, ArgObj* arg)
{
    SelfObj* self = (SelfObj*)self_ecx;

    Entity* entity = arg->vtbl->GetTargetEntity(arg);
    if (entity == 0)
        return;
    if (entity->m_Flags91 & 0x01)
        return;
    if ((entity->m_Flags6c & 0x08) == 0)
        return;

    void* localA;
    Local10 localB;

    int a = entity->Helper_ffdd1230();
    DefObj* defObj = entity->Helper_ffc36a50(&localA, a);
    void** defPtr = defObj->Helper_11bd80();
    void* value = *defPtr;
    g_obj13ca828.Helper_143a70(&localB, value);

    (self->*(self->vtbl->ResetCombatType))(arg, &localB);

    localB.Dtor_10dba0();
}