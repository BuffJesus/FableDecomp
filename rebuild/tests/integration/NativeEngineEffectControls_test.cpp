#include "fable_engine_internal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine_effect_aliases.h"
#include "engine_effect_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Spotlight(void*,void*,unsigned);
extern "C" void __fastcall Filter(void*,void*,const void*);
extern "C" void __fastcall Sea(void*,void*,unsigned);
extern "C" bool __fastcall Deallocate(void*);
extern "C" void __fastcall Reallocate(void*);
static unsigned char* arena;static FILE* output;static unsigned mode,events;
static unsigned& at(unsigned off){return *(unsigned*)(arena+off);}
static void record(unsigned kind,unsigned arg){if(++events>16)exit(20);unsigned h[6]={kind,arg,(unsigned)NEngineInternal::GPLightingManager,(unsigned)NEngineInternal::GPScreenEffectColourFilter,(unsigned)NEngineInternal::GPWaterRenderer,(unsigned)NEngineInternal::GPShadowRenderer};if(fwrite(h,4,6,output)!=6||fwrite(arena,1,32768,output)!=32768)exit(9);}
extern "C" void* __fastcall CopyString(void* self,void*,const void* other){unsigned value=*(unsigned*)other;record(1,value);*(unsigned*)self=value;if(value)++*(unsigned*)value;if(mode==1)NEngineInternal::GPWaterRenderer=(CEngineWaterRenderer*)(arena+0xc00);if(mode==4){*(unsigned*)other=(unsigned)(arena+0x180);++at(0x180);}return self;}
extern "C" void* __fastcall AssignString(void* self,void*,const void* other){unsigned value=*(unsigned*)other;record(2,(unsigned)self);*(unsigned*)self=value;if(value)++*(unsigned*)value;if(mode==2)NEngineInternal::GPWaterRenderer=(CEngineWaterRenderer*)(arena+0xc00);return self;}
extern "C" void __fastcall DestroyString(void* self){unsigned value=*(unsigned*)self;record(3,value);if(value)--*(unsigned*)value;if(mode==3)NEngineInternal::GPWaterRenderer=(CEngineWaterRenderer*)(arena+0xc00);}
int main(int argc,char** argv){
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 if(NEngineInternal::GPLightingManager||NEngineInternal::GPScreenEffectColourFilter||NEngineInternal::GPWaterRenderer||NEngineInternal::GPShadowRenderer)return 5;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<11;++i){unsigned off=retailFixups[i][0],v=0;switch(retailFixups[i][1]){
 case 0x1436e9c:v=(unsigned)&NEngineInternal::GPLightingManager;break;case 0x1436e40:v=(unsigned)&NEngineInternal::GPScreenEffectColourFilter;break;case 0x1436e54:v=(unsigned)&NEngineInternal::GPWaterRenderer;break;case 0x1436e60:v=(unsigned)&NEngineInternal::GPShadowRenderer;break;case 0x99ec30:v=(unsigned)CopyString;break;case 0x99efb0:v=(unsigned)AssignString;break;case 0x99eae0:v=(unsigned)DestroyString;break;case 0xb4b890:v=(unsigned)(code+256);break;case 0xb4b8a0:v=(unsigned)(code+272);break;default:return 8;}
 *(unsigned*)(code+off)=retailFixups[i][2]?v-(unsigned)(code+off+4):v;}
 typedef void (__fastcall *Setter)(void*,void*,unsigned);typedef void (__fastcall *FilterFn)(void*,void*,const void*);typedef bool (__fastcall *BoolFn)(void*);typedef void (__fastcall *VoidFn)(void*);
 bool nativeMode=argv[3][0]=='r';Setter spot=nativeMode?(Setter)code:Spotlight,sea=nativeMode?(Setter)(code+128):Sea;FilterFn filter=nativeMode?(FilterFn)(code+64):Filter;BoolFn deallocate=nativeMode?(BoolFn)(code+208):Deallocate;VoidFn reallocate=nativeMode?(VoidFn)(code+224):Reallocate;
 unsigned p[6],cases=0;while(fread(p,4,6,in)==6){if(fread(arena,1,32768,in)!=32768)return 6;mode=p[0];events=0;unsigned slot=p[1],filterOffset=slot?0x400:0x200,lightOffset=slot?0x2000:0x1000;NEngineInternal::GPLightingManager=(CEngineLightingManager*)(arena+lightOffset);NEngineInternal::GPScreenEffectColourFilter=(CEngineScreenEffectColourFilter*)(arena+filterOffset);NEngineInternal::GPWaterRenderer=(CEngineWaterRenderer*)(arena+(slot?0xc00:0x800));NEngineInternal::GPShadowRenderer=(CEngineShadowRenderer*)(arena+0x7000);
  spot(arena,0,p[2]);if(at(lightOffset+0x4694)!=((p[2]&255)?2u:1u))return 7;record(4,p[2]);
  unsigned expected[8];memcpy(expected,arena+p[3],32);filter(arena,0,arena+p[3]);if(!p[4]&&memcmp(expected,arena+filterOffset+16,32))return 10;record(5,p[4]);
  sea(arena,0,p[5]?(unsigned)(arena+0x100):0);record(6,0);
  if(deallocate(arena))return 11;record(7,0);reallocate(arena);record(8,0);++cases;
 }
 fclose(in);fclose(output);printf("NATIVE_ENGINE_EFFECT_CONTROLS_PASS cases=%u\n",cases);return 0;
}
