#pragma optimize("s",on)
// Byte-exact reconstruction of CLandscapeBackgroundPatch scalar deleting destructor @ 0x00656e6f
// __fastcall(this, unsigned int flags) -> void*
//   push esi; mov esi,ecx
//   lea ecx,[esi+0x10]; call 0x440c40   (inner member dtor)
//   test byte ptr[esp+8],1; if set push esi; call operator delete; pop ecx
//   mov eax,esi; pop esi; ret 4

// Inner member at +0x10 has a __fastcall destructor (0x440c40).
struct InnerMember {
    ~InnerMember();          // emitted __fastcall; defined out-of-line elsewhere (extern)
};

struct CLandscapeBackgroundPatch {
    char pad[0x10];
    InnerMember inner;       // at +0x10
    // scalar deleting destructor: run inner dtor, optionally free
    void* vector_deleting_destructor(unsigned int flags);
};

void* CLandscapeBackgroundPatch::vector_deleting_destructor(unsigned int flags)
{
    inner.~InnerMember();
    if (flags & 1)
        ::operator delete(this);
    return this;
}