#pragma optimize("s",on)
extern void __fastcall CLandscapeBackgroundPatch_dtor(void* self);

struct CLandscapeBackgroundPatch {
    int f0;
    int f4;
    void* f8;
    void* vector_deleting_destructor(unsigned int flags);
};

void* CLandscapeBackgroundPatch::vector_deleting_destructor(unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(this);
    if (flags & 1)
        ::operator delete(this);
    return this;
}