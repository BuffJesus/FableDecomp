#pragma optimize("s",on)
struct CLandscapeBackgroundPatch { int f0; };
extern void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self);
extern void __cdecl op_delete(void* p);

struct CLandscapeBackgroundPatch_shim {
    void* CLandscapeBackgroundPatch_vecdel(unsigned int flags);
};

void* CLandscapeBackgroundPatch_shim::CLandscapeBackgroundPatch_vecdel(unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor((CLandscapeBackgroundPatch*)this);
    if (flags & 1)
        op_delete(this);
    return this;
}