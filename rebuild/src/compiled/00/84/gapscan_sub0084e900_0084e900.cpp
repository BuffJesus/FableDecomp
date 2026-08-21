// `return this->ptr == 0;` (xor/test/sete). __fastcall this=ecx.
#pragma pack(push,1)
struct T {
    int ptr;
    bool IsNull();
};
#pragma pack(pop)
bool T::IsNull() { return this->ptr == 0; }