#pragma optimize("s",on)
// Byte-exact reconstruction of CLandscapeBackgroundPatch::`vector deleting destructor'
// retail 0x004dbee7. Synthesized by the compiler for a class with a virtual dtor;
// a delete site forces emission. The scalar dtor is noinline (retail 0x4d799a) so the
// synthesized vector deleting destructor keeps its `call` instead of inlining.

struct CLandscapeBackgroundPatch
{
    virtual ~CLandscapeBackgroundPatch();
};

__declspec(noinline) CLandscapeBackgroundPatch::~CLandscapeBackgroundPatch()
{
}

// force emission of the synthesized `vector deleting destructor'
void force_emit(CLandscapeBackgroundPatch *p)
{
    delete p;
}