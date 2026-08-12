// CLandscapeBackgroundPatch::`vector_deleting_destructor'  0x0048b660
// Standard MSVC vector deleting destructor for a class with a scalar
// (non-virtual) destructor at 0xa0d3d0 and operator delete at 0xbfe9bc.

struct CLandscapeBackgroundPatch
{
    // scalar destructor - emitted as a direct call in the vector-deleting stub
    void Destroy();  // maps to 0xa0d3d0
};

// operator delete used by the class (0xbfe9bc)
void ClassOperatorDelete(void* p);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(
    CLandscapeBackgroundPatch* self, int /*edx*/, unsigned int flags)
{
    self->Destroy();
    if (flags & 1)
        ClassOperatorDelete(self);
    return self;
}