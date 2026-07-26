#pragma optimize("s",on)
struct CLandscapeBackgroundPatch { int f0; };
extern "C" void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self);
extern void __cdecl op_delete(void* p);

struct CLBP_Vtbl {
    void* CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned int flags);
};

void* CLBP_Vtbl::CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned int flags)
{
    CLandscapeBackgroundPatch* self = (CLandscapeBackgroundPatch*)this;
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1) {
        op_delete(self);
    }
    return self;
}