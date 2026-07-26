#pragma optimize("s",on)
extern void __cdecl fable_operator_delete(void* p);

struct CLandscapeBackgroundPatch {
    void* vftable;
    void real_dtor();
    void* vector_deleting_destructor(unsigned int flags);
};

void* CLandscapeBackgroundPatch::vector_deleting_destructor(unsigned int flags)
{
    this->real_dtor();
    if (flags & 1) {
        fable_operator_delete(this);
    }
    return this;
}