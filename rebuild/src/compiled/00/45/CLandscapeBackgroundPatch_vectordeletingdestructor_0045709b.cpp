#pragma optimize("s",on)
struct CLandscapeBackgroundPatch;
extern void __fastcall CLBP_dtor(CLandscapeBackgroundPatch* self);
extern void __cdecl CLBP_operator_delete(void* self);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_dummy, unsigned int flags)
{
    (void)edx_dummy;
    CLBP_dtor(self);
    if (flags & 1) {
        CLBP_operator_delete(self);
    }
    return self;
}