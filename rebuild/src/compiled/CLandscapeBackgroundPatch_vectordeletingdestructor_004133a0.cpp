struct CLandscapeBackgroundPatch;
extern "C" void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self);
extern "C" void operator_delete_stub(void* p);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int /*edx_pad*/, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1) {
        operator_delete_stub((void*)self);
    }
    return (void*)self;
}