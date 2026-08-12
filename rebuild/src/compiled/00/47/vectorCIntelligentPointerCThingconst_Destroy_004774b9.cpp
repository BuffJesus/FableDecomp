#pragma optimize("s",on)
// Byte-parity reconstruction of
// std::vector<CIntelligentPointer<CThing const*>,...>::_Destroy at 0x004774b9
//
// Retail disasm iterates [first,last) with element stride 0x88 and, for each
// element, calls virtual slot 0 with a single int arg (0) -- a scalar-deleting
// destructor. The loop tests first!=last first (jmp to the compare), so an
// empty range does nothing. The body uses ecx=first and edx=last directly
// (two register params), so model as a two-arg __fastcall.

struct CIntelligentPointer_CThing
{
    // vtable ptr at +0; slot 0 = scalar-deleting dtor taking an int flag.
    virtual void slot0(int flag);
    // pad the object out to 0x88 bytes so pointer arithmetic strides 0x88.
    char pad[0x88 - 4];
};

void __fastcall vecCIP_Destroy(CIntelligentPointer_CThing* first,
                               CIntelligentPointer_CThing* last)
{
    for (; first != last;
         first = (CIntelligentPointer_CThing*)((char*)first + 0x88))
    {
        first->slot0(0);
    }
}