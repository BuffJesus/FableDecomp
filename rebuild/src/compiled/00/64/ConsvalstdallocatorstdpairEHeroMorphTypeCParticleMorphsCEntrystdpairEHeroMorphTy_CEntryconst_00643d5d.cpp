#pragma optimize("s",on)
// Byte-exact reconstruction of 0x00643d5d
// __fastcall member (this=ecx, one 4-byte stack arg -> ret 4).
// Returns (long)( (float)(unsigned char)key * this->inner->scale ).
// The float->long uses the CRT helper __ftol2 (retail 0x00bfea70), which
// takes its operand on the x87 stack and returns the long in edx:eax --
// hence retail emits fmul then a bare `call` with no intermediate store.

struct Inner {
    char  pad[0x2a0];
    float scale;      // +0x2a0
};

struct Outer {
    int    m0;        // +0
    Inner* inner;     // +4
};

// Model the this-call member as a __fastcall free fn: self in ecx, an unused
// edx register slot, then the real 4-byte stack argument (the EHeroMorphType
// key). VC7.1's cast-to-long emits the __ftol2 call automatically.
long __fastcall Cons_val_CEntry(Outer* self, int /*edx*/, int keyEHeroMorphType)
{
    Inner* p = self->inner;
    unsigned char b = (unsigned char)keyEHeroMorphType;
    float f = (float)(int)b;
    return (long)(f * p->scale);
}