// std::pair _Dest_val: release the second member, then tail-call the first
// (this+0x0 and this+0x4c). __fastcall this=ecx. pack(1) pins the offsets.
#pragma pack(push,1)
struct Part { void Release(); };
struct Pair {
    Part first;
    char pad[0x4b];
    Part second;
    void Destroy();
};
#pragma pack(pop)
void Pair::Destroy() {
    this->second.Release();
    this->first.Release();
}