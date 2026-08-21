// Clear the dword at this+0x1c and the byte flag at this+0x20.
#pragma pack(push,1)
struct T {
    char pad_0[0x1c];
    int count;
    bool flag;
    void Clear();
};
#pragma pack(pop)
void T::Clear() { this->count = 0; this->flag = false; }