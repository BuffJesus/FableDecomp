// Vector/scalar deleting destructor for CLandscapeBackgroundPatch @ 0048b5a0
#include <new>

// External destructor body (retail 0xa0d3d0) - kept out-of-line via extern.
extern void __fastcall CLandscapeBackgroundPatch_dtor(void* self);

struct CLandscapeBackgroundPatch {
    // The deleting destructor: __fastcall(this), uint flag arg, returns this.
    void* deleting_destructor(unsigned int flags);
};

void* CLandscapeBackgroundPatch::deleting_destructor(unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(this);       // call 0xa0d3d0
    if (flags & 1)                              // test byte [esp+8],1
        ::operator delete(this);                // call 0xbfe9bc
    return this;                                // mov eax,esi
}