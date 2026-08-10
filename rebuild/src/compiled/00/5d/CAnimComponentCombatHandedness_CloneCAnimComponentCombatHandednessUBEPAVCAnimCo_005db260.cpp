struct Self;
struct VTbl {
    void* s0;
    void* s1;
    void* s2;
    void* s3;
    Self* (__fastcall* clone)(Self* self); // slot at +0x10
};
struct Self {
    VTbl* vt;   // +0x00
    int   f4;   // +0x04
    int   f8;   // +0x08
    int   fc;   // +0x0c
};

Self* __fastcall Clone_CombatHandedness(Self* self)
{
    Self* n = self->vt->clone(self);
    n->f8 = self->f8;
    n->fc = self->fc;
    return n;
}