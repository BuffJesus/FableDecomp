// CTexture::GenerateMipmaps @ 009f9e60
// thiscall(this, ulong a, ulong b); this in ecx, a=[esp+4], b=[esp+8]; ret 8.
// Harness rewrites __fastcall->__fastcall, so add a dummy EDX param to keep
// both real args on the stack (matching retail's [esp+4]/[esp+8] reads).
// body: sub_bfefce( *(void**)this, 0, b, a ); callee cleans its own 16 bytes.

typedef unsigned long ulong;

struct CTexture {
    void* field0;
};

extern void __stdcall sub_bfefce(void* p, int zero, ulong b, ulong a);

void __fastcall GenerateMipmaps(CTexture* self, int /*edx*/, ulong a, ulong b)
{
    sub_bfefce(self->field0, 0, b, a);
}