struct S; typedef unsigned char (__fastcall* VFn)(S*);
struct S { VFn* vtbl; };
bool __fastcall IsOverrideableByActionOfSamePriority(S* self){ return self->vtbl[0xb4/4](self) ? false : true; }