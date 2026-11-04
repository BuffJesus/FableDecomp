#include "fable_surface_types.h"
bool CTexture::InitialiseFromPreallocatedTexture(IDirect3DTexture9* texture)
{
 if(PD3DTexture){PD3DTexture->Release();PD3DTexture=0;AllocationSource=0;}
 PD3DTexture=texture;AllocationSource=texture!=0;CalcByteLength();return true;
}
void CTexture::CalcByteLength()
{
 ByteLength=0;
 if(PD3DTexture)
 {
  long count=PD3DTexture->GetLevelCount();
  for(long level=0;level<count;++level)
  {
   D3DSURFACE_DESC desc,base;
   PD3DTexture->GetLevelDesc(level,&desc);PD3DTexture->GetLevelDesc(0,&base);
   CPixelFormat format(base.Format);
   unsigned long previous=LengthAndAllocation;
   unsigned long bytes=(format.GetColourDepth()*desc.Width*desc.Height)>>3;
   LengthAndAllocation=previous^((previous^(previous+bytes))&0x0fffffff);
  }
 }
}
