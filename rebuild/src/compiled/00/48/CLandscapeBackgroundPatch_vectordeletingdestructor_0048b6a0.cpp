// CLandscapeBackgroundPatch::`vector_deleting_destructor'  @ 0x0048b6a0
// Standard MSVC vector-deleting-destructor thunk shape:
//   push esi; mov esi,ecx; call <dtor>; test [esp+8],1; je L;
//   push esi; call <operator delete>; add esp,4; L: mov eax,esi; pop esi; ret 4

struct CLandscapeBackgroundPatch;

// base/scalar destructor (this in ecx) -> retail 0xa0d3d0
void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self);
// operator delete -> retail 0xbfe9bc
void operator_delete_stub(void* p);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(
        CLandscapeBackgroundPatch* self, int /*edx*/, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        operator_delete_stub(self);
    return self;
}