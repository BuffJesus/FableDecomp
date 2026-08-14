#pragma optimize("s",on)
// CLandscapeBackgroundPatch::`vector_deleting_destructor' @ 0x005799de
struct Inner { void dtor(); };
struct CLandscapeBackgroundPatch { void* pad0; Inner inner; };
void __fastcall Inner_dtor(Inner* self);
extern "C" void __cdecl sub_operator_delete(void* p);
void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(
        CLandscapeBackgroundPatch* self, unsigned int, unsigned int flags)
{
    Inner_dtor((Inner*)((char*)self + 4));
    if (flags & 1) { sub_operator_delete(self); }
    return self;
}