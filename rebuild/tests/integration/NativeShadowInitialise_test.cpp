#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shadow_initialise_aliases.h"
#include "shadow_initialise_retail.h"
#define REQUIRE(x) do {if(!(x)){fprintf(stderr,"line %d case %u\n",__LINE__,cases);exit(70);}}while(0)
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" bool __fastcall Initialise(void*,void*);
extern "C" void* DisplayGlobal;extern "C" void* SurfaceVtable[];
static unsigned char* arena;static unsigned char* shadow;static unsigned p[5],cases,events,textureCalls,fallbackCalls,builds,depthSize,fallbackSize;static bool firstValid;static void* temporary;static FILE* output;
static unsigned& A(void* x,unsigned off){return *(unsigned*)((unsigned char*)x+off);}
static unsigned Offset(void* self){unsigned off=(unsigned char*)self-arena;return off<8192?off:0xffffffff;}
static void Record(unsigned tag,void*self=0,unsigned a=0,unsigned b=0,unsigned c=0,unsigned d=0){
 REQUIRE(++events<100);unsigned words[7]={tag,Offset(self),a,b,c,d,(unsigned)DisplayGlobal};fwrite(words,4,7,output);fwrite(arena,1,8192,output);
}
extern "C" void __fastcall ShadowCleanup(void*self,void*){REQUIRE(self==shadow);shadow[0x2af]=0;Record(1,self);}
extern "C" void __fastcall PixelInit(void*self,void*,unsigned format){A(self,0)=format;Record(2,0,format);}
extern "C" bool __fastcall ValidTexture(void*self,void*,const unsigned* format){
 REQUIRE(self==DisplayGlobal);bool ok;
 if(firstValid){REQUIRE(*format==80);firstValid=false;ok=(p[2]&1)!=0;}
 else{unsigned i=fallbackCalls++;REQUIRE(i<8&&*format==(i==0?21:i==1?32:0));ok=i==p[3];}
 if(p[4]==1)DisplayGlobal=arena+0x1c40;Record(3,self,*format,ok);return ok;
}
extern "C" bool __fastcall BestOpaque(void*self,void*,unsigned bits,unsigned* format,bool target){
 REQUIRE(self==DisplayGlobal&&bits==16&&*format==0xffffffff&&target);*format=23;Record(4,self,bits);return false;
}
extern "C" bool __fastcall ValidTarget(void*self,void*,const unsigned* colour,const unsigned* depth){
 REQUIRE(self==DisplayGlobal&&*depth==80);bool ok=*colour==23?(p[2]&2)!=0:true;Record(5,self,*colour,*depth,ok);return ok;
}
extern "C" unsigned __fastcall ShaderVersion(void*self,void*){REQUIRE(self==DisplayGlobal);unsigned version=(p[2]&64)?1400:1399;Record(6,self,version);return version;}
extern "C" bool __fastcall TextureInit(void*self,void*,const long* size,long levels,const unsigned* format,unsigned flags,unsigned pool,bool dynamic,bool mips){
 REQUIRE(levels==1&&pool==0&&!dynamic&&mips&&size[0]==size[1]);bool ok;
 if(self==shadow+0x2e8&&flags==2){REQUIRE(*format==80&&(unsigned)size[0]==A(shadow,0x338));depthSize=(unsigned)size[0];ok=(p[2]&4)!=0;}
 else if(self==shadow+0x318){REQUIRE(flags==1&&*format==23&&(unsigned)size[0]==depthSize);ok=(p[2]&8)!=0;}
 else{REQUIRE(self==shadow+0x2e8&&flags==1&&(*format==21||*format==32||*format==0)&&(unsigned)size[0]==A(shadow,0x338));fallbackSize=(unsigned)size[0];ok=(p[2]&16)!=0;}
 if(ok){A(self,0)=(unsigned)(arena+0x1800+(self==shadow+0x318?32:0));A(self,4)=0x20012345;}
 if(p[4]==2)A(shadow,0x338)=512;++textureCalls;Record(7,self,(unsigned)size[0],*format,flags,ok);return ok;
}
extern "C" bool __fastcall DepthInit(void*self,void*,const long* size,const unsigned* format,unsigned multi,bool discard){
 REQUIRE(self==shadow+0x2f8&&(unsigned)size[0]==fallbackSize&&size[0]==size[1]&&*format==80&&multi==0&&discard);bool ok=(p[2]&32)!=0;
 if(ok){A(self,4)=(unsigned)(arena+0x1840);A(self,8)=1;}Record(8,self,(unsigned)size[0],ok);return ok;
}
extern "C" void* __fastcall TextureAssign(void*self,void*,void* source){REQUIRE(self==shadow+0x318&&source==shadow+0x2e8);memcpy(self,source,8);Record(9,self);return self;}
extern "C" void* __fastcall GetLevel(void*self,void*,void* out,long level){
 REQUIRE((self==shadow+0x2e8||self==shadow+0x318)&&level==0&&Offset(out)==0xffffffff&&!temporary);temporary=out;
 A(out,0)=(unsigned)SurfaceVtable;A(out,4)=A(self,0)+8;A(out,8)=3;A(out,12)=0xabcdef01;Record(10,self);return out;
}
extern "C" void* __fastcall SurfaceAssign(void*self,void*,void* source){
 REQUIRE((self==shadow+0x2f8||self==shadow+0x308)&&source==temporary);A(self,4)=A(source,4);A(self,8)=A(source,8);Record(11,self);return self;
}
extern "C" void __fastcall SurfaceCleanup(void*self,void*){
 if(Offset(self)==0xffffffff){REQUIRE(self==temporary&&A(self,0)==(unsigned)SurfaceVtable);temporary=0;}else REQUIRE(self==shadow+0x2f8||self==shadow+0x308);
 A(self,4)=A(self,8)=0;Record(12,self);
}
extern "C" void __fastcall TextureCleanup(void*self,void*){REQUIRE(self==shadow+0x2e8||self==shadow+0x318);A(self,0)=0;A(self,4)&=0x0fffffff;Record(13,self);}
extern "C" void __fastcall TargetCleanup(void*self,void*){REQUIRE(self==shadow+0x2c4);Record(14,self);}
extern "C" void __fastcall AttachTarget(void*self,void*,void* source){REQUIRE(self==shadow+0x2c4&&source==shadow+0x308);A(self,8)=A(source,4);Record(15,self);}
extern "C" void __fastcall AttachDepth(void*self,void*,void* source){REQUIRE(self==shadow+0x2c4&&source==shadow+0x2f8);A(self,0x18)=A(source,4);Record(16,self);}
extern "C" void __fastcall BuildLookup(void*self,void*){REQUIRE(self==shadow);++builds;if(p[4]==3)A(shadow,0x334)=0;Record(17,self);}
static unsigned Target(unsigned address){
 switch(address){
 case 0xb4b830:return (unsigned)&ShadowCleanup;case 0x9e3830:return (unsigned)&PixelInit;case 0x9be8f0:return (unsigned)&ValidTexture;case 0x9be610:return (unsigned)&BestOpaque;case 0x9be930:return (unsigned)&ValidTarget;case 0x9bee00:return (unsigned)&ShaderVersion;
 case 0x9fa280:return (unsigned)&TextureInit;case 0x9f32a0:return (unsigned)&DepthInit;case 0x9fa1c0:return (unsigned)&TextureAssign;case 0x9f9e00:return (unsigned)&GetLevel;case 0x9f2e70:return (unsigned)&SurfaceAssign;case 0x9f2e20:return (unsigned)&SurfaceCleanup;case 0x9f9f70:return (unsigned)&TextureCleanup;case 0xa10e30:return (unsigned)&TargetCleanup;case 0xa10e60:return (unsigned)&AttachTarget;case 0xa10e90:return (unsigned)&AttachDepth;case 0xb4f420:return (unsigned)&BuildLookup;case 0x1436e1c:return (unsigned)&DisplayGlobal;case 0x122f84c:return (unsigned)SurfaceVtable;
 default:abort();return 0;
 }
}
int main(int argc,char**argv){
 if(argc!=4||DisplayGlobal)return 2;FILE*in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned val=Target(retailFixups[i][1]);if(retailFixups[i][2])val-=(unsigned)(code+retailFixups[i][0]+4);*(unsigned*)(code+retailFixups[i][0])=val;}
 typedef bool(__fastcall *Fn)(void*,void*);Fn fn=strcmp(argv[3],"retail")==0?(Fn)code:&Initialise;
 while(fread(p,4,5,in)==5){
  if(fread(arena,1,8192,in)!=8192)return 6;shadow=arena+p[0];DisplayGlobal=arena+0x1c00;events=textureCalls=fallbackCalls=builds=0;firstValid=true;temporary=0;
  unsigned tech=0,wantCalls=0,wantFallback=0,wantBuilds=0;
  if((long)p[1]>0){
   if((p[2]&3)==3){++wantCalls;if(p[2]&4){++wantCalls;if(p[2]&8)tech=1;}}
   if(!tech&&(p[2]&64)){wantFallback=p[3]<8?p[3]+1:8;if(p[3]<8){++wantCalls;if((p[2]&48)==48){tech=2;wantBuilds=1;}}}
  }
  if(tech==2&&p[4]==3)tech=0;unsigned finalSize=p[4]==2&&wantCalls?512:p[1];
  bool result=fn(shadow,0);REQUIRE(!temporary&&textureCalls==wantCalls&&fallbackCalls==wantFallback&&builds==wantBuilds&&A(shadow,0x334)==tech&&A(shadow,0x338)==finalSize&&shadow[0x2af]==(tech!=0)&&result==(tech!=0||finalSize==0));Record(18,shadow,result,tech);++cases;
 }
 fclose(in);fclose(output);printf("NATIVE_SHADOW_INITIALISE_PASS cases=%u\n",cases);return 0;
}
