// CParticleSystem::Kill  0x004bfae0  __fastcall, void, (CParticleSystem*, StackArg)
// ret 4 => one incoming stack arg besides this; its address (lea eax,[esp+8]; push eax)
// is passed to the +0x18 subobject method. Then a no-arg method on the object at +0x04,
// then set bit 2 (value 4) of the flag byte at +0x58.
//
// Callees modeled as real __fastcall members so codegen loads ecx=this and does NOT
// touch edx (retail leaves edx alone; an explicit __fastcall edx param would emit
// xor edx,edx and inflate the body by 4 bytes).

struct Sub18 {
    // method at 0x4c4430; takes address of the incoming stack arg
    void Method(void* argAddr);
};

struct Obj4 {
    // method at 0x4c7ac0; no args
    void Method();
};

struct CParticleSystem {
    char pad0[4];        // +0x00
    Obj4* p4;            // +0x04
    char pad8[0x18-8];   // +0x08 .. +0x17
    Sub18 sub;           // +0x18
    char pad1c[0x58-0x18-(int)sizeof(Sub18)];
    unsigned char flags; // +0x58
};

// __fastcall models this-call: self in ecx, edx dead, arg on stack, ret 4.
void __fastcall Kill(CParticleSystem* self, int edx, int arg)
{
    self->sub.Method(&arg);
    self->p4->Method();
    self->flags |= 4;
}