struct S; typedef int (__fastcall* VFn)(S*);
struct S { VFn* vtbl; };
bool __fastcall IsInterruptableDueToZoneChange(S* self)
{
    return self->vtbl[0x10/4](self) == 2;
}