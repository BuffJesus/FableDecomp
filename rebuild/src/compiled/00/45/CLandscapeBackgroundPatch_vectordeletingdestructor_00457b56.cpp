#pragma optimize("s",on)
extern void __cdecl eng_operator_delete(void* p);

struct CLandscapeBackgroundPatch {
    void* vftable;
    void dtor();
    void* vector_deleting_destructor(unsigned int flags);
};

void* CLandscapeBackgroundPatch::vector_deleting_destructor(unsigned int flags)
{
    this->dtor();
    if (flags & 1)
        eng_operator_delete(this);
    return this;
}