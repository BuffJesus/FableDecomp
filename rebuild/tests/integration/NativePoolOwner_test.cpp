#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pool_owner_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void* __fastcall ConstructPool(void*,void*,unsigned,unsigned);
extern "C" void __fastcall DestroyPool(void*);
extern "C" void __fastcall ClearPool(void*);
extern "C" void* __fastcall DeletePool(void*,void*,unsigned);
extern "C" unsigned char PoolTable[];
static unsigned char* arena;static FILE* output;static unsigned mode,deletes;
static unsigned& word(unsigned o){return *(unsigned*)(arena+o);}
static void event(unsigned kind,unsigned a=0,unsigned b=0,unsigned c=0){unsigned h[]={kind,a,b,c};fwrite(h,4,4,output);unsigned save=word(0);if(save==(unsigned)PoolTable)word(0)=0x129a064;fwrite(arena,1,0x1000,output);word(0)=save;}
extern "C" void* __fastcall BaseCtorBoundary(void* self){event(1,(unsigned)self);*(unsigned*)self=0x129a7c4;if(mode==1){word(24)=0x11223344;word(8)=0x55667788;}else if(mode==2){word(4)=(unsigned)(arena+0x500);word(12)=0x98765432;}return self;}
extern "C" void __fastcall BaseDtorBoundary(void* self){event(2,(unsigned)self);*(unsigned*)self=0x129a7c4;}
extern "C" void* __fastcall DeleteBlock(void* self,void*,unsigned flags){if(flags!=1)exit(21);event(3,(unsigned)self,flags);++deletes;unsigned* w=(unsigned*)self;w[2]=0x65430000|deletes;if(mode==1)w[5]=0;else if(mode==2&&w[5])*(unsigned char*)(w[5]+12)=0;else if(mode==3){word(4)=(unsigned)(arena+0x500);word(16)=0x12344321;}return self;}
void operator delete(void* p){event(4,(unsigned)p);}
int main(int argc,char** argv){
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;word(0x8000)=(unsigned)&DeleteBlock;
 unsigned p[7],cases=0;while(fread(p,4,7,input)==7){if(fread(arena,1,0x1000,input)!=0x1000)return 5;mode=p[4];deletes=0;unsigned result=0;switch(p[0]){case 0:result=(unsigned)ConstructPool(arena,0,p[5],p[6]);break;case 1:DestroyPool(arena);break;case 2:ClearPool(arena);break;case 3:result=(unsigned)DeletePool(arena,0,p[3]);break;}event(5,p[0],result);++cases;}
 fclose(input);fclose(output);printf("NATIVE_POOL_OWNER_PASS cases=%u\n",cases);return 0;
}
