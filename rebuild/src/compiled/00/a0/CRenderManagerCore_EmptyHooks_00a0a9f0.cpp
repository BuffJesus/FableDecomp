// lib_render_manager_core.cpp, retail 0x00A0A9F0 / 0x00A0AA00 / 0x00A0AA50 /
// 0x00A0AAC0 (Ego_r CRenderManagerCore::SetDirectionalLight, SetPointLight,
// DiscardTextureManagerBytes, OnPostViewportChanged): empty on PC; only the
// stack cleanup (ret 0x10 / 0x20 / 4 / 0) remains.
#include "fable_render_state_manager.h"

void CRenderManagerCore::SetDirectionalLight(long, const CRGBColour&, const CRGBColour&, const C3DVector&)
{
}

void CRenderManagerCore::SetPointLight(long, const CRGBColour&, const CRGBColour&, const C3DVector&,
    float, float, float, float)
{
}

void CRenderManagerCore::DiscardTextureManagerBytes(unsigned long)
{
}

void CRenderManagerCore::OnPostViewportChanged()
{
}
