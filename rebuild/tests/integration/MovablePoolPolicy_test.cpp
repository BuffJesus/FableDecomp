#include "fable_handle_memory_pool.h"
#include "fable_game_text_bank.h"
#include <stdio.h>
#include <string.h>
struct BaseAccess:CHandleBasedMemoryPool {
 static void Move(CHandleBasedMemoryPool* p,void* r,long h,void* from,void* to,unsigned long size){((BaseAccess*)p)->CHandleBasedMemoryPool::MoveResource(r,h,from,to,size);}
 static bool Can(CHandleBasedMemoryPool* p,void* r,long h){return ((BaseAccess*)p)->CHandleBasedMemoryPool::CanMoveResource(r,h);}
 static void Validate(CHandleBasedMemoryPool* p,void* r){((BaseAccess*)p)->CHandleBasedMemoryPool::ValidateResource(r);}
};
struct MovableAccess:CMovableResourceMemoryPool {
 static void Move(CMovableResourceMemoryPool* p,void* r,long h,void* from,void* to,unsigned long size){((MovableAccess*)p)->CMovableResourceMemoryPool::MoveResource(r,h,from,to,size);}
 static bool Can(CMovableResourceMemoryPool* p,void* r,long h){return ((MovableAccess*)p)->CMovableResourceMemoryPool::CanMoveResource(r,h);}
 static void Validate(CMovableResourceMemoryPool* p,void* r){((MovableAccess*)p)->CMovableResourceMemoryPool::ValidateResource(r);}
};
static int moved,queried,verified,freed,failed;static bool allow;static long wantedHandle;static unsigned long wantedSize;static void* wantedFrom;static void* wantedTo;static void* wantedPool;static unsigned char* buffer;static unsigned char expected[96];
bool CMovableResource::CanRelocateData(long)const{return false;}
unsigned long CMovableResource::Verify()const{return 0;}
struct Resource:CMovableResource {
 virtual void RelocateData(long handle,void* from,void* to,long size){if(handle!=wantedHandle||from!=wantedFrom||to!=wantedTo||(unsigned long)size!=wantedSize||memcmp(buffer,expected,96))failed=1;++moved;}
 virtual bool CanRelocateData(long handle)const{if(handle!=wantedHandle)failed=2;++queried;return allow;}
 virtual unsigned long Verify()const{++verified;return 0x87654321;}
};
void CHandleBasedMemoryPool::Free(long handle){if(this!=wantedPool||handle!=wantedHandle)failed=3;++freed;}
int main(){const unsigned offsets[]={0,1,8,24,48},sizes[]={0,1,7,16,31};const long handles[]={0,1,-1,(long)0x80000000};int cases=0;Resource resource;
 for(int f=0;f<5;++f)for(int t=0;t<5;++t)for(int z=0;z<5;++z)for(int present=0;present<2;++present)for(int can=0;can<2;++can)for(int h=0;h<4;++h){
  union{double align;char bytes[0x464];} mem;memset(&mem,0xa5,sizeof(mem));CHandleBasedMemoryPool* base=(CHandleBasedMemoryPool*)&mem;CMovableResourceMemoryPool* pool=(CMovableResourceMemoryPool*)&mem;unsigned char data[96],original[96];for(int i=0;i<96;++i)original[i]=(unsigned char)(i*17+h*31+f);
  memcpy(expected,original,96);for(unsigned i=0;i<sizes[z];++i)expected[offsets[t]+i]=original[offsets[f]+i];buffer=data;wantedPool=pool;wantedHandle=handles[h];wantedFrom=data+offsets[f];wantedTo=data+offsets[t];wantedSize=sizes[z];allow=can!=0;moved=queried=verified=freed=failed=0;void* r=present?&resource:0;
  memcpy(data,original,96);BaseAccess::Move(base,r,wantedHandle,wantedFrom,wantedTo,wantedSize);if(memcmp(data,expected,96)||moved)return 1;
  memcpy(data,original,96);MovableAccess::Move(pool,r,wantedHandle,wantedFrom,wantedTo,wantedSize);if(memcmp(data,expected,96)||moved!=present||failed)return 2;
  if(!BaseAccess::Can(base,r,wantedHandle)||MovableAccess::Can(pool,r,wantedHandle)!=(present&&can)||queried!=present)return 3;
  BaseAccess::Validate(base,r);if(present)MovableAccess::Validate(pool,r);if(verified!=present)return 4;
  pool->FreeResource(wantedHandle);if(freed!=1||failed)return 5;for(unsigned i=0;i<sizeof(mem);++i)if((unsigned char)mem.bytes[i]!=0xa5)return 6;
  CHandleBasedMemoryPool::CHeader headers[3];CHandleBasedMemoryPool::CHandleInfo infos[3];memset(headers,0x6b,sizeof(headers));for(int i=0;i<3;++i)infos[i].Header=&headers[i];base->Handles._M_start=infos;int selected=(f+t+z)%3;void* value=(void*)(0x45670000+h);base->InternalSetResource(selected,value);for(int i=0;i<3;++i)if(headers[i].Resource!=(i==selected?value:(void*)0x6b6b6b6b))return 7;
  ++cases;
 }
 printf("MOVABLE_POOL_POLICY_BEHAVIOR_PASS scenarios=%d\n",cases);return 0;}
