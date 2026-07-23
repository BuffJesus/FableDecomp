extern void __cdecl engine_operator_delete(void* p);

struct CLandscapeBackgroundPatch {
    void* vftable;
    void RealDtor();
    void* CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned int flags);
};

void* CLandscapeBackgroundPatch::CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned int flags)
{
    this->RealDtor();
    if (flags & 1)
        engine_operator_delete(this);
    return this;
}