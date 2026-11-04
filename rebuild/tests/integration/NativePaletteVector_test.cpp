#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "palette_vector_aliases.h"
#include "palette_vector_retail.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall Resize(void*,void*,unsigned);
extern "C" void __fastcall Insert(void*,void*,void*,unsigned,const void*);
extern "C" void* __fastcall Erase(void*,void*,void*,void*);
static unsigned char* arena;static FILE* output;static unsigned mode,events;
static unsigned& at(unsigned off){return *(unsigned*)(arena+off);}
static void* stable(void* p){return (unsigned char*)p>=arena&&(unsigned char*)p<arena+4096?p:0;}
static void record(unsigned kind,void* self,unsigned arg){if(++events>100)exit(20);unsigned h[]={kind,(unsigned)self,arg};if(fwrite(h,4,3,output)!=3||fwrite(arena,1,4096,output)!=4096)exit(9);}
extern "C" void __fastcall Default(void* self){((unsigned*)self)[-1]=0x12345678;*(unsigned*)self=0x1111;record(1,0,0);if(mode==5){unsigned bytes=at(4)-at(0),cap=at(8)-at(0);at(0)=(unsigned)(arena+0x400);at(4)=at(0)+bytes;at(8)=at(0)+cap;}}
extern "C" void __fastcall Copy(void* self,void*,const void* other){record(2,stable(self),*(unsigned*)other);*(unsigned*)self=*(unsigned*)other;if(mode==3)at(0x604)+=7;}
extern "C" void* __fastcall Assign(void* self,void*,const void* other){record(3,stable(self),*(unsigned*)other);*(unsigned*)self=*(unsigned*)other;if(mode==1)at(0x600)+=9;return self;}
extern "C" void __fastcall Destroy(void* self){record(4,stable(self),*(unsigned*)self);if(mode==2)at(0x200)^=0x55;}
extern "C" void* __cdecl Allocate(unsigned n){if(n>128)exit(21);record(5,0,n);if(mode==4)at(0x204)+=3;return arena+0x800;}
extern "C" void __cdecl Free(void* p){record(6,p,0);}
int main(int argc,char** argv){
 if(argc!=4)return 2;FILE* in=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!in||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,65536,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);memcpy(code,retailCode,sizeof(retailCode));
 for(unsigned i=0;i<sizeof(retailFixups)/sizeof(retailFixups[0]);++i){unsigned off=retailFixups[i][0],v=0;switch(retailFixups[i][1]){
 case 0xbf97a0:v=(unsigned)code;break;case 0xbf9420:v=(unsigned)(code+128);break;case 0xbf95d0:v=(unsigned)(code+224);break;case 0xbf8e50:v=(unsigned)(code+512);break;case 0xbf8e90:v=(unsigned)(code+576);break;case 0xbf8e20:v=(unsigned)(code+640);break;case 0xbf8ff0:v=(unsigned)(code+704);break;case 0xbf94d0:v=(unsigned)(code+768);break;case 0xbf8f30:v=(unsigned)(code+1024);break;case 0xbf8f70:v=(unsigned)(code+1088);break;
 case 0x99e4b0:v=(unsigned)Default;break;case 0x99ec30:v=(unsigned)Copy;break;case 0x99efb0:v=(unsigned)Assign;break;case 0x99eae0:v=(unsigned)Destroy;break;case 0xbfea0e:v=(unsigned)Allocate;break;case 0xbfea14:v=(unsigned)Free;break;default:return 8;}
 *(unsigned*)(code+off)=v-(unsigned)(code+off+4);}
 typedef void (__fastcall *ResizeFn)(void*,void*,unsigned);typedef void (__fastcall *InsertFn)(void*,void*,void*,unsigned,const void*);typedef void* (__fastcall *EraseFn)(void*,void*,void*,void*);
 ResizeFn resize=argv[3][0]=='r'?(ResizeFn)code:Resize;InsertFn insert=argv[3][0]=='r'?(InsertFn)(code+224):Insert;EraseFn erase=argv[3][0]=='r'?(EraseFn)(code+128):Erase;
 unsigned p[4],cases=0;while(fread(p,4,4,in)==4){if(fread(arena,1,4096,in)!=4096)return 5;mode=p[0];events=0;
 if(p[1]==0)resize(arena,0,p[2]);else if(p[1]==1)insert(arena,0,(void*)(at(0)+p[2]*8),p[3],arena+0x600);else {void* first=(void*)(at(0)+p[2]*8);if(erase(arena,0,first,(void*)(at(0)+p[3]*8))!=first)return 6;}
 record(7,0,0);++cases;
 }
 fclose(in);fclose(output);printf("NATIVE_PALETTE_VECTOR_PASS cases=%u\n",cases);return 0;
}
