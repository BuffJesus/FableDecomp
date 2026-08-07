struct S; typedef int (__fastcall* VFn)(S*);
struct S { VFn* vtbl; };
bool __fastcall IsBoastSequence(S* self)
{
    return self->vtbl[0xc/4](self) == 3;
}