// Flag-selected float getter: `return flag ? high : low;`. __fastcall this=ecx.
#pragma pack(push,1)
struct T {
    char pad00[0x0c];
    char flag;
    char pad0d[0x13];
    float low;      // this+0x20
    char pad24[0xbc];
    float high;     // this+0xe0
    float Get();
};
#pragma pack(pop)
float T::Get() { return this->flag ? this->high : this->low; }
