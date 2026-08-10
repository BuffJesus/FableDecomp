// _Cons_val<std::allocator<pair<unsigned long,CAnimationEntry> > > forwarder.
// Retail: mov eax,[esp+8]; mov edx,[esp+4]; mov ecx,[ecx]; push eax; push edx;
//         mov edx,[esp+0x14]; call 0x5db6b0; ret 0xc.
// this-in-ecx; loads *this (allocator/first member) into ecx for the callee (also __fastcall),
// edx = 3rd stack arg, stack pushes = 1st and 2nd stack args.

struct Pair;      // pair<unsigned long, CAnimationEntry>
struct PairV;     // the "pair *" param (value)
struct Alloc;     // *this member type

// Callee at 0x5db6b0 — __fastcall(ecx=inner, edx=arg3, stack: arg1, arg2)
extern void __fastcall inner_cons(void* self, PairV* a3, Pair* a1, PairV* a2);

struct Cons {
    void* m0;   // *ecx
};

void __fastcall _Cons_val(Cons* self, int /*edx*/, Pair* a1, PairV* a2, PairV* a3)
{
    inner_cons(self->m0, a3, a1, a2);
}