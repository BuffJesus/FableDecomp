// ?GetStaticMapUpdatePassCount@CIEngine@@UAEJXZ @ 0x00b23470
// Retail: 33 c0 c3  ->  xor eax,eax ; ret
// Virtual this-only accessor returning long 0. Modeled as __fastcall so the
// object pointer arrives in ECX (byte-identical to __fastcall for a this-only
// method). VC7.1 /O2 emits "xor eax,eax; ret" for "return 0;".

struct CIEngine
{
    void *vftable;
};

long __fastcall CIEngine_GetStaticMapUpdatePassCount(CIEngine *thisptr)
{
    (void)thisptr;
    return 0;
}