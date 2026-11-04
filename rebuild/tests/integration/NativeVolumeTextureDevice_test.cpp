#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "fable_volume_types.h"
#include "fable_d3d_format_info.h"
#include "fable_display_viewport.h"
#include "fable_gfuninitialise.h"
#include "fable_memory_allocator.h"
#include "fable_d3dx9_texture.h"
#include "fable_volume_box.h"
#include "fable_managed_texture_data.h"
#include "volume_texture_device_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall LoadLibraryA(const char*);
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void*,const char*);
extern "C" __declspec(dllimport) int __stdcall FreeLibrary(void*);
extern "C" __declspec(dllimport) void* __stdcall CreateWindowExA(unsigned,const char*,const char*,unsigned,int,int,int,int,void*,void*,void*,void*);
extern "C" __declspec(dllimport) int __stdcall DestroyWindow(void*);
extern "C" void* __fastcall TextureLock(void*,void*,void*,unsigned long,unsigned long);
extern "C" void* __fastcall TextureDefaultCtor(void*,void*);
extern "C" bool __fastcall TextureCreate(void*,void*,const C3DExtentsI*,unsigned int,unsigned long,const CPixelFormat*,EVolumePool,bool);
extern "C" bool __fastcall TexturePreallocated(void*,void*,IDirect3DVolumeTexture9*);
extern "C" void* __fastcall TextureScalar(void*,void*,unsigned);
extern "C" unsigned TextureVTable[];
extern "C" void __fastcall TextureDeleteData(void*);
extern "C" void __fastcall VolumeTextureDeleteData(void*);
extern "C" void* __fastcall ManagedDataScalar(void*,void*,unsigned);
extern "C" unsigned ManagedDataVTable[];
extern "C" void AbortBoundary(){abort();}
static unsigned systemStorage[sizeof(CSystemManager)/4+1],displayStorage[sizeof(CDisplayManager)/4+1],renderStorage[sizeof(CRenderManagerCore)/4+1];
static unsigned systemCalls,creationCalls,copyCalls;
typedef HRESULT (__stdcall* CreateVolumeFunction)(IDirect3DDevice9*,unsigned int,unsigned int,unsigned int,unsigned int,unsigned long,D3DFORMAT,D3DPOOL,IDirect3DVolumeTexture9**);
static CreateVolumeFunction actualCreateVolume;
typedef HRESULT (__stdcall* LoadVolumeFunction)(IDirect3DVolume9*,const PALETTEENTRY*,const D3DBOX*,IDirect3DVolume9*,const PALETTEENTRY*,const D3DBOX*,unsigned long,D3DCOLOR);
static LoadVolumeFunction actualLoadVolume;
CSystemManager* FABLE_FASTCALL GFGetSystemManager(){++systemCalls;return (CSystemManager*)systemStorage;}
extern "C" HRESULT __stdcall D3DXCreateVolumeTexture(IDirect3DDevice9* device,unsigned int width,unsigned int height,unsigned int depth,unsigned int levels,unsigned long usage,D3DFORMAT format,D3DPOOL pool,IDirect3DVolumeTexture9** output){++creationCalls;return actualCreateVolume(device,width,height,depth,levels,usage,format,pool,output);}
extern "C" HRESULT __stdcall D3DXLoadVolumeFromVolume(IDirect3DVolume9* dst,const PALETTEENTRY* dp,const D3DBOX* db,IDirect3DVolume9* src,const PALETTEENTRY* sp,const D3DBOX* sb,unsigned long filter,D3DCOLOR key){++copyCalls;return actualLoadVolume(dst,dp,db,src,sp,sb,filter,key);}
static void require(bool value){if(!value)abort();}
static void* slot(void* object,unsigned index){return (*(void***)object)[index];}
static void release(void* object){if(object)((unsigned(__stdcall*)(void*))slot(object,2))(object);}
static int failure(const char* operation,long status){printf("VOLUME_DEVICE_FAIL %s %08lx\n",operation,status);return 1;}
#define CHECK(operation,expression) do{long status=(expression);if(status<0)return failure(operation,status);}while(0)
struct Present{unsigned width,height,format,count,multi,quality,swap;void* window;int windowed,depth;unsigned depthFormat,flags,refresh,interval;};
static void checkManaged(CManagedTextureData* managed,unsigned allocation,unsigned pool){
 require(managed->AllocSize==allocation&&managed->PoolIndex==pool&&managed->Texture.Data&&managed->Texture.Info);
 require(!managed->Texture.Data->PD3DTexture&&!managed->Texture.Data->LengthAndAllocation);
 require(managed->Texture.Info->RefCount==1&&managed->Texture.Info->Data==managed->Texture.Data&&managed->Texture.Info->DeleteFunc==TextureDeleteData);
 require(!managed->IsBusy()&&!managed->BusyFlag);
}
static void checkManagedVolume(CManagedTextureData* managed){
 require(managed->VolumeTexture.Data&&managed->VolumeTexture.Info);
 const unsigned* liveVolume=(const unsigned*)managed->VolumeTexture.Data;
 require(!liveVolume[1]&&!liveVolume[3]);
 require(managed->VolumeTexture.Info->RefCount==1&&managed->VolumeTexture.Info->Data==managed->VolumeTexture.Data&&managed->VolumeTexture.Info->DeleteFunc==VolumeTextureDeleteData);
 require(*(unsigned*)managed->VolumeTexture.Data==(unsigned)TextureVTable);
}
static void managedHeapSmoke(){
 for(unsigned cycle=0;cycle<256;++cycle){
  unsigned storageA[19],storageB[19];memset(storageA,0xa9,sizeof(storageA));memset(storageB,0xa9,sizeof(storageB));
  CManagedTextureData* a=new(storageA) CManagedTextureData;CManagedTextureData* b=new(storageB) CManagedTextureData;
  require(*(unsigned*)a==(unsigned)ManagedDataVTable&&*(unsigned*)b==(unsigned)ManagedDataVTable);
  require(a->IVRefCount==1&&b->IVRefCount==1&&!a->ResourceList&&!b->ResourceList);
  require(!a->Texture.Data&&!a->Texture.Info&&!b->Texture.Data&&!b->Texture.Info);
  require(!a->VolumeTexture.Data&&!a->VolumeTexture.Info&&!b->VolumeTexture.Data&&!b->VolumeTexture.Info);
  require(a->_pad_0x4b[0]==0xa9&&b->_pad_0x4b[0]==0xa9);
  a->Initialise(300+cycle,cycle&15);checkManaged(a,300+cycle,cycle&15);
  // Explicitly form a second original shared handle, then exercise Reset's
  // count-two and count-one paths using real CRT allocations and deletion.
  b->Texture.Data=a->Texture.Data;b->Texture.Info=a->Texture.Info;++a->Texture.Info->RefCount;
  b->Initialise(400+cycle,(cycle+1)&15);checkManaged(b,400+cycle,(cycle+1)&15);
  require(a->Texture.Info->RefCount==1&&a->Texture.Data!=b->Texture.Data);checkManaged(a,300+cycle,cycle&15);
  a->Initialise(500+cycle,(cycle+2)&15);checkManaged(a,500+cycle,(cycle+2)&15);
  a->VolumeTexture.Reset(new CVolumeTexture);checkManagedVolume(a);
  b->VolumeTexture.Data=a->VolumeTexture.Data;b->VolumeTexture.Info=a->VolumeTexture.Info;++a->VolumeTexture.Info->RefCount;
  b->VolumeTexture.Reset(new CVolumeTexture);checkManagedVolume(a);checkManagedVolume(b);
  require(a->VolumeTexture.Data!=b->VolumeTexture.Data);
  a->VolumeTexture.Reset(new CVolumeTexture);checkManagedVolume(a);checkManagedVolume(b);
  if(cycle&1){a->Texture.Reset(0);b->Texture.Reset(0);a->VolumeTexture.Reset(0);b->VolumeTexture.Reset(0);require(!a->Texture.Data&&!a->Texture.Info&&!b->Texture.Data&&!b->Texture.Info&&!a->VolumeTexture.Data&&!a->VolumeTexture.Info&&!b->VolumeTexture.Data&&!b->VolumeTexture.Info);}
  a->~CManagedTextureData();b->~CManagedTextureData();
 }
}
int main(int argc,char** argv){
 if(argc!=2)return 2;
 require(!CVolume::TotalMemory&&!CVolumeTexture::TotalMemory&&!g_FableAllocatorMemoryBaseline_013CAAB0&&!g_FableMemoryAllocator_013CAA90);
 require(TextureVTable[0]==(unsigned)&TextureScalar);
 require(ManagedDataVTable[0]==(unsigned)&ManagedDataScalar);
 managedHeapSmoke();
 void* d3dLibrary=LoadLibraryA("d3d9.dll");void* d3dxLibrary=LoadLibraryA("d3dx9_25.dll");require(d3dLibrary&&d3dxLibrary);
 typedef void* (__stdcall* CreateD3D)(unsigned);CreateD3D create=(CreateD3D)GetProcAddress(d3dLibrary,"Direct3DCreate9");actualCreateVolume=(CreateVolumeFunction)GetProcAddress(d3dxLibrary,"D3DXCreateVolumeTexture");require(create&&actualCreateVolume);
 void* api=create(32);require(api!=0);void* window=CreateWindowExA(0,"STATIC","Native volume texture test",0,0,0,128,128,0,0,0,0);require(window!=0);
 actualLoadVolume=(LoadVolumeFunction)GetProcAddress(d3dxLibrary,"D3DXLoadVolumeFromVolume");require(actualLoadVolume!=0);
 Present presentation={128,128,22,1,0,0,1,window,1,0,0,0,0,0x80000000};void* device=0;
 CHECK("CreateDevice",((long(__stdcall*)(void*,unsigned,unsigned,void*,unsigned,Present*,void**))slot(api,16))(api,0,1,window,0x20,&presentation,&device));
 *(CDisplayManager**)((unsigned char*)systemStorage+0x60)=(CDisplayManager*)displayStorage;
 *(CRenderManagerCore**)((unsigned char*)displayStorage+8)=(CRenderManagerCore*)renderStorage;
 ((CDisplayManager*)displayStorage)->D3DDevice=(IDirect3DDevice9*)device;
 unsigned ownerA[4]={0,0,0xdeadbeef,0},ownerB[4]={0,0,0xffffffff,0};
 require(TextureDefaultCtor(ownerA,0)==ownerA&&TextureDefaultCtor(ownerB,0)==ownerB);
 require(!ownerA[1]&&!ownerA[3]&&ownerA[2]==0xdeadbeef&&!ownerB[1]&&!ownerB[3]&&ownerB[2]==0xffffffff);
 C3DExtentsI dimensions;dimensions.X=8;dimensions.Y=4;dimensions.Z=2;CPixelFormat format((D3DFORMAT)21);
 require(TextureCreate(ownerA,0,&dimensions,0,0,&format,VOLUME_POOL_MANAGED,false)&&creationCalls==1&&ownerA[1]);
 IDirect3DVolumeTexture9* resource=(IDirect3DVolumeTexture9*)ownerA[1];ownerA[1]=0;
 require(TexturePreallocated(ownerB,0,resource));CVolumeTexture* texture=(CVolumeTexture*)ownerB;
 require(texture->GetNoLevels()==4&&ownerB[2]==300&&texture->Sizeof()==304);
 require(CVolume::TotalMemory==348&&CVolumeTexture::TotalMemory==304&&g_FableAllocatorMemoryBaseline_013CAAB0==652);
 unsigned total=0;
 for(unsigned level=0;level<4;++level){C3DExtentsI actual=texture->GetDimensions(level);unsigned width=8>>level,height=4>>level,depth=2>>level;if(!height)height=1;if(!depth)depth=1;require(actual.X==(long)width&&actual.Y==(long)height&&actual.Z==(long)depth);unsigned bytes=texture->GetLevelByteLength(level);require(bytes==width*height*depth*4);total+=bytes;}
 CPixelFormat actualFormat=texture->GetPixelFormat();require(GD3DFormats[actualFormat.FormatIndex].D3DFormat==(D3DFORMAT)21&&actualFormat.GetByteColourDepth()==4&&total==300);
 texture->SetPriority(7);require(texture->GetPriority()==7);texture->PreLoad();require(!texture->IsBusy()&&!texture->Compress());texture->GenerateMipmaps(0,3);
 CVolumeLockedBox locked;memset(&locked,0,sizeof(locked));require(TextureLock(texture,0,&locked,0,0)==&locked&&locked.Dimensions.X==8&&locked.Dimensions.Y==4&&locked.Dimensions.Z==2&&locked.RowPitch>=32&&locked.SlicePitch>=128&&locked.PMem);
 unsigned expected[64],readback[64];
 for(unsigned z=0;z<2;++z)for(unsigned y=0;y<4;++y)for(unsigned x=0;x<8;++x){unsigned voxel=0xff000000|(z<<16)|(y<<8)|x;expected[(z*4+y)*8+x]=voxel;*(unsigned*)((unsigned char*)locked.PMem+z*locked.SlicePitch+y*locked.RowPitch+x*4)=voxel;}
 texture->Unlock(0);memset(&locked,0,sizeof(locked));require(TextureLock(texture,0,&locked,0,0x10)==&locked&&locked.Dimensions.X==8&&locked.Dimensions.Y==4&&locked.Dimensions.Z==2&&locked.RowPitch>=32&&locked.SlicePitch>=128&&locked.PMem);
 for(unsigned z=0;z<2;++z)for(unsigned y=0;y<4;++y)memcpy(readback+(z*4+y)*8,(unsigned char*)locked.PMem+z*locked.SlicePitch+y*locked.RowPitch,32);
 require(!memcmp(expected,readback,sizeof(readback)));texture->Unlock(0);
 require(TextureCreate(ownerA,0,&dimensions,0,0,&format,VOLUME_POOL_MANAGED,false)&&creationCalls==2&&ownerA[1]);
 resource=(IDirect3DVolumeTexture9*)ownerA[1];ownerA[1]=0;require(TexturePreallocated(ownerA,0,resource));
 CVolumeTexture* destination=(CVolumeTexture*)ownerA;destination->CopyFromVolumeTexture(*texture,1);require(copyCalls==4);
 memset(&locked,0,sizeof(locked));require(TextureLock(destination,0,&locked,0,0x10)==&locked&&locked.Dimensions.X==8&&locked.Dimensions.Y==4&&locked.Dimensions.Z==2&&locked.RowPitch>=32&&locked.SlicePitch>=128&&locked.PMem);
 for(unsigned z=0;z<2;++z)for(unsigned y=0;y<4;++y)memcpy(readback+(z*4+y)*8,(unsigned char*)locked.PMem+z*locked.SlicePitch+y*locked.RowPitch,32);
 require(!memcmp(expected,readback,sizeof(readback)));destination->Unlock(0);
 {
  CVolume sourceLevel=texture->GetVolumeLevel(0);
  CVolume destinationLevel=destination->GetVolumeLevel(0);
  C3DBoxI sourceRegion,destinationRegion;
  sourceRegion.TLX=0;sourceRegion.TLY=0;sourceRegion.BRX=4;sourceRegion.BRY=2;sourceRegion.TLZ=1;sourceRegion.BRZ=2;
  destinationRegion.TLX=2;destinationRegion.TLY=1;destinationRegion.BRX=6;destinationRegion.BRY=3;destinationRegion.TLZ=0;destinationRegion.BRZ=1;
  destinationLevel.CopyFromVolume(&destinationRegion,sourceLevel,&sourceRegion,1);
 }
 require(copyCalls==5);
 for(unsigned y=1;y<3;++y)for(unsigned x=2;x<6;++x)expected[y*8+x]=0xff010000|((y-1)<<8)|(x-2);
 memset(&locked,0,sizeof(locked));require(TextureLock(destination,0,&locked,0,0x10)==&locked&&locked.Dimensions.X==8&&locked.Dimensions.Y==4&&locked.Dimensions.Z==2&&locked.RowPitch>=32&&locked.SlicePitch>=128&&locked.PMem);
 for(unsigned z=0;z<2;++z)for(unsigned y=0;y<4;++y)memcpy(readback+(z*4+y)*8,(unsigned char*)locked.PMem+z*locked.SlicePitch+y*locked.RowPitch,32);
 require(!memcmp(expected,readback,sizeof(readback)));destination->Unlock(0);
 FILE* output=fopen(argv[1],"wb");require(output&&fwrite(readback,1,sizeof(readback),output)==sizeof(readback));fclose(output);
 texture->Uninitialise();require(!ownerB[1]&&!ownerB[2]&&!ownerB[3]);((CVolumeTexture*)ownerA)->Uninitialise();
 release(device);release(api);DestroyWindow(window);FreeLibrary(d3dxLibrary);FreeLibrary(d3dLibrary);
 printf("NATIVE_VOLUME_TEXTURE_DEVICE_PASS dimensions=8x4x2 levels=4 texture_bytes=300 voxels=64 readback_bytes=256 cropped_voxels=8 managed_heap_cycles=256 system_calls=%u creation_calls=%u copy_calls=%u\n",systemCalls,creationCalls,copyCalls);return 0;
}
