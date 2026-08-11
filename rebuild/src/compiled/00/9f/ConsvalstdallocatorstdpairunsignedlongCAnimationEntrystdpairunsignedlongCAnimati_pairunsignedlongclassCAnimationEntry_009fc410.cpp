// _Cons_val<allocator<pair<unsigned long,CAnimationEntry> >, pair<...>, pair<...>&>
// retail 0x009fc410  __fastcall, ret void, ret 0xc
//
// Standard Dinkumware allocator _Cons_val forwarder: constructs one element by
// forwarding (ptr, value) to the construct helper living on the sub-allocator at
// this+0x40.  The 8-byte pair<unsigned long,CAnimationEntry> value is copied
// through the x87 stack (fld/fstp) -- VC7.1 models this 8-byte-by-value copy as a
// double, so the value parameter is typed accordingly to reproduce that codegen.

struct CAnimationEntry { unsigned long id; };
struct APair { unsigned long first; CAnimationEntry second; };

struct ConsAlloc {
    void construct(APair *ptr, double val);
};

struct OuterAlloc {
    char pad[0x40];
    ConsAlloc sub;   // sub-allocator with the construct helper at this+0x40
    void Cons_val(APair *ptr, double val);
};

void OuterAlloc::Cons_val(APair *ptr, double val)
{
    sub.construct(ptr, val);
}