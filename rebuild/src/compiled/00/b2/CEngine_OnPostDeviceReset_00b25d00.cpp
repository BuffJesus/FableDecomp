// Byte-exact reconstruction of CEngine::OnPostDeviceReset @ 0x00b25d00
// this (ecx) = CEngine*; callee receives (this - 0xc).
// [ecx+0xb1] is a bool flag; if clear, return true immediately.
// else call B::DoReset() (0xb24500) on (this-0xc) and return result != 2.

struct B {
    int DoReset(); // __fastcall member @ 0xb24500, returns int
};

// __fastcall on B: model as __fastcall free fn (this in ecx).
int __fastcall B_DoReset(B* self);

struct CEngine {
    char pad1[0xb1];
    unsigned char flag_b1; // at +0xb1
};

bool __fastcall CEngine_OnPostDeviceReset(CEngine* self)
{
    if (self->flag_b1) {
        B* base = (B*)((char*)self - 0xc);
        return B_DoReset(base) != 2;
    }
    return true;
}