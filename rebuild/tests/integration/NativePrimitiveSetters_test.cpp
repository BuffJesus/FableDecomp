#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "primitive_setters_aliases.h"
#include "primitive_setters_retail.h"
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u stage %u\n",__LINE__,cases,stage);exit(70);}}while(0)
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Animation(void*,void*,void*,void*);
extern "C" void __fastcall Position(void*,void*,void*,void*);
extern "C" void __fastcall DirectAnimation(void*,void*,void*,void*);
extern "C" void __fastcall DirectPosition(void*,void*,void*,void*);
extern "C" void* PrimitiveGlobal;extern "C" void* ResourceGlobal;
static unsigned char*arena,expected[4096];static unsigned p[7],cases,stage,sets,updates,current,expectPrimitive,expectResource;static FILE*output;
static unsigned& W(unsigned char*a,unsigned off){return *(unsigned*)(a+off);}
static void Record(){REQUIRE(!memcmp(arena,expected,4096)&&(unsigned)PrimitiveGlobal==expectPrimitive&&(unsigned)ResourceGlobal==expectResource);fwrite(arena,1,4096,output);fwrite(&PrimitiveGlobal,4,1,output);fwrite(&ResourceGlobal,4,1,output);}
static void Set(void*self,void*arg,unsigned which){
 REQUIRE(which==stage&&(unsigned)self==current&&++sets==1&&arg==arena+p[5]);unsigned off=current-(unsigned)arena;unsigned value=W(expected,p[5]);
 W(arena,off+0x10)=W(expected,off+0x10)=value^(0x12345678+stage);
 if(p[3]==1)arena[off+0x24]=expected[off+0x24]=(unsigned char)(expected[off+0x24]^0x80);
 if(p[3]>=2){W(arena,0x804)=W(expected,0x804)=p[3]==2?0:(unsigned)(arena+0xb00);arena[off+0x24]=expected[off+0x24]=0;W(arena,off+12)=W(expected,off+12)=value^0xabcdef01;}
 if(p[3]==3){PrimitiveGlobal=arena+0x400;expectPrimitive=(unsigned)PrimitiveGlobal;}
 Record();
}
static void __fastcall SetAnimation(void*self,void*,void*arg){Set(self,arg,0);}
static void __fastcall SetPosition(void*self,void*,void*arg){Set(self,arg,1);}
extern "C" void __fastcall Update(void*self,unsigned flags){
 REQUIRE((unsigned)self==current&&sets==1&&++updates==1);unsigned off=current-(unsigned)arena;REQUIRE((signed char)expected[off+0x24]>=0&&flags==W(expected,off+12));
 W(arena,off+0x1c)=W(expected,off+0x1c)=flags^0x87654321;Record();
}
extern "C" void* __fastcall AllocateExternal(void*,void*,unsigned,void*,long,void*){abort();return 0;}
extern "C" void __fastcall FreeExternal(void*,void*,void*){abort();}
int main(int argc,char**argv){
 if(argc!=4||PrimitiveGlobal||ResourceGlobal)return 2;FILE*in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned target=retailFixups[i][1],value=0;switch(target){case 0x1436e80:value=(unsigned)&PrimitiveGlobal;break;case 0xb31e90:value=(unsigned)(code+32);break;case 0xb31ec0:value=(unsigned)(code+80);break;case 0xb91840:value=(unsigned)&Update;break;default:return 6;}if(retailFixups[i][2])value-=(unsigned)(code+retailFixups[i][0]+4);*(unsigned*)(code+retailFixups[i][0])=value;}
 static void*vt[25];vt[23]=(void*)&SetAnimation;vt[24]=(void*)&SetPosition;typedef void(__fastcall *Fn)(void*,void*,void*,void*);bool native=strcmp(argv[3],"retail")==0;
 while(fread(p,4,7,in)==7){
  memset(arena,0x95,4096);W(arena,0xa00)=W(arena,0xb00)=(unsigned)vt;W(arena,0x804)=p[1]?(unsigned)(arena+p[1]):0;arena[0xa24]=arena[0xb24]=(unsigned char)p[2];W(arena,0xa0c)=W(arena,0xb0c)=p[4];
  PrimitiveGlobal=arena+p[0];ResourceGlobal=arena+0x700;expectPrimitive=(unsigned)PrimitiveGlobal;expectResource=(unsigned)ResourceGlobal;memcpy(expected,arena,4096);
  for(stage=0;stage<2;++stage){
   current=W(expected,0x804);sets=updates=0;Fn fn=native?(Fn)(code+(p[6]?(stage?80:32):(stage?16:0))):(p[6]?(stage?&DirectPosition:&DirectAnimation):(stage?&Position:&Animation));
   fn(arena+p[0],0,arena+0x800,arena+p[5]);unsigned wantUpdate=current&&((signed char)expected[current-(unsigned)arena+0x24]>=0);REQUIRE(sets==(current?1:0)&&updates==wantUpdate);Record();
  }
  ++cases;
 }
 fclose(in);fclose(output);printf("NATIVE_PRIMITIVE_SETTERS_PASS cases=%u\n",cases);return 0;
}
