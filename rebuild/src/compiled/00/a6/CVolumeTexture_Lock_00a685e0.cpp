#include "fable_volume_types.h"
#include "fable_gfuninitialise.h"
#include "fable_display_viewport.h"
CVolumeLockedBox CVolumeTexture::Lock(unsigned long level, unsigned long flags)
{
 D3DVOLUME_DESC description;
 PD3DVolumeTexture->GetLevelDesc(level,&description);
 long width=description.Width,height=description.Height;
 D3DBOX box={0,0,width,height,0,description.Depth};
 CSystemManager* system=GFGetSystemManager();
 system->GetDisplayManager()->GetRenderManager().WaitWhileResourceIsBusy(PD3DVolumeTexture,1.0f);
 D3DLOCKED_BOX locked;
 if(PD3DVolumeTexture->LockBox(level,&locked,&box,flags)<0) return CVolumeLockedBox();
 CVolumeLockedBox result;
 result.Dimensions.X=width;result.Dimensions.Y=height;result.Dimensions.Z=description.Depth;
 result.RowPitch=locked.RowPitch;result.SlicePitch=locked.SlicePitch;result.PMem=(unsigned char*)locked.pBits;
 return result;
}
