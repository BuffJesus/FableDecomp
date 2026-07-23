#pragma optimize("s",on)
extern "C" void __cdecl fable_operator_delete(void* p);

struct CLandscapeBackgroundPatch {
    int f0;
    void CLandscapeBackgroundPatch_dtor();
    void* vector_deleting_destructor(unsigned int flags);
};

void* CLandscapeBackgroundPatch::vector_deleting_destructor(unsigned int flags)
{
    this->CLandscapeBackgroundPatch_dtor();
    if (flags & 1)
        fable_operator_delete(this);
    return this;
}