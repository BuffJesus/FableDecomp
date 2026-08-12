// CLandscapeBackgroundPatch::`vector_deleting_destructor' @ 0x0048b520
// Standard MSVC scalar/vector deleting destructor:
//   push esi; mov esi,ecx; call <scalar dtor>; test [esp+8],1; je L; push esi; call operator delete; add esp,4; L: mov eax,esi; ret 4
#include <new>

struct CLandscapeBackgroundPatch { };

// The non-deleting (scalar) destructor lives at 0xa0d3d0; declared extern so the
// authored fn emits a real 'call' to it rather than inlining an empty body.
extern void __fastcall CLandscapeBackgroundPatch_scalar_destructor(CLandscapeBackgroundPatch* self);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(
    CLandscapeBackgroundPatch* self, int /*edx*/, unsigned int flags)
{
    CLandscapeBackgroundPatch_scalar_destructor(self);
    if (flags & 1)
        ::operator delete((void*)self);
    return self;
}