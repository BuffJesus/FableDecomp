// CLandscapeBackgroundPatch::`vector deleting destructor' @ 0x0048b500
// Standard MSVC vector-deleting-destructor thunk:
//   push esi; mov esi,ecx; call <dtor>; test [esp+8],1; jz L;
//   push esi; call operator delete; add esp,4; L: mov eax,esi; pop esi; ret 4

struct CLandscapeBackgroundPatch
{
    void dtor();  // real scalar destructor @ 0xa0d3d0 (declaration only -> real call)
};

void operator_delete_stub(void* p); // operator delete @ 0xbfe9bc (declaration only)

void* __fastcall vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx, unsigned int flags)
{
    self->dtor();
    if (flags & 1)
        operator_delete_stub(self);
    return self;
}