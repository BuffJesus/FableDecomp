extern void __cdecl eng_operator_delete(void* p);

struct CLandscapeBackgroundPatch {
    void* vftable;
    void base_dtor();
    void* CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned int flags);
};

void* CLandscapeBackgroundPatch::CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned int flags)
{
    this->base_dtor();
    if (flags & 1)
        eng_operator_delete(this);
    return this;
}