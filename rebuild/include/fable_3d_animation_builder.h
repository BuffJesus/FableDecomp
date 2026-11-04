#pragma once
// Hand-owned (retail model): C3DAnimationBuilder (lib_3d_animation_builder), the tool-side animation compiler that
// matches source bone hierarchies to a mesh skeleton. Partial view: only the nested CBoneInfo's destructor, which the
// CCountedPointer<C3DAnimationBuilder::CBoneInfo> delete function calls.
// Ego_r.pdb CBoneInfo (0x28): Name CCharString +0x00, Index +0x04, OriginalNoChildren +0x08,
// Children CArray<CCountedPointer<CBoneInfo> > +0x0c, AnimSource CCountedPointer<CBoneInfo> +0x1c,
// ExtensionChild +0x24, ExtensionParent +0x25 (retail's STLport CArray is 12 bytes, so the tail sits 4 bytes lower).
// Retail destructor 0x00AB34C0 (not yet reconstructed); retail 0x00AB3660 builds a CCountedPointer<CBoneInfo> whose
// shared block stores DeleteData 0x00AB3640.
#include "rebuild_abi.h"

class C3DAnimationBuilder
{
public:
    class CBoneInfo
    {
    public:
        ~CBoneInfo();
    };
};
