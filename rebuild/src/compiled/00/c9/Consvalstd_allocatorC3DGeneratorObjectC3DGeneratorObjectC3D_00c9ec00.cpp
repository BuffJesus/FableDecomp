// Byte-exact reconstruction of 0x00c9ec00
// _Cons_val<std::allocator<C3DGeneratorObject>,C3DGeneratorObject,C3DGeneratorObject>
// zeroes 0x322 dwords of the target then invokes a __cdecl helper via indirect ptr.

struct C3DGeneratorObject {
    unsigned long fields[0x322];
};

// __cdecl callee at [0x1440154], takes the object pointer, cleans its own arg (add esp,4)
extern void (__cdecl *g_ctorHelper)(C3DGeneratorObject *);

void __fastcall Cons_val(C3DGeneratorObject *self)
{
    if (self) {
        unsigned long *p = (unsigned long *)self;
        for (int i = 0; i < 0x322; ++i)
            p[i] = 0;
        g_ctorHelper(self);
    }
}