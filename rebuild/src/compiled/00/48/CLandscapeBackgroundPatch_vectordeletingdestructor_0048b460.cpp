// CLandscapeBackgroundPatch::`vector_deleting_destructor' 0x0048b460
// Standard MSVC scalar/vector deleting destructor idiom:
//   push esi; mov esi,ecx; call dtor; test [esp+8],1; je L; push esi; call delete; add esp,4; L: mov eax,esi; ret 4

struct CLandscapeBackgroundPatch
{
    void Destroy();  // real scalar destructor at 0xa0d3d0 (__fastcall)
};

// operator delete equivalent at 0xbfe9bc (cdecl)
extern "C" void FreeMem(void* p);

// scalar destructor body (external target)
void __fastcall CLandscapeBackgroundPatch_Destroy(CLandscapeBackgroundPatch* self);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(
    CLandscapeBackgroundPatch* self, int /*edx*/, unsigned int flags)
{
    CLandscapeBackgroundPatch_Destroy(self);
    if (flags & 1)
        FreeMem(self);
    return self;
}