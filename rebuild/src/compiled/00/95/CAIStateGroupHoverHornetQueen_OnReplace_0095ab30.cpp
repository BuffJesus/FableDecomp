// CAIStateGroup_HoverHornetQueen::OnReplace @ 0095ab30

struct Sub50 {
    void Method(unsigned int a, unsigned int b);   // thiscall @ 0x4ac0f0
};

struct CAIStateGroup_HoverHornetQueen {
    char pad[0x50];
    Sub50 m50;                 // +0x50
    unsigned int m54;          // +0x54
    void OnReplaceBase();      // non-virtual thiscall @ 0x95aa00
    void OnReplace();
};

void CAIStateGroup_HoverHornetQueen::OnReplace()
{
    OnReplaceBase();
    unsigned int a = *(unsigned int*)((char*)this + 0x50);
    unsigned int b = *(unsigned int*)((char*)this + 0x54);
    m50.Method(a, b);
}