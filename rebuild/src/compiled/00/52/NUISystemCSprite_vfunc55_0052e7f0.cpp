// Copy the members at this+0x3c / this+0x40 into the caller's 2-dword struct.
// __fastcall this=ecx, out=stack (ret 4).
#pragma pack(push,1)
struct Out { int a; int b; };
struct T {
    char pad_0[0x3c];
    int first;
    int second;
    void Get(Out* out);
};
#pragma pack(pop)
void T::Get(Out* out) {
    out->a = this->first;
    out->b = this->second;
}