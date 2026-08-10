// COptimisedPrimitive::ActualToUniqueIndex @ 009ad390
// __fastcall long (COptimisedPrimitive* this, ulong actual)
// but retail reads two stack dwords ([esp+4],[esp+8]) and ret 8.
// this is passed in ECX (unused), stack args: arg1 (passed to callee 0x9ad2e0), arg2 (index).

struct SubTable {
    char pad[8];
    long* data;   // +8
};

// callee 0x9ad2e0 : __stdcall (self-cleaning, no add esp after the call), returns SubTable*
extern "C" SubTable* __stdcall sub_9ad2e0(unsigned long a);

struct COptimisedPrimitive;

long __fastcall ActualToUniqueIndex(COptimisedPrimitive* self, int /*edx*/, unsigned long a, unsigned long b)
{
    (void)self;
    SubTable* t = sub_9ad2e0(a);
    return t->data[b];
}