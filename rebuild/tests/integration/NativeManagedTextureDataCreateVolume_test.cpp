#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_surface_types.h"
#include "fable_volume_types.h"
#include "managed_texture_data_create_volume_retail.h"
#include "managed_texture_data_create_volume_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" bool __fastcall ManagedVolumeCreate(void*,void*,unsigned long,unsigned long,unsigned long,unsigned,const CPixelFormat*);
extern "C" void __fastcall VolumeDeleteData(void*);
extern "C" void* __fastcall VolumeScalar(void*,void*,unsigned);
extern "C" unsigned VolumeVTable[],BaseVTable[];
extern "C" void AbortBoundary(){abort();}
void CVolumeTexture::Uninitialise(){abort();}
static unsigned arena[4096],initial[4096],expected[7][4096],captures[7][4096];
static unsigned owner,location,profile,mutation,width,height,depth,heap,control,captured;static unsigned* roundedOutput;static bool success;
static unsigned ptr(unsigned offset){return (unsigned)arena+location+offset;}
static unsigned& word(unsigned pointer,unsigned offset){return *(unsigned*)(pointer+offset);}
static void require(bool value){if(!value)abort();}
static unsigned pattern(unsigned n){const unsigned values[8]={0,1,2,3,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};return values[n&7];}
static void capture(){require(captured<7);memcpy(captures[captured++],arena,sizeof(arena));}
static void event(unsigned id,unsigned arg){unsigned n=word(ptr(0x2000),0)++;require(n<6);word(ptr(0x2000),4+n*8)=id;word(ptr(0x2000),8+n*8)=arg;capture();}
extern "C" void* __fastcall DimensionsBoundary(void* output,void* input,const CPixelFormat* format){require((unsigned)format==ptr(0x1600));unsigned* in=(unsigned*)input;require(in[0]==width&&in[1]==height);if(mutation&1){word(owner,0x28)=pattern(profile+5);word(ptr(0x1600),0)^=0xdeadbeef;in[0]^=0x80000000;}((unsigned*)output)[0]=pattern(profile+2);((unsigned*)output)[1]=pattern(profile+4);roundedOutput=(unsigned*)output;event(1,width);return output;}
extern "C" void* Allocate(unsigned size){require(size==16||size==12);if(size==16){require(roundedOutput!=0);if(mutation&1){roundedOutput[0]^=0xffffffff;roundedOutput[1]^=0x80000000;word(owner,0x2c)=ptr(0x600);word(owner,0x28)=pattern(profile+6);}event(2,size);return (void*)heap;}require(heap!=0&&!word(owner,0x2c)&&word(owner,0x28)==heap);if(mutation&2){word(owner,0x28)=ptr(0xa00);word(owner,0x2c)=ptr(0x400);}event(3,size);return (void*)control;}
void* operator new(unsigned size){return Allocate(size);}
static void __fastcall drop(void* value){if(mutation&1){word(owner,0x2c)=word(owner,0x2c)==ptr(0x400)?ptr(0x600):ptr(0x400);word(owner,0x28)=pattern(profile+1);}event(4,(unsigned)value);}
extern "C" void Delete(void* value){require((unsigned)value==word(owner,0x2c));if(mutation&2){word(owner,0x2c)=ptr(0x600);word(owner,0x28)=pattern(profile+3);}event(5,(unsigned)value);}
void operator delete(void* value){Delete(value);}
bool CVolumeTexture::Initialise(const C3DExtentsI& dimensions,unsigned int levels,const CPixelFormat& format,unsigned long flags,EVolumePool pool,bool automatic){
 require((unsigned)this==word(owner,0x28)&&(unsigned)&format==ptr(0x1600));require(levels==1&&flags==0&&pool==VOLUME_POOL_MANAGED&&!automatic);require((unsigned)dimensions.X==pattern(profile+2)&&(unsigned)dimensions.Y==pattern(profile+4)&&(unsigned)dimensions.Z==depth);require(*(unsigned short*)(owner+0x1c)==(unsigned short)dimensions.X&&*(unsigned short*)(owner+0x1e)==(unsigned short)dimensions.Y);roundedOutput=0;
 if(mutation&2){memset((void*)(owner+0x1c),0xcc,48);C3DExtentsI& live=const_cast<C3DExtentsI&>(dimensions);live.X=0x87654321;live.Y=0x12345678;live.Z=0xabcdef01;}event(6,(unsigned)this);return success;
}
static void resetModel(unsigned data){unsigned info=word(owner,0x2c);if(info){--word(info,0);info=word(owner,0x2c);if(!word(info,0)){drop((void*)word(info,8));Delete((void*)word(owner,0x2c));}}word(owner,0x2c)=0;word(owner,0x28)=data;if(data){unsigned block=(unsigned)Allocate(12);if(block){word(block,0)=1;word(block,4)=(unsigned)VolumeDeleteData;word(block,8)=word(owner,0x28);}word(owner,0x2c)=block;}}
static bool model(){unsigned input[2]={width,height},output[2];DimensionsBoundary(output,input,(CPixelFormat*)ptr(0x1600));unsigned x=output[0],y=output[1];unsigned data=(unsigned)Allocate(16);if(data){word(data,0)=(unsigned)BaseVTable;word(data,4)=word(data,12)=0;word(data,0)=(unsigned)VolumeVTable;}resetModel(data);*(unsigned short*)(owner+0x1c)=(unsigned short)x;*(unsigned short*)(owner+0x1e)=(unsigned short)y;C3DExtentsI dimensions(x,y,depth);bool result=((CVolumeTexture*)word(owner,0x28))->Initialise(dimensions,1,*(CPixelFormat*)ptr(0x1600),0,VOLUME_POOL_MANAGED,false);*(unsigned char*)(owner+0x48)=result;return result;}
int main(){unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));bool(CVolumeTexture::*method)(const C3DExtentsI&,unsigned int,const CPixelFormat&,unsigned long,EVolumePool,bool)=&CVolumeTexture::Initialise;unsigned initAddress=0;require(sizeof(method)==4);memcpy(&initAddress,&method,4);unsigned targets[7]={(unsigned)DimensionsBoundary,(unsigned)Allocate,(unsigned)Delete,(unsigned)VolumeDeleteData,(unsigned)VolumeVTable,(unsigned)BaseVTable,initAddress};require(sizeof(fixups)/sizeof(*fixups)==10);for(unsigned f=0;f<10;++f){const Fixup& fix=fixups[f];unsigned target=fix.target&0x80000000?targets[fix.target&0x7fffffff]:(unsigned)(code+fix.target);*(unsigned*)(code+fix.site)=fix.kind==20?target-(unsigned)(code+fix.site+4):target;}require(VolumeVTable[0]==(unsigned)VolumeScalar);unsigned cases=0,snapshots=0;
for(profile=0;profile<8;++profile)for(unsigned old=0;old<3;++old)for(unsigned countIndex=0;countIndex<4;++countIndex)for(unsigned hi=0;hi<3;++hi)for(unsigned ci=0;ci<3;++ci)for(mutation=0;mutation<4;++mutation)for(unsigned result=0;result<2;++result)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill){
 location=place?0x1800:0;owner=ptr(0x100);width=pattern(profile);height=pattern(profile+1);depth=pattern(profile+countIndex);heap=hi?ptr(hi==1?0xe00:0x1000):0;control=ci?ptr(ci==1?0x1200:0x1400):0;success=result!=0;memset(arena,fill?0xa9:0,sizeof(arena));for(unsigned i=0;i<19;++i)word(owner,i*4)=pattern(profile+i);word(owner,0x28)=ptr(0x800);word(owner,0x2c)=old?ptr(old==1?0x400:0x600):0;for(unsigned b=0;b<2;++b){unsigned info=ptr(b?0x600:0x400);word(info,0)=pattern(countIndex==3?7:countIndex);word(info,4)=(unsigned)drop;word(info,8)=pattern(profile+b);}memset((void*)ptr(0x2000),0,64);memcpy(initial,arena,sizeof(arena));unsigned expectedCount=0;
 for(unsigned mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));captured=0;roundedOutput=0;bool returned=mode==0?model():(mode==1?((bool(__fastcall*)(void*,void*,unsigned long,unsigned long,unsigned long,unsigned,const CPixelFormat*))code):ManagedVolumeCreate)((void*)owner,0,width,height,depth,pattern(profile+5),(CPixelFormat*)ptr(0x1600));require(returned==success&&roundedOutput==0);capture();if(!mode){expectedCount=captured;memcpy(expected,captures,sizeof(expected));}else{require(captured==expectedCount);for(unsigned i=0;i<captured;++i){require(!memcmp(expected[i],captures[i],sizeof(arena)));++snapshots;}}}++cases;
}VirtualFree(code,0,0x8000);printf("NATIVE_MANAGED_TEXTURE_DATA_CREATE_VOLUME_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
