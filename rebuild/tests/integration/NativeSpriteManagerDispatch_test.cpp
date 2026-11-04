#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine/CEnginePrimitiveManager2DSprites.h"
#include "engine/CEnginePrimitiveManagerSprites.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "sprite_dispatch_aliases.h"
#include "sprite_dispatch_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall TwoD(void*,void*,void*,void*,unsigned);
extern "C" unsigned __fastcall Noop(void*,void*,void*,unsigned,void*);
extern "C" unsigned __fastcall ThreeD(void*,void*,void*,unsigned,void*);
static unsigned char *arena,initial[4096],expected[4096];static unsigned cases,snapshots,kind,mode,type,layer,wanted,calls;static void*table[15],*replacement[15];
static unsigned& W(unsigned char*a,unsigned o){return *(unsigned*)(a+o);}
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u\n",__LINE__,cases);exit(70);}}while(0)
static void Mutation(unsigned char*a){a[4095]^=(unsigned char)(wanted+7);if(mode==1)a[0x108]=0;else if(mode==2)W(a,0x208)=35;else if(mode==3){W(a,0x100)=(unsigned)replacement;W(a,0x114)^=0x1234567;}}
static void Visit(unsigned selected,void*self,void*primitive,void*chain,unsigned argument){REQUIRE(calls++==0&&selected==wanted&&self==arena+0x100&&primitive==arena+0x200&&((kind==0&&chain==arena+0x400&&argument==type)||(kind==2&&!chain&&!argument))&&!memcmp(arena,initial,sizeof(initial)));++snapshots;Mutation(arena);}
extern "C" void __fastcall Clock(void*self,void*,void*primitive,void*chain,unsigned argument){Visit(1,self,primitive,chain,argument);}
extern "C" void __fastcall Sprite(void*self,void*,void*primitive,void*chain,unsigned argument){Visit(2,self,primitive,chain,argument);}
extern "C" void __fastcall SpriteBoundary(void*self,void*,void*primitive){Visit(3,self,primitive,0,0);}
extern "C" void __fastcall Trap(){REQUIRE(false);}
static unsigned Invoke(){CEnginePrimitiveManager*owner=(CEnginePrimitiveManager*)(arena+0x100);CEngineInternalPrimitiveBase&primitive=*(CEngineInternalPrimitiveBase*)(arena+0x200);CEngineSubPrimitiveChain&chain=*(CEngineSubPrimitiveChain*)(arena+0x400);if(kind==0){owner->Render2DPrimitive(primitive,chain,(ESubPrimitiveType)type);return 0;}return owner->RenderPrimitive(primitive,(EEngineRenderLayer)layer,chain);}
int main(){arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);REQUIRE(arena==(void*)0x24000000);unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);REQUIRE(code);memcpy(code,retailCode,sizeof(retailCode));*(unsigned*)(code+spriteFixup)=(unsigned)&SpriteBoundary-(unsigned)(code+spriteFixup+4);
 const unsigned flags[]={0,1,2,255},types[]={0,1,34,35,36,0xffffffff};
 for(kind=0;kind<3;++kind)for(unsigned flag=0;flag<4;++flag)for(unsigned t=0;t<6;++t)for(type=0;type<16;++type)for(mode=0;mode<4;++mode){
  layer=type*0x10203041;for(unsigned i=0;i<4096;++i)arena[i]=(unsigned char)(i*17+t*31);W(arena,0x100)=(unsigned)table;arena[0x108]=(unsigned char)flags[flag];W(arena,0x208)=types[t];wanted=!flags[flag]||kind==1?0:kind==2?3:types[t]==35?1:2;
  memcpy(initial,arena,sizeof(initial));memcpy(expected,initial,sizeof(initial));if(wanted)Mutation(expected);
  for(unsigned run=0;run<2;++run){for(i=0;i<15;++i)table[i]=replacement[i]=(void*)&Trap;table[13]=(void*)&Clock;table[14]=(void*)&Sprite;table[10]=run?(void*)&TwoD:(void*)(code+0);table[7]=kind==1?(run?(void*)&Noop:(void*)(code+64)):(run?(void*)&ThreeD:(void*)(code+80));memcpy(arena,initial,sizeof(initial));calls=0;unsigned result=Invoke();REQUIRE(result==(kind==2&&flags[flag]?2:0)&&calls==(wanted?1:0)&&!memcmp(arena,expected,sizeof(expected)));++snapshots;}++cases;
 }printf("NATIVE_SPRITE_DISPATCH_PASS cases=%u snapshots=%u\n",cases,snapshots);return 0;}
