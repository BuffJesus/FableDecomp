// ?GetAnimDelayTime@CCreatureActionBase@@UBEJXZ @ 0x00692460
// Retail: 33 c0 c3  ->  xor eax,eax ; ret
// Virtual const this-only accessor returning long 0.
// Modeled as __fastcall with 'this' in ecx (byte-identical to __fastcall for a this-only accessor).

struct CCreatureActionBase {
    void* vtbl;
};

// long GetAnimDelayTime() const  ->  xor eax,eax ; ret
long __fastcall CCreatureActionBase_GetAnimDelayTime(const CCreatureActionBase* self)
{
    (void)self;
    return 0;
}