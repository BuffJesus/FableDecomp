#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "primitive_add_aliases.h"
#include "primitive_add_retail.h"
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u\n",__LINE__,cases);exit(70);}}while(0)
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall AddPrimitive(void*,void*,void*);
extern "C" void* HandleTable[];extern "C" void* BaseTable[];
static unsigned char *arena,expected[1024];static unsigned p[6],cases,creates,notifications,deletes,initial,finalPointee;static void* temporary;static FILE* output;
static unsigned& W(unsigned char*a,unsigned off){return *(unsigned*)(a+off);}
static void Record(){REQUIRE(!memcmp(arena,expected,1024));fwrite(arena,1,1024,output);}
static void CheckHandle(){REQUIRE(temporary&&(unsigned)temporary<(unsigned)arena&&*(void**)temporary==HandleTable);}
extern "C" void __fastcall Persistent(void*self,void*,void*h,void*v,unsigned flags,unsigned nonpersistent){
 REQUIRE(self==arena+p[0]*64&&v==arena+(p[5]==0?0x80:p[5]==1?0x100:0x180)&&flags==4&&nonpersistent==1&&++creates==1);temporary=h;CheckHandle();REQUIRE(((unsigned*)h)[1]==0);Record();
 ((unsigned*)h)[1]=initial;finalPointee=initial;Record();
}
static void __fastcall Notify(void*self,void*,void*h){
 REQUIRE((unsigned)self==initial&&h==temporary&&++notifications==1&&p[2]==1);CheckHandle();Record();
 if(p[4]==1)finalPointee=0;
 if(p[4]==2)finalPointee=(unsigned)(arena+(self==arena+0x100?0x180:0x100));
 if(p[4]==3){unsigned off=(unsigned char*)self-arena;W(arena,off+4)=W(expected,off+4)=2;}
 if(p[4]==4){unsigned off=(unsigned char*)self-arena;W(arena,off+4)=W(expected,off+4)=0;}
 ((unsigned*)h)[1]=finalPointee;Record();
}
static void __fastcall Destroy(void*self,void*){
 REQUIRE((unsigned)self==finalPointee&&++deletes==1);unsigned off=(unsigned char*)self-arena;REQUIRE(W(expected,off+4)==1);W(expected,off+4)=0;CheckHandle();++W(arena,off+12);++W(expected,off+12);if(p[4]==5)((unsigned*)temporary)[1]=(unsigned)(arena+0x200);Record();
}
int main(int argc,char**argv){
 if(argc!=4)return 2;FILE*in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<7;++i){unsigned v;switch(retailFixups[i][1]){case 0x99a2d0:v=(unsigned)(code+144);break;case 0x99a2e0:v=(unsigned)(code+160);break;case 0x122f598:v=(unsigned)HandleTable;break;case 0x1231710:v=(unsigned)BaseTable;break;case 0xb324a0:v=(unsigned)&Persistent;break;default:return 8;}*(unsigned*)(code+retailFixups[i][0])=retailFixups[i][2]?v-(unsigned)(code+retailFixups[i][0]+4):v;}
 static void* table[4];table[1]=(void*)&Destroy;table[3]=(void*)&Notify;typedef void(__fastcall*Fn)(void*,void*,void*);Fn fn=!strcmp(argv[3],"retail")?(Fn)code:&AddPrimitive;
 while(fread(p,4,6,in)==6){if(fread(arena,1,1024,in)!=1024)return 6;W(arena,0x100)=W(arena,0x180)=(unsigned)table;W(arena,0x104)=W(arena,0x184)=p[3];initial=p[1]?(unsigned)(arena+(p[1]==1?0x100:0x180)):0;if(initial)W(arena,initial-(unsigned)arena+4)=p[2];memcpy(expected,arena,1024);creates=notifications=deletes=0;temporary=0;
  fn(arena+p[0]*64,0,arena+(p[5]==0?0x80:p[5]==1?0x100:0x180));REQUIRE(creates==1&&notifications==(initial&&p[2]==1?1:0));if(finalPointee){unsigned off=finalPointee-(unsigned)arena;if(!deletes){--W(expected,off+4);REQUIRE(W(expected,off+4)!=0);}else REQUIRE(W(expected,off+4)==0);}else REQUIRE(deletes==0);Record();++cases;
 }fclose(in);fclose(output);printf("NATIVE_PRIMITIVE_ADD_PASS cases=%u\n",cases);return 0;
}
