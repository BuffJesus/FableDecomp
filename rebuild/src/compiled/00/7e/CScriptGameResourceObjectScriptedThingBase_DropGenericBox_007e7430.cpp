struct Inner; typedef void (__fastcall* VFn)(Inner*);
struct Inner { void** vtbl; };
struct Outer { char pad[8]; Inner* inner; };
void __fastcall DropGenericBox(Outer* self)
{
    Inner* p = self->inner;
    if (p)
    {
        VFn fn = *reinterpret_cast<VFn*>(*reinterpret_cast<char**>(p) + 0x60);
        fn(p);
    }
}