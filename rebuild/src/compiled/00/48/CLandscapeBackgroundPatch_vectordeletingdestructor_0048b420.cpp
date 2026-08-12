// CLandscapeBackgroundPatch::`vector_deleting_destructor'
// __fastcall (this in ecx), uint flags on stack, returns this.

struct CLandscapeBackgroundPatch {
    // scalar destructor lives at 0xa0d3d0 in retail; model as an extern.
};

extern "C" void __fastcall LandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self);
extern "C" void operator_delete_impl(void* p);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(
        CLandscapeBackgroundPatch* self, int /*edx*/, unsigned int flags)
{
    LandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        operator_delete_impl(self);
    return self;
}