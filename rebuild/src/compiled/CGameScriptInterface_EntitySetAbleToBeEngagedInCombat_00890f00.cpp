struct Entity {
    unsigned char pad0[0x6c];
    unsigned char f6c;      // +0x6c
    unsigned char pad6d[0x91 - 0x6d];
    unsigned char f91;      // +0x91
    unsigned char pad92[0xbc - 0x92];
    unsigned char fbc;      // +0xbc
};

struct GSIVtbl {
    void* s0; void* s1; void* s2; void* s3;  // 0x00..0x0c
    void* s4; void* s5; void* s6; void* s7;  // 0x10..0x1c
    void* s8; void* s9; void* sa;            // 0x20..0x28
    Entity* (__fastcall *getEntity)(void*);  // +0x2c
};

struct CGameScriptInterface { GSIVtbl* vt; };

void __stdcall CGameScriptInterface_EntitySetAbleToBeEngagedInCombat(CGameScriptInterface* self, char enable)
{
    Entity* e = self->vt->getEntity(self);
    if (!e) return;
    if (e->f91 & 1) return;
    if (!(e->f6c & 2)) return;
    unsigned char c = (unsigned char)(enable << 5);
    e->fbc ^= (unsigned char)((c ^ e->fbc) & 0x20);
}