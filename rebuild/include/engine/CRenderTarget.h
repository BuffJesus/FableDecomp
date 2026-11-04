#pragma once
#include <stddef.h>
#include "fable_base_class.h"
#include "fable_surface_types.h"
// Donor and retail A10E30 agree on both surface subobjects.
class CRenderTarget : public CBaseClass {
public:
    // Ego_r ??0CRenderTarget@@QAE@AAVCSurface@@0@Z, retail 0x00A10DC0.
    CRenderTarget(CSurface& target, CSurface& zbuffer);
    // Ego_r ?DetachTarget@CRenderTarget@@QAEXXZ, retail 0x00A10E10.
    void DetachTarget();
    // Ego_r ?DetachZBuffer@CRenderTarget@@QAEXXZ, retail 0x00A10E20.
    void DetachZBuffer();
    void Uninitialise();
    // Ego_r ?SetZBufferAsScreenZBuffer@CRenderTarget@@QAEXXZ, retail 0x00A10EC0.
    void SetZBufferAsScreenZBuffer();
    void AttachTarget(const CSurface&);
    void AttachZBuffer(const CSurface&);
    CSurface RenderToSurface;
    CSurface ZBufferSurface;
};
FABLE_STATIC_ASSERT(offsetof(CRenderTarget,RenderToSurface)==4);
FABLE_STATIC_ASSERT(offsetof(CRenderTarget,ZBufferSurface)==0x14);
FABLE_STATIC_ASSERT(sizeof(CRenderTarget)==0x24);
