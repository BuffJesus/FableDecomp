#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "primitive_2d_lists_aliases.h"
#include "primitive_2d_lists_retail.h"
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"line %d case %u\n",__LINE__,cases);exit(70);}}while(0)
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Add(void*,void*,void*);
extern "C" void __fastcall Remove(void*,void*);
static unsigned char*arena,expected[4096];static unsigned p[10],cases,order[12],count,head;static FILE*output;
static unsigned& W(unsigned char*a,unsigned off){return *(unsigned*)(a+off);}
static unsigned Node(unsigned id){return p[0]+0x100+id*0x80;}
static unsigned Ptr(unsigned off){return (unsigned)(arena+off);}
static void Links(unsigned char*a){
 W(a,head)=count?Ptr(Node(order[0])):0;
 for(unsigned i=0;i<count;++i){unsigned at=Node(order[i]);W(a,at+0x38)=i+1<count?Ptr(Node(order[i+1])):0;W(a,at+0x3c)=Ptr(i?Node(order[i-1])+0x38:head);W(a,at+0x40)=W(a,at+0x44)=0;}
 unsigned previous=0xffffffff;
 for(unsigned i=0;i<count;++i)if(i==0||W(a,Node(order[i])+0x30)!=W(a,Node(order[i-1])+0x30)){
  unsigned at=Node(order[i]);W(a,at+0x44)=Ptr(previous==0xffffffff?head:previous+0x40);if(previous!=0xffffffff)W(a,previous+0x40)=Ptr(at);previous=at;
 }
}
static void Record(){REQUIRE(!memcmp(arena,expected,4096));fwrite(arena,1,4096,output);}
int main(int argc,char**argv){
 if(argc!=4)return 2;FILE*in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char*code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);if(!code)return 5;memcpy(code,retailCode,sizeof(retailCode));*(unsigned*)(code+240+22)=(unsigned)code-(unsigned)(code+240+26);*(unsigned*)(code+240+34)=(unsigned)code-(unsigned)(code+240+38);
 typedef void(__fastcall *AF)(void*,void*,void*);typedef void(__fastcall *RF)(void*,void*);bool native=strcmp(argv[3],"retail")==0;AF add=native?(AF)(code+240):&Add;RF remove=native?(RF)(code+112):&Remove;
 const unsigned masks[]={1,0x80000000,0xfedcba98};
 while(fread(p,4,10,in)==10){
  memset(arena,0x95,4096);count=0;head=p[0]+((p[9]&0x400)?0x44:0x40);W(arena,p[0]+((p[9]&0x400)?0x40:0x44))=0;
  for(unsigned g=0;g<3;++g)for(unsigned j=0;j<p[g+1];++j){unsigned at=Node(count);order[count]=count;W(arena,at+0x30)=masks[g];++count;}
  unsigned added=count,selected=p[5],at=Node(added);REQUIRE(selected<=added);W(arena,at+0x30)=p[4];W(arena,at+0xc)=p[9];W(arena,at+0x38)=W(arena,at+0x3c)=W(arena,at+0x40)=W(arena,at+0x44)=0;
  for(unsigned i=0;i<=count;++i){unsigned n=Node(i);W(arena,n+0x20)=p[7]?Ptr(0xf00):0;arena[n+0x34]=(unsigned char)p[6];}
  W(arena,0xf34)=p[8];W(arena,0xf38)=p[9];Links(arena);memcpy(expected,arena,4096);
  unsigned insertion=0;for(unsigned i=0;i<count;++i)if(W(expected,Node(order[i])+0x30)==p[4]){insertion=i+1;break;}
  for(unsigned i=count;i>insertion;--i)order[i]=order[i-1];order[insertion]=added;++count;Links(expected);
  add(arena+p[0],0,arena+Node(added));Record();
  unsigned off=Node(selected);if(!(p[6]&2)&&p[7]){W(expected,0xf34)=p[8]-1;W(expected,off+0x20)=0;}
  unsigned pos=0;while(order[pos]!=selected)++pos;for(unsigned i=pos;i+1<count;++i)order[i]=order[i+1];--count;Links(expected);W(expected,off+0x38)=W(expected,off+0x3c)=W(expected,off+0x40)=W(expected,off+0x44)=0;
  remove(arena+off,0);Record();++cases;
 }
 fclose(in);fclose(output);printf("NATIVE_PRIMITIVE_2D_LISTS_PASS cases=%u\n",cases);return 0;
}
