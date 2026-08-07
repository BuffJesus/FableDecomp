struct S; typedef int (__fastcall* VFn)(S*);
struct S { VFn* vtbl; };
bool __fastcall IsOuterRingSequence(S* self){ return self->vtbl[0x3c/4](self) == 0x80; }