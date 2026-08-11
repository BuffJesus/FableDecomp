// CSystemRegistry destructor @ 0067c350
// push esi; mov esi,ecx; mov eax,[esi+8]; test eax; je; push eax; call operator delete; add esp,4
// mov [esi], 0x125b6d0 (vtable); pop esi; ret
//
// Non-virtual model so only ONE function is emitted (a virtual dtor makes MSVC also
// emit a larger vector-deleting-destructor thunk that the harness would mis-select).
// The trailing vtable store is modeled as an explicit assignment of the class vtable
// address into [this+0] (a reloc slot, masked by the harness).

extern void* const CSystemRegistry_vftable[];

struct CSystemRegistry
{
    void* vtbl;      // +0x00 vtable ptr
    int   pad4;      // +0x04
    void* buffer;    // +0x08 owned allocation

    void dtor();
};

void CSystemRegistry::dtor()   // ~CSystemRegistry
{
    if (buffer)
        operator delete(buffer);
    vtbl = (void*)CSystemRegistry_vftable;
}