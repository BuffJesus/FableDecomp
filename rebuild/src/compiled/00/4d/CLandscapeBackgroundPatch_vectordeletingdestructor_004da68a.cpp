#pragma optimize("s",on)
struct Sub {
};

extern "C" void __fastcall Sub_dtor(Sub* self);
extern "C" void __cdecl eng_delete(void* p);

struct CLandscapeBackgroundPatch {
    int pad0;
    Sub sub;
};

void* __fastcall vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_unused, unsigned int flags)
{
    (void)edx_unused;
    Sub_dtor(&self->sub);
    if (flags & 1) {
        eng_delete(self);
    }
    return self;
}