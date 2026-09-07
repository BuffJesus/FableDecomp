// CEnginePrimitiveManagerSprites::RenderPrimitive @ 0x00baf9e0
#include "engine/CEnginePrimitiveManagerSprites.h"  // retyped onto the PDB layout; byte parity re-verified
typedef int ERendered;
typedef int EEngineRenderLayer;

struct CEngineInternalPrimitiveBase { int dummy; };
struct CEngineSubPrimitiveChain { int dummy; };


// callee at 0xbade40: takes the primitive ref (one stack arg), stdcall (self-cleans)
extern "C" void __stdcall SpriteHelper(CEngineInternalPrimitiveBase &prim);

ERendered __fastcall RenderPrimitive(
    CEnginePrimitiveManagerSprites *self,  // ecx
    int edx_dummy,                         // edx (unused by thiscall)
    CEngineInternalPrimitiveBase &prim,    // [esp+4]
    EEngineRenderLayer layer,              // [esp+8]
    CEngineSubPrimitiveChain &chain)       // [esp+c]
{
    (void)edx_dummy; (void)layer; (void)chain;
    if (self->enabled == 0)
        return 0;
    SpriteHelper(prim);
    return 2;
}