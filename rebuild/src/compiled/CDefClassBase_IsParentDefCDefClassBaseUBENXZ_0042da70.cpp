// ?IsParentDef@CDefClassBase@@UBE_NXZ @ 0x0042da70
// Retail: 32 c0 c3  ->  xor al,al ; ret
//
// Virtual const accessor returning bool. Base-class default implementation
// simply returns false. Modeled as __fastcall with the object pointer in ecx
// (byte-identical to __fastcall for a this-only accessor). VC7.1 emits the
// bool-false return as 'xor al,al' rather than 'mov eax,0'.

struct CDefClassBase
{
    void* vftable;
};

extern "C" bool __fastcall CDefClassBase_IsParentDef(CDefClassBase* self)
{
    (void)self;      // 'this' arrives in ecx, unused by the accessor
    return false;    // -> xor al,al ; ret
}