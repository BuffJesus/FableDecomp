// std::pair _Dest_val: release the second member, then tail-call the first
// (this+0xc and this+0x14). __fastcall this=ecx. pack(1) pins the offsets.
#pragma pack(push,1)
struct Part { void Release(); };
struct Pair {
    char lead[0xc];
    Part first;
    char pad[0x7];
    Part second;
    void Destroy();
};
#pragma pack(pop)
void Pair::Destroy() {
    this->second.Release();
    this->first.Release();
}