#include "fable_volume_types.h"
#include "fable_volume_box.h"
#include "fable_d3dx9_texture.h"
static inline void ConvertVolumeBox(const C3DBoxI& source, D3DBOX& box)
{
 box.Top=source.TLY;box.Bottom=source.BRY;
 box.Left=source.TLX;box.Right=source.BRX;
 box.Front=source.TLZ;box.Back=source.BRZ;
}
void CVolume::CopyFromVolume(const C3DBoxI* destinationBox, const CVolume& source,
 const C3DBoxI* sourceBox, unsigned long filter)
{
 D3DBOX destinationRegion,sourceRegion;
 const D3DBOX* destinationPointer=0;
 const D3DBOX* sourcePointer=0;
 if(destinationBox){ConvertVolumeBox(*destinationBox,destinationRegion);destinationPointer=&destinationRegion;}
 if(sourceBox){ConvertVolumeBox(*sourceBox,sourceRegion);sourcePointer=&sourceRegion;}
 D3DXLoadVolumeFromVolume(PD3DVolume,0,destinationPointer,source.PD3DVolume,0,sourcePointer,filter,0);
}
