#pragma optimize("s",on)
struct CLandscapeBackgroundPatch {
    void* vftable;
    void Dtor();
    void* vector_deleting_destructor(unsigned int flags);
};
extern void __cdecl op_delete(void* p);

void* CLandscapeBackgroundPatch::vector_deleting_destructor(unsigned int flags)
{
    this->Dtor();
    if (flags & 1)
        op_delete(this);
    return this;
}