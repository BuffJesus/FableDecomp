#pragma once
// Hand-owned (retail model): C3DClothPrimitive, a mesh primitive simulated as cloth (lib_3d_cloth_primitive).
// Retail RTTI .?AVC3DClothPrimitive@@ : CBaseClassNonCopyable : CBaseClass, vtable 0x0129DF74.
// Ego_r.pdb names and members (0x14): ParticleProgram CCountedPointer<CParticleProgram> +0x04, PrimitiveIndex +0x0c,
// MaterialIndex +0x10. Methods are declared as reconstructed (Sizeof/SaveBinary/LoadBinary/ctor/dtor not yet).
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_base_class.h"
#include "fable_counted_pointer.h"
#include "fable_particle_program.h"

class C3DMeshFileClothChunk;
class CDataOutputStream;
class CMemoryDataInputStream;

// Ego_r N3DMeshAllocators::ClothMemoryPool (Ego_r 0x01330110, retail 0x013D2D70): the movable-resource pool cloth
// particle programs are read into.
namespace N3DMeshAllocators
{
extern CMovableResourceMemoryPool* ClothMemoryPool;
}

class C3DClothPrimitive : public CBaseClassNonCopyable
{
public:
    // Ego_r ??0C3DClothPrimitive@@QAE@ABVC3DMeshFileClothChunk@@@Z (retail 0x00AC3E00): build the particle program
    // from the chunk's compiled code and give it every group sub-chunk.
    explicit C3DClothPrimitive(const C3DMeshFileClothChunk& chunk);
    // Ego_r ??1C3DClothPrimitive@@UAE@XZ (retail 0x00AC3B80): releases the particle program.
    virtual ~C3DClothPrimitive();
    // Ego_r ?SaveBinary@C3DClothPrimitive@@QBEXAAVCDataOutputStream@@@Z (retail 0x00AC3B00) and
    // ?LoadBinary@C3DClothPrimitive@@QAE_NAAVCMemoryDataInputStream@@PAVCMovableResourceMemoryPool@@@Z (retail 0x00AC3BD0):
    // the primitive and material indices, then the particle program as one compressed block.
    void SaveBinary(CDataOutputStream& stream) const;
    bool LoadBinary(CMemoryDataInputStream& stream, CMovableResourceMemoryPool* pool);
    // Ego_r ?GetUsedBones@C3DClothPrimitive@@QBEXAAV?$CArray@_N@@@Z (retail 0x00AC3AE0) and
    // ?RemapBoneIndices@C3DClothPrimitive@@QAEXAAV?$map@KK...@Z (retail 0x00AC3AF0): forward to the particle program.
    void GetUsedBones(CArray<bool>& usedBones) const;
    void RemapBoneIndices(_STL::map<unsigned long, unsigned long>& boneRemap);

    CCountedPointer<CParticleProgram> ParticleProgram;   // +0x04
    unsigned long PrimitiveIndex;                        // +0x0c
    unsigned long MaterialIndex;                         // +0x10
};

FABLE_STATIC_ASSERT(sizeof(C3DClothPrimitive) == 0x14);
FABLE_STATIC_ASSERT(offsetof(C3DClothPrimitive, ParticleProgram) == 0x04);
FABLE_STATIC_ASSERT(offsetof(C3DClothPrimitive, PrimitiveIndex) == 0x0c);
