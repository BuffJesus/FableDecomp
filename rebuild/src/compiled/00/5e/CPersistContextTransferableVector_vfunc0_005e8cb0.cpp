// Element count of the vector reached through the member pointer at this+0x4
// (`(last - first) >> 2`). __fastcall this=ecx.
#pragma pack(push,1)
struct Vec { int* first; int* last; };
struct T {
    char pad_0[0x4];
    Vec* vec;
    int Size();
};
#pragma pack(pop)
int T::Size() { return this->vec->last - this->vec->first; }