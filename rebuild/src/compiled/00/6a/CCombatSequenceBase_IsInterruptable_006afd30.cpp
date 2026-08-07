struct S; typedef int (__fastcall* VFn)(S*);
struct S { VFn* vtbl; };
bool __fastcall IsInterruptable(S* self){ return self->vtbl[0x174/4](self) != 0; }