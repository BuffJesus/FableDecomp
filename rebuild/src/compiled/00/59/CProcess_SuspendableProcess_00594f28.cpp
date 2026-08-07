struct S; typedef void (__fastcall* VFn)(S*);
struct S { void** vtbl; char pad[0x25]; unsigned char b29; };
void __fastcall SuspendableProcess(S* self)
{
    VFn fn = reinterpret_cast<VFn>(self->vtbl[0x10 / 4]);
    fn(self);
    self->b29 = 1;
}