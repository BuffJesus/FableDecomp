// CLandscapeBackgroundPatch::`vector_deleting_destructor'  @ 0x0048b5c0
// Standard MSVC7.1 vector deleting destructor thunk:
//   this->~T(); if (flags & 1) operator delete(this); return this;

struct CLandscapeBackgroundPatch
{
    void Destroy();  // scalar destructor body @ 0xa0d3d0 (this in ecx)
};

// operator delete forwarder @ 0xbfe9bc
void FreeMem(void* p);

void* __fastcall LandscapeBackgroundPatch_vector_deleting_destructor(
    CLandscapeBackgroundPatch* self, int /*edx*/, unsigned int flags)
{
    self->Destroy();
    if (flags & 1)
        FreeMem(self);
    return self;
}