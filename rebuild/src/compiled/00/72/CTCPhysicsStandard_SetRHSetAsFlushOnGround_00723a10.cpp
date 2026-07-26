// CTCPhysicsStandard::SetRHSetAsFlushOnGround @ 00723a10
struct Vec { int x, y, z; };
struct Helper;

struct CTCPhysicsStandard {
    void** vtbl;      // +0x00
    int pad4;         // +0x04
    int pad8;         // +0x08
    Vec ground;       // +0x0c
    Helper* compute(Vec* out, Vec* ground);   // __fastcall member
};

extern void __fastcall Helper_use(Helper* h);

void __fastcall CTCPhysicsStandard_SetRHSetAsFlushOnGround(CTCPhysicsStandard* self)
{
    Vec local;
    Helper* h = self->compute(&local, &self->ground);
    Helper_use(h);
    typedef void (CTCPhysicsStandard::*Fn)(Vec*);
    Fn fn = *(Fn*)((char*)self->vtbl + 0x134);
    (self->*fn)(&local);
}