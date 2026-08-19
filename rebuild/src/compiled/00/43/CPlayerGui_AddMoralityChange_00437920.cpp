// Store a stack arg into a field of the sub-object POINTER at this+0x1a8
// (field at sub+0x28). __fastcall this=ecx (ret 4).
#pragma pack(push,1)
struct Sub {
    char pad[0x28];
    int field;
};
struct T {
    char pad[0x1a8];
    Sub* sub;
    void Set(int value);
};
#pragma pack(pop)
void T::Set(int value) { this->sub->field = value; }