#pragma optimize("s",on)
struct CLandscapeBackgroundPatch {
    int f0;
    int f4;
    void* f8;
    void* vector_deleting_destructor(unsigned int flags);
};

extern void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self);
extern void __cdecl eng_operator_delete(void* p);

void* CLandscapeBackgroundPatch::vector_deleting_destructor(unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(this);
    if (flags & 1)
        eng_operator_delete(this);
    return this;
}