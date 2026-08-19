// Byte-exact reconstruction of CTCVillage::OnInitialActivate @ 0x004bbbe0

struct CInner;

// __fastcall/__fastcall member on the inner object:
//   ecx = inner (self->m0), edx = self->m4, stack = byte value.
extern void __fastcall InnerActivate(CInner* self, void* p, unsigned char flag);

struct CTCVillage
{
    CInner* m0;        // +0x00  -> ecx for the call
    void*   m4;        // +0x04  -> edx for the call
    unsigned char m8;  // +0x08
    unsigned char m9;  // +0x09
    unsigned char ma;  // +0x0a
    unsigned char mb;  // +0x0b
    unsigned char mc;  // +0x0c  pushed byte arg
    unsigned char md;  // +0x0d  guard flag
};

void __fastcall OnInitialActivate(CTCVillage* self)
{
    if (self->md)
    {
        InnerActivate(self->m0, self->m4, self->mc);
        self->md = 0;
    }
}