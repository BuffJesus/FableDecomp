#pragma optimize("s",on)
// Byte-exact reconstruction of CLandscapeBackgroundPatch::`vector deleting destructor'
// retail 0x004dd7ea (28 bytes)
//
// Layout:
//   push esi; mov esi,ecx
//   call 0x4da6c8            ; scalar/inner destructor
//   test byte ptr [esp+8],1  ; flags & 1  (flags is a STACK arg -> real __fastcall)
//   je +7
//   push esi; call operator delete (0xbfe9bc); pop ecx
//   mov eax,esi; pop esi; ret 4

typedef unsigned int uint;

// operator delete @ 0xbfe9bc
extern void __cdecl operator_delete(void* p);

struct CLandscapeBackgroundPatch
{
    // inner scalar destructor @ 0x4da6c8
    void ScalarDtor();
    // the vector deleting destructor itself (real member -> native __fastcall:
    //   this in ecx, flags pushed on the stack)
    void* VectorDeletingDtor(uint flags);
};

void* CLandscapeBackgroundPatch::VectorDeletingDtor(uint flags)
{
    this->ScalarDtor();
    if (flags & 1)
        operator_delete(this);
    return this;
}