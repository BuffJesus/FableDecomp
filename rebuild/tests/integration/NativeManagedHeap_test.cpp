#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "fable_volume_types.h"
#include "fable_surface_types.h"
#include "fable_managed_texture_data.h"
#include "engine/CPreallocTexturePool.h"
#include "managed_heap_aliases.h"
extern "C" void __fastcall TextureDeleteData(void*);
extern "C" void __fastcall VolumeTextureDeleteData(void*);
extern "C" void* __fastcall TextureScalar(void*,void*,unsigned);
extern "C" unsigned TextureVTable[];
extern "C" void* __fastcall ManagedDataScalar(void*,void*,unsigned);
extern "C" unsigned ManagedDataVTable[];
extern "C" void __fastcall ManagedVirtualDelete(void*,void*);
extern "C" void AbortBoundary(){abort();}
static void require(bool value){if(!value)abort();}
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
static void managedArrayHeapSmoke(){
 for(unsigned cycle=0;cycle<256;++cycle){
  CArray<CIVCountedPointer<CManagedTextureData> > array;
  array._M_start=(CIVCountedPointer<CManagedTextureData>*)malloc(5*sizeof(CIVCountedPointer<CManagedTextureData>));
  require(array._M_start!=0);
  for(unsigned i=0;i<3;++i)new(array._M_start+i) CIVCountedPointer<CManagedTextureData>;
  array._M_finish=array._M_start+3;array._M_end_of_storage._M_data=array._M_start+5;
  CManagedTextureData* a=new CManagedTextureData;
  CManagedTextureData* b=new CManagedTextureData;
  a->Initialise(600+cycle,cycle&15);b->Initialise(700+cycle,(cycle+1)&15);
  a->VolumeTexture.Reset(new CVolumeTexture);b->VolumeTexture.Reset(new CVolumeTexture);
  checkManaged(a,600+cycle,cycle&15);checkManaged(b,700+cycle,(cycle+1)&15);
  checkManagedVolume(a);checkManagedVolume(b);
  array[0].data_=a;array[1].data_=a;a->AddRef();array[2].data_=b;
  require(a->IVRefCount==2&&b->IVRefCount==1);
  array.DeallocateMemory();
  require(!array._M_start&&!array._M_finish&&!array._M_end_of_storage._M_data);
  // Both owners were destroyed through original virtual deletion; inspect
  // only the still-live empty array after that lifetime boundary.
 }
}
int main(){require(TextureVTable[0]==(unsigned)&TextureScalar);require(ManagedDataVTable[0]==(unsigned)&ManagedDataScalar);require(ManagedDataVTable[1]==(unsigned)&ManagedVirtualDelete);managedHeapSmoke();managedArrayHeapSmoke();puts("NATIVE_MANAGED_HEAP_PASS cycles=256 array_cycles=256 graphics_calls=0");return 0;}
