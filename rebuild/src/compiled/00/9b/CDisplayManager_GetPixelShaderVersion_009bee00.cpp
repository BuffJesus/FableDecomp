#include "fable_display_viewport.h"

unsigned long CDisplayManager::GetPixelShaderVersion() const
{
    return (((D3DDeviceCaps.PixelShaderVersion >> 8) & 255) * 10
        + (D3DDeviceCaps.PixelShaderVersion & 255)) * 100;
}
