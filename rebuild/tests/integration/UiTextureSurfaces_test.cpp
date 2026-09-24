#include "fable_ui_texture_surfaces.h"
#include <stdio.h>
#include <string.h>
void* FableUiSurfaceVtable=reinterpret_cast<void*>(0x70000108);
FableUiPixelFormatDescription FableUiPixelFormats[64];
FableUiSystemManagerView FableUiSystemManagerState;
static FableUiDisplayFormatView display;
static FableUiNativeSurface surfaces[2];
static FableUiNativeTexture textures[2];
static FableUiNativeDevice device;
static CSurface surface,copy;
static CTexture texture;
static FableUiGraphicsBankRuntimeView bank;
static unsigned char pixels[8192];
static unsigned mode,seed,width,height,pool,mutation,nullInput,source,descriptions,levelDescriptions;
static int status,levels,format;
static unsigned SurfaceId(void* p) { return p ? (static_cast<FableUiNativeSurface*>(p)-surfaces)+1 : 0; }
static unsigned TextureId(void* p) { return p ? (static_cast<FableUiNativeTexture*>(p)-textures)+1 : 0; }
static unsigned __stdcall SurfaceAdd(FableUiNativeSurface* p)
{ printf(" SA%u",SurfaceId(p)); return 9; }
static unsigned __stdcall SurfaceRelease(FableUiNativeSurface* p)
{
    printf(" SR%u",SurfaceId(p));
    if(mutation==2) { surface.PD3DSurface=reinterpret_cast<IDirect3DSurface9*>(&surfaces[1]); surface.AllocationSource=0x98765432; surface.PAllocatedMemory=reinterpret_cast<void*>(0x76543210); }
    return 8;
}
static long __stdcall Description(FableUiNativeSurface* p,FableUiSurfaceDescription* out)
{
    unsigned n=descriptions++;
    memset(out,0x63,sizeof(*out)); out->Width=width+n%3; out->Height=height+n%2;
    out->Format=FableUiPixelFormats[(seed+n)%4].D3DFormat;
    printf(" SD%u:%u:%u:%u:%u",SurfaceId(p),n,out->Width,out->Height,out->Format);
    if(mutation==1) surface.PD3DSurface=reinterpret_cast<IDirect3DSurface9*>(&surfaces[1]);
    return status;
}
static long __stdcall Lock(FableUiNativeSurface* p,FableUiLockRectangle* out,const FableUiRectangle* rect,unsigned flags)
{
    printf(" SL%u:%d:%d:%d:%d:%u",SurfaceId(p),rect->Left,rect->Top,rect->Right,rect->Bottom,flags);
    out->Pitch=static_cast<int>(width*7+seed); out->Bits=pixels+16;
    return mode==1 ? status : 0;
}
static long __stdcall Unlock(FableUiNativeSurface* p) { printf(" SU%u",SurfaceId(p)); return status; }
static unsigned __stdcall TextureRelease(FableUiNativeTexture* p)
{
    printf(" TR%u",TextureId(p));
    if(mutation==2) { FableUiSetTextureState(&texture,0xCAFEBABE); texture.PD3DTexture=reinterpret_cast<IDirect3DTexture9*>(&textures[1]); }
    return 3;
}
static unsigned __stdcall LevelCount(FableUiNativeTexture* p)
{
    unsigned n=seed%5; if(n==4) n=0xFFFFFFFF;
    printf(" TC%u:%u",TextureId(p),n); return n;
}
static long __stdcall LevelDescription(FableUiNativeTexture* p,unsigned level,FableUiSurfaceDescription* out)
{
    unsigned n=levelDescriptions++;
    memset(out,0x47,sizeof(*out)); unsigned shift=level<5 ? level : 5;
    out->Width=(width>>shift)+1; out->Height=(height>>shift)+1;
    out->Format=FableUiPixelFormats[(seed+n)%4].D3DFormat;
    printf(" TD%u:%u:%u:%u:%u",TextureId(p),level,out->Width,out->Height,out->Format);
    if(mutation==1) texture.PD3DTexture=reinterpret_cast<IDirect3DTexture9*>(&textures[1]);
    return status;
}
static long __stdcall SurfaceLevel(FableUiNativeTexture* p,unsigned level,IDirect3DSurface9** out)
{
    printf(" TS%u:%u",TextureId(p),level);
    *out=mode==0 && nullInput ? 0 : reinterpret_cast<IDirect3DSurface9*>(&surfaces[seed%2]);
    return status;
}
static long __stdcall Create(FableUiNativeDevice*,unsigned w,unsigned h,unsigned count,unsigned usage,unsigned fmt,unsigned memoryPool,IDirect3DTexture9** out,void* shared)
{
    printf(" DC%u:%u:%u:%u:%u:%u:%u",w,h,count,usage,fmt,memoryPool,shared!=0);
    *out=mode==3 && status<0 && nullInput ? 0 : reinterpret_cast<IDirect3DTexture9*>(&textures[1]);
    return mode==4 ? 0 : status;
}
static void DumpSurface(CSurface* s)
{ printf(" S%08x:%u:%08x:%08x",reinterpret_cast<unsigned>(s->__vftable),SurfaceId(s->PD3DSurface),s->AllocationSource,reinterpret_cast<unsigned>(s->PAllocatedMemory)); }
static void DumpTexture(CTexture* t)
{ printf(" T%u:%08x",TextureId(t->PD3DTexture),FableUiTextureState(t)); }
static void DumpPixels()
{ unsigned hash=2166136261u; for(unsigned i=0;i<sizeof(pixels);++i) hash=(hash^pixels[i])*16777619u; printf(" P%08x",hash); }
int main(int argc,char** argv)
{
    if(argc!=2) return 2; FILE* file=fopen(argv[1],"r"); if(!file) return 3;
    FableUiNativeSurfaceVtable sv={}; sv.AddRef=SurfaceAdd; sv.Release=SurfaceRelease; sv.GetDescription=Description; sv.Lock=Lock; sv.Unlock=Unlock;
    FableUiNativeTextureVtable tv={}; tv.Release=TextureRelease; tv.GetLevelCount=LevelCount; tv.GetLevelDescription=LevelDescription; tv.GetSurfaceLevel=SurfaceLevel;
    FableUiNativeDeviceVtable dv={}; dv.CreateTexture=Create; device.Vtable=&dv;
    for(unsigned i=0;i<2;++i) { surfaces[i].Vtable=&sv; textures[i].Vtable=&tv; }
    display.D3DDevice=&device; FableUiSystemManagerState.Display=&display;
    unsigned formats[4]={21,22,0x31545844,34}; int bits[4]={32,24,4,16};
    for(i=0;i<4;++i) { FableUiPixelFormats[i].D3DFormat=formats[i]; FableUiPixelFormats[i].Bits=bits[i]; }
    FableUiPixelFormats[4].Bits=-1;
    while(fscanf(file,"%u %u %d %u %u %d %d %u %u %u %u",&mode,&seed,&status,&width,&height,&levels,&format,&pool,&mutation,&nullInput,&source)==11)
    {
        descriptions=levelDescriptions=0; memset(pixels,seed&255,sizeof(pixels)); memset(&surface,0xA5,sizeof(surface)); memset(&copy,0x5A,sizeof(copy));
        surface.PD3DSurface=nullInput ? 0 : reinterpret_cast<IDirect3DSurface9*>(&surfaces[0]); surface.AllocationSource=source;
        texture.PD3DTexture=nullInput ? 0 : reinterpret_cast<IDirect3DTexture9*>(&textures[0]); FableUiSetTextureState(&texture,0xD7654321);
        printf("TRACE");
        if(mode==0)
        {
            FableUiCopySurface(&copy,0,&surface); DumpSurface(&copy);
            FableUiCopySurface(&surface,0,&surface); DumpSurface(&surface);
            FableUiReleaseSurface(&surface,0); DumpSurface(&surface); FableUiReleaseSurface(&surface,0); DumpSurface(&surface);
            surface.PD3DSurface=reinterpret_cast<IDirect3DSurface9*>(&surfaces[0]);
            FableUiAttachSurface(&surface,0,nullInput ? 0 : reinterpret_cast<IDirect3DSurface9*>(&surfaces[1])); DumpSurface(&surface);
            texture.PD3DTexture=reinterpret_cast<IDirect3DTexture9*>(&textures[0]);
            if(FableUiGetTextureSurface(&texture,0,&copy,seed)!=&copy) return 4; DumpSurface(&copy);
            FableUiReleaseSurface(&copy,0); DumpSurface(&copy);
        }
        else if(mode==1)
        {
            surface.PD3DSurface=reinterpret_cast<IDirect3DSurface9*>(&surfaces[0]);
            FableUiSurfaceLock lock; memset(&lock,0xA5,sizeof(lock));
            if(FableUiLockSurface(&surface,0,&lock,pool)!=&lock) return 5;
            printf(" L%u:%u:%d:%u",lock.Width,lock.Height,lock.Pitch,lock.Bits ? 16 : 0); DumpSurface(&surface);
        }
        else if(mode==2)
        {
            surface.PD3DSurface=reinterpret_cast<IDirect3DSurface9*>(&surfaces[0]);
            FableUiClearSurface(&surface,0); DumpSurface(&surface); DumpPixels();
        }
        else if(mode==3)
        {
            FableUiUpdateTextureByteLength(&texture,0); DumpTexture(&texture);
            FableUiDisplayExtent dimensions={static_cast<int>(width),static_cast<int>(height)};
            bool result=FableUiCreateBlankTexture(&texture,0,&dimensions,levels,&format,seed,pool,seed&1,source);
            printf(" R%u",result); DumpTexture(&texture); FableUiReleaseTexture(&texture,0); DumpTexture(&texture);
        }
        else if(mode==4)
        {
            memset(&bank,0xA5,sizeof(bank)); memset(bank.BlankTextures,0,sizeof(bank.BlankTextures));
            FableUiGraphicBankInit init; memset(&init,0,sizeof(init));
            for(i=0;i<7;++i) reinterpret_cast<int*>(&init)[i]=(seed&(1u<<i)) ? i%4 : -1;
            FableUiInitialiseGraphicsBank(reinterpret_cast<FableUiGraphicsBank*>(&bank),0,&init);
            for(i=0;i<11;++i) DumpTexture(&bank.BlankTextures[i]);
            printf(" B%u:%u:%u:%u:%u:%u:%u:%u:%u",bank.Initialised,bank.UnloadFrameDelay,bank.ReduceSizeFrameDelay,bank.TexturePointersValidForFrames,bank.MinAvailablePreloadMemory,bank.AvailableMemory,bank.FreezeGraphics,bank.TextureSizeWarningMaxWidth,bank.TextureSizeWarningMaxHeight);
            DumpPixels();
        }
        printf(" END\n");
    }
    fclose(file); return 0;
}
