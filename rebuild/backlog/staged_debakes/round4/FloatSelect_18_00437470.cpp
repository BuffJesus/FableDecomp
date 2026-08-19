// Flag-selected float getter: `return flag ? high : low;`. __fastcall this=ecx.
#pragma pack(push,1)
struct T {
    char pad00[0x0c];
    char flag;
    char pad0d[0x0f];
    float low;      // this+0x1c
    char pad20[0xbc];
    float high;     // this+0xdc
    float Get();
};
#pragma pack(pop)
float T::Get() { return this->flag ? this->high : this->low; }
