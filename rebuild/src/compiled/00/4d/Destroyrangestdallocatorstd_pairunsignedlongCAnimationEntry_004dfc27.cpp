// std::_Destroy_range<std::allocator<std::pair<unsigned long, CAnimationEntry> > >
// Retail 0x004dfc27 (__fastcall, ret 4):
//   push esi; push edi; edi=last(edx); esi=first(ecx); jmp cond
//   body: ecx=esi(element); call pair::~pair(0x4d799a); esi+=8
//   cond: cmp esi,edi; jne body; pop edi; pop esi; ret 4
// The pair element is 8 bytes (4-byte key + 4-byte CAnimationEntry). The per-element
// destructor at 0x4d799a is a plain __fastcall on the element base pointer; modelled
// here as a free __fastcall(self) (ABI-identical: this in ecx). The trailing allocator
// argument is passed on the stack and cleaned by ret 4, but otherwise unused.
#pragma optimize("s",on)

struct pair_unsigned_long_CAnimationEntry {
    unsigned long first;
    void* second;
};

struct MyAlloc {
    char dummy;
};

// element destructor (retail 0x4d799a); ecx = element base pointer
void __fastcall destroy_pair_unsigned_long_CAnimationEntry(
    pair_unsigned_long_CAnimationEntry* self);

void __fastcall Destroy_range_pair_unsigned_long_CAnimationEntry(
    pair_unsigned_long_CAnimationEntry* first,
    pair_unsigned_long_CAnimationEntry* last,
    MyAlloc* al)
{
    for (; first != last; ++first)
        destroy_pair_unsigned_long_CAnimationEntry(first);
    (void)al;
}