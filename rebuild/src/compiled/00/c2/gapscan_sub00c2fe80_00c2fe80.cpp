// Tail-forward to vtable slot 0 of the sub-object pointer at this+0x4,
// passing the DEREFERENCED stack argument (VC7.1 rewrites the incoming arg
// slot and `jmp`s). __fastcall this=ecx.
#pragma pack(push,1)
struct Sub {
    virtual void Do(int v);
};
struct T {
    char pad_0[0x4];
    Sub* sub;
    void Do(int* p);
};
#pragma pack(pop)
void T::Do(int* p) { this->sub->Do(*p); }