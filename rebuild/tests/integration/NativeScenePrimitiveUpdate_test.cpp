#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scene_primitive_update_aliases.h"
#include "scene_primitive_update_retail.h"
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u\n",__LINE__,cases);exit(70);}}while(0)
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Update(void*,unsigned);
extern "C" void* TypeInfoGlobal;
static unsigned char*arena,expected[4096];static unsigned p[4],cases,removed,added,expectedGlobal;static FILE*output;
static unsigned& W(unsigned char*a,unsigned off){return *(unsigned*)(a+off);}
static unsigned Ptr(unsigned off){return (unsigned)(arena+off);}
static void Record(){REQUIRE(!memcmp(arena,expected,4096)&&(unsigned)TypeInfoGlobal==expectedGlobal);fwrite(arena,1,4096,output);fwrite(&TypeInfoGlobal,4,1,output);}
extern "C" void __fastcall Remove(void*self,void*){
 REQUIRE(self==arena+p[0]&&!(p[2]&0x40)&&++removed==1&&added==0);
 if(p[3]==1||p[3]==3)W(arena,p[0]+8)=W(expected,p[0]+8)=46-p[1];
 if(p[3]==2||p[3]==3){TypeInfoGlobal=arena+0x500;expectedGlobal=Ptr(0x500);}
 if(p[3]==3)W(arena,p[0]+12)=W(expected,p[0]+12)=p[2]^0xffffffff;
 if(p[3]==4){TypeInfoGlobal=arena+0x400;expectedGlobal=Ptr(0x400);}
 ++W(arena,0xfc0);++W(expected,0xfc0);Record();
}
extern "C" void __fastcall SceneAdd(void*self,void*,void*primitive,unsigned flags){
 REQUIRE(removed==1&&++added==1&&primitive==arena+p[0]&&flags==p[2]);
 unsigned info=expectedGlobal-Ptr(0),type=W(expected,p[0]+8);REQUIRE(type<47);unsigned manager=W(expected,info+16+4*type)-Ptr(0);unsigned list=W(expected,manager+4)-Ptr(0);unsigned elements=W(expected,list)-Ptr(0);REQUIRE((unsigned)self==W(expected,elements));
 W(arena,0xfc4)=W(expected,0xfc4)=(unsigned)self;W(arena,0xfc8)=W(expected,0xfc8)=flags;Record();
}
int main(int argc,char**argv){
 if(argc!=4||TypeInfoGlobal)return 2;FILE*in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned target=retailFixups[i][1],value=0;switch(target){case 0xb8fe60:value=(unsigned)&Remove;break;case 0x1436e84:value=(unsigned)&TypeInfoGlobal;break;case 0xbacdd0:value=(unsigned)(code+48);break;case 0xb94d30:value=(unsigned)&SceneAdd;break;default:return 6;}if(retailFixups[i][2])value-=(unsigned)(code+retailFixups[i][0]+4);*(unsigned*)(code+retailFixups[i][0])=value;}
 typedef void(__fastcall *Fn)(void*,unsigned);Fn fn=strcmp(argv[3],"retail")==0?(Fn)code:&Update;
 while(fread(p,4,4,in)==4){
  memset(arena,0x95,4096);W(arena,p[0]+8)=p[1];W(arena,p[0]+12)=p[2];
  for(unsigned i=0;i<47;++i){W(arena,0x410+4*i)=Ptr(0x600+16*i);W(arena,0x510+4*i)=Ptr(0x600+16*(46-i));W(arena,0x604+16*i)=Ptr(0xa00+12*i);W(arena,0xa00+12*i)=Ptr(0xd00+4*i);W(arena,0xa04+12*i)=W(arena,0xa08+12*i)=Ptr(0xd04+4*i);W(arena,0xd00+4*i)=Ptr(0xe00+4*i);}
  TypeInfoGlobal=p[3]==4?0:arena+0x400;expectedGlobal=(unsigned)TypeInfoGlobal;memcpy(expected,arena,4096);removed=added=0;
  fn(arena+p[0],p[2]);REQUIRE(removed==((p[2]&0x40)?0:1)&&added==removed);Record();++cases;
 }
 fclose(in);fclose(output);printf("NATIVE_SCENE_PRIMITIVE_UPDATE_PASS cases=%u\n",cases);return 0;
}
