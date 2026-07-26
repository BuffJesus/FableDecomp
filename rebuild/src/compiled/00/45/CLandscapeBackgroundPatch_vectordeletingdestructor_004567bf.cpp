#pragma optimize("s",on)
extern void __cdecl operator_delete(void* p);

struct CLandscapeBackgroundPatch {
    int f0;
    void __fastcall scalar_dtor();
    void* vector_deleting_destructor(unsigned int flags);
};

void* CLandscapeBackgroundPatch::vector_deleting_destructor(unsigned int flags)
{
    this->scalar_dtor();
    if (flags & 1)
        operator_delete(this);
    return this;
}