#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_display_engine.h"
#include "display_fade_sequence_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
static unsigned storage[2048],expected[2048];static unsigned profiles,mutation;static unsigned char* object;
static void require(bool b){if(!b)abort();}
extern "C" void __fastcall ProfileBoundary(long n){require(n==1);++profiles;if(mutation)object[0xd8]^=1;}
#pragma comment(linker,"/alternatename:?EndProfile@NProfileTimer@@YIXJ@Z=@ProfileBoundary@4")
static float& scalar(unsigned char* p,unsigned off){return *(float*)(p+off);}
static void modelAbout(unsigned char* p,bool global){
 if(!p[0xd8]&&global){scalar(p,0xc4)=3;scalar(p,0xd0)=3;scalar(p,0xc0)=0;p[0xbc]=1;p[0xc8]=0;p[0xc9]=0;scalar(p,0xcc)=0.1f;p[0xd4]=p[0xd5]=p[0xd6]=0;p[0xd7]=0xff;scalar(p,0xf0)=-1;scalar(p,0xf4)=0;p[0xec]=1;p[0xc8]=1;}
 if(mutation)p[0xd8]^=1;
}
static void modelUpdate(unsigned char* p,float step){
 if(p[0xc9]){scalar(p,0xcc)+=step;if(scalar(p,0xcc)>=scalar(p,0xc0))p[0xc9]=0;}
 else if(p[0xc8]){scalar(p,0xd0)-=step;if(scalar(p,0xd0)<=0){if(scalar(p,0xd0)<0)scalar(p,0xd0)=0;p[0xbc]=p[0xc8]=p[0xc9]=0;}}
}
static float modelAlpha(unsigned char* p){
 if(p[0xc9])return scalar(p,0xc0)<0.0001f?1:scalar(p,0xcc)/scalar(p,0xc0);
 if(p[0xc8])return scalar(p,0xc4)<0.0001f?1:scalar(p,0xd0)/scalar(p,0xc4);
 return p[0xbc]?1:0;
}
static void modelEnd(unsigned char* p){p[0xec]=0;if(p[0xbc]&&!p[0xd8]){scalar(p,0xf0)=-1;scalar(p,0xf4)=0;p[0xc9]=0;p[0xc8]=1;}}
int main(){
 unsigned char* code=(unsigned char*)VirtualAlloc(0,sizeof(retailCode),0x3000,0x40);require(code!=0);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(fixups)/sizeof(fixups[0]);++i){const Fixup& f=fixups[i];unsigned target=f.target==0xffffffff?(unsigned)&GInitialFadeAllowedToBeActive:f.target==0xfffffffe?(unsigned)ProfileBoundary:(unsigned)(code+f.target);*(unsigned*)(code+f.site)=f.kind==6?target:target-(unsigned)(code+f.site+4);}
 unsigned cases=0,snapshots=0;const float steps[6]={0,0.1f,0.25f,1,3,-0.125f};
 for(unsigned lock=0;lock<2;++lock)for(unsigned global=0;global<2;++global)for(mutation=0;mutation<2;++mutation)for(unsigned elapsed=0;elapsed<6;++elapsed)for(unsigned finish=0;finish<4;++finish)for(unsigned pos=0;pos<2;++pos)for(unsigned fill=0;fill<2;++fill){
  unsigned initial[2048];memset(initial,fill?0x95:0,sizeof(initial));unsigned off=pos?0x200:0;unsigned char* start=(unsigned char*)initial+off;start[0xd8]=(unsigned char)lock;start[0xbc]=start[0xc8]=start[0xc9]=start[0xec]=0;scalar(start,0xc0)=2;scalar(start,0xc4)=3;scalar(start,0xcc)=0;scalar(start,0xd0)=3;
  for(unsigned run=0;run<2;++run){
   memcpy(storage,initial,sizeof(storage));memcpy(expected,initial,sizeof(expected));object=(unsigned char*)storage+off;unsigned char* model=(unsigned char*)expected+off;bool expectedGlobal=global!=0;GInitialFadeAllowedToBeActive=expectedGlobal;profiles=0;
   for(unsigned frame=0;frame<16;++frame){
    if(frame==0||frame==8){if(run)((CDisplayEngine*)object)->SetAboutToRun();else ((void(__fastcall*)(void*,void*))code)(object,0);modelAbout(model,expectedGlobal);}
    if(frame==finish*3){if(run)((CDisplayEngine*)object)->EndScreenFadeOut();else ((void(__fastcall*)(void*,void*))(code+512))(object,0);modelEnd(model);expectedGlobal=false;}
    if(run)((CFadeInFadeOutBase*)(object+0xbc))->Update(steps[elapsed]);else ((void(__fastcall*)(void*,void*,float))(code+256))(object+0xbc,0,steps[elapsed]);modelUpdate(model,steps[elapsed]);
    float value=run?((CFadeInFadeOutBase*)(object+0xbc))->GetFadeInOutAlpha():((float(__fastcall*)(void*,void*))(code+768))(object+0xbc,0);float wanted=modelAlpha(model);if(memcmp(&value,&wanted,4)||memcmp(storage,expected,sizeof(storage))||GInitialFadeAllowedToBeActive!=expectedGlobal||profiles!=(frame<8?1u:2u)){fprintf(stderr,"sequence lock=%u global=%u mutation=%u elapsed=%u finish=%u pos=%u fill=%u run=%u frame=%u alpha=%g wanted=%g profiles=%u\n",lock,global,mutation,elapsed,finish,pos,fill,run,frame,value,wanted,profiles);for(unsigned k=0;k<sizeof(storage);++k)if(((unsigned char*)storage)[k]!=((unsigned char*)expected)[k]){fprintf(stderr,"memory %x actual=%02x expected=%02x\n",k,((unsigned char*)storage)[k],((unsigned char*)expected)[k]);break;}fflush(stderr);abort();}++snapshots;
   }
  }
  ++cases;
 }
 VirtualFree(code,0,0x8000);printf("NATIVE_DISPLAY_FADE_SEQUENCE_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;
}
