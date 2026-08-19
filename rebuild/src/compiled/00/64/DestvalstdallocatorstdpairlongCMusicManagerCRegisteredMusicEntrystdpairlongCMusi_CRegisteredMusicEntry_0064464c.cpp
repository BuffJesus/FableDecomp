// std::pair _Dest_val: release the member at this+0x28, then TAIL-call the one
// at this+0x34. __fastcall this=ecx. pack(1) pins both offsets.
#pragma pack(push,1)
struct Part { void Release(); };
struct Pair {
    char lead[0x28];
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