// std::pair _Dest_val: release the member at this+0x58, then TAIL-call the one
// at this+0x64. __fastcall this=ecx. pack(1) pins both offsets.
#pragma pack(push,1)
struct Part { void Release(); };
struct Pair {
    char lead[0x58];
    Part second;
    char gap[0xb];
    Part first;
    void Destroy();
};
#pragma pack(pop)
void Pair::Destroy() {
    this->second.Release();
    this->first.Release();
}