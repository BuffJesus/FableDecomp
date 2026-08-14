// _Cons_val<...> : placement copy-construct a pair<CCharString const, CArray<ulong>>
// dest layout: [0..0xc) = CCharString first, [0xc..) = CArray second.
// The two member copy-ctors are real thiscall methods: ecx=dest, src on stack,
// no edx setup -> matches retail push/call (no xor edx).

struct First  { void Copy(void* src); };   // CCharString copy-ctor @0xad26d0
struct Second { void Copy(void* src); };    // CArray copy-ctor @0xad2730

void __fastcall ConsValPairCCharStringCArray(char* self, char* src)
{
    if (self != 0)
    {
        ((First*)self)->Copy(src);
        ((Second*)(self + 0xc))->Copy(src + 0xc);
    }
}