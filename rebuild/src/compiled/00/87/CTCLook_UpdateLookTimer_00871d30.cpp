// CTCLook::UpdateLookTimer  @ 0x00871d30
// Retail codegen:
//   push esi; mov esi,ecx            ; save this
//   call 0x84f1b0                    ; this->GetA()  (thiscall, ecx=this) -> ptr in eax
//   mov ecx,eax; call 0x4c73d0       ; a->GetB()     (thiscall) -> ptr in eax
//   fld  dword [eax+8]               ; load b->f8
//   fadd dword [esi+0xb0]            ; + this->fB0
//   pop esi; ret                     ; return float in ST0

struct B {
    char pad[8];
    float f8;    // +0x08
};

struct A {
    // GetB(): thiscall returning B*
    B* GetB();   // 0x4c73d0
};

struct CTCLook {
    char pad[0xb0];
    float fB0;   // +0xb0

    A* GetA();   // 0x84f1b0  (thiscall on this)

    float UpdateLookTimer();
};

float CTCLook::UpdateLookTimer()
{
    A* a = this->GetA();
    B* b = a->GetB();
    return b->f8 + this->fB0;
}