#pragma optimize("s",on)
struct CLandscapeBackgroundPatch {
    int f0;
    int f4;
    void* f8;
    void CLandscapeBackgroundPatch_dtor();
    void* CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned int flags);
};
extern void __cdecl op_delete(void* p);
void* CLandscapeBackgroundPatch::CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned int flags)
{
    this->CLandscapeBackgroundPatch_dtor();
    if (flags & 1)
        op_delete(this);
    return this;
}