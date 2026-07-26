// ?GetPSubDefInfoMap@CDefClassBase@@UAEPAV?$CVectorMap@KVCSubDefInfo@CDefClassBase@@V?$CKeyPairCompareLess@KVCSubDefInfo@CDefClassBase@@@@@@XZ
// Retail 0x0042daa0 (3 bytes): 33 c0 c3  ->  xor eax,eax ; ret
//
// Virtual accessor CDefClassBase::GetPSubDefInfoMap() that returns a null map
// pointer in the base class (derived classes override it to return a real map).
// Modeled as a __fastcall free function: the object pointer arrives in ECX
// (byte-identical to __fastcall for a this-only member). VC7.1 / MSVC 2003.
//
// The body simply returns 0, which the compiler emits as `xor eax,eax; ret`.

// Opaque forward declaration of the returned map type (never dereferenced here).
struct CVectorMap_ulong_CSubDefInfo;

// The CDefClassBase object; layout is irrelevant to this null-returning accessor,
// but we give it a vtable slot so the shape mirrors a real object.
struct CDefClassBase {
    void **vptr;
};

// __fastcall models __fastcall: `this` in ECX. No 2nd arg, so EDX is unused.
CVectorMap_ulong_CSubDefInfo * __fastcall
CDefClassBase_GetPSubDefInfoMap(CDefClassBase *thisptr)
{
    (void)thisptr;
    return 0;   // -> xor eax,eax ; ret
}