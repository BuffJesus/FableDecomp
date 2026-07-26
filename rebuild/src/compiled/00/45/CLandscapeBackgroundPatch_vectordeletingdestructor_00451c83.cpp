#pragma optimize("s",on)
extern "C" void __fastcall inner_dtor(void* p);
extern void __cdecl op_delete(void* p);

struct CInner { int a; };
struct CLandscapeBackgroundPatch {
    int f0;
    CInner inner; // at +0x4
};

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int /*edx*/, unsigned int flags)
{
    inner_dtor(&self->inner);
    if (flags & 1) {
        op_delete(self);
    }
    return self;
}