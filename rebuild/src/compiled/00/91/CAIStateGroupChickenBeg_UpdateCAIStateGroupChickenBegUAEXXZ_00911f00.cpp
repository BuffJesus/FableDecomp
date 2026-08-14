// Byte-exact reconstruction of CAIStateGroup_ChickenBeg::Update @ 0x00911f00
struct A; struct B; struct C;

// engine callees (this-call modeled as __fastcall, self in ecx)
A* __fastcall f1(void* self);   // 0x8fcf90
B* __fastcall f2(A* self);      // 0x449970
C* __fastcall f3(B* self);      // 0x487dd0
void __fastcall f4(void* self); // 0x8fcf40

struct CAIStateGroup_ChickenBeg {
    virtual void Update();   // default thiscall -> mangles ?Update@...@@UAEXXZ
};

void CAIStateGroup_ChickenBeg::Update()
{
    f3( f2( f1(this) ) );
    f4(this);
}