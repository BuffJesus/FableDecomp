// std::pair<K,V> _Dest_val: destroy the second member, then tail-call the first.
// __fastcall this=ecx. Offsets are byte-exact, so pack(1).
#pragma pack(push,1)
struct Part { void Release(); };
struct Pair {
    Part first;
    char pad[0x08 - 1];
    Part second;
    void Destroy();
};
#pragma pack(pop)
void Pair::Destroy() {
    this->second.Release();
    this->first.Release();
}
