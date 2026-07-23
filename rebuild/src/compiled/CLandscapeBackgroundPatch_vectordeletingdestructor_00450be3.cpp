#pragma optimize("s",on)
// CLandscapeBackgroundPatch::`vector deleting destructor'
// this in ecx (__fastcall), flags on stack. Calls scalar dtor then optional delete.
extern void __fastcall CLandscapeBackgroundPatch_dtor(void* self);
extern void __cdecl operator_delete_impl(void* p);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(void* self, int /*edx*/, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        operator_delete_impl(self);
    return self;
}