// `return this->[0xc] >= this->[0x4];` (unsigned: cmp / sbb / inc).
#pragma pack(push,1)
struct T {
    char pad_0[0x4];
    unsigned int f_4;
    char pad_8[0x4];
    unsigned int f_c;
    int AtLeast();
};
#pragma pack(pop)
int T::AtLeast() { return this->f_c >= this->f_4; }