// CLandscapeBackgroundPatch::`vector_deleting_destructor' @ 0x0048b580
// Standard MSVC scalar/deleting destructor thunk shape:
//   save this; call real dtor; if(flags&1) operator delete(this); return this;

struct CLandscapeBackgroundPatch
{
    void Destroy();  // the real (scalar) destructor body @ 0xa0d3d0
    void* VectorDeletingDestructor(unsigned int flags);
};

// operator delete @ 0xbfe9bc
void operator_delete_impl(void* p);

void* CLandscapeBackgroundPatch::VectorDeletingDestructor(unsigned int flags)
{
    this->Destroy();
    if (flags & 1)
        operator_delete_impl(this);
    return this;
}