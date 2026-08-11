#pragma optimize("s",on)
// pair<unsigned long, CAnimationEntry> is 8 bytes; its destructor is external.
struct Pair {
    unsigned long first;
    // CAnimationEntry second; -- modeled so sizeof(Pair)==8
    unsigned long second;
    void destroy();  // external non-inline dtor-equivalent at 0x4db5bb (__fastcall this in ecx)
};

struct Alloc { char dummy; };

// std::_Destroy_range: destroy each element in [first,last), stride sizeof(Pair)=8.
// __fastcall: first=ecx, last=edx, al on stack (ret 4).
void __fastcall Destroy_range_allocator(Pair* first, Pair* last, Alloc* al)
{
    for (; first != last; ++first)
        first->destroy();
}