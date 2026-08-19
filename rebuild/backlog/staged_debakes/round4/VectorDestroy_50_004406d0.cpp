// ~vector<T>: destroy every element through its vtable slot 0 (flag arg 0, stride 8),
// then release the block. __fastcall this=ecx, no args.
struct Elem {
    virtual void Destroy(int flags);
    int extra;                       // stride 8
};
struct Vec {
    Elem* first;
    Elem* last;
    void Destroy();
};
extern "C" void __cdecl Free2(void* p);   // 0x00bfea14
void Vec::Destroy() {
    Elem* p = this->first;
    Elem* end = this->last;
    while (p != end) {
        p->Destroy(0);
        ++p;
    }
    if (this->first) Free2(this->first);
}
