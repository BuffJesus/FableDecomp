#pragma optimize("s",on)
extern "C" void operator_delete_stub(void*);
struct CLandscapeBackgroundPatch {
    void dtor();
    void* vector_deleting_destructor(unsigned int flags);
};

void* CLandscapeBackgroundPatch::vector_deleting_destructor(unsigned int flags)
{
    this->dtor();
    if (flags & 1)
        operator_delete_stub(this);
    return this;
}