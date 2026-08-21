// `return this->ptr != 0;` (xor/test/setne). __fastcall this=ecx.
#pragma pack(push,1)
struct T {
    char pad_0[0x14];
    int ptr;
    bool IsSet();
};
#pragma pack(pop)
bool T::IsSet() { return this->ptr != 0; }