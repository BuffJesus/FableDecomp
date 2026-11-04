#include "fable_volume_types.h"
#include "fable_d3d_format_info.h"
#include "fable_gfuninitialise.h"
#include "fable_display_viewport.h"
#include "fable_d3dx9_texture.h"
bool CVolumeTexture::CreateTexture(const C3DExtentsI& dimensions, unsigned int levels,
 unsigned long usage, const CPixelFormat& format, EVolumePool pool, bool)
{
 IDirect3DDevice9* device=GFGetSystemManager()->GetDisplayManager()->D3DDevice;
 D3DFORMAT d3dFormat=GD3DFormats[format.FormatIndex].D3DFormat;
 HRESULT status=D3DXCreateVolumeTexture(device,dimensions.X,dimensions.Y,dimensions.Z,levels,
  usage,d3dFormat,(D3DPOOL)pool,&PD3DVolumeTexture);
 if (status<0) return false;
 return true;
}
