struct Inner; typedef void (__fastcall* VFn)(Inner*);
struct Inner { void** vtbl; };
struct Outer { char pad[8]; Inner* inner; };
void __fastcall ClearAllActions(Outer* self)
{
    Inner* p = self->inner;
    if (p)
    {
        VFn fn = *reinterpret_cast<VFn*>(*reinterpret_cast<char**>(p) + 0x54);
        fn(p);
    }
}