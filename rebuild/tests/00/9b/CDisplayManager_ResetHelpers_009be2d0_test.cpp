#include "fable_surface.h"
#include <stdio.h>
#include <string.h>
static unsigned long storage[0x2c8/4];
static unsigned char expected[0x2c8];
static unsigned errors,formatCalls,textureCalls,mutation;
static bool textureResult;
static long formatIndex;
static CDisplayManager* Object(){return reinterpret_cast<CDisplayManager*>(storage);}
struct CDisplayManagerTestAccess {
 static unsigned Interval(const CDisplayManager::CPresentationIntervalDesc& desc){return Object()->CalculateD3DPresentationInterval(desc);}
 static bool Texture(){return Object()->SetupDepthAndBackBufferTextures();}
};
CPixelFormat CSurface::GetPixelFormat() const {
 if(this!=&Object()->RenderTargetSurface)++errors;++formatCalls;
 if(mutation&1)Object()->D3DDeviceCaps.TextureCaps^=2;
 if(mutation&2){Object()->RenderTargetDimensionsStorage_[0]=73;Object()->RenderTargetDimensionsStorage_[1]=-19;}
 memcpy(expected,Object(),sizeof(storage));CPixelFormat result;result.FormatIndex=formatIndex;return result;
}
bool CTexture::Initialise(const C2DExtentsI& dims,long levels,const CPixelFormat& format,unsigned long flags,ESurfacePool pool,bool dynamic,bool automaticMipmaps){
 if(this!=&Object()->BackBufferCopyTexture || formatCalls!=1 || levels!=1 || format.FormatIndex!=formatIndex || flags!=1 || pool!=SURFACE_POOL_DEFAULT_LOCATION || dynamic || automaticMipmaps)++errors;
 bool power=(Object()->D3DDeviceCaps.TextureCaps&2)!=0;
 if(dims.X!=(power?1024:Object()->RenderTargetDimensionsStorage_[0]) || dims.Y!=(power?1024:Object()->RenderTargetDimensionsStorage_[1]))++errors;
 ++textureCalls;return textureResult;
}
int main(){unsigned intervals=0,textures=0;
 memset(storage,0x73,sizeof(storage));memcpy(expected,storage,sizeof(storage));
 for(long index=-256;index<258;++index)for(unsigned immediate=0;immediate<2;++immediate)for(unsigned missed=0;missed<2;++missed){
  CDisplayManager::CPresentationIntervalDesc desc;memset(&desc,0xc9,sizeof(desc));desc.VSyncCount=index==256?(-2147483647L-1):index==257?2147483647L:index;desc.PresentImmediate=immediate!=0;desc.PresentImmediateIfSyncMissed=missed!=0;
  unsigned result=0x80000000;if(!immediate){switch(desc.VSyncCount){case 0:case 1:result=1;break;case 2:result=2;break;case 3:result=4;break;case 4:result=8;break;}}
  if(CDisplayManagerTestAccess::Interval(desc)!=result || memcmp(expected,storage,sizeof(storage)))++errors;++intervals;
 }
 const unsigned caps[]={0,1,2,3,0x80000000,0x80000002,0xfffffffd,0xffffffff};const long dimensions[]={-1,0,640,1920};
 for(unsigned c=0;c<8;++c)for(unsigned x=0;x<4;++x)for(unsigned y=0;y<4;++y)for(unsigned result=0;result<2;++result)for(mutation=0;mutation<4;++mutation){
  memset(storage,0x9d,sizeof(storage));Object()->D3DDeviceCaps.TextureCaps=caps[c];Object()->RenderTargetDimensionsStorage_[0]=dimensions[x];Object()->RenderTargetDimensionsStorage_[1]=dimensions[y];formatIndex=x*11+y-7;formatCalls=textureCalls=0;textureResult=result!=0;
  if(CDisplayManagerTestAccess::Texture()!=textureResult || formatCalls!=1 || textureCalls!=1 || memcmp(expected,storage,sizeof(storage)))++errors;++textures;
 }
 if(errors){printf("DISPLAY_RESET_HELPERS_FAIL errors=%u\n",errors);return 1;}printf("DISPLAY_RESET_HELPERS_BEHAVIOR_PASS intervals=%u textures=%u\n",intervals,textures);return 0;
}
