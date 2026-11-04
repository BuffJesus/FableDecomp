#pragma once
// Hand-owned (retail model): C3DAnimationManager, the mesh bank's animation sampler/blender (lib_3d_animation_manager).
// Retail RTTI .?AVC3DAnimationManager@@ : CBaseClassNonCopyable : CBaseClass, vtable 0x0129E134; destructor 0x00AA4170
// tears down MemoryPool2 (+0x30), MemoryPool (+0x14) and the BoneIndicesBuffer storage (+0x08) before the base.
// Ego_r.pdb members: MeshBank +0x04, BoneIndicesBuffer CArray<unsigned long> +0x08, MemoryPool / MemoryPool2
// CSimpleMemoryPool (Ego_r 0x50; retail's STLport CArray is 12 bytes, so the pools sit 4 bytes lower).
// Partial view: only the members read by a reconstructed retail body are declared.
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_base_class.h"

class CMeshDataBank;

class C3DAnimationManager : public CBaseClassNonCopyable
{
public:
    // Ego_r ?Initialise@C3DAnimationManager@@QAEXPAVCMeshDataBank@@@Z (retail 0x00A9D730) and
    // ?Uninitialise@C3DAnimationManager@@QAEXXZ (retail 0x00A9D740): attach to / detach from the owning mesh bank.
    void Initialise(CMeshDataBank* meshBank);
    void Uninitialise();

private:
    friend struct C3DAnimationManagerLayoutCheck;
    CMeshDataBank* MeshBank;                  // +0x04
};

struct C3DAnimationManagerLayoutCheck
{
    FABLE_STATIC_ASSERT(offsetof(C3DAnimationManager, MeshBank) == 0x04);
};
