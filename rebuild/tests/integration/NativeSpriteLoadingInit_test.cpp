#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "engine/CEngineInternalPrimitive2DSprite.h"
#include "fable_surface_types.h"
#include "fable_engine_internal.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGraphicDataBank.h"
#include "sprite_loading_init_retail.h"
#include "sprite_loading_init_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" void __fastcall InternalSpriteUpdate(void*,void*,const void*,unsigned long);
extern "C" unsigned FrameVTable[],ManagedVTable[],ResourceVTable[],BaseVTable[];
extern "C" void AbortBoundary(){abort();}
static const float Two32=4294967296.0f;
static unsigned arena[16384],initial[16384],modelImages[4][16384],nativeImages[4][16384],modelGlobals[4][3];
static unsigned mode,steps,count,snapshots,self,valuePointer,framePointer,profile,mutation,precision,frameCalls,textureCalls,caseNumber;
static unsigned& word(unsigned p,unsigned off){return *(unsigned*)(p+off);}
static unsigned ptr(unsigned off){return (unsigned)arena+off;}
static float real(unsigned b){float f;memcpy(&f,&b,4);return f;}
static unsigned bits(float f){unsigned b;memcpy(&b,&f,4);return b;}
static unsigned pattern(unsigned n){const unsigned p[16]={0,0x80000000,1,0x3f800000,0xbf800000,0x7f800000,0xff800000,0x7fc12345,0x38d1b717,0x38d1b716,0x38d1b718,0x3a83126f,0x4f000000,0xcf000000,0x7f7fffff,0x00800000};return p[n&15];}
static bool nan(unsigned b){return (b&0x7f800000)==0x7f800000&&(b&0x7fffff)!=0;}
static void require(bool b){if(!b){fprintf(stderr,"sprite-update failure case=%u mode=%u step=%u profile=%u mutation=%u precision=%u\n",caseNumber,mode,steps,profile,mutation,precision);fflush(stderr);abort();}}
static bool modelEqual(const unsigned* expected){
 const unsigned char* a=(const unsigned char*)arena;const unsigned char* e=(const unsigned char*)expected;
 for(unsigned i=0;i<sizeof(arena);++i){if(a[i]==e[i])continue;unsigned off=(unsigned)(a+i)-(self+0x48);if(off<16){unsigned f=off&~3U;if(nan(word(self+0x48,f))&&nan(*(const unsigned*)(e+self-ptr(0)+0x48+f)))continue;}fprintf(stderr,"byte difference offset=%x actual=%x expected=%x\n",i,a[i],e[i]);return false;}return true;
}
static void snapshot(){
 if(mode==0){require(count<4);memcpy(modelImages[count],arena,sizeof(arena));modelGlobals[count][0]=(unsigned)NEngineInternal::GPResourceManager;modelGlobals[count][1]=frameCalls;modelGlobals[count][2]=textureCalls;++count;}
 else{require(steps<count&&modelEqual(modelImages[steps]));require(modelGlobals[steps][0]==(unsigned)NEngineInternal::GPResourceManager&&modelGlobals[steps][1]==frameCalls&&modelGlobals[steps][2]==textureCalls);if(mode==1)memcpy(nativeImages[steps],arena,sizeof(arena));else require(!memcmp(nativeImages[steps],arena,sizeof(arena)));++steps;++snapshots;}
}
static void frameCallback(unsigned frame,unsigned a,unsigned b){
 require(frame==framePointer&&a==0&&b==0);++frameCalls;
 if(mutation){if(frame)word(frame,12)=ptr(0x5800);NEngineInternal::GPResourceManager=(CEngineResourceManager*)ptr(0x4000);if(mutation==1)*(unsigned char*)(valuePointer+0x38)=0;if(mutation>=2)*(unsigned char*)(valuePointer+0x3c)^=1;if(mutation==3)word(self,0x50)=0xbf800000;}
 snapshot();
}
void* __cdecl operator new(size_t bytes){require(bytes==68);snapshot();return (void*)ptr(0x6000);}
class Boundaries{public:void Load(unsigned long,unsigned long);};
void Boundaries::Load(unsigned long a,unsigned long b){require((unsigned)this==ptr(0x6000));frameCallback(word((unsigned)this,64),a,b);}
static void modelFrameLoading(unsigned frame){require(frame==framePointer);operator new(68);unsigned data=ptr(0x6000),list=ptr(0x3168),head=list+4;word(data,0)=(unsigned)FrameVTable;word(data,4)=1;for(unsigned o=8;o<28;o+=4)word(data,o)=0;word(data,28)=0;word(data,32)=word(data,40)=(unsigned)ManagedVTable;word(data,36)=word(data,44)=word(data,48)=word(data,52)=0;word(data,64)=frame;*(unsigned char*)(data+56)=255;*(unsigned char*)(data+58)=127;word(data,60)=word(list,44)-1;word(data,20)=72;word(data,8)=list;word(data,24)=word(list,44);word(data,12)=word(head,12);word(data,16)=head;word(word(data,12),16)=data;word(head,12)=data;++word(list,32);word(list,36)+=72;word(frame,8)=data;frameCallback(frame,0,0);}

static long __stdcall levelDesc(void* texture,unsigned long level,D3DSURFACE_DESC* description){
 require(level==0&&((unsigned)texture==ptr(0x5d00)||(unsigned)texture==ptr(0x5e00)));++textureCalls;unsigned which=(unsigned)texture==ptr(0x5d00)?0:2;memset(description,0xa9,sizeof(*description));description->Width=pattern(profile+which);description->Height=pattern(profile+which+1);
 if(mutation){NEngineInternal::GPResourceManager=(CEngineResourceManager*)ptr(0x4000);if(mutation==1||mutation==3)word(self,0x70)=ptr(0x5c10);if(mutation>=2){word(self,0x50)=0xbf800000;word(self,0x54)=0xc0000000;}*(unsigned char*)(valuePointer+0x38)=0;}
 snapshot();return mutation==3?(long)0x80004005:0;
}
static unsigned dimensions(bool height){D3DSURFACE_DESC desc;unsigned texture=word(word(self,0x70),0);levelDesc((void*)texture,0,&desc);return height?desc.Height:desc.Width;}
static unsigned addSize(unsigned f,unsigned size,bool subtract){
 double n=(double)size;if(precision==0&&(size&0x80000000)){volatile float rounded=(float)n;n=rounded;}
 float result=subtract?(float)((double)real(f)-n):(float)((double)real(f)+n);return bits(result);
}
static bool nonzero(unsigned p,float threshold){double x=real(word(p,0)),y=real(word(p,4));return x*x+y*y>threshold;}
static void model(unsigned flags){
 word(self,12)=flags;const unsigned from[13]={12,16,20,24,28,32,36,40,44,48,52,4,64};const unsigned to[13]={72,76,80,84,88,92,96,100,104,136,108,44,112};
 for(unsigned i=0;i<13;++i)word(self,to[i])=word(valuePointer,from[i]);*(unsigned char*)(self+116)=(word(valuePointer,60)&2)!=0;
 for(i=0;i<4;++i)word(self,117+i*4)=word(valuePointer,68+i*4);
 volatile float threshold=DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;*(unsigned char*)(self+133)=nonzero(self+117,threshold)||nonzero(self+125,threshold);
 if(flags&128)word(self,12)=flags|128;word(self,48)=(flags&1024)?0x00200000:0x80000000;
 unsigned bankIndex=word(self,92);
 if(bankIndex){unsigned bank=word((unsigned)NEngineInternal::GPResourceManager,0x954),begin=word(bank,0x1e0),end=word(bank,0x1e4);unsigned frame=bankIndex<(end-begin)/44?word(begin+bankIndex*44,40)+word(self,96)*16:0;modelFrameLoading(frame);
  if(*(unsigned char*)(valuePointer+56)){unsigned info=word(frame,12);unsigned width=*(unsigned short*)(info+6),height=*(unsigned short*)(info+8);if(*(unsigned char*)(valuePointer+60)&1){width>>=1;height>>=1;word(self,72)=addSize(word(self,72),width,true);word(self,76)=addSize(word(self,76),height,true);word(self,80)=addSize(word(self,80),width,false);word(self,84)=addSize(word(self,84),height,false);}else{word(self,80)=addSize(word(self,80),width,false);word(self,84)=addSize(word(self,84),height,false);}}
 }else if(word(valuePointer,64)&&*(unsigned char*)(valuePointer+56)){unsigned width=dimensions(false);word(self,80)=addSize(word(self,80),width,false);unsigned height=dimensions(true);word(self,84)=addSize(word(self,84),height,false);}
 snapshot();
}
int main(){
 unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));void(Boundaries::*load)(unsigned long,unsigned long)=&Boundaries::Load;unsigned loadAddress=0;memcpy(&loadAddress,&load,4);unsigned targets[12]={(unsigned)FrameVTable,(unsigned)ManagedVTable,(unsigned)ResourceVTable,(unsigned)BaseVTable,(unsigned)&operator new,loadAddress,(unsigned)&AbortBoundary,(unsigned)&AbortBoundary,(unsigned)&DELTA_NOTIONAL_ZERO,(unsigned)&NEngineInternal::GPResourceManager,(unsigned)&Two32};for(unsigned i=0;i<sizeof(fixups)/sizeof(fixups[0]);++i){unsigned t=fixups[i].target;unsigned v=t>=0x80000000?targets[t-0x80000000]:(unsigned)(code+t);*(unsigned*)(code+fixups[i].site)=fixups[i].kind==20?v-(unsigned)(code+fixups[i].site+4):v;}

 unsigned table[18]={0};table[17]=(unsigned)levelDesc;const unsigned pc[3]={_PC_24,_PC_53,_PC_64};unsigned saved=_controlfp(0,0),cases=0;
 for(unsigned bankMode=0;bankMode<2;++bankMode)for(unsigned position=0;position<2;++position)for(unsigned flagProfile=0;flagProfile<4;++flagProfile)for(profile=0;profile<16;++profile)for(mutation=0;mutation<4;++mutation)for(precision=0;precision<3;++precision)for(unsigned place=0;place<2;++place)for(unsigned fill=0;fill<2;++fill)for(unsigned alias=0;alias<2;++alias){
 if(bankMode==2&&(position||(alias&&(flagProfile&2))))continue;
 memset(arena,fill?0xa9:0,sizeof(arena));self=ptr(place?0x8000:0x100);valuePointer=alias?self+60:ptr(0x1000);word(ptr(0x2000),0x954)=ptr(0x3000);word(ptr(0x4000),0x954)=ptr(0x3000);word(ptr(0x3000),0x1e0)=ptr(0x5000);word(ptr(0x3000),0x1e4)=ptr(0x5000)+(bankMode==2?0:4*44);word(ptr(0x3000),0x1e8)=ptr(0x5000)+4*44;word(ptr(0x5000)+44,40)=ptr(0x5200);
 framePointer=bankMode==2?0:ptr(0x5200)+(profile&3)*16;for(i=0;i<4;++i){unsigned frame=ptr(0x5200)+16*i;word(frame,12)=ptr(0x5400);word(frame,8)=0;*(unsigned short*)(frame+4)=0;}word(ptr(0x5400),32)=word(ptr(0x5800),32)=ptr(0x3000);word(ptr(0x316c),12)=ptr(0x6200);word(ptr(0x316c),16)=ptr(0x316c);word(ptr(0x3168),44)=profile;word(ptr(0x3168),32)=pattern(profile);word(ptr(0x3168),36)=pattern(profile+1);*(unsigned short*)ptr(0x5406)=(unsigned short)pattern(profile);*(unsigned short*)ptr(0x5408)=(unsigned short)pattern(profile+1);*(unsigned short*)ptr(0x5806)=(unsigned short)pattern(profile+2);*(unsigned short*)ptr(0x5808)=(unsigned short)pattern(profile+3);
 word(ptr(0x5c00),0)=ptr(0x5d00);word(ptr(0x5c10),0)=ptr(0x5e00);word(ptr(0x5d00),0)=word(ptr(0x5e00),0)=(unsigned)table;
 for(i=0;i<4;++i)word(valuePointer,12+4*i)=pattern(profile+i);word(valuePointer,28)=profile;word(valuePointer,32)=bankMode?1:0;word(valuePointer,36)=profile&3;word(valuePointer,40)=pattern(profile+5);word(valuePointer,44)=pattern(profile+6);word(valuePointer,48)=profile;word(valuePointer,52)=pattern(profile+7);word(valuePointer,4)=profile;*(unsigned char*)(valuePointer+56)=(unsigned char)position;word(valuePointer,60)=flagProfile;word(valuePointer,64)=ptr(0x5c00);for(i=0;i<4;++i)word(valuePointer,68+4*i)=pattern(profile+8+i);
 if(profile<3){word(valuePointer,68)=pattern(profile+8);word(valuePointer,72)=word(valuePointer,76)=word(valuePointer,80)=0;}
 memcpy(initial,arena,sizeof(arena));count=0;caseNumber=cases;unsigned flags=(profile*0x13579bdf&~0x480U)|((flagProfile&1)?0x80:0)|((flagProfile&2)?0x400:0);
 for(mode=0;mode<3;++mode){memcpy(arena,initial,sizeof(arena));NEngineInternal::GPResourceManager=(CEngineResourceManager*)ptr(0x2000);steps=frameCalls=textureCalls=0;_controlfp(pc[precision],_MCW_PC);if(mode==0)model(flags);else{(mode==1?(void(__fastcall*)(void*,void*,const void*,unsigned long))(code+768):InternalSpriteUpdate)((void*)self,0,(void*)valuePointer,flags);snapshot();require(steps==count);}}
 ++cases;
 }_controlfp(saved,_MCW_PC);VirtualFree(code,0,0x8000);printf("NATIVE_SPRITE_LOADING_INIT_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
