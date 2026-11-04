#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scene_primitive_lists_aliases.h"
#include "scene_primitive_lists_retail.h"
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u\n",__LINE__,cases);exit(70);}}while(0)
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Add(void*,void*,void*);
extern "C" void __fastcall Update(void*,unsigned);
extern "C" void* TypeInfoGlobal;
static unsigned char*arena,expected[8192];static unsigned p[12],expectedGlobal,sceneCalls,cases,order[12],count;static FILE*output;
static unsigned& W(unsigned char*a,unsigned off){return *(unsigned*)(a+off);}
static unsigned Node(unsigned id){return p[0]+0x100+id*0x80;}
static unsigned Ptr(unsigned off){return (unsigned)(arena+off);}
static void Links(unsigned char*a){
 W(a,p[0]+0x20)=count?Ptr(Node(order[0])):0;
 for(unsigned i=0;i<count;++i){unsigned at=Node(order[i]);W(a,at+0x38)=i+1<count?Ptr(Node(order[i+1])):0;W(a,at+0x3c)=Ptr(i?Node(order[i-1])+0x38:p[0]+0x20);W(a,at+0x40)=W(a,at+0x44)=0;}
 unsigned previous=0xffffffff;
 for(unsigned i=0;i<count;++i)if(i==0||W(a,Node(order[i])+0x30)!=W(a,Node(order[i-1])+0x30)){
  unsigned at=Node(order[i]);W(a,at+0x44)=Ptr(previous==0xffffffff?p[0]+0x20:previous+0x40);if(previous!=0xffffffff)W(a,previous+0x40)=Ptr(at);previous=at;
 }
}
static void Record(){REQUIRE(!memcmp(arena,expected,8192)&&(unsigned)TypeInfoGlobal==expectedGlobal);fwrite(arena,1,8192,output);fwrite(&TypeInfoGlobal,4,1,output);}
extern "C" void __fastcall SceneAdd(void*self,void*,void*primitive,unsigned flags){
 REQUIRE(!(p[10]&0x40)&&++sceneCalls==1&&primitive==arena+Node(p[5])&&flags==p[10]&&self==arena+0x1e00+4*p[11]);
 REQUIRE(!memcmp(arena,expected,8192));W(arena,0x1f00)=W(expected,0x1f00)=flags;
 if(p[5]&1){TypeInfoGlobal=0;expectedGlobal=0;}Record();
}
int main(int argc,char**argv){
 if(argc!=4||TypeInfoGlobal)return 2;FILE*in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned target=retailFixups[i][1],value=0;switch(target){case 0xb8fe60:value=(unsigned)(code+112);break;case 0x1436e84:value=(unsigned)&TypeInfoGlobal;break;case 0xbacdd0:value=(unsigned)(code+288);break;case 0xb94d30:value=(unsigned)&SceneAdd;break;default:return 6;}if(retailFixups[i][2])value-=(unsigned)(code+retailFixups[i][0]+4);*(unsigned*)(code+retailFixups[i][0])=value;}

 typedef void(__fastcall *AF)(void*,void*,void*);typedef void(__fastcall *RF)(void*,unsigned);bool native=strcmp(argv[3],"retail")==0;AF add=native?(AF)code:&Add;RF update=native?(RF)(code+240):&Update;
 const unsigned masks[]={1,0x80000000,0xfedcba98};
 while(fread(p,4,12,in)==12){
  memset(arena,0x95,8192);count=0;
  for(unsigned g=0;g<3;++g)for(unsigned j=0;j<p[g+1];++j){unsigned at=Node(count);order[count]=count;W(arena,at+0x30)=masks[g];++count;}
  unsigned added=count,selected=p[5],at=Node(added);REQUIRE(selected<=added);W(arena,at+0x30)=p[4];W(arena,at+0x38)=W(arena,at+0x3c)=W(arena,at+0x40)=W(arena,at+0x44)=0;
  for(unsigned i=0;i<=count;++i){unsigned n=Node(i);W(arena,n+8)=p[11];W(arena,n+0x20)=p[7]?Ptr(0xf00):0;arena[n+0x34]=(unsigned char)p[6];}
  W(arena,0xf34)=p[8];W(arena,0xf38)=p[9];Links(arena);
  for(unsigned i=0;i<47;++i){W(arena,0x1010+4*i)=Ptr(0x1200+16*i);W(arena,0x1204+16*i)=Ptr(0x1600+12*i);W(arena,0x1600+12*i)=Ptr(0x1a00+4*i);W(arena,0x1604+12*i)=W(arena,0x1608+12*i)=Ptr(0x1a04+4*i);W(arena,0x1a00+4*i)=Ptr(0x1e00+4*i);}
  TypeInfoGlobal=arena+0x1000;expectedGlobal=Ptr(0x1000);sceneCalls=0;memcpy(expected,arena,8192);
  unsigned insertion=0;for(unsigned i=0;i<count;++i)if(W(expected,Node(order[i])+0x30)==p[4]){insertion=i+1;break;}
  for(unsigned i=count;i>insertion;--i)order[i]=order[i-1];order[insertion]=added;++count;Links(expected);
  add(arena+Node(added),0,arena+p[0]+0x20);Record();
  if(!(p[10]&0x40)){unsigned off=Node(selected);if(!(p[6]&2)&&p[7]){W(expected,0xf34)=p[8]-1;W(expected,off+0x20)=0;}
  unsigned pos=0;while(order[pos]!=selected)++pos;for(unsigned i=pos;i+1<count;++i)order[i]=order[i+1];--count;Links(expected);W(expected,off+0x38)=W(expected,off+0x3c)=W(expected,off+0x40)=W(expected,off+0x44)=0;
  } update(arena+Node(selected),p[10]);REQUIRE(sceneCalls==((p[10]&0x40)?0:1));Record();++cases;
 }
 fclose(in);fclose(output);printf("NATIVE_SCENE_PRIMITIVE_LISTS_PASS cases=%u\n",cases);return 0;
}
