// CEngineSubPrimitiveGameMesh::`vector deleting destructor'
// Retail 0x005298c0. Standard MSVC vector-deleting-destructor thunk:
//   push esi; mov esi,ecx; call <scalar dtor>; test [esp+8],1; je L;
//   mov ecx,esi; call <thiscall free>; L: mov eax,esi; pop esi; ret 4
//
// Scalar destructor @0x838a00 is a direct (non-virtual) member call.
// Deallocator @0x4c73b0 is called thiscall (self in ecx).
// Both callees are declared but NOT defined in this TU so /O2 cannot
// inline them away -- the compiler must emit a real `call`.

struct CEngineSubPrimitiveGameMesh
{
    void Destroy();                 // @0x838a00
    void FreeSelf();                // @0x4c73b0
    void *VecDelDtor(unsigned int flags);
};

void *CEngineSubPrimitiveGameMesh::VecDelDtor(unsigned int flags)
{
    Destroy();
    if (flags & 1)
        FreeSelf();
    return this;
}