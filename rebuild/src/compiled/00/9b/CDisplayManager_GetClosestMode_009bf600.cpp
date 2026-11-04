#include "fable_surface.h"
#include "fable_d3d_format_info.h"
#include <stdlib.h>
#include <limits.h>

D3DDISPLAYMODE CDisplayManager::GetClosestMode(const C2DExtentsI& dimensions, long bitDepth, bool exclusive) const
{
    D3DDISPLAYMODE result;
    if (!exclusive)
    {
        if (D3DDevice)
            D3DDevice->GetDisplayMode(0,&result);
        else
            D3D->GetAdapterDisplayMode(D3DDeviceCaps.AdapterOrdinal,&result);
        result.Width=dimensions.X;
        result.Height=dimensions.Y;
        return result;
    }
    long bestDistance=LONG_MAX;
    long bestDepth=LONG_MAX;
    result.Format=D3DFMT_UNKNOWN;
    for (unsigned long format=0;format<BackBufferFormats.size();++format)
    {
        for (unsigned int index=0;index<D3D->GetAdapterModeCount(D3DDeviceCaps.AdapterOrdinal,GD3DFormats[BackBufferFormats[format].FormatIndex].D3DFormat);++index)
        {
            D3DDISPLAYMODE mode;
            D3D->EnumAdapterModes(D3DDeviceCaps.AdapterOrdinal,GD3DFormats[BackBufferFormats[format].FormatIndex].D3DFormat,index,&mode);
            CPixelFormat pixelFormat(mode.Format);
            long depth=abs(static_cast<long>(pixelFormat.GetColourDepth()-bitDepth));
            if (depth<=bestDepth)
            {
                long distance=abs(static_cast<long>(dimensions.Y-mode.Height))+abs(static_cast<long>(dimensions.X-mode.Width));
                if (distance<=bestDistance)
                {
                    result=mode;
                    bestDistance=distance;
                    bestDepth=depth;
                }
            }
        }
    }
    return result;
}
