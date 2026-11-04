#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shader_manager_ctor_aliases.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" void* __fastcall ConstructShaderManager(void*);
extern "C" void* __fastcall ConstructPixelShader(void*);
extern "C" unsigned char ManagerTable[],PixelTable[];
static unsigned char* arena;static FILE* output;static unsigned mode;
static unsigned& word(unsigned o){return *(unsigned*)(arena+o);}
static void event(unsigned kind,unsigned a=0,unsigned b=0,unsigned c=0){unsigned h[]={kind,a,b,c};fwrite(h,4,4,output);unsigned offsets[]={0,0x1e0,0x400},save[3];for(int i=0;i<3;++i){save[i]=word(offsets[i]);if(save[i]==(unsigned)ManagerTable)word(offsets[i])=0x129a080;else if(save[i]==(unsigned)PixelTable)word(offsets[i])=0x129a054;}fwrite(arena,1,0x1000,output);for(int j=0;j<3;++j)word(offsets[j])=save[j];}
extern "C" void* __fastcall BaseCtorBoundary(void* self){event(1,(unsigned)self);*(unsigned*)self=0x129a7c4;if(mode==1){word(4)=0x11223344;word(0x38c)=0x55667788;}return self;}
extern "C" void* __fastcall LayoutCtorBoundary(void* self){event(2,(unsigned)self);memset(self,0xa5,0x14c);if(mode==2){word(0x3ac)=0x12345678;word(0x1e4)=0x87654321;}return self;}
extern "C" void* __fastcall PoolCtorBoundary(void* self,void*,unsigned capacity,unsigned growth){if(capacity||growth!=0x1000)exit(21);event(3,(unsigned)self,capacity,growth);memset(self,0x55,28);if(mode==3){word(0x3c0)=0x44556677;word(0x194)=0x33445566;word(0x3e0)=0x22334455;}return self;}
extern "C" void* __cdecl FableManagerMalloc(size_t size){if(size!=24)exit(22);event(4,size);if(mode==4){word(0x3c0)=0x77889900;word(0x3c4)=0x11223344;word(0x3e4)=0x33445566;}else if(mode==5){word(0x198)=0x44556677;word(0x1d8)=0x12344321;}return arena+0x800;}
extern "C" void UnusedShaderManagerCtorBoundary(){abort();}
int main(int argc,char** argv){
 if(argc!=3)return 2;FILE* input=fopen(argv[1],"rb");output=fopen(argv[2],"wb");if(!input||!output)return 3;
 arena=(unsigned char*)VirtualAlloc((void*)0x24000000,0x10000,0x3000,4);if(arena!=(void*)0x24000000)return 4;
 unsigned p[3],cases=0;while(fread(p,4,3,input)==3){if(fread(arena,1,0x1000,input)!=0x1000)return 5;mode=p[2];void* result=p[0]?ConstructPixelShader(arena+0x400):ConstructShaderManager(arena);event(5,p[0],(unsigned)result);++cases;}
 fclose(input);fclose(output);printf("NATIVE_SHADER_MANAGER_CTOR_PASS cases=%u\n",cases);return 0;
}
