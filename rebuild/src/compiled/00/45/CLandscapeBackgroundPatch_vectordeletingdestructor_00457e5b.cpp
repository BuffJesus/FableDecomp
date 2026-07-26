#pragma optimize("s",on)
extern "C" void __cdecl eng_operator_delete(void* p);

struct CLandscapeBackgroundPatch {
    void* vftable;
    void Dtor();
    void* vector_deleting_destructor(unsigned int flags);
};

void* CLandscapeBackgroundPatch::vector_deleting_destructor(unsigned int flags)
{
    this->Dtor();
    if (flags & 1) {
        eng_operator_delete(this);
    }
    return this;
}