#pragma optimize("s",on)
extern void __cdecl engine_operator_delete(void *p);

struct CLandscapeBackgroundPatch {
    void *vptr;
    void real_dtor();
    void * CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned int flags);
};

void * CLandscapeBackgroundPatch::CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned int flags)
{
    this->real_dtor();
    if (flags & 1)
        engine_operator_delete(this);
    return this;
}