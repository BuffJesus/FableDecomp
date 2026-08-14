// Byte-exact reconstruction of _Cons_val<allocator<C3DGeneratorObject>,...> @ 0x00ca3580
#include <string.h>

struct C3DGeneratorObject { char raw[96]; };

typedef void (__cdecl *ctor_fn)(C3DGeneratorObject*);
extern ctor_fn g_ctorptr;

// ecx = self (the object pointer to fill). Copies ecx->edx, null-checks,
// zeroes 96 bytes via rep stosd, then a cdecl-cleanup call on the object.
void __fastcall Cons_val_C3DGeneratorObject(C3DGeneratorObject* self)
{
    C3DGeneratorObject* p = self;
    if (p) {
        memset(p, 0, 96);
        g_ctorptr(p);
    }
}