// _Cons_val<std::allocator<std::pair<unsigned long,CAnimationEntry>>,...>
// retail 006b83f0: mov eax,[esp+4]; mov edx,[esp+8]; push eax; push edx;
//                  add ecx,0x90; call helper; ret 8
struct Pair;

// storage object living at Alloc+0x90; helper is a this-in-ecx member (no edx)
struct Storage {
    void Construct(Pair* a, Pair* b);  // __fastcall: ecx=this, push a then b
};

struct Alloc {
    char pad[0x90];
    Storage storage_area;   // at +0x90
    void ConsVal(Pair* a, Pair* b);
};

void Alloc::ConsVal(Pair* a, Pair* b)
{
    this->storage_area.Construct(b, a);
}