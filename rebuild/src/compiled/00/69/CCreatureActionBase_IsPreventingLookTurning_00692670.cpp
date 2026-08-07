struct S; typedef unsigned char (__fastcall* VFn)(S*);
struct S { VFn* vtbl; };
bool __fastcall IsPreventingLookTurning(S* self)
{
    return self->vtbl[0xC4/4](self) ? true : false;
}