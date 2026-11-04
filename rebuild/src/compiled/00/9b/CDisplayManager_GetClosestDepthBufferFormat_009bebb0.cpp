#include "fable_surface.h"
#include "fable_d3d_format_info.h"
#include <stdlib.h>
#include <limits.h>

CPixelFormat CDisplayManager::GetClosestDepthBufferFormat(const D3DDISPLAYMODE& mode, long bitDepth) const
{
    long bestFormat=-1;
    long bestDistance=LONG_MAX;
    for (long index=0;GD3DFormats[index].BitDepth!=-1;++index)
    {
        const CD3DFormatInfo& format=GD3DFormats[index];
        if (format.PFType==PF_TYPE_DEPTH && format.StencilBits>=8 &&
            D3D->CheckDeviceFormat(D3DDeviceCaps.AdapterOrdinal,D3DDeviceCaps.DeviceType,mode.Format,2,D3DRTYPE_SURFACE,format.D3DFormat)>=0 &&
            D3D->CheckDepthStencilMatch(D3DDeviceCaps.AdapterOrdinal,D3DDeviceCaps.DeviceType,mode.Format,mode.Format,format.D3DFormat)>=0)
        {
            long distance=abs(bitDepth-GD3DFormats[index].BitDepth);
            if (distance<bestDistance)
            {
                bestFormat=index;
                bestDistance=distance;
            }
            else if (distance==bestDistance && bitDepth<GD3DFormats[index].BitDepth)
            {
                bestFormat=index;
                bestDistance=distance;
            }
        }
    }
    return CPixelFormat(GD3DFormats[bestFormat].D3DFormat);
}
