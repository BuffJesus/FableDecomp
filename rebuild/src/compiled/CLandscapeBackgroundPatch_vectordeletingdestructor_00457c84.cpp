#pragma optimize("s",on)
struct CLandscapeBackgroundPatch { void* vftable; };

extern void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self);
void __cdecl operator delete(void* p);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_pad, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        operator delete((void*)self);
    return self;
}