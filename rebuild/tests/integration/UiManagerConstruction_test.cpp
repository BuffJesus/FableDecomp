#include "fable_ui_bank_ownership.h"
#include "fable_ui_manager_singleton.h"
#include "fable_ui_display_formats.h"
#include "fable_ui_bank_factory.h"
#include "fable_ui_bank_runtime.h"
#include "fable_ui_texture_manager.h"
#include "fable_ui_bank_file.h"
#include "fable_ui_bank_stream.h"
#include "fable_ui_bank_registry.h"
#include <stdlib.h>
#include "fable_ui_texture_surfaces.h"
#include <stdio.h>
#include <string.h>
#include "UiBankPathUnavailableServices.h"
FableUiManagerObserverVtable FableUiObservableVtable={},FableUiManagerVtable={};
void* FableUiDisplayManager;
FableUiGraphicModeContext* FableUiGraphicMode;
unsigned char FableUiCoordinateConversionEnabled;
FableUiStateVector2 FableUiCoordinateDestinationExtent,FableUiCoordinateSourceExtent={1280,720};
extern "C" void* FableFrontEndManagerInstance=0;
static FableUiManagerView manager;
FableUiSystemManagerView FableUiSystemManagerState;
static FableUiSystemManagerView& systemManager=FableUiSystemManagerState;
FableUiPixelFormatDescription FableUiPixelFormats[64];
static FableUiDisplayFormatView displays[2];
static FableUiD3D9 devices[2];
static FableUiGraphicModeContext graphicsMode;
__declspec(align(8)) static unsigned char pool[128][64];
static unsigned sizes[128],allocated,bankMode;
static FableReferenceCount bankInfo;
__declspec(align(8)) static unsigned char bankStorage[0x30C];
static unsigned char textureManagerStorage[0x5D4];
static FableReferenceCount textureInfo;
static unsigned referenceAllocations;
void* FableUiBaseVtable=reinterpret_cast<void*>(0x70000100);
void* FableUiResourceBankVtable=reinterpret_cast<void*>(0x70000101);
void* FableUiGraphicResourceBankVtable=reinterpret_cast<void*>(0x70000102);
void* FableUiResourceListVtable=reinterpret_cast<void*>(0x70000103);
void* FableUiResourceVtable=reinterpret_cast<void*>(0x70000104);
void* FableUiGraphicStateVtable=reinterpret_cast<void*>(0x70000105);
void* FableUiSurfaceVtable=reinterpret_cast<void*>(0x70000108);
void* FableUiPreallocPoolVtable=reinterpret_cast<void*>(0x7000010A);
static FableUiGraphicsBankNode bankHead,bankNode;
static FableUiGraphicsBankManagerView bankManager;
unsigned char FableUiGraphicsBankOpenMode;
FableUiProgressView* FableUiProgressDisplay=0;
fable_i32 g_CCharStringInstanceCount_013BD800;
fable_i32 g_CWideStringInstanceCount_013BCA20;
void* FableUiBankFileVtable=reinterpret_cast<void*>(0x70000110);
void* FableUiBankFileAsyncVtable=reinterpret_cast<void*>(0x70000111);
static CCharStringData stringRecords[4];
static char stringBuffers[4][64];
static unsigned stringCount,bufferCount;
void* __cdecl FableUiAllocateStringRecord(unsigned size)
{ printf(" SR%u",size); return &stringRecords[stringCount++]; }
void __cdecl FableUiFreeStringRecord(void* p)
{ printf(" SF%u",static_cast<unsigned>(static_cast<CCharStringData*>(p)-stringRecords)); }
void* __cdecl FableUiAllocateStringBuffer(unsigned size)
{ printf(" SB%u",size); return size<=64 ? stringBuffers[bufferCount++] : 0; }
void __cdecl FableUiFreeStringBuffer(void* p)
{ printf(" ST%u",static_cast<unsigned>(static_cast<char*>(p)-stringBuffers[0])/64); }
static int formatSeed,width,height;
static unsigned Bits(float value) { unsigned out; memcpy(&out,&value,4); return out; }
static void* Allocate(unsigned size)
{ if(allocated>=128 || size>64) return 0; printf(" A%u:%u",allocated,size); sizes[allocated]=size; return pool[allocated++]; }
void* FableUiAllocateObserverNode(unsigned size) { return Allocate(size); }
void* FableUiAllocateIntegerMapNode(unsigned size) { return Allocate(size); }
void* FableUiAllocateManagerStorage(unsigned size) { return Allocate(size); }
extern "C" void* FableFrontEndManagerAllocate(unsigned long size)
{ printf(" J%lu",size); return size==sizeof(manager) ? &manager : 0; }
static long __stdcall CheckFormat(FableUiD3D9* device,unsigned adapter,unsigned type,unsigned mode,unsigned usage,unsigned resource,unsigned format)
{
    unsigned id=static_cast<unsigned>(device-devices);
    printf(" CAP%u:%u:%u:%u:%u:%u:%u",id,adapter,type,mode,usage,resource,format);
    FableUiDisplayManager=&displays[1-id];
    return formatSeed<0 ? -1 : 0;
}
void* __cdecl FableUiAllocateGraphicsBank(unsigned size)
{
    printf(" GA%u",size);
    if(size==0x30C) return bankStorage;
    if(size==0x5D4) return textureManagerStorage;
    FableReferenceCount* info=referenceAllocations++ ? &bankInfo : &textureInfo;
    return bankMode ? info : 0;
}
void* __cdecl FableUiAllocateGraphicsBankNode(unsigned size)
{ if(size!=64) return Allocate(size); printf(" GN%u",size); return &bankNode; }
static void __fastcall BankDelete(FableUiGraphicsBank*,void*,unsigned flags) { printf(" GDELETE%u",flags); }
static void __fastcall BankOpen(FableUiGraphicsBank* bank,void*,const FableUiStringValue* name,unsigned flags)
{ printf(" GOPEN%u:%s:%u",bank==reinterpret_cast<FableUiGraphicsBank*>(bankStorage),FableUiStringText(name),flags); FableUiOpenAsyncBankReadOnly(reinterpret_cast<FableUiBankFileAsyncView*>(bank),0,name,flags); }
FableUiBankRegistryView FableUiBankRegistryState;
void* FableUiThreadedFileVtable=reinterpret_cast<void*>(0x70000201);
void* FableUiDataInputStreamVtable=reinterpret_cast<void*>(0x70000301);
void* FableUiDestroyedBaseVtable=reinterpret_cast<void*>(0x70000302);
FableUiBankStreamVtable FableUiFileInputStreamVtable={0,0,FableUiBankStreamPosition};
static FableUiBankTreeNode registryAliases;
static FableUiRegisteredBankNode registryFiles;
static FableUiBankTreeNode registryPaths;
static FableUiBankPathNode registryPathNodes[2];
static CCharStringData registryPathNames[2];
static bool __fastcall OpenMissingPath(void* p,void*,const FableUiWideStringValue*,unsigned flags)
{ printf(" OPEN_MISSING:%u",flags); static_cast<FableUiBankFileView*>(p)->RetailMode=false; return false; }
bool __fastcall FableUiReadBankEntries(FableUiBankFileView*,void*,CFileDataInputStream*,unsigned,unsigned) { abort(); }
bool __fastcall FableUiOpenThreadedFile(CThreadedFile*,void*,const FableUiWideStringValue*,bool) { abort(); return false; }
FableUiGraphicsBankVtable FableUiGraphicsBankRuntimeVtable={BankDelete,BankOpen};
void __stdcall FableUiInitialiseBankCriticalSection(void* p) { printf(" CS%u",static_cast<unsigned>(static_cast<unsigned char*>(p)-bankStorage)); memset(p,0x6C,24); }
static void __fastcall TextureDelete(FableUiGraphicsBank*,void*,unsigned flags) { printf(" TDELETE%u",flags); }
FableUiGraphicsBankVtable FableUiTextureManagerVtable={TextureDelete,0};
static FableUiNativeTexture nativeTextures[11];
static FableUiNativeSurface nativeSurfaces[11];
static FableUiNativeDevice nativeDevice;
static FableUiSurfaceDescription nativeDescriptions[11];
static unsigned nativeLevels[11],surfaceReferences[11];
static unsigned char nativePixels[11][4112];
static unsigned NativeTextureId(FableUiNativeTexture* p) { return static_cast<unsigned>(p-nativeTextures); }
static unsigned NativeSurfaceId(FableUiNativeSurface* p) { return static_cast<unsigned>(p-nativeSurfaces); }
static unsigned __stdcall NativeAdd(FableUiNativeSurface* p)
{ unsigned i=NativeSurfaceId(p); printf(" SA%u",i); return ++surfaceReferences[i]; }
static unsigned __stdcall NativeRelease(FableUiNativeSurface* p)
{ unsigned i=NativeSurfaceId(p); printf(" SRF%u",i); return --surfaceReferences[i]; }
static long __stdcall NativeDescription(FableUiNativeSurface* p,FableUiSurfaceDescription* out)
{ unsigned i=NativeSurfaceId(p); printf(" SD%u",i); *out=nativeDescriptions[i]; return 0; }
static long __stdcall NativeLock(FableUiNativeSurface* p,FableUiLockRectangle* out,const FableUiRectangle* rect,unsigned flags)
{
    unsigned i=NativeSurfaceId(p); printf(" SL%u:%d:%d:%d:%d:%u",i,rect->Left,rect->Top,rect->Right,rect->Bottom,flags);
    out->Pitch=nativeDescriptions[i].Width*4+16; out->Bits=nativePixels[i]+8; return 0;
}
static long __stdcall NativeUnlock(FableUiNativeSurface* p) { printf(" SU%u",NativeSurfaceId(p)); return 0; }
static unsigned __stdcall NativeLevelCount(FableUiNativeTexture* p)
{ unsigned i=NativeTextureId(p); printf(" TC%u",i); return nativeLevels[i]; }
static long __stdcall NativeLevelDescription(FableUiNativeTexture* p,unsigned level,FableUiSurfaceDescription* out)
{
    unsigned i=NativeTextureId(p); printf(" TD%u:%u",i,level); *out=nativeDescriptions[i];
    out->Width>>=level; out->Height>>=level; if(!out->Width) out->Width=1; if(!out->Height) out->Height=1; return 0;
}
static long __stdcall NativeSurfaceLevel(FableUiNativeTexture* p,unsigned level,IDirect3DSurface9** out)
{
    unsigned i=NativeTextureId(p); printf(" TS%u:%u",i,level); ++surfaceReferences[i];
    *out=reinterpret_cast<IDirect3DSurface9*>(&nativeSurfaces[i]); return 0;
}
static long __stdcall NativeCreate(FableUiNativeDevice*,unsigned w,unsigned h,unsigned levels,unsigned usage,unsigned format,unsigned memoryPool,IDirect3DTexture9** out,void* shared)
{
    unsigned i=static_cast<unsigned>(reinterpret_cast<CTexture*>(out)-reinterpret_cast<FableUiGraphicsBankRuntimeView*>(bankStorage)->BlankTextures);
    printf(" DC%u:%u:%u:%u:%u:%u:%u:%u",i,w,h,levels,usage,format,memoryPool,shared!=0);
    nativeLevels[i]=levels; surfaceReferences[i]=1;
    FableUiSurfaceDescription& d=nativeDescriptions[i]; memset(&d,0,sizeof(d)); d.Width=w; d.Height=h; d.Format=format;
    *out=reinterpret_cast<IDirect3DTexture9*>(&nativeTextures[i]); return 0;
}
void __cdecl FableUiDeleteReference(FableReferenceCount* info) { printf(" FREEBANK:%u",info==&bankInfo); }
static unsigned Normalize(unsigned word)
{
    unsigned stringStart=reinterpret_cast<unsigned>(stringRecords);
    if(word>=stringStart && word<stringStart+sizeof(stringRecords)) return 0x64000000+word-stringStart;
    if(word==reinterpret_cast<unsigned>(&FableUiManagerVtable)) return 0x70000001;
    if(word==reinterpret_cast<unsigned>(&bankInfo)) return 0x1A005678;
    if(word==reinterpret_cast<unsigned>(bankStorage)) return 0x1A001234;
    if(word==reinterpret_cast<unsigned>(&FableUiGraphicsBankRuntimeVtable)) return 0x70000106;
    if(word==reinterpret_cast<unsigned>(&FableUiTextureManagerVtable)) return 0x70000109;
    if(word==reinterpret_cast<unsigned>(&textureInfo)) return 0x63000000;
    if(word==reinterpret_cast<unsigned>(textureManagerStorage)) return 0x62000000;
    if(word>reinterpret_cast<unsigned>(textureManagerStorage) && word<reinterpret_cast<unsigned>(textureManagerStorage)+sizeof(textureManagerStorage)) return 0x62000000+word-reinterpret_cast<unsigned>(textureManagerStorage);
    if(word>reinterpret_cast<unsigned>(bankStorage) && word<reinterpret_cast<unsigned>(bankStorage)+sizeof(bankStorage)) return 0x61000000+word-reinterpret_cast<unsigned>(bankStorage);
    for(unsigned t=0;t<11;++t) if(word==reinterpret_cast<unsigned>(&nativeTextures[t])) return 0x65000000+t*4;
    for(unsigned i=0;i<allocated;++i)
    { unsigned start=reinterpret_cast<unsigned>(pool[i]); if(word>=start && word<start+sizes[i]) return 0x60000000+i*256+word-start; }
    return word;
}
static void Dump(const void* data,unsigned bytes)
{ for(unsigned i=0;i<bytes;i+=4) { unsigned word; memcpy(&word,static_cast<const unsigned char*>(data)+i,4); printf(":%08x",Normalize(word)); } }
int main(int argc,char** argv)
{
    if(argc!=3) return 2; FILE* f=fopen(argv[1],"r"); if(!f) return 3;
    FILE* formats=fopen(argv[2],"r"); if(!formats) return 5;
    for(unsigned i=0;i<64;++i) FableUiPixelFormats[i].Bits=-1;
    unsigned row=0;
    while(row<63)
    {
        FableUiPixelFormatDescription& p=FableUiPixelFormats[row];
        if(fscanf(formats,"%u %d %d %d %d %d %d %d %d",&p.D3DFormat,&p.Type,&p.Bits,&p.Alpha,&p.Red,&p.Green,&p.Blue,&p.Stencil,&p.FloatingPoint)!=9) break;
        ++row;
    }
    fclose(formats);
    FableUiD3D9Vtable vtable={}; vtable.CheckDeviceFormat=CheckFormat;
    FableUiNativeSurfaceVtable sv={}; sv.AddRef=NativeAdd; sv.Release=NativeRelease; sv.GetDescription=NativeDescription; sv.Lock=NativeLock; sv.Unlock=NativeUnlock;
    FableUiNativeTextureVtable tv={}; tv.GetLevelCount=NativeLevelCount; tv.GetLevelDescription=NativeLevelDescription; tv.GetSurfaceLevel=NativeSurfaceLevel;
    FableUiNativeDeviceVtable dv={}; dv.CreateTexture=NativeCreate; nativeDevice.Vtable=&dv;
    for(unsigned n=0;n<11;++n) { nativeTextures[n].Vtable=&tv; nativeSurfaces[n].Vtable=&sv; }
    FableUiGraphicsBankRuntimeVtable.BankMethods08[1]=reinterpret_cast<void*>(OpenMissingPath);
    memset(&registryAliases,0,sizeof(registryAliases));
    registryFiles.Next=registryFiles.Previous=&registryFiles;
    FableUiBankRegistryState.Aliases.Head=&registryAliases; FableUiBankRegistryState.Files=&registryFiles;
    registryPathNames[0].text="GBANK_FRONT_END"; registryPathNames[1].text="GBANK_MAIN";
    for(unsigned pathIndex=0;pathIndex<2;++pathIndex) { registryPathNames[pathIndex].owners=10; registryPathNodes[pathIndex].Node.Key.Storage=registryPathNames+pathIndex; }
    registryPaths.Parent=&registryPathNodes[0].Node.Links;
    registryPathNodes[0].Node.Links.Right=&registryPathNodes[1].Node.Links;
    FableUiBankRegistryState.Paths.Head=&registryPaths;
    unsigned mode,enabled,pattern;
    while(fscanf(f,"%u %u %d %d %d %u %u",&mode,&enabled,&formatSeed,&width,&height,&pattern,&bankMode)==7)
    {
        memset(&manager,pattern,sizeof(manager)); memset(pool,0xCD,sizeof(pool)); allocated=0;
        memset(&bankInfo,0,sizeof(bankInfo)); memset(&textureInfo,0,sizeof(textureInfo)); memset(bankStorage,0xA5,sizeof(bankStorage)); memset(textureManagerStorage,0xA5,sizeof(textureManagerStorage)); referenceAllocations=0; memset(&bankNode,0xA5,sizeof(bankNode));
        bankHead.Next=bankHead.Previous=&bankHead; bankManager.Banks=&bankHead; FableUiGraphicsBankOpenMode=mode==2;
        memset(stringRecords,0xA5,sizeof(stringRecords)); memset(stringBuffers,0xCD,sizeof(stringBuffers)); stringCount=bufferCount=0; g_CCharStringInstanceCount_013BD800=100; g_CWideStringInstanceCount_013BCA20=50;
        memset(nativePixels,0xA7,sizeof(nativePixels)); memset(surfaceReferences,0,sizeof(surfaceReferences));
        FableFrontEndManagerInstance=0; FableUiCoordinateConversionEnabled=static_cast<unsigned char>(enabled);
        FableUiCoordinateDestinationExtent.x=1600; FableUiCoordinateDestinationExtent.y=900;
        graphicsMode.Frontend=mode==2; FableUiGraphicMode=mode ? &graphicsMode : 0;
        for(unsigned d=0;d<2;++d)
        {
            memset(&displays[d],0,sizeof(displays[d])); devices[d].Vtable=&vtable; displays[d].D3D=&devices[d]; displays[d].D3DDevice=&nativeDevice;
            displays[d].AdapterOrdinal=7+d; displays[d].DeviceType=2+d; displays[d].CurrentModeFormat=21+d;
            displays[d].RenderTargetDimensions.Width=width; displays[d].RenderTargetDimensions.Height=height;
        }
        FableUiDisplayManager=&displays[0];
        systemManager.Display=&displays[0]; systemManager.GraphicsBankManager=&bankManager;
        printf("TRACE");
        if(CFrontEndManager_GetInstance_0041e5f2()!=&manager || CFrontEndManager_GetInstance_0041e5f2()!=&manager || FableFrontEndManagerInstance!=&manager) return 4;
        printf(" MODE%u:%08x:%08x M",FableUiCoordinateConversionEnabled,Bits(FableUiCoordinateDestinationExtent.x),Bits(FableUiCoordinateDestinationExtent.y)); Dump(&manager,sizeof(manager));
        for(unsigned i=0;i<allocated;++i) { printf(" H%u",i); Dump(pool[i],sizes[i]); }
        printf(" BANK:%d",bankInfo.owners);
        printf(" STR:%d",g_CCharStringInstanceCount_013BD800);
        for(unsigned s=0;s<stringCount;++s)
        { CCharStringData& r=stringRecords[s]; printf(":%u:%u:%08x:%u:%d",r.text==0,r.unknown04,r.unknown08,r.flags0C,r.owners); }
        printf(" CACHE:%u:%u:%u:%u:%u:%u",bankHead.Next==&bankNode,bankHead.Previous==&bankNode,bankNode.Next==&bankHead,bankNode.Previous==&bankHead,bankNode.Name.Storage==&stringRecords[0],bankNode.Bank.Info==&bankInfo);
        printf(" GDATA"); Dump(bankStorage,sizeof(bankStorage)); printf(" TREF:%d",textureInfo.owners);
        printf(" TDATA"); Dump(textureManagerStorage,sizeof(textureManagerStorage));
        printf(" WIDE:%d GPU",g_CWideStringInstanceCount_013BCA20);
        unsigned hash=2166136261u;
        for(unsigned pixel=0;pixel<sizeof(nativePixels);++pixel) hash=(hash^reinterpret_cast<unsigned char*>(nativePixels)[pixel])*16777619u;
        printf(":%08x",hash); for(unsigned r=0;r<11;++r) printf(":%u",surfaceReferences[r]);
        printf(" END\n");
    }
    fclose(f); return 0;
}
