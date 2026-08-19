// Store a stack arg into a field of the sub-object POINTER at this+0x220.
// __fastcall this=ecx (ret 4).
#pragma pack(push,1)
struct Sub { char pad[0x40]; int field; };
struct T {
    char pad[0x220];
    Sub* sub;
    void Set(int value);
};
#pragma pack(pop)
void T::Set(int value) { this->sub->field = value; }
