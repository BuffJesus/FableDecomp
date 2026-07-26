// ?GetMorphInfo@CEngineInternalPrimitiveMeshBase@@UAEPAVCEngineInternalPrimitiveMorphInfo@@XZ
// retail 0x00ba7fe0 : 33 c0 c3  ->  xor eax,eax ; ret
//
// Virtual accessor. The base-class implementation returns a null
// CEngineInternalPrimitiveMorphInfo* (no morph info by default); derived
// meshes that actually carry morph data override this slot.
//
// VC7.1 note: modelled as __fastcall with the object pointer as the first
// argument. A this-only accessor arrives with 'this' in ecx, byte-identical
// to __fastcall. Nothing touches ecx here, so the reg convention is moot for
// codegen; cl /O2 emits 'xor eax,eax ; ret' either way.

class CEngineInternalPrimitiveMorphInfo;  // opaque; return is relocation-agnostic (null)

class CEngineInternalPrimitiveMeshBase
{
public:
    // Real declaration would be:
    //   virtual CEngineInternalPrimitiveMorphInfo* GetMorphInfo();
    // but to guarantee __fastcall-identical bytes without a vtable emit we
    // expose the body as a free __fastcall accessor below.
};

CEngineInternalPrimitiveMorphInfo * __fastcall
CEngineInternalPrimitiveMeshBase_GetMorphInfo(CEngineInternalPrimitiveMeshBase * /*this_ in ecx*/)
{
    return 0;   // xor eax,eax ; ret
}