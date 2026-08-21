// Clear the dword at this+0x10 and the byte flag at this+0xc.
#pragma pack(push,1)
struct T {
    char pad_0[0xc];
    bool flag;
    char pad_d[0x3];
    int count;
    void Clear();
};
#pragma pack(pop)
void T::Clear() { this->count = 0; this->flag = false; }