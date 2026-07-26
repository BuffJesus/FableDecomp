#pragma optimize("s",on)
extern "C" void __cdecl eng_free(void*);
void __fastcall CLandscapeBackgroundPatch_dtor(void* self);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(void* self, int edx, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        eng_free(self);
    return self;
}