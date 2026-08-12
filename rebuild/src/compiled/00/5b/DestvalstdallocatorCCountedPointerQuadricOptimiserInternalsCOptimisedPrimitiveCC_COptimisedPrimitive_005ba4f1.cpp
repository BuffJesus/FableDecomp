// Retail 0x005ba4f1:  jmp 0x5ba0f8  (5-byte tail-call thunk)
//
// std::_Dest_val<allocator<CCountedPointer<...>>, CCountedPointer<...>>(al, p)
// forwards to the out-of-line destroy target; VC7.1 /O2 tail-calls it,
// producing a single 'jmp rel32'.

struct CountedPtr {
    void* ptr;
};

struct Alloc {
    int dummy;
};

// The out-of-line jmp target (the 0x5ba0f8 body). noinline so it stays a call.
void __fastcall Alloc_destroy(Alloc* self, CountedPtr* p);

// The thunk: whole body is a forwarding call -> tail-call jmp (same ecx/edx).
void __fastcall _Dest_val(Alloc* al, CountedPtr* p)
{
    Alloc_destroy(al, p);
}