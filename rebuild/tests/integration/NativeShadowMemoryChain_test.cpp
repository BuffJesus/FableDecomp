#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_display_viewport.h"
#include "shadow_memory_chain_aliases.h"
#include "shadow_memory_chain_retail.h"
#define REQUIRE(x) do {if(!(x)){fprintf(stderr,"line %d case %u\n",__LINE__,cases);exit(70);}}while(0)
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" bool __fastcall Initialise(void*,void*);
extern "C" const unsigned char FormatTable[1692];
extern "C" void* DisplayGlobal;extern "C" void* SurfaceVtable[];
static unsigned expectedBytes[2],descriptionCalls;
static unsigned opaqueCalls,rootCalls,depthCalls,calcCalls,successfulTextures;
static unsigned char* nativeCode;
static unsigned char* arena;static unsigned char* shadow;static unsigned p[5],cases,events,textureCalls,fallbackCalls,builds,depthSize,fallbackSize;static bool firstValid;static void* temporary;static FILE* output;
static unsigned& A(void* x,unsigned off){return *(unsigned*)((unsigned char*)x+off);}
static unsigned Offset(void* self){unsigned off=(unsigned char*)self-arena;return off<8192?off:0xffffffff;}
static void Record(unsigned tag,void*self=0,unsigned a=0,unsigned b=0,unsigned c=0,unsigned d=0){
 REQUIRE(++events<100);unsigned words[7]={tag,Offset(self),a,b,c,d,(unsigned)DisplayGlobal};fwrite(words,4,7,output);fwrite(arena,1,8192,output);
}
static unsigned FormatCode(const unsigned* format){if(*format==0xffffffff)return 0;REQUIRE(*format<46);return *(const unsigned*)(FormatTable+36**format);}
static unsigned IndexFor(unsigned code){for(unsigned i=0;i<46;++i)if(*(const unsigned*)(FormatTable+36*i)==code)return i;return 0xffffffff;}
static void CheckDisplayArgs(void*self,unsigned adapter,unsigned device,unsigned mode){
 CDisplayManager* display=(CDisplayManager*)DisplayGlobal;REQUIRE(self==display->D3D&&adapter==display->D3DDeviceCaps.AdapterOrdinal&&device==(unsigned)display->D3DDeviceCaps.DeviceType&&mode==(unsigned)display->CurrentMode.Format);
}
static long __stdcall CheckFormat(void*self,unsigned adapter,unsigned device,unsigned mode,unsigned usage,unsigned type,unsigned format){
 CheckDisplayArgs(self,adapter,device,mode);REQUIRE(type==3);if(usage==1){REQUIRE(format==23&&++opaqueCalls==1);Record(4,self,format);return 0;}REQUIRE(usage==0);bool ok;
 if(firstValid){REQUIRE(format==80);firstValid=false;ok=(p[2]&1)!=0;}
 else{REQUIRE(fallbackCalls++==0&&format==21);ok=true;}
 if(p[4]==1)DisplayGlobal=arena+0x1c40;Record(3,self,format,ok);return ok?0:(long)0x80004005;
}
static long __stdcall CheckTarget(void*self,unsigned adapter,unsigned device,unsigned mode,unsigned colour,unsigned depth){
 CheckDisplayArgs(self,adapter,device,mode);REQUIRE(depth==80&&(colour==23||colour==21));bool ok=colour==23?(p[2]&2)!=0:true;Record(5,self,colour,depth,ok);return ok?0:(long)0x80004005;
}
extern "C" void* __fastcall GetSystem(){++rootCalls;A(arena,0x1be0)=(unsigned)DisplayGlobal;Record(22,arena+0x1b80);return arena+0x1b80;}
static unsigned __stdcall GetLevelCount(void*self){
 REQUIRE(self==arena+0x1800||self==arena+0x1820);unsigned role=self==arena+0x1820?1:0;void*texture=shadow+(role?0x318:0x2e8);
 REQUIRE((A(texture,4)&0xf0000000)==0x10000000&&(A(texture,4)&0x0fffffff)==0);++calcCalls;
 unsigned index=IndexFor(A(self,0x10));REQUIRE(index<46);unsigned bits=*(const unsigned*)(FormatTable+36*index+8);
 unsigned bytes=(bits*A(self,8)*A(self,12))>>3;expectedBytes[role]=bytes&0x0fffffff;Record(23,self);return 1;
}
static long __stdcall GetLevelDesc(void*self,unsigned level,D3DSURFACE_DESC*out){
 REQUIRE((self==arena+0x1800||self==arena+0x1820)&&level==0&&Offset(out)==0xffffffff);++descriptionCalls;
 memset(out,0,sizeof(*out));out->Format=(D3DFORMAT)A(self,0x10);out->Width=A(self,8);out->Height=A(self,12);Record(24,self,level);return 0;
}
static long __stdcall CreateTexture(void* device,unsigned x,unsigned y,unsigned levels,unsigned flags,unsigned format,unsigned pool,void** out,void* shared){
 REQUIRE(device==((CDisplayManager*)DisplayGlobal)->D3DDevice&&levels==1&&pool==0&&shared==0&&x==y);void*self=out;bool ok;
 if(self==shadow+0x2e8&&flags==2){REQUIRE(format==80&&x==A(shadow,0x338));depthSize=x;ok=(p[2]&4)!=0;}
 else if(self==shadow+0x318){REQUIRE(flags==1&&format==23&&x==depthSize);ok=(p[2]&8)!=0;}
 else{REQUIRE(self==shadow+0x2e8&&flags==1&&format==21&&x==A(shadow,0x338));fallbackSize=x;ok=(p[2]&16)!=0;}
 if(ok){*out=arena+0x1800+(self==shadow+0x318?32:0);A(*out,8)=x;A(*out,12)=y;A(*out,0x10)=format;++successfulTextures;}
 if(p[4]==2)A(shadow,0x338)=512;++textureCalls;Record(7,self,x,format,flags,ok);return ok?0:(long)0x80004005;
}
static long __stdcall CreateDepth(void*device,unsigned x,unsigned y,unsigned format,unsigned multi,unsigned quality,unsigned discard,void**out,void*shared){
 void*self=(unsigned char*)out-4;REQUIRE(device==((CDisplayManager*)DisplayGlobal)->D3DDevice&&self==shadow+0x2f8&&x==fallbackSize&&x==y&&format==80&&multi==0&&quality==0&&discard==0&&shared==0);++depthCalls;bool ok=(p[2]&32)!=0;
 if(ok)*out=arena+0x1840;Record(8,self,x,ok);return ok?0:(long)0x80004005;
}
static unsigned __stdcall AddRef(void*self){++A(self,4);Record(19,self);return A(self,4);}
static unsigned __stdcall Release(void*self){--A(self,4);Record(20,self);return A(self,4);}
static long __stdcall Mipmap(void*self,unsigned level,void** out){
 REQUIRE(level==0&&Offset(out)==0xffffffff&&(self==arena+0x1800||self==arena+0x1820));
 *out=arena+(self==arena+0x1800?0x1880:0x18a0);Record(21,self);return 0;
}
extern "C" void __fastcall BuildLookup(void*self,void*){REQUIRE(self==shadow);++builds;expectedBytes[1]=expectedBytes[0];if(p[4]==3)A(shadow,0x334)=0;Record(17,self);}
static unsigned Target(unsigned address){
 switch(address){
 case 0xb4b830:return (unsigned)(nativeCode+816);case 0x9e3830:return (unsigned)(nativeCode+1552);case 0x9be8f0:return (unsigned)(nativeCode+1648);case 0x9be610:return (unsigned)(nativeCode+1792);case 0x9be930:return (unsigned)(nativeCode+1712);case 0x9bee00:return (unsigned)(nativeCode+1616);
 case 0x9fa280:return (unsigned)(nativeCode+2080);case 0x9f32a0:return (unsigned)(nativeCode+1968);case 0x9fa1c0:return (unsigned)(nativeCode+1200);case 0x9f9e00:return (unsigned)(nativeCode+1312);case 0x9f2e70:return (unsigned)(nativeCode+1040);case 0x9f2e20:return (unsigned)(nativeCode+1120);case 0x9f9f70:return (unsigned)(nativeCode+1152);case 0xa10e30:return (unsigned)(nativeCode+896);case 0xa10e60:return (unsigned)(nativeCode+944);case 0xa10e90:return (unsigned)(nativeCode+992);case 0xb4f420:return (unsigned)&BuildLookup;case 0x1436e1c:return (unsigned)&DisplayGlobal;case 0x122f84c:return (unsigned)SurfaceVtable;
 case 0x9f2d60:return (unsigned)(nativeCode+1408);case 0x9f2f10:return (unsigned)(nativeCode+1472);case 0x129ba40:return (unsigned)FormatTable;case 0x129ba48:return (unsigned)(FormatTable+8);case 0x9a4ec0:return (unsigned)&GetSystem;case 0x9f9ee0:return (unsigned)(nativeCode+2272);case 0x9e3820:return (unsigned)(nativeCode+2416);default:abort();return 0;
 }
}
int main(int argc,char**argv){
 if(argc!=4||DisplayGlobal)return 2;FILE*in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;nativeCode=code;memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned val=Target(retailFixups[i][1]);if(retailFixups[i][2])val-=(unsigned)(code+retailFixups[i][0]+4);*(unsigned*)(code+retailFixups[i][0])=val;}
 static void* allocationTable[30];allocationTable[23]=(void*)&CreateTexture;allocationTable[29]=(void*)&CreateDepth;
 static void* capabilityTable[13];capabilityTable[10]=(void*)&CheckFormat;capabilityTable[12]=(void*)&CheckTarget;
 static void* resourceTable[19];resourceTable[13]=(void*)&GetLevelCount;resourceTable[17]=(void*)&GetLevelDesc;resourceTable[1]=(void*)&AddRef;resourceTable[2]=(void*)&Release;resourceTable[18]=(void*)&Mipmap;
 unsigned char immutableTable[1692];memcpy(immutableTable,FormatTable,1692);
 typedef bool(__fastcall *Fn)(void*,void*);Fn fn=strcmp(argv[3],"retail")==0?(Fn)code:&Initialise;
 while(fread(p,4,5,in)==5){
  if(fread(arena,1,8192,in)!=8192)return 6;shadow=arena+p[0];DisplayGlobal=arena+0x1c00;events=textureCalls=fallbackCalls=builds=0;firstValid=true;temporary=0;opaqueCalls=rootCalls=depthCalls=calcCalls=successfulTextures=descriptionCalls=0;expectedBytes[0]=A(shadow,0x2ec)&0x0fffffff;expectedBytes[1]=A(shadow,0x31c)&0x0fffffff;
  for(unsigned di=0;di<2;++di){CDisplayManager* display=(CDisplayManager*)(arena+0x1c00+0x40*di);A(arena,0x1b00+0x40*di)=(unsigned)capabilityTable;display->D3D=(IDirect3D9*)(arena+0x1b00+0x40*di);display->D3DDeviceCaps.AdapterOrdinal=17+di;display->D3DDeviceCaps.DeviceType=(D3DDEVTYPE)(1+di);display->CurrentMode.Format=(D3DFORMAT)22;A(arena,0x1b20+0x40*di)=(unsigned)allocationTable;display->D3DDevice=(IDirect3DDevice9*)(arena+0x1b20+0x40*di);}

  ((CDisplayManager*)(arena+0x1c00))->D3DDeviceCaps.PixelShaderVersion=((CDisplayManager*)(arena+0x1c40))->D3DDeviceCaps.PixelShaderVersion=(p[2]&64)?0xffff0104:0xffff0103;
  for(unsigned ri=0;ri<24;++ri)A(arena,0x1800+32*ri)=(unsigned)resourceTable;
  const unsigned slots[6]={0x2cc,0x2dc,0x30c,0x2fc,0x318,0x2e8};for(ri=0;ri<6;++ri)A(shadow,slots[ri])=(p[2]&(1<<ri))?(unsigned)(arena+0x1900+32*ri):0;
  unsigned tech=0,wantCalls=0,wantFallback=0,wantBuilds=0;
  if((long)p[1]>0){
   if((p[2]&3)==3){++wantCalls;if(p[2]&4){++wantCalls;if(p[2]&8)tech=1;}}
   if(!tech&&(p[2]&64)){wantFallback=p[3]<8?p[3]+1:8;if(p[3]==0){++wantCalls;if((p[2]&48)==48){tech=2;wantBuilds=1;}}}
  }
  if(tech==2&&p[4]==3)tech=0;unsigned finalSize=p[4]==2&&wantCalls?512:p[1];
  bool result=fn(shadow,0);REQUIRE((A(shadow,0x2ec)&0x0fffffff)==expectedBytes[0]&&(A(shadow,0x31c)&0x0fffffff)==expectedBytes[1]&&descriptionCalls==2*calcCalls&&rootCalls==textureCalls+depthCalls&&calcCalls==successfulTextures&&opaqueCalls==((long)p[1]>0&&(p[2]&1)?1:0)&&!memcmp(immutableTable,FormatTable,1692)&&!temporary&&textureCalls==wantCalls&&fallbackCalls==wantFallback&&builds==wantBuilds&&A(shadow,0x334)==tech&&A(shadow,0x338)==finalSize&&shadow[0x2af]==(tech!=0)&&result==(tech!=0||finalSize==0));Record(18,shadow,result,tech);++cases;
 }
 fclose(in);fclose(output);printf("NATIVE_SHADOW_MEMORY_CHAIN_PASS cases=%u\n",cases);return 0;
}
