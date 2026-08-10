struct CAnimComponentCombatMisc;
struct Vtbl {
    void* s0;
    void* s1;
    void* s2;
    void* s3;
    CAnimComponentCombatMisc* (__fastcall* clone)(CAnimComponentCombatMisc* self); // +0x10
};
struct CAnimComponentCombatMisc {
    Vtbl* vptr;      // +0
    char pad4[4];    // +4
    char b8;         // +8
    char b9;         // +9
};

void* __fastcall Clone_CombatMisc(CAnimComponentCombatMisc* self)
{
    CAnimComponentCombatMisc* n = self->vptr->clone(self);
    n->b8 = self->b8;
    n->b9 = self->b9;
    return n;
}