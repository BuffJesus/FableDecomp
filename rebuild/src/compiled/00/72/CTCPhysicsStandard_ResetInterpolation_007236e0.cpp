// CTCPhysicsStandard::ResetInterpolation @ 007236e0
struct Xform6 { unsigned int m[6]; };
struct CTCPhysicsStandard {
    char pad0[0x50];
    Xform6 cur;         // +0x50
    Xform6 interp;      // +0x68
    unsigned int flag;  // +0x80
};

extern void __fastcall CTCPhysicsStandard_helper(CTCPhysicsStandard* self);
extern unsigned int __cdecl CTCPhysicsStandard_getFlag(void);
extern void __fastcall CTCPhysicsStandard_tail(CTCPhysicsStandard* self);

void __fastcall CTCPhysicsStandard_ResetInterpolation(CTCPhysicsStandard* self)
{
    self->interp = self->cur;
    CTCPhysicsStandard_helper(self);
    self->flag = CTCPhysicsStandard_getFlag();
    CTCPhysicsStandard_tail(self);
}