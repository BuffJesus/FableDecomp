// Byte-exact reconstruction of CCharString::SetUseFastExtend @ 0x0099ef70

struct CharBuf {
    char pad[0xc];
    unsigned char flags; // offset 0xc
};

struct CCharString {
    CharBuf* buf; // offset 0
};

// this->EnsureUnique() (thiscall, ecx=this) @ 0x99eb40; modeled as __fastcall.
extern void __fastcall CCharString_EnsureUnique(CCharString* self);

// Retail is __fastcall(this, bool) with the bool passed on the STACK and a
// ret 4 epilogue. The harness rewrites __fastcall->__fastcall, so to keep the
// bool on the stack (edx unused) we add a dummy edx register slot and let the
// real bool land as the stack parameter.
void __fastcall CCharString_SetUseFastExtend(CCharString* self, int /*edx*/, bool value)
{
    CCharString_EnsureUnique(self);
    CharBuf* b = self->buf;
    unsigned char cur = b->flags;
    unsigned char x = (unsigned char)(cur ^ (unsigned char)value);
    x &= 1;
    b->flags = (unsigned char)(b->flags ^ x);
}