// Byte-exact reconstruction of CAIStateGroup_HoverHornetQueen destructor @0x0045ce22
// Retail:
//   push esi; mov esi,ecx; lea ecx,[esi+0x48]; call 0x99eae0;
//   lea ecx,[esi+0x28]; call 0x45645d; mov dword[esi],0x1230ba0;
//   mov ecx,esi; pop esi; jmp 0x9fc550

struct MemberA48 {
    void dtorA48();
};
struct MemberB28 {
    void dtorB28();
};
struct Base9fc550 {
    void* vtbl;
    void dtorBase();
};

struct CAIStateGroup_HoverHornetQueen {
    void* vtbl;      // +0x00
    char pad04[0x28 - 0x04];
    MemberB28 mB;    // +0x28
    char pad2C[0x48 - 0x28 - sizeof(MemberB28)];
    MemberA48 mA;    // +0x48
};

extern "C" void __fastcall MemberA48_dtor(MemberA48* self);   // 0x99eae0
extern "C" void __fastcall MemberB28_dtor(MemberB28* self);   // 0x45645d
extern "C" void __fastcall Base_dtor(Base9fc550* self);       // 0x9fc550

extern int HHQ_vtbl;  // 0x1230ba0

void __fastcall CAIStateGroup_HoverHornetQueen_dtor(CAIStateGroup_HoverHornetQueen* self)
{
    MemberA48_dtor(&self->mA);
    MemberB28_dtor(&self->mB);
    self->vtbl = &HHQ_vtbl;
    Base_dtor(reinterpret_cast<Base9fc550*>(self));
}